/*Projekt c.2 pro predmet IJC - Jazyk C
*Autor: Jan Pavlica - 1BIT, VUT FIT
*       xpavli78@stud.fit.vutbr.cz
*Datum: 27.4.2015
*Prelozeno: gcc 4.5.2
*Popis: volani funkce predane parametrem
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"

void htab_foreach(htab_t * hash_table, void (*funciton)(const char* key, const int value))
{
  if (hash_table != NULL) // pokud je platny ukazatel na tabulku
  {
    struct htab_listitem * a_item = NULL; //ukazatel na aktualni prvek
    for(int i = 0; i < hash_table->htab_size; i++)
    {
      a_item = hash_table->list[i]; //do ukazatele je prirazen prvek z pole
      
      while(a_item) //dokud je prvek ruzny od NULL
      {
	funciton(a_item->key, a_item->data); //je zavolana predana funkce s udaji
	a_item = a_item->next;//ukazatel se posune na dalsi prvek
      }
    }
  }
  else
    fprintf(stderr,"Nevalidní tabulka.\n");
  
  return;
}

