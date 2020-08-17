
/* c201.c *********************************************************************}
{* Téma: Jednosmìrný lineární seznam
**
**                     Návrh a referenèní implementace: Petr Pøikryl, øíjen 1994
**                                          Úpravy: Andrea Nìmcová listopad 1996
**                                                   Petr Pøikryl, listopad 1997
**                                Pøepracované zadání: Petr Pøikryl, bøezen 1998
**                                  Pøepis do jazyka C: Martin Tuèek, øíjen 2004
**	                                      Úpravy: Bohuslav Køena, øíjen 2015
**
** Implementujte abstraktní datový typ jednosmìrný lineární seznam.
** Uµiteèným obsahem prvku seznamu je celé èíslo typu int.
** Seznam bude jako datová abstrakce reprezentován promìnnou typu tList.
** Definici konstant a typù naleznete v hlavièkovém souboru c201.h.
** 
** Va±ím úkolem je implementovat následující operace, které spolu s vý±e
** uvedenou datovou èástí abstrakce tvoøí abstraktní datový typ tList:
**
**      InitList ...... inicializace seznamu pøed prvním pouµitím,
**      DisposeList ... zru±ení v±ech prvkù seznamu,
**      InsertFirst ... vloµení prvku na zaèátek seznamu,
**      First ......... nastavení aktivity na první prvek,
**      CopyFirst ..... vrací hodnotu prvního prvku,
**      DeleteFirst ... zru±í první prvek seznamu,
**      PostDelete .... ru±í prvek za aktivním prvkem,
**      PostInsert .... vloµí nový prvek za aktivní prvek seznamu,
**      Copy .......... vrací hodnotu aktivního prvku,
**      Actualize ..... pøepí±e obsah aktivního prvku novou hodnotou,
**      Succ .......... posune aktivitu na dal±í prvek seznamu,
**      Active ........ zji±Ÿuje aktivitu seznamu.
**
** Pøi implementaci funkcí nevolejte µádnou z funkcí implementovaných v rámci
** tohoto pøíkladu, není-li u dané funkce explicitnì uvedeno nìco jiného.
**
** Nemusíte o±etøovat situaci, kdy místo legálního ukazatele na seznam 
** pøedá nìkdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodnì komentujte!
**
** Terminologická poznámka: Jazyk C nepouµívá pojem procedura.
** Proto zde pouµíváme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c201.h"

int solved;
int errflg;

void Error() {
/*
** Vytiskne upozornìní na to, µe do±lo k chybì.
** Tato funkce bude volána z nìkterých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;                      /* globální promìnná -- pøíznak chyby */
}

void InitList (tList *L) {
/*
** Provede inicializaci seznamu L pøed jeho prvním pouµitím (tzn. µádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádìt nad jiµ inicializovaným
** seznamem, a proto tuto moµnost neo±etøujte. Vµdy pøedpokládejte,
** µe neinicializované promìnné mají nedefinovanou hodnotu.
**/
	
L->Act = NULL;
L->First = NULL;	
}

void DisposeList (tList *L) {
/*
** Zru±í v±echny prvky seznamu L a uvede seznam L do stavu, v jakém se nacházel
** po inicializaci. Ve±kerá pamìŸ pouµívaná prvky seznamu L bude korektnì
** uvolnìna voláním operace free.
***/
	
while(L->First != NULL) {
    tElemPtr item = L->First;
    L->First = L->First->ptr;
    free(item);
}
L->Act = NULL;
}

void InsertFirst (tList *L, int val) {
/*
** Vloµí prvek s hodnotou val na zaèátek seznamu L.
** V pøípadì, µe není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci Error().
**/
    
tElemPtr new_item = NULL;
new_item = malloc(sizeof(struct tElem));
if(new_item == NULL)
	Error();
else {
	new_item->data = val;
	new_item->ptr = L->First;
	L->First = new_item;
}
}

void First (tList *L) {
/*
** Nastaví aktivitu seznamu L na jeho první prvek.
** Funkci implementujte jako jediný pøíkaz, aniµ byste testovali,
** zda je seznam L prázdný.
**/
	
L->Act = L->First;
}

void CopyFirst (tList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci Error().
**/
	
if(L->First == NULL)
	Error();
else
	*val = L->First->data;   	
}

void DeleteFirst (tList *L) {
/*
** Zru±í první prvek seznamu L a uvolní jím pouµívanou pamìŸ.
** Pokud byl ru±ený prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se nedìje.
**/
	
if(L->First != NULL){
	tElemPtr item = L->First;
	if(item == L->Act)
		L->Act = NULL;
	L->First = L->First->ptr;
	free(item);
}
}	

void PostDelete (tList *L) {
/* 
** Zru±í prvek seznamu L za aktivním prvkem a uvolní jím pouµívanou pamìŸ.
** Pokud není seznam L aktivní nebo pokud je aktivní poslední prvek seznamu L,
** nic se nedìje.
**/
	
if ((L->Act != NULL) && (L->Act->ptr != NULL)) {
	tElemPtr item = L->Act->ptr;
	L->Act->ptr = L->Act->ptr->ptr;
	free(item);
}		
}

void PostInsert (tList *L, int val) {
/*
** Vloµí prvek s hodnotou val za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se nedìje!
** V pøípadì, µe není dostatek pamìti pro nový prvek pøi operaci malloc,
** zavolá funkci Error().
**/
	
if (L->Act == NULL) {
	return; 
}
tElemPtr item; 
if ((item = malloc(sizeof(struct tElem))) == NULL ) { 
	Error(); 
	return;
}
item->data = val; 
item->ptr = L->Act->ptr; 
L->Act->ptr = item; 	
}

void Copy (tList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam není aktivní, zavolá funkci Error().
**/
	
if(L->Act == NULL)
    Error();
else
    *val = L->Act->data;	
}

void Actualize (tList *L, int val) {
/*
** Pøepí±e data aktivního prvku seznamu L hodnotou val.
** Pokud seznam L není aktivní, nedìlá nic!
**/
	
if(L->Act != NULL)
    L->Act->data = val;	
}

void Succ (tList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** V±imnìte si, µe touto operací se mùµe aktivní seznam stát neaktivním.
** Pokud není pøedaný seznam L aktivní, nedìlá funkce nic.
**/
	
 if (L->Act != NULL){
    if (L->Act->ptr == NULL)
    	L->Act = NULL;
    else
    	L->Act = L->Act->ptr;
  
}	
}

int Active (tList *L) {		
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Tuto funkci je vhodné implementovat jedním pøíkazem return. 
**/
	
return (L->Act != NULL ? TRUE : FALSE);	
}

/* Konec c201.c */
