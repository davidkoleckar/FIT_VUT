/**
 * predmet:  ISA
 * projekt:  FTP Klient
 * @author: David Koleckar, xkolec07
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdbool.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include <sstream>
#include <vector>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <sys/socket.h>

//#include <sys/types.h>
//#include <sys/socket.h>
//#include <ctype.h>
//#include <iterator>
using namespace std;

//definice konstant
#define NO_ERROR 0
#define ERROR_PAR 1
#define ERROR_FILE 2
#define ERROR_CON 3
#define ERROR_CODE 4

bool setPort = false;

//struktura pro parametry
typedef struct params{
  int port; //-a
  string server; //-s
  string path; //-P 
  string user; //-c 
  string pass; //-c
  string activity;
  bool mod; //-p
  string file; //-c file
  string filename; // -d-r-u filename
}Tparams;

/**
 * Funkce otevre a rozparsuje zadany soubor s loginem a heslem
 * @param Tparams *params 
 */
void openFile(Tparams *params){
  ifstream someStream(params->file.c_str());
  string line1,line2;

  if (someStream.is_open())
  {
    getline(someStream, line1);
    getline(someStream, line2);
  }
  else{
    fprintf(stderr,"Nelze otevrit soubor\n");
    exit(ERROR_FILE);
  }

  size_t aux1 = line1.find(string("username:"));
  size_t aux2 = line2.find(string("password:"));

  if (aux1 == string::npos || aux2 == string::npos){
    fprintf(stderr, "Soubor neobsahuje jmeno nebo heslo\n");
    exit(ERROR_FILE);
  }
  else{
   line1.erase(0,10);
   line2.erase(0,10);
  }

  params->user = line1;
  params->pass = line2;
}

/**
 * Funkce pro vypis napovedy na stdout
 */
void help(){
  fprintf(stdout, "Nápověda\n"
    "Parametry programu:\n"
    "\t-s server IP adresa nebo doménové jméno serveru\n"
    "\t-c filename  soubor s uživatelským jménem a heslem\n"
    "\t-a port_number  aktivní mód\n"
    "\t-p pasivní mód\n"
    "\t-u filename soubor, který se uloží na server\n"
    "\t-d filename soubor, který se stáhne ze serveru\n"
    "\t-r filename soubor, který se má smazat ze serveru\n"
    "\t-P path  cesta k souboru, volitelný přepínač k -u, -d, -r\n"
    "\n"
    "Příklad:\n"
    "./fclient -s 192.168.1.102 -c credentials.txt [-p]|[-a port_number] [-d|-u|-r filename] [-P path]\n"
    );
  exit(NO_ERROR);
}

/**
 * Funkce pro zpracovani parametru, kontrola duplicitnich parametru
 * @param Tparams *params 
 * @param int argc 
 * @param char* argv[]
 * @return int NO_ERROR(0)
 */
