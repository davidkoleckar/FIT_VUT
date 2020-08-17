#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <iterator>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h> 

using namespace std;

#define ERROR 1
#define NO_ERROR 0

typedef struct urlparsed{
  int port;
  string host;
  string path;
  string file;
}TUrlData;

const char *HELP = {
  "-------------------------------------------------------------\n"
  "--help               Vytiskne napovedu?\n"
  "jako prvni argument zadejte url adresu objektu ktery chcete stahnout\n"
  "-------------------------------------------------------------\n"
};

void urlparser(string url, TUrlData *urldata){
  
  //kontrola url zda obsahuje http://
  size_t aux = url.find(string("http://"));
  if (aux == string::npos) {
    fprintf(stderr, "v url neni http://\n");
    exit(ERROR);
  }
  else{
    url.erase(aux, 7); // http:// 7 znaku, smazani
  }

  //port
  size_t dvojtecka = url.find_first_of(":");
  size_t lomeno = url.find_first_of("/");
  if(dvojtecka < (lomeno - 1)){
    string port(url,dvojtecka+1,url.size()-lomeno-1);
    urldata->port=atoi(port.c_str());
    if(urldata->port > 0)
      url.erase(dvojtecka,lomeno-dvojtecka);
    else
      urldata->port=80;
  }
  else{// neni tam smazu jen /
    if(dvojtecka == string::npos)
      urldata->port=80;
    else
      url.erase(lomeno,1);
  }

  //path
  size_t lomeno1 = url.find("/");
  if(lomeno1 != string::npos){
    urldata->path = url.substr(lomeno1);
    url.erase(lomeno1,urldata->path.length());
  }
  else
    urldata->path = "/";

  //filename z cesty
  size_t pom;
  if((pom = urldata->path.rfind("/")) != string::npos){
    if(urldata->path.length()-1 != pom){
      urldata->file = urldata->path.substr(pom+1);
      //kotva
      if(urldata->file.find("#") != string::npos){
        urldata->file.erase(urldata->file.find("#"));
        if((urldata->file.find("#"))==0)
          urldata->file="index.html";
      }
      //formulare
      if(urldata->file.find("?") != string::npos){
        urldata->file.erase(urldata->file.find("?"));
        if((urldata->file.find("?"))==0)
          urldata->file="index.html";
      }
    }
  }
  
  //host
  urldata->host = url;

  string path;
  //mezery v path nahradim %20
  for (string::const_iterator i = urldata->path.begin(); i != urldata->path.end(); ++i){
    if (*i == ' ') 
      path += "%20";
    else
      path += *i;
  } 
  urldata->path = path;

  //for debug
  //cout << "\n" << urldata->port << "\n" << urldata->path << "\n" << urldata->host <<"\n" << urldata->file <<"\n";
}

void pripoj(int *sock, TUrlData urldata){
  hostent *host;  
  int mysocket;            
  sockaddr_in socad;  


  if ((host = gethostbyname(urldata.host.c_str())) == NULL) {
    fprintf(stderr, "error gethostbyname\n");
    exit(ERROR);
  }

  if((mysocket = socket(AF_INET,SOCK_STREAM,0)) <= 0){
    fprintf(stderr, "error socket\n");
    exit(ERROR);
  }  

  socad.sin_family = AF_INET;
  socad.sin_port = htons(urldata.port);
  memcpy(&(socad.sin_addr), host->h_addr, host->h_length);

  if(connect(mysocket, (const struct sockaddr*)&socad, sizeof(socad)) != 0){
    fprintf(stderr, "error connect\n");
    exit(ERROR);
  }

  *sock = mysocket;
}


