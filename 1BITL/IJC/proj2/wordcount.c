/*Projekt c.2 pro predmet IJC - Jazyk C
*Autor: Jan Pavlica - 1BIT, VUT FIT
*       xpavli78@stud.fit.vutbr.cz
*Datum: 27.4.2015
*Prelozeno: gcc 4.5.2
*Popis: funkce pro cteni a pocitani slov ze vstupu
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"
#include "io.h"

#define MAX_WORD 127 //maximalni pocet nacitanych znaku
#define TAB_SIZE 12289 //http://planetmath.org/goodhashtableprimes

void print(const char *key, int counted);


int main(void)
{
  htab_t *new_hash; //ukazatel na novou tabulku
  new_hash = htab_init(TAB_SIZE); //inicializace
    
    
  if (new_hash == NULL) //kontrola uspecnosti alokace
  {
    fprintf(stderr, "Chyba pri alokaci pameti pro tabulku.\n");
    return EXIT_FAILURE;
  }
  
  
  char get_word[MAX_WORD+1] = {'\0'};	//buffer pro nacitana slova
  
  while (fgetw(get_word, MAX_WORD, stdin) != EOF) //nacitani slov ze vstupu
  {
    if (htab_lookup(new_hash, get_word) == NULL) //vyhledavani 
    {
      fprintf(stderr, "Chyba pri alokaci pameti pro zaznam.\n");
      htab_free(new_hash);
      return EXIT_FAILURE;
    }
  }
  
    htab_foreach(new_hash, print);
    htab_statistics(new_hash);
    htab_free(new_hash);

    return EXIT_SUCCESS;
}

void print(const char *key, int counted)
{
    printf("%s\t%u\n", key, counted);
}