int doParams(int argc, char* argv[], Tparams *params){
  //vypsani napovedy
  if((argc == 2) && (strcmp(argv[1], "-h") == 0) )
    help();
  //nejsou zadany povine parametry
  if ((argc < 5) || (strcmp(argv[1], "-s") != 0) || (strcmp(argv[3], "-c") != 0)){
    fprintf(stderr,"Chyba parametrů, zkus fclient -s 192.168.1.102 -c credentials.txt\n");
    return ERROR_PAR;
  }

  int opt;
  int p1 =0; int p2=0; int p3=0; int p4=0; int p5=0; int p6=0; int p7=0; int p8=0;
  while ((opt = getopt (argc, argv, "hs:c:pa:u:d:r:P:")) != -1)
  {
    switch (opt)
    {
      case 's':
        p1++;
        params->server = optarg;
        break;
      case 'c':
        p2++;
        params->file = optarg;
        break;
      case 'p':
        p3++;
        setPort = false;
        params->mod = false;
        break;
      case 'a':
        p4++;
        setPort = false;
        params->mod = true;
        params->port = atoi(optarg);
        break;
      case 'u':
        p5++;
        params->filename = optarg;
        params->activity = "upload";
        break;
      case 'd':
        p6++;
        params->filename = optarg;
        params->activity = "download";
        break;
      case 'r':
        p7++;
        params->filename = optarg;
        params->activity = "remove";
        break;  
      case 'P':{
        p8++;
        params->path = optarg;
        //kontrola zda parametr P zacina lomitkem a take konci
        string pom = (params->path).substr(0,1);
        if(pom == "/")
          (params->path).erase(0,1);
        
        pom = (params->path)[(params->path).length()-1];
        if(pom != "/")
          (params->path).insert((params->path).end(),'/');

        break;  
      }
      case '?':
        fprintf(stderr,"Neznamy parametr\n");
        return ERROR_PAR;   
    }
  }
  //kontrola duplicitnich parametru
  if(p1 > 1 || p2 > 1 || p3 > 1 || p4 > 1 || p5 > 1 || p6 > 1 || p7 > 1 || p8 > 1){
    fprintf(stderr,"Duplicitni parametr\n");
    return ERROR_PAR; 
  }

  return NO_ERROR;
}

/**
 * Funkce pro vytvoreni socketu na danem portu a serveru
 * @param string server
 * @param int port
 * @return int mysocket
 */
int connection(int port, string server){
  struct hostent *host;  
  int mysocket;            
  struct sockaddr_in socad;  

  if((mysocket = socket(AF_INET,SOCK_STREAM,0)) <= 0){
    fprintf(stderr, "error socket\n");
    exit(ERROR_CON );
  }  

  socad.sin_family = AF_INET;
  socad.sin_port = (setPort ? htons(port) : htons(21));

  
  if ((host = gethostbyname(server.c_str())) == NULL){
    fprintf(stderr, "error gethostbyname\n");
    exit(ERROR_CON );
  }

  memcpy(&(socad.sin_addr), host->h_addr, host->h_length);

  if(connect(mysocket, (const struct sockaddr*)&socad, sizeof(socad)) != 0){
    fprintf(stderr, "error connect\n");
    exit(ERROR_CON );
  }

  return mysocket;
}

/**
 * Funkce pro odeslani prikazu na server
 * @param string msg
 * @param int socket 
 */
void sendMsg(string msg, int socket){
  if (write(socket, msg.c_str(), strlen(msg.c_str())) < 0){
    fprintf(stderr, "Eror: sendMsg\n");
    exit(ERROR_CON);
  }
}

/**
 * Funkce pro precteni odpovedi ze serveru, probiha zde kontrola navratoveho kodu
 * @param int sock
 * @param string code
 * @return string str
 */
string readMsg(int sock, string code){
  int recv_size;
  const int flags = 0;
  const int size_buf = 4096;
  char buf[size_buf];

  if ((recv_size = recv(sock, buf, size_buf, flags)) == -1){
    fprintf(stderr, "Eror: recv\n");
    exit(ERROR_CON);
  }
  string str(buf, recv_size);
  //kontrola navratoveho kodu z ftp serveru
  string pom = str.substr(0,3);
  if(pom != code){
    fprintf(stderr, "Eror:  %s", str.c_str());
    exit(ERROR_CODE);
  }

  return str;
}

/**
 * Funkce pro precteni odpovedi ze serveru na datovem socketu (pro list)
 * @param int sock
 * @return string str
 */
string readMsgAll(int sock){
  int recv_size; 
  const int flags = 0;
  const int size_buf = 4096;
  char buf[size_buf];
  string str="";

  while(1){
    if((recv_size = recv(sock, buf, size_buf, flags)) == -1){
      fprintf(stderr, "Eror: recv\n");
      exit(ERROR_CON);
    }
    if(recv_size == 0){
      break;
    }
    for(int i = 0; i < recv_size; i++){
      str += buf[i];
    }
  }

  return str;
}

