/**
 * Projekt c.3 - Pruchod bludistem
 * Autor: David Koleckar, xkolec07@stud.fit.vutbr.cz
 * Skupina: 17 (1BIA)
 * Datum: 2014/11/22
 * Nazev souboru: proj3.c
 * Popis programu: Cilem programu je vypis souradnic policek bludiste,
 * pres ktere vede cesta z vchodu bludiste do jeho vychodu.
 */

#include <stdio.h> /* prace se vstupem/vystupem*/
#include <ctype.h> /*testovani znaku - isalpha, isdigit...*/
#include <stdlib.h> /* obecné funkce jazyka C*/
#include <string.h>  /*kvuli funkci strcmp*/
#include <stdbool.h>


/**
 *Kody chyb programu
 */
enum tecodes
{
  EOK,          /** OK*/
  ECLWRONG,    /** Chybny prikazovy radek. */
  EFILE,
  EUNKNOWN,    /** Neznama chyba */
};

/**
 *Chybova hlaseni odpovidajici chybovym kodum ve vyctu tecodes.
 */
const char *ECODEMSG[] =
{
  "OK",
  "Chybne parametry prikazoveho radku!\n",
  "Chyba ve vstupnim souboru\n",
  "Nastala nepredvidana chyba! \n",

};

/**
 *Stavove kody programu
 */
enum Estates
{
  SHELP,
  STEST,
  SRPATH,
  SLPATH,
};

/**
 *Struktura mapy
 */
typedef struct {
  int rows;
  int cols;
  unsigned char *cells;
} Map;

enum Steny{
  LEFT,
  RIGHT,
  UPDOWN,
};

/**
 *Konstanta HELPMSG, vypise se pri zavolani parametru --help
 */
const char *HELPMSG =
"PROGRAM: Pruchod bludistem.\n"
"AUTOR: David Koleckar\n"
"PROGRAM PROVADI:  Vypis souradnic policek bludiste, pres ktere vede cesta z vchodu bludiste do jeho vychodu\n"
"POPIS PARAMETRU:\n"
" --help   Vypise na obrazovku napovedu.\n"
"--test    Pouze zkontroluje, ze soubor dany druhym argumentem programu obsahuje radnou definici mapy bludiste.\n"
"--rpath R C bludiste.txt   hleda pruchod bludistem na vstupu na radku R a sloupci C. Pruchod hleda pomoci pravidla prave ruky\n"
"--lpath R C bludiste.txt   hleda pruchod bludistem na vstupu na radku R a sloupci C. Pruchod hleda pomoci pravidla leve ruky\n";

/**
 * Struktura obsahujici hodnoty parametru prikazove radky.
 */
typedef struct
{
  int ecode;         /** Chybovy kod programu, odpovida vyctu tecodes. */
  int state;         /** Stavovy kod programu, odpovida vyctu Estates. */
  char *nazev;
  int radek;
  int sloupec;
} TParams;

/**
 * Vytiskne hlaseni odpovidajici chybovemu kodu.
 * ecode kod chyby programu
 */
void printECode(int ecode)
{
  if (ecode < EOK || ecode > EUNKNOWN)
  {
    ecode = EUNKNOWN;
  }
  fprintf(stderr, "%s", ECODEMSG[ecode]);
}

/**
 * Zpracuje argumenty prikazoveho radku a vrati je ve strukture TParams.
 * argc pocet argumentu.
 * argv pole textovych retezcu s argumenty.
 * return vraci analyzovane argumenty prikazoveho radku.
 */
TParams getParams(int argc, char *argv[])
{

  TParams result =
  {
    .ecode = EOK,
  };

  if(argc == 2)
  {
    if(strcmp("--help", argv[1]) == 0)
    {
      result.state = SHELP;
    }
    else result.ecode=ECLWRONG;
  }

  else if(argc == 3)
  {
    if(strcmp("--test", argv[1]) == 0)
    {
      result.state = STEST;
      result.nazev = argv[2];
    }
    else result.ecode=ECLWRONG;
  }

  else if(argc == 5)
  {
    if(strcmp("--rpath", argv[1]) == 0)
    {
      result.nazev = argv[4];
      result.sloupec = atoi(argv[3]);
      result.radek = atoi(argv[2]);
      result.state = SRPATH;
    }
    else if(strcmp("--lpath", argv[1]) == 0)
    {
      result.nazev = argv[4];
      result.sloupec = atoi(argv[3]);
      result.radek = atoi(argv[2]);
      result.state = SLPATH;
    }
    else result.ecode=ECLWRONG;
  }

  else
  {
    result.ecode = ECLWRONG;
  }

  return result;
}


/**
 *Alokace mapy
 */
unsigned char *allocate(int rows, int cols, Map *map)
{
  map->cells = NULL;
  map->rows = rows;
  map->cols = cols;
  map->cells = malloc(rows * cols * sizeof(unsigned char));

  return map->cells;
}

/**
 *Dealokace mapy
 */
void dealloc(Map *map)
{
  free(map->cells);
  map->cells = NULL;
}

/**
 *Pristup z 2D pole na jednorozmerne
 */
