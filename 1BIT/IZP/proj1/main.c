/**
 * Projekt c.1 - Tabulkovy kalkulator
 * Autor: David Kolečkář, xkolec07@stud.fit.vutbr.cz
 * Skupina: 17 (1BIA)
 * Datum: 2014/11/15
 * Nazev souboru: proj1.c
 * Popis programu:Tabulkový kalkulátor provádí funkce vyhledání maxima, minima,
 * funkce součtu a aritmetického průměru vybraných buněk.
 * Tabulku ve formě textového souboru bude program očekávat na standardním vstupu.
 * Požadovanou operaci a výběr buněk specifikuje uživatel v argumentu příkazové řádky.
 */

#include <stdio.h> /* prace se vstupem/vystupem*/
#include <ctype.h> /*testovani znaku - isalpha, isdigit...*/
#include <stdlib.h> /* obecné funkce jazyka C*/
#include <string.h>  /*kvuli funkci strcmp*/
#include <float.h> /* maximální a minimální hodnota double*/

#define MAX_LINE 1025 /* konstatna maximální délka řádku*/

/**
Deklarace funkcí
*/
int inRange(int row, int col);
char *readcell(char *str);
char *readline(char* line);
int min();
int selected();
int max();
int suma();
int avg();

/**
Kódy chyb programu
*/
enum tecodes
{
  ECLWRONG,    /** Chybný příkazový řádek. */
  EUNKNOWN,    /** Neznámá chyba */
  EALPHA,      /** Chyba při kontrole zda v bunce neni cislo*/
};

/**
Chybová hlášení odpovídající chybovým kódům ve výčtu tecodes.
*/
const char *ECODEMSG[] =
{
  "Chybné parametry příkazového řádku!\n",
  "Nastala nepředvídaná chyba! \n",
  "V zadaném rozsahu buněk se vyskytují znaky",
};

/**
Stavové kódy programu
*/
enum Estates
{
  SHELP,
  SMIN,
  SMAX,
  SSUM,
  SAVG,
  SELECTED,
};

enum Erange
{
  Row,
  Col,
  Rows,
  Cols,
  Range
};

/**
Konstanta HELPMSG, vypíše se při zavolání parametru --help
*/
const char *HELPMSG =
"PROGRAM: Tabulkovy kalkulator.\n"
"AUTOR: David Kolečkář\n"
"PROGRAM PROVÁDÍ: Tabulkový kalkulátor provádí funkce vyhledání maxima, minima,\n"
"funkce součtu a aritmetického průměru vybraných buněk.\n"
"POPIS PARAMETRŮ:\n"
"--help   -Vypise na obrazovku napovedu.\n"
"select   -vybere a následně vytiskne hodnoty daných buněk\n"
"min      -značí vyhledání a následný tisk minimální hodnoty \n"
"max      -značí vyhledání a následný tisk maximální hodnoty \n"
"sum      -značí výpočet a následný tisk sumy hodnot všech vybraných buněk,\n"
"avg      -značí výpočet a následný tisk aritmetického průměru vybraných buněk.\n"
"rozsah tabulky, nad kterým má být provedena daná operace.\n"
"row X    -značí výběr všech buněk na řádku X \n"
"col X    -značí výběr všech buněk ve sloupci X \n"
"rows X Y -značí výběr všech buněk od řádku X  až po Y \n"
"cols X Y -značí výběr všech buněk od sloupce X  až po Y \n"
"range A B X Y -značí výběr buněk od řádku A po řádek B a od sloupce X po sloupec Y\n";



/**
 * Struktura obsahující hodnoty parametrů příkazové řádky.
 */
typedef struct
{
  int A,B,X,Y;
  int ecode;         /** Chybový kód programu, odpovídá výčtu tecodes. */
  int state;         /** Stavový kód programu, odpovídá výčtu Estates. */
  int range;        /** Značí výběr buněk(row,col...), odpovídá výčtu Erange. */
} TParams;

/**
 * Inicializace struktury.
 */
TParams result =
{
  .X = 0, .Y=0 , .A=0, .B=0
};