/**
 * Funkce pro parsovani portu po prikazu PASV
 * @param string *server2
 * @param string reply
 * @param int *port2
 */
void parsePort(string *server2, string reply, int *port2){
  string s = reply;
  string p1,p2;

  s = s.substr(s.find("(")+1, s.find(")") - s.find("(") - 1);
  p2 = s.substr(s.find_last_of(",") + 1);
  s.erase(s.begin()+s.find_last_of(","), s.end());
  p1 = s.substr(s.find_last_of(",") + 1);
  s.erase(s.begin()+s.find_last_of(","), s.end());

  s.replace( s.find(","), 1, ".");
  s.replace( s.find(","), 1, ".");
  s.replace( s.find(","), 1, ".");

  *server2 = s;
  *port2 = (atoi(p1.c_str())*256) + atoi(p2.c_str());
}

/**
 * Funkce stahnuti souboru ze serveru
 * @param int sock
 * @param char *file_name
 */
void saveFile(int sock, const char *file_name){
  int recv_size; 
  const int size_buf = 1024;
  char buf[size_buf];
  FILE *file;

  file = fopen(file_name, "wb");
  if(!file){
    fprintf(stderr, "Eror: zadany adresar neexistuje\n");
    exit(ERROR_FILE); 
  }
  while (1)
  {
    if ((recv_size = recv(sock, buf, size_buf, 0)) == -1)
    {
      fprintf(stderr, "Eror: recv\n");
      exit(ERROR_CON); 
    }
    if (recv_size == 0)
    {
      fclose(file);
      return;
    }
    fwrite(buf, recv_size, 1, file);
  }
  fclose(file);
  return;
}

/**
 * Funkce ulozeni souboru na server
 * @param int sock
 * @param char *file_name
 */
void uploadFile(int sock, const char *file_name){
  int recv_size;
  const int size_buf = 4096;
  char buf[size_buf];

  FILE *file;
  if((file = fopen(file_name, "rb")) == NULL){
    fprintf(stderr, "Eror: open file\n");
    exit(ERROR_FILE); 
  }
   
  do{
    recv_size = fread(buf, sizeof(char), size_buf, file); 
    if(write(sock, buf, recv_size) < 0){
      fprintf(stderr, "Eror: write\n");
      exit(ERROR_CON);
    }
  } while (recv_size == size_buf);

  fclose(file);
  return;
}

/**
 * Funkce pro otevreni pasivniho rezimu, navaze datovy socket
 * @param int sock
 * @return int sockdata
 */
int pasv(int sock){
  string reply;

  string message = "PASV\r\n";
  sendMsg(message.c_str(), sock);
  reply = readMsg(sock, "227");
  setPort = true;

  string server2;
  int port2,sockdata;

  parsePort(&server2,reply,&port2);
  sockdata = connection(port2,server2);

  return sockdata;
}

/**
 * Funkce pro otevreni datoveho socketu po prikazu PORT
 * kontroluje zda port probehl uspesne
 * @param int sock
 * @return int sockdata
 */
int readMsgPort(int sock, string code){
  int recv_size;
  const int flags = 0;
  const int size_buf = 4096;
  char buf[size_buf];

  if ((recv_size = recv(sock, buf, size_buf, flags)) == -1){
    fprintf(stderr, "Eror: recv\n");
    exit(ERROR_CON);
  }
  string str(buf, recv_size);
  //kontrola navratoveho kodu z ftp serveru
  string pom = str.substr(0,3);
  if(pom != code){
    return 1;
  }

  return 0;
}

/**
 * Funkce pro navazani aktivniho rezimu, ziskam ip adresu
 * kterou nasledne upravim do formatu pro prikaz PORT
 * @param int sock
 * @param int p
 * @return int sockdata
 */