unsigned char *get_item(int row, int col, Map *map)
{
  return &map->cells[row*map->cols + col];
}

/**
 *Funkce vraci true, pokud na zminene hranici policka stoji stena
 */
bool isborder(Map *map, int r, int c, int border){
  int value=*get_item(r, c, map);
  switch (border){
    case LEFT: switch (value) {    //0 je levá
      case 1: return true;
      case 3: return true;
      case 5: return true;
      case 7: return true;
      default: return false;}
    case RIGHT: switch (value) {    //1 je prava
      case 2: return true;
      case 3: return true;
      case 6: return true;
      case 7: return true;
      default: return false;}
    case UPDOWN: switch (value) {    //2 je dolni/horni
      case 4: return true;
      case 5: return true;
      case 6: return true;
      case 7: return true;
      default: return false;}
  }
  return false;
}

/**
 *Funkce start_border vrati, ktera hranice se ma po vstupu do bludiste nasledovat
 */
int start_border(Map *map, int r, int c, int leftright){
  switch (leftright) {
    case 0: //pravaruka
      if(r%2==1 && c==1) return RIGHT;
      if(r%2==0 && c==1) return UPDOWN;
      if(r==1 && c%2==1) return LEFT;
      if(r==map->rows && c%2==0) return RIGHT;
      if(c==map->cols  && (r%2)==1) return UPDOWN;
      if(c==map->cols  && (r%2)==0) return RIGHT;
    case 1: //levaruka
      if(r%2==1 && c==1) return UPDOWN;
      if(r%2==0 && c==1) return RIGHT;
      if(r==1 && c%2==1) return RIGHT;
      if(r==map->rows && c%2==0) return LEFT;
      if(c==map->cols  && (r%2)==1) return LEFT;
      if(c==map->cols  && (r%2)==0) return UPDOWN;
    default: return 0;
  }
}

/**
 *Funkce read_file ulozi hodnoty ze souboru do pole
 */
int read_file(Map *map, TParams *parametry){

  FILE *f;
  f = fopen(parametry->nazev, "r");
  if(f==NULL){printECode(EFILE); return 0;}
  int nactene_cislo;
  int rows,cols;
  fscanf(f,"%d",&rows);
  fscanf(f,"%d",&cols);
  if(allocate(rows+1, cols, map)==NULL)
  {
      printECode(EUNKNOWN);
      return 0;
  }
  int n = map->cols * map->rows;

    for(int i = cols+1; i <= n; i++)
    {
    fscanf(f,"%d",&nactene_cislo);
    map->cells[i] = nactene_cislo;
    }

  fclose(f);
  return 1;
}

/**
 *Funkce test, kontroluje zda je mapa validni
 *Pokud je validni navratova hodnota funkce je 1, v opacnem pripade 0
 */
int test(Map *map){
  int r,c;
  int stav1=1;
  //kontrola prave hranice s levou sousedici
  for(r = 1; r < map->rows; r++){
    for(c = 1; c < map->cols; c++){
    if(stav1==0) break;
    if(isborder(map,r,c,RIGHT)==isborder(map,r,c+1,LEFT))
        stav1=1;
        else  stav1=0;}
    }

    int stav2=1;
    //kontrola horni/dolni hranice
    for(r = 1; r < map->rows-1; r++){
        for(c = 1; c <= map->cols; c++){
          if(stav2==0) break;
          if(((c%2)==0 && (r%2)==1) || ((c%2)==1 && (r%2)==0)){
            if(isborder(map,r,c,UPDOWN)==isborder(map,r+1,c,UPDOWN)) stav2=1; else stav2=0;
            }
        }
    }

  if(stav1==1 && stav2==1) return 1;
  else return 0;
}

/**
 *Funkce turnright vraci hranu trojuhelnika kterou ma projit
 *Definovano pro pravidlo prave ruky
 */
int turnright(int hrana,int r, int c, Map *map){
  if((r%2 == 1 && c%2 == 1) || (r%2 == 0 && c%2==0)){ //trojuhelnik smerem dolu

    if(hrana==RIGHT){
      if(!isborder(map,r,c,UPDOWN)) return UPDOWN;
      else if(!isborder(map,r,c,LEFT)) return LEFT; else return RIGHT;
    }

  else if(hrana==LEFT){
    if(!isborder(map,r,c,RIGHT)) return RIGHT;
    else if(!isborder(map,r,c,UPDOWN)) return UPDOWN; else return LEFT;
  }

  else if(hrana==UPDOWN){
    if(!isborder(map,r,c,LEFT)) return LEFT;
    else if(!isborder(map,r,c,RIGHT)) return RIGHT; else return UPDOWN;
  }

  }

  else if((r%2==0 && c%2==1) || (r%2==1 && c%2==0)){  //trojuhelnik smerem nahoru

    if(hrana==RIGHT){
      if(!isborder(map,r,c,LEFT)) return LEFT;
      else if(!isborder(map,r,c,UPDOWN)) return UPDOWN; else return RIGHT;
    }

    else if(hrana==LEFT){
      if(!isborder(map,r,c,RIGHT)) return RIGHT;
      else if(!isborder(map,r,c,UPDOWN)) return UPDOWN; else return LEFT;
    }

    else if(hrana==UPDOWN){
      if(!isborder(map,r,c,RIGHT)) return RIGHT;
      else if(!isborder(map,r,c,LEFT)) return LEFT; else return UPDOWN;
    }
  }
  return 0;
}