/**
 * Zpracuje argumenty příkazového řádku a vrátí je ve struktuře TParams.
 * argc počet argumentů.
 * argv pole textových řetězců s argumenty.
 * return Vrací analyzované argumenty příkazového řádku.
 */
TParams getParams(int argc, char *argv[])
{
  if(strcmp("select", argv[1]) == 0){
    result.state = SELECTED; }

  if(strcmp("min", argv[1]) == 0){
    result.state = SMIN; }

  if(strcmp("max", argv[1]) == 0){
    result.state = SMAX;}

  if(strcmp("sum", argv[1]) == 0){
    result.state = SSUM; }

  if(strcmp("avg", argv[1]) == 0){
    result.state = SAVG; }

  if(argc == 2)
  {
    if(strcmp("--help", argv[1]) == 0){
    result.state = SHELP;}
  }

  if(argc == 4){
    if(strcmp("row", argv[2]) == 0){
        result.X=strtol(argv[3], NULL, 10);
        result.range=Row;
    }
    if(strcmp("col", argv[2]) == 0){
        result.X=strtol(argv[3], NULL, 10);
        result.range=Col;
    }
  }

  if(argc == 5){
    if(strcmp("cols", argv[2]) == 0){
      result.X=strtol(argv[3], NULL, 10);
      result.Y=strtol(argv[4], NULL, 10);
      result.range=Cols;
    }
    if(strcmp("rows", argv[2]) == 0){
      result.X=strtol(argv[3], NULL, 10);
      result.Y=strtol(argv[4], NULL, 10);
      result.range=Rows;
    }
  }

  if(argc == 7){
    if(strcmp("range", argv[2]) == 0){
      result.A=strtol(argv[3], NULL, 10);
      result.B=strtol(argv[4], NULL, 10);
      result.X=strtol(argv[5], NULL, 10);
      result.Y=strtol(argv[6], NULL, 10);
      result.range=Range;
    }
  }

return result;
}


/**
 * Hlavní program, probíhá zde zpracování struktury TParams.
 */
int main(int argc, char *argv[])
{
  TParams params = getParams(argc, argv);

  if (params.state == SHELP){
    printf("%s", HELPMSG);
    return EXIT_SUCCESS;
  }

  if (params.state == SELECTED){
    selected();
    return EXIT_SUCCESS;
  }

  if (params.state == SMIN){
    min();
    return EXIT_SUCCESS;
  }

  if (params.state == SMAX){
    max();
    return EXIT_SUCCESS;
  }

  if (params.state == SSUM){
    sum();
    return EXIT_SUCCESS;
  }

  if (params.state == SAVG){
    avg();
    return EXIT_SUCCESS;
  }

  return EXIT_SUCCESS;
}

/**
 * Funkce InRange, zpracovává výběr buněk
 */
int inRange(int row, int col)
{
    switch(result.range)
    {
        case Row:
          return result.X == row;
        case Col:
          return result.X == col;
        case Rows:
          return row >= result.X && row <= result.Y;
        case Cols:
          return col >= result.X && col <= result.Y;
        case Range:
          return row >= result.A && row <= result.B && col >= result.X && col <= result.Y;
        default:
          return 0;
    }
}
/**
 * Funkce readline, přečte jeden řádek ze standartního vstupu
 * return vrací jeden řádek, pokud dojde nakonec souboru vrací hodnotu NULL
 */
char *readline(char* line){
    int input, line_length = 0;

    while ((input = getchar()) != EOF && input != '\n' && line_length < MAX_LINE - 1)
    {
        line[line_length++] = input;
    }
    line[line_length] = 0;
    if(!line_length && input == EOF)
        return NULL;

    return line;
}
/**
 * Funkce readcell, pomocí funkce strtok přečte jednu bunku
 * return vrací jednu bunku z tabulky
 */
char *readcell(char *str){
    return  strtok(str, " ");
}

/**
 * Funkce selected
 * z dané tabulky pouze vybere a následně vytiskne hodnoty daných buněk
 * dále provádí kontrolu zda je bunka číslo pokud ano, vypíše se pomocí %.10g jako typ double
 */
