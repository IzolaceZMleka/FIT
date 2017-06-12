/*Projekt c.2 pro predmet IJC - Jazyk C
*Autor: Jan Pavlica - 1BIT, VUT FIT
*       xpavli78@stud.fit.vutbr.cz
*Datum: 27.4.2015
*Prelozeno: gcc 4.5.2
*Popis: dealokace hashovaci tabulky
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"

void htab_free(htab_t * hash_table)
{
  htab_clear(hash_table);//vycisteni tabulky
  free(hash_table->list);//dealokace pole
  free(hash_table);//dealokace struktury
  hash_table = NULL;//zahozeni ukazatele
  return;
}
