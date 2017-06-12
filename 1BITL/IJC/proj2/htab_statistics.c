/*Projekt c.2 pro predmet IJC - Jazyk C
*Autor: Jan Pavlica - 1BIT, VUT FIT
*       xpavli78@stud.fit.vutbr.cz
*Datum: 27.4.2015
*Prelozeno: gcc 4.5.2
*Popis: statisticka funkce pro hasovaci tabulku
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "htable.h"

void htab_statistics(htab_t *hash_table)
{
  if(hash_table == NULL)
  {
    fprintf(stderr,"Nevalidni tabulka.");
    return;
  }

  int max = INT_MIN; //promenna pro uchovani maxima
  int min = INT_MAX; //promenna pro uchovani minima
  double sum = 0; //promenna pro uchovani souctu
  int entries_counter = 0; //pocitani poctu prvku 
  int list_counter = 0; //pocitani seznamu
  
  struct htab_listitem * a_item=NULL; //ukazatel na aktualni prvek
  
  for(int i = 0; i < hash_table->htab_size; i++) //projdeme celou tabulku
    {
      a_item = hash_table->list[i]; //ukazatel na prvni prvek v seznamu
      if(a_item == NULL) //pokud je seznam prazdy nepocitame jej
      {
	continue;
      }
   
      
      while(a_item) //dokud neni ukazatel NULL
      {
	a_item = a_item->next; //odkazujeme na dalsi prvek
	entries_counter++; //zvysujeme pocitadlo prvku
      }
      list_counter++;
      
      sum += entries_counter;
      
      if(max < entries_counter)
	max = entries_counter;
      if(min > entries_counter)
	min = entries_counter;
      
      entries_counter=0;
    }
    
    printf("V tabulce je celkem %d záznamů.\nMinimum záznamů je %d.\nMaximum záznamů je %d.\nPrůměrný počet záznamů je %f.\n",list_counter,min,max,sum/list_counter);
}