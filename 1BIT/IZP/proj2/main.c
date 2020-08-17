/**
 * Projekt c.2 - Iteracni vypocty
 * Autor: David Kolečkář, xkolec07@stud.fit.vutbr.cz
 * Skupina: 17 (1BIA)
 * Datum: 2014/11/22
 * Nazev souboru: proj2.c
 * Popis programu: Implementace funkce tangens pomoci Taylorova polynomu.
 * Implementace funkce tangens pomoci zretezenych zlomku.
 * Vypocet vzdalenosti a vysky mereneho objektu pomoci udaju ze senzoru natoceni mericiho pristroje.
 */

#include <stdio.h> /* prace se vstupem/vystupem*/
#include <ctype.h> /*testovani znaku - isalpha, isdigit...*/
#include <stdlib.h> /* obecné funkce jazyka C*/
#include <string.h>  /*kvuli funkci strcmp*/
#include <math.h>  /*matematicka knihovna - porovnani vysledku tangens*/

#define MAXUHEL 1.4
#define MINUHEL 0.0
#define MAXITERACI 14
#define MINITERACI 0.0
#define MINVYSKA 0.0
#define MAXVYSKA 100

/**
Deklarace prototypu funkci
*/
double taylor_tan(double x, unsigned int n);
double cfrac_tan(double x, unsigned int n);
void compare_value(double A,int N,int M);
void calculate(double alfa, double beta, double vyska);
double myabs(double value) {return value<0 ? -value : value;}
int find_iteration(double uhel);

/**
Kody chyb programu
*/
enum tecodes
{
  EOK,          /** OK*/
  ECLWRONG,    /** Chybny prikazovy radek. */
  EVALUE,       /** Chybna hodnota v prikazove radce*/
  EUNKNOWN,    /** Neznama chyba */
};

/**
Chybova hlaseni odpovidajici chybovým kodum ve vyctu tecodes.
*/
const char *ECODEMSG[] =
{
  "OK",
  "Chybne parametry prikazového radku!\n",
  "Zadana hodnota NENI v danem rozsahu",
  "Nastala nepredvidana chyba! \n",
};

/**
Stavove kody programu
*/
enum Estates
{
  SHELP,
  STAN,
  SCALCULATE,
};


/**
Konstanta HELPMSG, vypise se pri zavoláni parametru --help
*/
const char *HELPMSG =
"PROGRAM: Iterační výpočty.\n"
"AUTOR: David Kolečkář\n"
"PROGRAM PROVÁDÍ: Výpočet funkce tangens pomocí Taylorova polynomu a zřetězených zlomů.\n"
"Dále výpočet vzdálenosti a výšky měřeného objektu pomocí údajů ze senzorů natočení měřicího přístroje.\n"
"POPIS PARAMETRŮ:\n"
" --help      Vypise na obrazovku napovedu.\n\n"
"--tan A N M    Srovná přesnosti výpočtu tangens úhlu A (v radiánech) mezi\n"
"               voláním tan z matematické knihovny,\n"
"               a výpočtu tangens pomocí Taylorova polynomu a zřetězeného zlomku.\n"
"               Argumenty N a M udávají, ve kterých iteracích iteračního výpočtu\n"
"               má srovnání probíhat. 0 < N <= M < 14\n\n"
"[-c X] -m A [B]  Vypočítá a změří vzdálenosti.\n"
"               Úhel α (viz obrázek) je dán argumentem A v radiánech. Program\n"
"               vypočítá a vypíše vzdálenost měřeného objektu. 0 < A <= 1.4 < π/2\n"
"               Pokud je zadán, úhel β udává argument B v radiánech. Program \n"
"               vypočítá a vypíše i výšku měřeného objektu. 0 < B <= 1.4 < π/2\n"
"               Argument -c nastavuje výšku měřicího přístroje c pro výpočet.\n"
"               Výška c je dána argumentem X (0 < X <= 100).\n"
"               Argument je volitelný - implicitní výška je 1.5 metrů.\n";



/**
 * Struktura obsahujici hodnoty parametru prikazove radky.
 */