int activ(int sock, int p){
  //ziskani ip adres
  struct ifaddrs *ifap, *ifa;
  struct sockaddr_in *sa;
  char *addr;
  string adr[128];
  getifaddrs (&ifap);
  ifa = ifap;
  int x =0;
  for (ifa = ifap; ifa; ifa = ifa->ifa_next){
    if (ifa->ifa_addr->sa_family==AF_INET) {
      sa = (struct sockaddr_in *) ifa->ifa_addr;
      addr = inet_ntoa(sa->sin_addr);
      adr[x] = addr;
      x++;
    }
  }
  freeifaddrs(ifap);
  //prevedeni ip adresy a portu do pozadovaneho formatu pro prikaz PORT
  //staci localhost a adresu na dalsim rozhrani
  for(int i =0; i<=1;i++){
    adr[i].replace( adr[i].find("."), 1, ",");
    adr[i].replace( adr[i].find("."), 1, ",");
    adr[i].replace( adr[i].find("."), 1, ",");
    adr[i]+=",";
    int p1 = p/256;
    int p2 = p%256;
    adr[i] += static_cast<ostringstream*>( &(ostringstream() << p1) )->str();
    adr[i] +=",";
    adr[i] += static_cast<ostringstream*>( &(ostringstream() << p2) )->str();
  }

  //vytvoreni data socketu
  struct sockaddr_in sin;
  int sockdata;

  sockdata = socket(AF_INET, SOCK_STREAM, 0);
  if (sockdata < 0) {
    fprintf(stderr, "Eror: sock\n");
    exit(ERROR_CON);
   }

  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = htons(p);

  if (bind(sockdata, (struct sockaddr*)&sin, sizeof(sin)) < 0){
    fprintf(stderr, "Eror: bind\n");
    exit(ERROR_CON);
  }

  if (listen(sockdata,5) < 0){
    fprintf(stderr, "Eror: listen\n");
    exit(ERROR_CON);
  }

  string message = "PORT "+adr[0]+"\r\n";
  sendMsg(message.c_str(), sock);
  int q = readMsgPort(sock, "200");//zkouska zda PORT probehl v poradku
  if(q==1){//pokud ne zkusim dalsi adresu 
    string message = "PORT "+adr[1]+"\r\n";
    sendMsg(message.c_str(), sock);
    string reply = readMsg(sock, "200");
  }

  return sockdata;
}


/**
 * Funkce pro vypis adresarove struktury serveru
 * @param int sock
 * @param string path
 * @param int port
 * @param bool mod
 * @return vector dirs (seznam slozek)
 */
vector<string> list(int sock, string path, bool mod, int sd){
  string reply, message;

  int sockdata=sd;

  if(!mod) //false je pasivni
    sockdata = pasv(sock); 

  message = "LIST "+path+"\r\n";
  sendMsg(message.c_str(), sock);
  reply = readMsg(sock, "150");

  if(mod){//pokud je aktivni zavolam accept data
    struct sockaddr_in sas;
    socklen_t sas_len = sizeof(sas);
    sockdata = accept(sockdata, (struct sockaddr *)&sas, &sas_len);
  }

  reply = readMsgAll(sockdata);

  vector<string> dirs; 
  istringstream f(reply);  
  string line;

  while(getline(f, line)){
    char c = line.at(0);
    if(c == 'd'){
      size_t last = line.rfind(" ");
      
      string pom = line.substr(last+1);
      string pom2 = pom.substr(0,pom.size()-1);

      if(path.compare("/"))
        pom2.insert(pom2.begin(),'/');

      pom2 = path+pom2;
      fprintf(stdout, "%s\n",pom2.c_str());
      dirs.push_back(pom2); 
    }
    else if(c == '-'){
      size_t last = line.rfind(" ");     
      string pom = line.substr(last+1);
      string pom2 = pom.substr(0,pom.size()-1);

      if(path.compare("/"))
        pom2.insert(pom2.begin(),'/');

      pom2 = path+pom2;
      fprintf(stdout, "%s\n",pom2.c_str());
    }
  }

  close(sockdata);  
  reply = readMsg(sock, "226");
  return dirs;
}


