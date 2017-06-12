/*Projekt c.2 pro predmet IJC - Jazyk C
*Autor: Jan Pavlica - 1BIT, VUT FIT
*       xpavli78@stud.fit.vutbr.cz
*Datum: 27.4.2015
*Prelozeno: gcc 4.5.2
*Popis: Implementace POSIX prikazu tail v jazyce C99
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CHARMAX 513 // maximálni pocet nacitanych radku
#define LINES_DEFAULT 10 // zakladni pocet radku
#define OK 1
#define ERROR_WRONG_PARAMS 2
#define ERROR_MORE_CHARS 3
#define ERROR_WRONG_NUMBER 4
#define ERROR_WRONG_FILE_INPUT 5
#define ERROR_MORE_PARAMS 6

typedef struct Params //struktura pro uchování parametru
{
  int c_lines; //pocet radku
  FILE *input_file; //vstupni soubor
  bool from; // zda je prepinac -n
  int errors; // cislo erroru
} params_t;

params_t do_params(int argc, char **argv);// funkce pro zpracovani parametru
void tail(int c_line, FILE *input_file, bool from);
void error(int error_code); //fuknce pro tisk erroru


int main(int argc, char **argv)
{
  params_t my_params;
  my_params = do_params(argc, argv);
  if(my_params.errors != OK) // kontrola, zda se nevyskytnul problem
  {
    error(my_params.errors);
    return EXIT_FAILURE;
  }
  tail(my_params.c_lines, my_params.input_file, my_params.from);
  return EXIT_SUCCESS;
}

params_t do_params(int argc, char **argv)
{
  params_t my_params;
  
  my_params.c_lines = LINES_DEFAULT;
  my_params.from = false;
  my_params.errors = OK;
  my_params.input_file = NULL;
  
  if (argc > 4) // kontrola poctu parametru
  {
    my_params.errors = ERROR_MORE_PARAMS;
  }
  else if (argc == 2) // pokud jsou 2 parametry, tak se jedna o soubor a tisknout bude 10 radku
  {
    my_params.input_file = fopen(argv[1], "r");
    if (my_params.input_file == NULL) //kontrola zda se otevrel soubor
    {
      my_params.errors = ERROR_WRONG_FILE_INPUT;
    }
  }
  else if (argc == 1) // pokud je parametr jeden jedna se o cteni z stdin
  {
    my_params.input_file = stdin;
  }
  else if (strcmp(argv[1], "-n") == 0) //kontrola na prepinas -n
  {
    if (argc == 4) // pokud jsou 4 parametry jedna se o soubor, jinak se cte ze stdin
    {
      my_params.input_file = fopen(argv[3], "r");
      if (my_params.input_file == NULL)
      {
	my_params.errors = ERROR_WRONG_FILE_INPUT;
      }
    }
    else
    {
      my_params.input_file = stdin;
    }
    
    my_params.c_lines = atoi(argv[2]);
    
    
    if (my_params.c_lines <= 0)
    {
      my_params.errors = ERROR_WRONG_NUMBER;
    }
    
    if (argv[2][0] == '+') // pokud je u cisla +, tak se nastavi priznak pro cteni do konce souboru
    {
      my_params.from = true;
    }
  }
  else
  {
    my_params.errors = ERROR_WRONG_PARAMS;
  }
  return my_params;
}

void tail(int c_line, FILE *input_file, bool from)
{
  
  bool max = false; //priznak pro prekroceni maximalniho poctu znaku na radek pro tisk erroru
  bool eon = false; //priznak pri prekroceni poctu znaku na radek pro zahozeni zbytku
  int line_counter = 0; //pocitadlo radku
  int char_counter = 0; //pociradlo znaku
  int c; //promenna pro nacitany znak
  
  if(from == false)
  {
    int j; 
    char buffer [c_line][CHARMAX]; //pole pro ukladani znaku
    
    while((c = getc(input_file)) != EOF)//cteni, dokud nenarazi na EOF
    { 
      if(eon == false) //pokud se znak nema zahodit, tak je ulozen
      {
	buffer[line_counter%c_line][char_counter]=(char)c;//pole je cyklicky prepisovano
      }
      
      char_counter++;
      
      if(char_counter==CHARMAX-1) //pokud je prekrocen pocet znaku je zahlasen error
      {
	if (max == false)
	{
	  error(ERROR_MORE_CHARS);
	  max = true; //nastavi se priznak, aby se dale netisknul
	}
	
	buffer[line_counter%c_line][char_counter]='\n'; //do pole se prida nakonec znak noveho radku
	eon = true; //nastavi se priznak pro zahazovani znaku
      }
      
      if ((c == '\n' && eon == true) || c == '\n') //pokud se narazi na konec radku
      {
	line_counter++;
	char_counter = 0; //vynulovani pocitadla znaku
	eon = false; //vynulovani priznaku pro zahazovani
      }
    }
    if(line_counter%c_line==0)//zjisteni indexu pole od ktereho se ma zacit tisknout
      j = 0;
    else
      j = line_counter%c_line;
    
    if(line_counter<c_line)
      c_line=line_counter;
    
    for(int i=0; i<c_line; i++) //cyklus pro prochazeni radku
    {
      if (j == c_line)
      {
	j = 0;
      }
      
      for (int k=0; k<CHARMAX; k++) //cyklus pro tisk znaku
      {
	printf("%c",buffer[j][k]);
	if(buffer[j][k]=='\n')
	{
	  break;
	}
      }
      j++;
    }
  }
  else //pokud je tisk do konce souboru
  {
    line_counter = 1;
    while((c = getc(input_file)) != EOF)
    {
      char_counter++;
      if(line_counter >= c_line)
      {
	putchar(c);
      }
      if (c == '\n')
      {
	line_counter++;
	char_counter = 0;
      }
    }
  }
  
  
  return;
}

void error(int error_code) 
{ 
  switch(error_code) 
  { 
    case ERROR_WRONG_PARAMS: 
      fprintf(stderr, "\n\nChybne parametr prikazove radky! Program bude ukoncen. \n\n");  
      break; 
    case ERROR_MORE_CHARS: 
      fprintf(stderr, "\n\nNa radku je vice znaku nez je povoleno (512). Zbytek radku bude orezan a zahozen. \n\n"); 
      break; 
    case ERROR_WRONG_NUMBER: 
      fprintf(stderr, "\n\nChybne zadane cislo! Program bude ukoncen. \n\n"); 
      break; 
    case ERROR_WRONG_FILE_INPUT: 
      fprintf(stderr, "\n\nZdrojovy soubor nelze otevrit! Program bude ukoncen. \n\n"); 
      break; 
    case ERROR_MORE_PARAMS: 
      fprintf(stderr, "\n\nZadano vice parametru, nez je povoleno! Program bude ukoncen. \n\n"); 
      break; 
    default: 
      break; 
  } 
  return;
}
