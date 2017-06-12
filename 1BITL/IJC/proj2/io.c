/*Projekt c.2 pro predmet IJC - Jazyk C
*Autor: Jan Pavlica - 1BIT, VUT FIT
*       xpavli78@stud.fit.vutbr.cz
*Datum: 27.4.2015
*Prelozeno: gcc 4.5.2
*Popis: funkce pro cteni slov ze vstupu
*/

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_CHARS 128 //maximalni pocet nacitanych znaku

int fgetw(char *s, int max, FILE *f)
{
  
  int c; //pro ulozeni znaku
  bool over = false; //priznak pri prekroceni maxima znaku
  int i; //pocitadlo
  
  while ((c=fgetc(f)) != EOF) //nacitani znaku dokud nenarazi na EOF
  {
    if (isspace(c))//pokud je bily znak zahodime jej
      continue;
    else
    {
      i = 0;
      s[i]=c; //ulozime prvni znak do pole
      for (i = 1; isspace(c=fgetc(f))==0; i++)//dokud nenacteme bily znak
      {
	if (i == max) //pri prekroceni maxima znaku nastavime priznak a ukoncime cyklus
	{
	  over = true;
	  break;
	}
	s[i]=c; //ulozime nacteny znak na aktualni pozici
      }
      s[i]='\0'; //pridame ukoncujici znak
      break; //mame slovo, muzeme ukoncit cyklus
    }
    
  }
  if(over == true) //pri priznaku tisk erroru
    fprintf(stderr, "Nacteno vice znaku nez maximum (127), slovo bude orezano.");
  
  if (c == EOF) //pri nacteni EOFu vracime EOF
    return EOF;
  
  return i; //jinak vracime pocet nactenych znaku
}