int main(int argc, char *argv[])
{
  //help
  if ((argc == 2) && (strcmp(argv[1], "--help") == 0)) {
    cout<<HELP;
    return NO_ERROR;
  }
  //bez url nebo help
  if(argc != 2){
    fprintf(stderr, "neni zadana url nebo --help\n");
    return ERROR;
  }
  else{
    string url = argv[1];
    bool end=false; //pro konec do while cyklu
    int redlimit=0; //pocitadlo limitu pro presmerovani
    do{
      TUrlData urldata={80,"","","index.html"};
      int sock;
      //zavolani funkce pro parsovani adresy
      urlparser(url, &urldata);
      //pripojeni
      pripoj(&sock, urldata);
 
      //zjistim porotokol HTTP
      string test ="HEAD " + urldata.path + " HTTP/1.1\r\n"
                   "Host: " + urldata.host + "\r\n"
                   "Connection: close\r\n\r\n";
      int size;
      if ((size = send(sock, test.c_str(), test.size(), 0)) <= 0) {
        fprintf(stderr, "sending error\n");
        exit(ERROR);
      }
      string pom2,http;
      char buf[1024];
      while((size = recv(sock, buf, 1024, 0)) > 0){
        pom2.append(buf,size);
        if(http.length() == 0 && (pom2.find("\r\n\r\n") != string::npos)){
          http = pom2.substr(0,8);
        }
      }

      pripoj(&sock, urldata);
      //zvolim vhodnou hlavicku
      string get;
      get =  "GET " + urldata.path + " HTTP/1.1\r\n"
                      "Host: " + urldata.host + "\r\n"
                      "Connection: close\r\n\r\n";
      
      if(http == "HTTP/1.0"){
        get =  "GET " + urldata.path + " HTTP/1.0\r\n"
                      "Host: " + urldata.host + "\r\n"
                      "Connection: close\r\n\r\n";
      }
      

      if ((size = send(sock, get.c_str(), get.size(), 0)) <= 0) {
      fprintf(stderr, "sending error\n");
      exit(ERROR);
      }

      char buffer[1024];
      string pom,headers;
      int statecode;
      bool chuncked; 
      while((size = recv(sock, buffer, 1024, 0)) > 0){
        pom.append(buffer,size);
        if(headers.length() == 0 && (pom.find("\r\n\r\n") != string::npos)){
          headers = pom.substr(0, pom.find("\r\n\r\n"));
          //vymazani hlavicky
          pom.erase(pom.begin(), pom.begin()+headers.length()+4);
          //chuncked?
          if (headers.find("Transfer-Encoding: chunked") != string::npos){
            chuncked = true;
          }
          else{
            chuncked = false;
          }
        }

      }//konec while

      //http status kod
      size_t space = headers.find(" ");
      statecode = atol(headers.substr(space + 1, 3).c_str());
      //cout << statecode;
      close(sock);
      switch(statecode){
          case 200:
          case 201:
          case 202:
          case 203:
          case 204:
          case 205:
          case 206:{
            //chuncked
            if(chuncked){
              int chunksize = -1;
              size_t index;
              string aux,hex;
              while(chunksize != 0){
                if((index = pom.find("\r\n")) != string::npos){
                  hex = pom.substr(0,index);
                  chunksize = strtol(hex.append("\0").c_str(), NULL, 16);
                  pom.erase(0, index+2);
                  aux.append(pom, 0, chunksize);
                  pom.erase(0, chunksize+2);
                }
              }
              pom = aux;
            }
            //vypsani do souboru
            ofstream outputfile(urldata.file.c_str());
            copy(pom.begin(), pom.end(), ostream_iterator<char>(outputfile,""));
            //konec while cyklu
            end = true;
            }
            break;

          case 301:
          case 302:{
            //limit redirect
            redlimit++;
            if(redlimit > 5){
              fprintf(stderr, "mnoho presmerovani > 5 \n");
              exit(ERROR);
            }

            //location
            size_t pos = headers.find("\r\nLocation: ");
            if(pos == string::npos){
              fprintf(stderr, "neni location\n");
              exit(ERROR);
            }
            size_t pos2 =headers.substr(pos+2).find("\r\n") + pos + 2;
            url = headers.substr(pos + 12, pos2 - pos - 12);
            }
            break;

          case 400:
          case 401:
          case 402:
          case 403:
          case 404:
          case 405:
          case 406:
          case 407:
          case 408:
          case 409:
          case 410:
          case 411:
          case 412:
          case 413:
          case 414:
          case 415:
          case 416:
          case 417:
          case 500:
          case 501:
          case 502:
          case 503:
          case 504:
          case 505:{
            fprintf(stderr, "chybovy kod: %d \n", statecode);
            exit(ERROR);
            }
            break;
        
          default:
            fprintf(stderr, "neznamy kod\n");
            exit(ERROR);
      }//konec switch

    }while(!end);

  }//konec else
 return NO_ERROR;
}//konec main