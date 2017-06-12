/* ***************************************************************** */
/*
	projekt: Implementace interpretu imperativniho jazyka IFJ15
	autori: Jan Velecky, Adam Piecek, Tomas Zahradnicek
	xvelec07: Jan Velecky
	xzahra24: Tomas Zahradnicek
	xpiece00: Adam Piecek
	xpavli78: Jan Pavlica
*/
/* ***************************************************************** */
#include "ial.h"

/* IMPLEMENTACE RADICIHO ALGORITMU
 * autor: Jan Velecky
 * (dvoucestny) QUICK SORT
 * pracuje nad polozkami typu char
 */
void QuickSort(arr, left, right)
	char arr[];
	int left;  // index nejlevejsiho prvku mnoziny k serazeni v poli
	int right; // nejpravejsiho -||-
{
	int i = left, j = right;
	static int aux, pivot;
	
	pivot = arr[(left + right) / 2];
	
	do { // cyklus roztridi cisla z pole do dvou mnozin (vetsich a mensich cisel)
		while(arr[i] < pivot) i++;
		while(arr[j] > pivot) j--;
		
		if(i < j) // swap operace arr[i] <=> arr[j]
			aux = arr[i],
			arr[i] = arr[j],
			arr[j] = aux;
		else
			break; // ukazatele i a j se jiz potkaly, tj. i = j, nebo j + 1 = i
		
		i++; j--;
	} while(1);
	
	
	if(j > left) QuickSort(arr, left, j);
	if(j + 1 < right) QuickSort(arr, j + 1, right);
}

/* IMPLEMENTACE BOYER-MOOREOVA ALGORITMU
 * autor: Adam Piecek
 */

#include <limits.h>

#define min(X, Y) ((X) < (Y) ? (X) : (Y))
#define max(X, Y) ((X) > (Y) ? (X) : (Y))

void ComputeJump(const char P[], int len, int CharJump[])
{ // vypocet tabulky bad character table
	int i;
	for(i = 0; i <= CHAR_MAX; i++)
		CharJump[i] = len;
	// doplneni skoku pro "nejpravejsi" pismeno od konce
	for(i = 1; i <= len; i++)
		CharJump[(int)P[i-1]] = len - i;
	return;
}

void ComputeMatchJump(const char P[], int len, int MatchJump[])
{ // vypocet tabulky good suffix
	int k; // iterator
	int q; // zacatek suffixu
	int qq; 
	int Backup[len];

	for (k = 0; k < len; k ++) // nejvyssi mozny skok
		MatchJump[k] = 2*len - k + 1;
	k = len - 1;
	q = len;

	/* V Backup:
	 * Za poslednim pismenem vzorce je bran v potaz jeste prazdny znak epsilon. Vzorec cten od konce.
	 * Pokud je pismenko pri pruchodu zaroven pismenkem suffixu, tak se v nasledujicim pismenku,
	 * dosadi hodnota "index pismenka suffixu" + 1.
	 * Jinak se do Backup dosazuje hodnota delky retezce.
	*/
	while (k >= 0){
		Backup[k] = q + 1;
		while ((q < len) && (P[k] != P[q])){
			MatchJump[q] = min(MatchJump[q],len - (k+1));
			q = Backup[q]-1;
		}
		k = k-1;
		q = q-1;
	}

	// q ukazuje na zacatek suffixu

	for (k = 0; k < q; k++) // upravime skoky do pozice zacatku suffixu
		MatchJump[k] = min(MatchJump[k], len + (q+1) - (k+1));
	qq = Backup[q];
	while (q < (len-1)){ // doupravi se skoky
		while (q < qq){
			MatchJump[q] = min(MatchJump[q], (qq-1) - (q+1) + len);
			q = q+1;
		}
		qq = Backup[qq-1];
	}

	return;
}

