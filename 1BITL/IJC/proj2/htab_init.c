/*Projekt c.2 pro predmet IJC - Jazyk C
*Autor: Jan Pavlica - 1BIT, VUT FIT
*       xpavli78@stud.fit.vutbr.cz
*Datum: 27.4.2015
*Prelozeno: gcc 4.5.2
*Popis: incializacce hashovaci tabulky
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"

htab_t* htab_init(int htab_size)
{
  htab_t* allocated = NULL; //ukazatel pro novou tabulku 
  
  if (htab_size > 0) //pokud je zadana velikost tabulky
  {
   allocated = malloc(sizeof(htab_t)); //alokace struktury
  }

  if (allocated != NULL) //kontrola, zda probehla dobre
    {
      allocated->htab_size = htab_size; //zapis do velikosti do struktury
      allocated->list=calloc(htab_size, sizeof(struct htab_listitem *));//alokace pole
	for(int i = 0; i < htab_size; i++)
	{
	  allocated->list[i] = NULL; //vynulovani ukazatelu v poli
	}
    }
  
  return allocated;
}
