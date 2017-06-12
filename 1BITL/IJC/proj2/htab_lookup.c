/*Projekt c.2 pro predmet IJC - Jazyk C
*Autor: Jan Pavlica - 1BIT, VUT FIT
*       xpavli78@stud.fit.vutbr.cz
*Datum: 27.4.2015
*Prelozeno: gcc 4.5.2
*Popis: vyhledavani zaznamu v hashovaci tabulce
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"

extern unsigned int hash_function(const char *str, unsigned htab_size);

struct htab_listitem *htab_lookup(htab_t *hash_table, const char *key)
{
  if (hash_table == NULL || key == NULL) //pokud je zadana neplatna tabulka nebo ukazetel na zacatek klice
  {
    return NULL;
  }
  unsigned int hash_index = hash_function(key, hash_table->htab_size); //zjisteni indexu do tabulky

  struct htab_listitem *a_item = hash_table->list[hash_index]; //aktualni polozka
  struct htab_listitem *p_item = NULL; //nasledujici polozka

  while (a_item != NULL) //dokud nenarazime na NULL v seznamu
  {
    if (strcmp(a_item->key, key) == 0)//pokud se retezec rovna polozce v tabulce
    {
      a_item->data++;
      return a_item;
    }
    p_item = a_item;
    a_item = a_item->next; 
  }
  
  //pokud ne musime vytvorit novy
  struct htab_listitem *n_item = malloc(sizeof(struct htab_listitem)); //alokace pro novou polozku
  
  if (n_item == NULL) //kontrola zda se povedla
    return NULL;

  n_item->key = malloc((strlen(key) + 1) * sizeof(char)); //alokace pro ulozeni klice
  
  if (n_item->key == NULL) //kontrola zda se alokace povedla
  {
    return NULL;
  }
  
  (void) strcpy(n_item->key, key); //prekopirovani klice, do zaznamu
  n_item->data = 1;
  n_item->next = NULL; //vynulovani nasledujiciho ukazatele, jelikoz jsme na konci

  if (p_item != NULL) //pokud nejsme na zacatku seznamu, ulozi na dalsi pozici
    p_item->next = n_item;
  else
    hash_table->list[hash_index] = n_item; //jinak ulozime na zacatek seznamu

    return n_item;
}