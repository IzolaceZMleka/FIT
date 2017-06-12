/*Projekt c.2 pro predmet IJC - Jazyk C
*Autor: Jan Pavlica - 1BIT, VUT FIT
*       xpavli78@stud.fit.vutbr.cz
*Datum: 27.4.2015
*Prelozeno: gcc 4.5.2
*Popis: vycisteni hashovaci tabulky
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"

void htab_clear(htab_t *hash_table)
{
  if (hash_table == NULL) //pokud je ukazatel NULL, neni co cistit
    return;
  else
  {
    struct htab_listitem *a_item = NULL; //ukazatel na aktualni prvek, ktery se maze
    struct htab_listitem *n_item = NULL; //ukazatel na prvek, ktery ma byt dalsi v rade
    
    for(int i = 0; i < hash_table->htab_size; i++)
    {
      a_item = hash_table->list[i]; //ukazatel na prvni prvek v seznamu
      if(a_item == NULL) //pokud je prazdy, neni co cistit
	continue;
      
      while(a_item) //dokud neni ukazatel NULL
      {
	n_item = a_item->next; //zachovani ukazatele na dalsi prvek
	free(a_item->key); //dealokace pole pro klic
	free(a_item); //dealokace struktury
	a_item = n_item;
      }
      hash_table->list[i] = NULL; //zahozeni ukazatele
    }
  }
}