int Bma(const char T[], int Tlen, const char P[], int Plen)
{
/* zadani:
 * Vyhleda prvni vyskyt zadaneho podretezce search v retezci s
 * a vrati jeho pozici (pocitano od nuly). Prazdny retezec se vyskytuje
 * v libovolnem retezci na pozici 0. Pokud podretezec neni nalezen,
 * je vracena hodnota -1.
*/
 
	int CharJump[CHAR_MAX+1]; // bad character table
	int MatchJump[Plen]; // good suffix table

	ComputeJump(P, Plen, CharJump); // dosazeni skoku do tabulky
	ComputeMatchJump(P, Plen, MatchJump); 

	int j = Plen-1; // index do textu
	int k = Plen-1; // index do vzoru
	char first; // prvni pismenko, ktere se po skoku porovnava

	while (j < Tlen){ // kontrola, zda uz jsme nedosli na konec textu
		if ((first = T[j]) != P[k]){ // otestujeme prvni pismenka v textu a patternu
			// muzeme rovnou pouzit hodnotu z CharJump, protoze prvni pismenko v ComputeMatch ma vzdy hodnotu 1
			j = j + (CharJump[(int)first]); 
			continue;
		}
		do { // prvni pismenka jsou stejne, kontrolujeme dal
			j = j-1;
			k = k-1;
		} while((k >= 0) && (T[j] == P[k])); // postupne porovnavame pismena

		if (k < 0) // pismenka se rovnaji = nasli jsme pattern v textu
			return j + 1;
		else{ // pismenka se nerovnaji, vypocitame skok v textu
			j = j + max(CharJump[(int)T[j]], MatchJump[k]);
			k = Plen-1; // prenastavime index do patternu
		}
	}
	return -1;	// pattern nenalezen
}

/* IMPLEMENTACE HASHOVACI TABULKY
 * autor: Tomas Zahradnicek
*/

#include <string.h>
#include <stdlib.h>

unsigned int hash_function(const char *str, unsigned htab_size)
/* mapovaci funkce hashovaci tabulky, vraci index do rozptyloveho pole, na kterem zacina seznam polozek.
 * parametr *str je klic (ukazatel na retezec).
 * parametr htab_size je velikost rozptyloveho pole hashovaci tabulky.
*/
{
    unsigned int h=0;
    const unsigned char *p;
    for(p=(const unsigned char*)str; *p!='\0'; p++)
        h = 65599*h + *p;
    return h % htab_size;
}

void htab_datainit(htab_data* data) { // funkce je zavolana pokazde kdyz je vytvorena nova polozka
	data->defined = 0;
	listParam_init(&data->listParam);
	data->strvars = NULL;
	return;
}

void htab_datadelete(htab_data* data) { // funkce je zavloana pred kazdym uvolnenim polozky
	listParam_dispose(&data->listParam);
	free(data->strvars);
	return;
}

htab_t* htab_init()
{
    htab_t* res;
    res = malloc(sizeof(htab_t));
    if (res == NULL)
        return NULL;
    for (int i = 0; i < HTABSIZE; i++)
        (*res)[i] = NULL;
    return res;
}

htab_data* htab_search(htab_t* t, const char* key)
{
    struct htab_listitem** p;
    p = &((*t)[hash_function(key, HTABSIZE)]);
    while (*p != NULL)
    {
        if (strcmp((*p)->key, key) == 0)
            return &((*p)->data); // byla nalezena polozka s odpovidajcim klicem
        p = &(*p)->next; // prechod na dalsi polozku v seznamu
    }
	return NULL; // polozka se v seznamu nenachazela
}
	
htab_data* htab_insert(htab_t* t, const char* key)
{
    struct htab_listitem** p;
    htab_data* ret;
	p = &((*t)[hash_function(key, HTABSIZE)]);
    while (*p != NULL)
    {
        if (strcmp((*p)->key, key) == 0)
            return &((*p)->data); // byla nalezena polozka s odpovidajcim klicem
        p = &(*p)->next; // prechod na dalsi polozku v seznamu
    }
	
	// polozka se v seznamu nenachazela
    *p = malloc(sizeof(struct htab_listitem)); // p ukazovala na ukazatel next polozky posledni v seznamu, tedy *p ukazuje na NULL
    if (*p != NULL)
    {
        (*p)->next = NULL;
        if (((*p)->key = malloc(strlen(key)+1)) != NULL) // +1 pro '\0'
            strcpy((*p)->key, key);
        else // pokud selhala alokace pameti pro zapis klice, musi se dealokovat cela nova polozka
        {
            free(*p);
            *p = NULL;
			return NULL;
        }
    }
	else
		return NULL;
	ret = &((*p)->data);
	htab_datainit(ret);
    return ret;
}

void htab_free(htab_t* t)
{
    struct htab_listitem *p, *tmp; // p je ukazatel na zbytek seznamu, tmp ukazatel na polozku ktera se zrusi
    for (int i = 0; i < HTABSIZE; i++) // pro vsechny polozky v rozptylovem poli
    {
        p = (*t)[i];
        while (p != NULL) // vsechny polozky do konce seznamu
        {
            tmp = p;
            p = p->next;
            free(tmp->key);
			htab_datadelete(&tmp->data);
            free(tmp);
        }
    }
	free(t);
    return;
}