/**
 *Funkce goright hleda pruchod bludistem podle pravidel prave ruky
 */
int goright(Map *map, int r, int c){
  int smer = start_border(map,r,c,0); //0 pro pravidlo prave ruky
  int prosel=smer;

  while((r < 1 || c < 1 || r > map->rows || c > map->cols) != true){

    printf("%d,%d\n",r,c);
    smer = turnright(prosel,r,c,map);

    if (smer == LEFT){
      prosel=RIGHT;
      c--;}
    else if (smer == RIGHT){
      prosel=LEFT;
      c++;}
    else if (smer == UPDOWN){
      prosel=UPDOWN;
      if ((c % 2 == 0 && r % 2 == 1) ||(c % 2 ==1 && r %2 == 0))
        r++;
      else if ( (c % 2 == 1 && r % 2 == 1) ||(c % 2 ==0 && r %2 == 0))
        r--;
  }
  }
  return 1;
}

/**
 *Funkce turnleft vraci hranu trojuhelnika kterou ma projit
 *Definovano pro pravidlo leve ruky
 */
int turnleft(int hrana,int r, int c, Map *map){
  if((r%2 == 1 && c%2 == 1) || (r%2 == 0 && c%2==0)){ //trojuhelnik smerem dolu

    if(hrana==RIGHT){
      if(!isborder(map,r,c,LEFT)) return LEFT;
      else if(!isborder(map,r,c,UPDOWN)) return UPDOWN; else return RIGHT;
    }

  else if(hrana==LEFT){
    if(!isborder(map,r,c,UPDOWN)) return UPDOWN;
    else if(!isborder(map,r,c,RIGHT)) return RIGHT; else return LEFT;
  }

  else if(hrana==UPDOWN){
    if(!isborder(map,r,c,RIGHT)) return RIGHT;
    else if(!isborder(map,r,c,LEFT)) return LEFT; else return UPDOWN;
  }

  }

  else if((r%2==0 && c%2==1) || (r%2==1 && c%2==0)){  //trojuhelnik smerem nahoru

    if(hrana==RIGHT){
      if(!isborder(map,r,c,UPDOWN)) return UPDOWN;
      else if(!isborder(map,r,c,LEFT)) return LEFT; else return RIGHT;
    }

    else if(hrana==LEFT){
      if(!isborder(map,r,c,RIGHT)) return RIGHT;
      else if(!isborder(map,r,c,UPDOWN)) return UPDOWN; else return LEFT;
    }

    else if(hrana==UPDOWN){
      if(!isborder(map,r,c,LEFT)) return LEFT;
      else if(!isborder(map,r,c,RIGHT)) return RIGHT; else return UPDOWN;
    }
  }
  return 0;
}

/**
 *Funkce goleft hleda pruchod bludistem podle pravidel leve ruky
 */
int goleft(Map *map, int r, int c){
  int smer = start_border(map,r,c,1); //1 pro pravidlo leve ruky
  int prosel=smer;

  while((r < 1 || c < 1 || r > map->rows || c > map->cols) != true){


    printf("%d,%d\n",r,c);
    smer = turnleft(prosel,r,c,map);

    if (smer == LEFT){
      prosel=RIGHT;
      c--;}
    else if (smer == RIGHT){
      prosel=LEFT;
      c++;}
    else if (smer == UPDOWN){
      prosel=UPDOWN;
      if ((c % 2 == 0 && r % 2 == 1) ||(c % 2 ==1 && r %2 == 0))
        r++;
      else if ( (c % 2 == 1 && r % 2 == 1) ||(c % 2 ==0 && r %2 == 0))
        r--;
  }

  }
  return 1;
}

/**
 *Hlavni program, probiha zde zpracovani struktury TParams.
 */
int main(int argc, char *argv[])
{
  TParams params = getParams(argc, argv);
  Map map;

  if (params.ecode != EOK)
  {
    printECode(params.ecode);
    return EXIT_FAILURE;
  }

  if (params.state == SHELP){
    printf("%s", HELPMSG);
    return EXIT_SUCCESS;
  }

  if (params.state == STEST){
    if(!read_file(&map, &params))
      return 0;
    else if(test(&map)) printf("Valid\n");
      else printf("Invalid\n");
    dealloc(&map);
    return EXIT_SUCCESS;
  }

  if(params.state == SRPATH){
    read_file(&map, &params);
    goright(&map,params.radek,params.sloupec);
    dealloc(&map);
    return EXIT_SUCCESS;
  }
  if(params.state == SLPATH){
    read_file(&map, &params);
    goleft(&map,params.radek,params.sloupec);
    dealloc(&map);
    return EXIT_SUCCESS;
  }


  return EXIT_SUCCESS;
}
/* konec souboru proj3.c */
