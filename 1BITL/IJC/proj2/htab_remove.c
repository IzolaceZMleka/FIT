/*Projekt c.2 pro predmet IJC - Jazyk C
*Autor: Jan Pavlica - 1BIT, VUT FIT
*       xpavli78@stud.fit.vutbr.cz
*Datum: 27.4.2015
*Prelozeno: gcc 4.5.2
*Popis: odstraneni zaznamu z hashovaci tabulky
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"

extern unsigned int hash_function(const char *str, unsigned htab_size);

void htab_remove(htab_t * hash_table, const char *key)
{
  if (hash_table != NULL && key != NULL) //pokud je platna tabulka a klic
  {
    struct htab_listitem * a_item=NULL; //aktualni ukazatel
    struct htab_listitem * p_item=NULL; //predchozi ukazatel
  
    unsigned int list_index = hash_function(key, hash_table->htab_size); //zjisteni indexu v tabulce
    a_item = hash_table->list[list_index];
  
    while(a_item != NULL) //pokud nejsme na zacatku seznamu, prochazime jej
    {
      if (strcmp(a_item->key, key) == 0) //pokud je shoda klice
      {
	if (a_item == hash_table->list[list_index])//pokud je klic na zacatku
	{
	  hash_table->list[list_index] = a_item->next;//prepiseme aktualni prvek dalsim
	}
	else
	{
	  p_item->next = a_item->next; //jinak posuneme ukazatel na dalsi prvek ob jedno
	}
	free(a_item->key); //dealokace pole pro klic
	free(a_item); //dealokace struktury
      }
      else
      {
	p_item = a_item;
	a_item = a_item->next;  
      } 
    }
  }
  return;
}
