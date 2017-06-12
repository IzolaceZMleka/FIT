
/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                      Úpravy: Radek Hranický, říjen 2014
**                      Úpravy: Radek Hranický, listopad 2015
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit ( tHTable* ptrht ) {
	
	if((*ptrht) == NULL) // v připade, ze tabulka neni
		return;
	
	for(int i = 0; i < HTSIZE; i++) // jinak pro kazdy prvek z tabulky
	{
		((*ptrht)[i]) = NULL; // vyNULLuji
	}
}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není, 
** vrací se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {
	
	if((*ptrht) == NULL) // v pripade, ze tabulka neni
		return NULL;
	
	int hash = hashCode(key); // zjistim hash podle klice
	
	tHTItem *tmp = (*ptrht)[hash]; // prvni polozka podle hashe
	
	if(tmp == NULL) // pokud neni vracim NULL
		return NULL;
	else
	{
		while ((tmp) != NULL) // porovnavam dokud neprojdu vsechny prvky
		{
			if(tmp->key == key) // v pripade shody vracim ukazatel na polozku
				return tmp;
			else
				tmp = tmp->ptrnext; // jinak posunu na dalsi polozku
		}
		return NULL; // pokud nenaleznu vracim NULL
	}
}

/* 
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data ) {
		
	if((*ptrht) == NULL)
		return;
	
	int hash = hashCode(key); // zjistim hash
	
	tHTItem *tmp = htSearch(ptrht, key); // pomocna promenna, zjistim jestli neni v tabulkce
	
	if(tmp == NULL) // pokud polozka nebyla nalezena
	{
		tHTItem * new = malloc(sizeof(struct tHTItem)); // alokuji novou polozku
		
		if(new == NULL) // pri neuspesne alokaci
		{
			return;
		}
		else // ulozim data do nove polozky a zaclenim ji do tabulky
		{
			new->key = key;
			new->data = data;
			new->ptrnext = (*ptrht)[hash];
			(*ptrht)[hash] = new;
		}
	}
	else // ulozim data do vyhledane polozky
	{
		tmp->data = data;
	}
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key ) {
	
	if((*ptrht) == NULL)
		return NULL;
	
	tHTItem *tmp = htSearch(ptrht, key); // pomocna promenna, zjistim jestli neni v tabulce
	
	if(tmp == NULL) // polozka nenalezena
		return NULL;
	else
	{
		return &((*tmp).data); // vracim data
	}
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key ) {
	
	if((*ptrht) == NULL)
		return;
	
	int hash = hashCode(key);
	
	tHTItem * tmp1 = (*ptrht)[hash];
	tHTItem * tmp2 = NULL;
	
	while(tmp1 != NULL)
	{
		if(tmp1->key == key)
		{
			if(tmp1 == (*ptrht)[hash]) // pokud je prvni v tabulce
				(*ptrht)[hash] = tmp1->ptrnext;
			else
				tmp2->ptrnext = tmp1->ptrnext;
			
			free(tmp1);
			return;
		}
		
		tmp2 = tmp1;
		tmp1 = tmp1->ptrnext;
		
	}
}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll ( tHTable* ptrht ) {
	
	if((*ptrht) == NULL)
		return;
	
	tHTItem * tmp1 = NULL;
	tHTItem * tmp2 = NULL;
	
	for(int i = 0; i < HTSIZE; i++)
	{
		tmp1 = (*ptrht)[i];
		
		while(tmp1 != NULL)
		{
			tmp2 = tmp1->ptrnext;
			free(tmp1);
			tmp1 = tmp2;
		}
		
		(*ptrht)[i] = NULL;
	}
}