/**
 * Hlavni funkce programu, probiha zde inicializace parametru
 * a provadeni jednotlivych funkci
 * @param int sock
 * @param char *file_name
 */
int main(int argc, char *argv[])
{

  Tparams params={0,"","","","","",false,"",""};

  int err;
  if((err = doParams(argc,argv,&params)) != NO_ERROR)
    return err;

  openFile(&params);

  int sock;
  sock = connection(params.port,params.server);

  string reply,message;

  sendMsg("",sock);
  reply = readMsg(sock,"220");

  message = "USER " + params.user + "\r\n";
  sendMsg(message.c_str(), sock);
  reply = readMsg(sock, "331");

  message = "PASS " + params.pass + "\r\n";
  sendMsg(message.c_str(), sock);
  reply = readMsg(sock, "230");

  //stahnuti souboru
  if(params.activity == "download"){

    message = "TYPE I\r\n";
    sendMsg(message.c_str(), sock);
    reply = readMsg(sock, "200");

    int sockdata;
    if(!params.mod) //false je pasivni
      sockdata = pasv(sock); 
    else//true je aktivni
      sockdata = activ(sock,params.port); 

    message = "RETR "+ params.filename +"\r\n";
    sendMsg(message.c_str(), sock);
    reply = readMsg(sock, "150");

    string name = params.path+params.filename;

    if(params.mod){//pokud je aktivni zavolam accept data
      struct sockaddr_in sas;
      socklen_t sas_len = sizeof(sas);
      sockdata = accept(sockdata, (struct sockaddr *)&sas, &sas_len);
    }
    saveFile(sockdata, name.c_str());

    close(sockdata);

    reply = readMsg(sock, "226");
  }
  //nahrani souboru
  else if(params.activity == "upload"){

    message = "TYPE I\r\n";
    sendMsg(message.c_str(), sock);
    reply = readMsg(sock, "200");

    if(params.path != ""){
      message = "CWD "+params.path+"\r\n";
      sendMsg(message.c_str(), sock);
      reply = readMsg(sock, "250");
    }

    int sockdata;
    if(!params.mod) //false je pasivni
      sockdata = pasv(sock); 
    else//true je aktivni
      sockdata = activ(sock,params.port); 

    message = "STOR "+ params.filename +"\r\n";
    sendMsg(message.c_str(), sock);
    reply = readMsg(sock, "150");

    if(params.mod){//pokud je aktivni zavolam accept data
      struct sockaddr_in sas;
      socklen_t sas_len = sizeof(sas);
      sockdata = accept(sockdata, (struct sockaddr *)&sas, &sas_len);
    }

    uploadFile(sockdata,params.filename.c_str());

    close(sockdata);

    reply = readMsg(sock, "226");
  }
  //smazani souboru
  else if(params.activity == "remove"){
    message = "DELE "+ params.path + params.filename +"\r\n";
    sendMsg(message.c_str(), sock);
    reply = readMsg(sock, "250");
  }
  //vypis adresarove struktury
  else{
    //ziskam adresar ve kterem se nachazim po prihlaseni
    message = "PWD\r\n";
    sendMsg(message.c_str(), sock);
    reply = readMsg(sock, "257");
    size_t first = reply.find("\"");
    size_t last = reply.rfind("\"");
    string p = reply.substr(first+1,last-first-1);

    vector<string> dirs,pom;

    int sockdata=0;
    if(params.mod) //je aktivni rezim
      sockdata = activ(sock,params.port); 

    dirs = list(sock,p,params.mod,sockdata);

    for (int unsigned i=0; i<dirs.size();i++){
      pom = list(sock,dirs[i],params.mod,sockdata);
      dirs.insert(dirs.end(),pom.begin(),pom.end());
    }
  }

  message = "QUIT\r\n";
  sendMsg(message.c_str(), sock);
  reply = readMsg(sock, "221");

  close(sock);
  return 0;
}