int selected(){
    char line[MAX_LINE];
    char *cell;
    int row=1;
    int col=1;

    while(readline(line)){  //cyklus se opakuje dokud nepřečte všechny řádky

        for(cell = readcell(line); cell ; cell = readcell(NULL)){  //cyklus se opakuje dokud nepřečte všechny bunky z aktuální řádku
            if(inRange(row,col)){   //pokud je bunka v daném rozsahu - kontroluje to funkce inRange
              if(isalpha(cell[0]))  //zkontroluje se zda jsou v bunce jen znaky
                printf("%s\n", cell);   //pokud ano vypise se pomoci %s
              if(isdigit(cell[0])){     //zkontroluje zda bunka obsahuje cislo
                printf("%.10g\n",atof(cell));   //pokud ano vypise se pomoci %.10g jako typ double
              }
            col++;
            }
        }
    col=1;
    row++;
    }
    return 0;
}

/**
 * Funkce min
 * vyhledání a následný tisk minimální hodnoty z daného rozsahu buněk,
 */
int min(){
    char line[MAX_LINE];
    char *cell;
    int row=1;
    int col=1;
    double min=DBL_MAX;

    while(readline(line)){

       for(cell = readcell(line); cell ; cell = readcell(NULL)){

            if(inRange(row,col)){
              if (isdigit(cell[0])){
                if(atof(cell)<min)
                min=atof(cell);
              }
              else{
               fprintf(stderr, "%s\n", ECODEMSG[EALPHA]);
               return EXIT_FAILURE;
               }
            }
            col++;
            }
    col=1;
    row++;
    }
    printf("%.10g\n",min);
    return 0;
}

/**
 * Funkce max
 * vyhledání a následný tisk maximální hodnoty z daného rozsahu buněk,
 */
int max(){
    char line[MAX_LINE];
    char *cell;
    int row=1;
    int col=1;
    double max=DBL_MIN;

    while(readline(line)){

       for(cell = readcell(line); cell ; cell = readcell(NULL)){

            if(inRange(row,col)){
              if (isdigit(cell[0])){
                if(atof(cell)>max)
                max=atof(cell);
              }
              else{
                fprintf(stderr, "%s\n", ECODEMSG[EALPHA]);
                return EXIT_FAILURE;
              }
            }
            col++;
            }
    col=1;
    row++;
    }
    printf("%.10g\n",max);
    return 0;
}

/**
 * Funkce sum
 * značí výpočet a následný tisk sumy hodnot všech vybraných buněk
 */
int sum(){
    char line[MAX_LINE];
    char *cell;
    int row=1;
    int col=1;
    double suma=0;

    while(readline(line)){

       for(cell = readcell(line); cell ; cell = readcell(NULL)){

            if(inRange(row,col)){
              if (isdigit(cell[0]))
                suma+=atof(cell);

              else{
               fprintf(stderr, "%s\n", ECODEMSG[EALPHA]);
               return EXIT_FAILURE;
              }
            }
            col++;
            }
    col=1;
    row++;
    }
    printf("%.10g\n",suma);
    return 0;
}

/**
 * Funkce avg
 * značí výpočet a následný tisk aritmetického průměru vybraných buněk.
 */
int avg(){
    char line[MAX_LINE];
    char *cell;
    int row=1;
    int col=1;
    double deleno=0;        /* deklarace proměné deleno do které se ukláda počet hodnot*/
    double soucet=0;        /* deklarace proměné soucet do které se uloží suma všech hodnot*/

    while(readline(line)){

       for(cell = readcell(line); cell ; cell = readcell(NULL)){

            if(inRange(row,col)){
                if (isdigit(cell[0])){
                  soucet+=atof(cell);
                  deleno++;
                }
                else{
                  fprintf(stderr, "%s\n", ECODEMSG[EALPHA]);
                  return EXIT_FAILURE;
                }
            }
            col++;
            }
    col=1;
    row++;
    }
    printf("%.10g\n",soucet/deleno);
    return 0;
}
/* konec souboru proj1.c */