typedef struct
{
  double A,B,X;
  int N,M;
  int ecode;         /** Chybovy kod programu, odpovida vyctu tecodes. */
  int state;         /** Stavovy kod programu, odpovida vyctu Estates. */
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
    .X=1.5, .B=0.0, .A=0.0,
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
      if(strcmp("-m", argv[1]) == 0)
      {
        result.A=atof(argv[2]);
        result.state=SCALCULATE;
        if(result.A>MINUHEL && result.A <=MAXUHEL) {result.ecode=EOK;} else {result.ecode=EVALUE;}
      }
  }

  else if(argc == 4)
  {
        if(strcmp("-m", argv[1]) == 0)
        {
          result.A=atof(argv[2]);
          result.B=atof(argv[3]);
          result.state=SCALCULATE;
          if((result.A>MINUHEL && result.A <=MAXUHEL) && (result.B>MINUHEL && result.B <=MAXUHEL)) {result.ecode=EOK;} else {result.ecode=EVALUE;}
        }
        else result.ecode=ECLWRONG;
  }

  else if(argc == 5)
  {
    if(strcmp("--tan", argv[1]) == 0)
    {
      result.A=atof(argv[2]);
      result.N=atoi(argv[3]);
      result.M=atoi(argv[4]);
      result.state = STAN;
      if((result.A>MINUHEL && result.A <=MAXUHEL) && (result.N>MINITERACI && result.M<MAXITERACI)) {result.ecode=EOK;} else {result.ecode=EVALUE;}
    }

    else if((strcmp("-c", argv[1]) == 0) && (strcmp("-m", argv[3]) == 0))
    {
      result.X=atof(argv[2]);
      result.A=atof(argv[4]);
      result.state=SCALCULATE;
      if((result.A>MINUHEL && result.A <=MAXUHEL) && (result.X>MINVYSKA && result.X <=MAXVYSKA)) {result.ecode=EOK;} else {result.ecode=EVALUE;}
    }

     else result.ecode=ECLWRONG;
  }

  else if(argc == 6)
  {
    if((strcmp("-c", argv[1]) == 0) && (strcmp("-m", argv[3]) == 0)){
      result.X=atof(argv[2]);
      result.A=atof(argv[4]);
      result.B=atof(argv[5]);
      result.state=SCALCULATE;
      if((result.A>MINUHEL && result.A <=MAXUHEL) && (result.B>MINUHEL && result.B <=MAXUHEL) && (result.X>MINVYSKA && result.X <=MAXVYSKA)) {result.ecode=EOK;} else {result.ecode=EVALUE;}
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
 * Hlavni program, probiha zde zpracovani struktury TParams.
 */
int main(int argc, char *argv[])
{
  TParams params = getParams(argc, argv);

  if (params.ecode != EOK)
  {
    printECode(params.ecode);
    return EXIT_FAILURE;
  }

  if (params.state == SHELP){
    printf("%s", HELPMSG);
    return EXIT_SUCCESS;
  }

  if (params.state == STAN){
    compare_value(params.A,params.N,params.M);
    return EXIT_SUCCESS;
  }

  if (params.state == SCALCULATE){
    calculate(params.A,params.B,params.X);
    return EXIT_SUCCESS;
  }
  return EXIT_SUCCESS;
}

/**
 * Vypocet tangensu pomoci taylorovy rady
 * return vraci vypocitanou hodnotu
 */
double taylor_tan(double x, unsigned int n){
    double citatel[13] = {1, 1, 2, 17, 62, 1382, 21844, 929569, 6404582, 443861162,
    18888466084, 113927491862, 58870668456604};
    double jmenovatel[13] = {1, 3, 15, 315, 2835, 155925, 6081075, 638512875, 10854718875,
    1856156927625, 194896477400625, 49308808782358125, 3698160658676859375};
    unsigned int i;
    double vysledek = x;
    double const xx = x*x;

    for (i = 1; i < n; i++){
      x = x*xx;
      vysledek += (citatel[i]*x)/jmenovatel[i];
    }

    return vysledek;
}

/**
 * Vypocet tangensu pomoci zretezenych zlomku
 * return vraci vypocitanou hodnotu
 */
double cfrac_tan(double x, unsigned int n){
  double zlomek = 1.0 ;
  double scitanec;
  double citatel = x*x;
  int k = n;

  for ( ; k >= 1 ; k--)
  {
    scitanec = (k+1)*2-1 ;
    zlomek = citatel / (scitanec - zlomek);
  }
  return x / (1.0 - zlomek );
}

/**
 * Funkce compare_value, vypise hodnoty funkce tangens pomoci taylorovy rady,
 * zretezených zlomu a matematicke knihovny.
 * Urci absolutni rozdil mezi jednotlivymi funkcemi a matematickou knihovnou.
 * Funkce nema návratovou hodnotu
 */
void compare_value(double A,int N,int M){

  for(;N<=M;N++){
    double odchylkataylor=myabs(tan(A)-taylor_tan(A,N));
    double odchylkacfrac=myabs(tan(A)-cfrac_tan(A,N));
    printf("%d %e %e %e %e %e\n",N,tan(A),taylor_tan(A,N),odchylkataylor,cfrac_tan(A,N),odchylkacfrac);
  }

}

/**
 * Funkce find_iteration, hleda potrebny pocet iteraci k vypoctu pomoci cfrac_tan ve funkci calculate
 * S presnosti na 10 desetinych mist
 * Funkce vraci potrebny pocet iteraci
 */
int find_iteration(double uhel){
  int i=0;
  double predchozi=0.0;
  double aktualni=0.0;
  double rozdil=1.0;

  for(i=1; rozdil>1e-10;i++){
    predchozi=aktualni;
    aktualni=cfrac_tan(uhel,i);
    rozdil=myabs(predchozi-aktualni);
  }

  return i;
}

/**
 * Funkce calculate, vypise vzdalenost objektu a pokud je zadan uhel beta tak i vysku mereneho objektu.
 * Implicitni vyska je nastavena na 1.5m
 * K vypoctu se pouziva funkce cfrac_tan, tedy metoda zretezenych zlomku.
 * Funkce nema navratovou hodnotu
 */
void calculate(double alfa, double beta, double vyska){
  double vzdalenost;
  double celkovavyska;
  vzdalenost=vyska/cfrac_tan(alfa,find_iteration(alfa));
  printf("%.10e\n",vzdalenost);
  if(beta!=0.0){
    celkovavyska=(vzdalenost*cfrac_tan(beta,find_iteration(beta)))+vyska;
    printf("%.10e\n",celkovavyska);
  }
}


/* konec souboru proj2.c */
