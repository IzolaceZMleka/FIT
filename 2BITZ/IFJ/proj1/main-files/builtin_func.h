/* ***************************************************************** */
/*
	projekt: Implementace interpretu imperativniho jazyka IFJ15
	autori: Jan Velecky, Adam Piecek
	xvelec07: Jan Velecky
	xzahra24: Tomas Zahradnicek
	xpiece00: Adam Piecek
	xpavli78: Jan Pavlica
*/
/* ***************************************************************** */
#include <stdlib.h>

/* VESTAVENE DATOVE TYPY JAZYKU */

/* Datovy typ string (Pascalovsky typ retezce /navic/ ukonceny NULL bajtem)
 */
typedef struct {
	size_t len; // delka ulozeneho retezce bez ukoncovaciho znaku (NULL bajtu)
	unsigned refs; // pocet odkazujicich promennych na tento retezec (0 je specialni hodnota a znaci retezec literal)
	char str[];
} Tstring;

#define STR_TO_DELETE 1 // pocet odkazu pri dekrementaci, pri kolika uz na behovy retezec nic neukazuje
#ifdef RUNTIME_ENV
	#define STR_INIT_VAL 1 // retezec vytvoreny za behu
#else
	#define STR_INIT_VAL 0 // retezec literal
#endif
// "Makro" pro alokaci retezce o delce 'len' vcetne inicializace delky na hodnotu 'len'.
#define allocString(len) allocString(len, STR_INIT_VAL)
static inline Tstring * (allocString)(const size_t len, unsigned type) {
	Tstring * str = malloc(sizeof(Tstring) + len + 1); // velikost struktury + pocet znaku + ukoncujici znak
	if(str != NULL)
		str->len = len,
		str->refs = type; // pokud se jedna o retezec vytvoreny za behu, vytvarejici nastavi na hodnotu 1
	return str;
}



/* VESTAVENE FUNKCE JAZYKU */

/* int length(string s)
 * Vrati delku zadaneho retezce.
 */
static inline int length(const Tstring * const string){
	return string->len;
}

/* string sort(string s)
 * Seradi znaky retezce 's' podle jejich ordinalnich hodnot (tj. retezec je inrepretovan jako pole znaku) a vraci odkaz na novy serazeny retezec.
 * V nasem zadani se vyuziva metoda quick sort.
 */
Tstring * sort(const Tstring * s) ;

/* int find(string text, string pattern);
 * obalka nad booyer-moorovym algoritmem
 * najde prvni vyskyt patternu v textu. Pri nenalezeni nebo v pripade, ze je delka textu mensi nez delka patternu, vraci -1
 * pokud je pattern prazdny retezec, vraci se 0
*/
int find(const Tstring * const text, const Tstring * const pattern);

/* string substr(string text, int i, int n)
 * Vrati podretezec zadaneho retezce text. Druhym parametrem i je dan zacatek pozadovaneho podretezce (pocitano od nuly) a treti 
 * parametr n urcuje delku podretezce. 
 * Pokud je n mimo interval <0;length(text)>, prepocita se n na length(text)
 * Pokud je n rovno 0, vraci se prazdny retezec
 * Pokud je i mimo interval <0;length(text)>, zpusobi  chybu
 * Pokud je i rovno length(text), vraci se prazdny retezec
*/
Tstring * substr(const Tstring * const string, int i, int n);

/* string concat(string s1, string s2)
*  Vrati retezec, ktery je konkatenaci retezce s1 a retezce s2.
*/
Tstring * concat(const Tstring * const string1, const Tstring * const string2);
