/*
 * Soubor:  proj1.c
 * Datum:   15. 11. 2014
 * Autor:   Jan Pavlica, xpavli78@stud.fit.vutbr.cz
 * Projekt: IZP c. 1 - Vypocty v tabulce
 * Popis:   Program na tisk, soucet, prumer, vyber maxima a minima v danem rozsahu
 */

#include <stdio.h> /* vstup a vystup */ 
#include <stdlib.h> /* obecne funkce */ 
#include <ctype.h> /* funkce pro testovani znaku - isdigit, isprint atd. */ 
#include <string.h> /* funkce strcmp */ 
#include <float.h> /* minima a maxima */


#define NOK_PARAMETR_1 1 /* chybny 1. parametr prikazove radky */ 
#define NOK_PARAMETR_2 2 /* chybny 2. parametr prikazove radky */ 
#define NOK_ROZSAH 3 /* chybny rozsah */
#define NOK_POCET_ZNAKU 4 /* chybny pocet znaku */
#define NOK_VSTUPNI_DATA 5 /* spatna vstupni data */
#define NOK_POCET_PARAMETRU 6 /* chybny pocet paramteru */


void solution (int row_start, int row_end, int col_start, int col_end, int w); /* funkce pro tisk */
void print_help (void); /* funkce pro napovedu */
void print_error (int kod); /* funkce pro tisk erroru */
void range (int argc, char *argv[], int *row_start, int *row_end, int *col_start, int *col_end); /*funkce pro urceni rozsahu*/
void operation (char *argv[], int row_start, int row_end, int col_start, int col_end); /*funkce provadejici jednotlive operace*/


int main(int argc, char *argv[])
{
  int row_start, row_end, col_start, col_end; /* promenne pro ulozeni rozsahu */
  
  if (strcmp("--help",argv[1])==0 && argc == 2) /* spusteni help */
    print_help();
  
  if (argc<3 || argc >7) /*kontrola poctu argumentu */
    print_error(NOK_POCET_PARAMETRU);
  
  /* porovnani druheho argumentu a urceni rozsahu */ 
  
  range(argc, argv, &row_start, &row_end, &col_start, &col_end);

  /*kontrola rozsahu*/

  if((row_start > row_end || col_start > col_end) && ((col_end && row_end) !=0))
    print_error(NOK_ROZSAH);

/* porovnani prvniho argumentu a zavolani funkce */
  operation(argv, row_start, row_end, col_start, col_end);

  return EXIT_SUCCESS;
}

/*zpracovani prvniho argumentu*/

void operation (char *argv[], int row_start, int row_end, int col_start, int col_end)
{
  if (strcmp("select",argv[1])==0) 
    solution(row_start,row_end,col_start,col_end,4);
  else if (strcmp("min",argv[1])==0) 
    solution(row_start,row_end,col_start,col_end,1);
  else if (strcmp("max",argv[1])==0) 
    solution(row_start,row_end,col_start,col_end,0);
  else if (strcmp("avg",argv[1])==0) 
    solution(row_start,row_end,col_start,col_end,3);
  else if (strcmp("sum",argv[1])==0) 
    solution(row_start,row_end,col_start,col_end,2);
  else print_error(NOK_PARAMETR_1);
}

/* zpracovani rozsahu */

void range(int argc, char *argv[], int *row_start, int *row_end, int *col_start, int *col_end)
{
  if ((strcmp("row",argv[2])==0) && argc==4 && atoi(argv[3])>0)
  {
    *row_start = *row_end = atoi(argv[3]);	
    *col_start = 1;
    *col_end = 0; 
  } 
  else if ((strcmp("col",argv[2])==0) && argc==4 && atoi(argv[3])>0)
  {
    *col_start = *col_end = atoi(argv[3]);
    *row_start = 1;
    *row_end = 0;
  } 
  else if ((strcmp("rows",argv[2])==0) && argc==5 && atoi(argv[3])>0 && atoi(argv[4])>0)
  {
    *row_start = atoi(argv[3]);
    *row_end = atoi(argv[4]);
    *col_start = 1;
    *col_end = 0;
  } 
  else if ((strcmp("cols",argv[2])==0) && argc==5 && atoi(argv[3])>0 && atoi(argv[4])>0)
  {
    *col_start = atoi(argv[3]);
    *col_end = atoi(argv[4]);
    *row_start = 1;
    *row_end = 0;
  } 
  else if ((strcmp("range",argv[2])==0) && argc==7 && atoi(argv[3])>0 && atoi(argv[4])>0 
    && atoi(argv[5])>0 && atoi(argv[6])>0)
  {
    *row_start = atoi(argv[3]);
    *row_end = atoi(argv[4]);
    *col_start = atoi(argv[5]);
    *col_end = atoi(argv[6]);
  }
  else print_error(NOK_PARAMETR_2);
}

void solution(int row_start, int row_end, int col_start, int col_end, int w)
{
  int row_counter = 1; //pocitadlo radku
  int col_counter = 1; //pocitadlo sloupcu
  int number_counter = 0; // pocitadlo pouzitych cisel
  int char_counter = 1; //pocitadlo znaku
  
  double number = 0.00; //promenna pro ulozeni cisla
  double sum = 0.00; //promenna pro ulozeni souctu)
  
  int previous = getchar(); //predchozi znak
  int aktual; //aktualni znak
  
  int alpha_digit = 0; //priznak urcujici text/cislo
  int sign = 1; //priznak pro zaporne cislo
  
  double check1 = 0.00; //kontrola vstupnich dat
  double check2 = 0.00; //kontrola vstupnich dat
  
  double max = DBL_MIN; //minimalni hodnota pro srovani
  double min = DBL_MAX; //maximalni hodnota pro srovani
  
  /* cteni znaku */
  
  while((aktual = getchar()) != EOF){
    
    char_counter++;
    
    if (char_counter > 1023) /*kontrola pro prekroceni 1024 znaku na radku*/
      print_error(NOK_POCET_ZNAKU);
    
     /* kontroluje zda jde o cislo nebo text */
    if ((isalpha(previous) || ispunct(previous)) && previous != '-')
      alpha_digit = 1;
    
    if((isdigit(previous) && (isspace(aktual) || aktual == EOF)) && alpha_digit != 1)
      check1 += (1*col_counter);
    
    if(alpha_digit == 1 && previous != '\n' && isspace(aktual))
      check1 += (512*col_counter);
    
    
    if(((row_start <= row_counter) 
      && ((row_counter <= row_end) || (row_end == 0))) 
      && ((col_start <= col_counter) 
      && ((col_counter <= col_end) || (col_end == 0)))) /* podminka urcujici rozsah */
    {
      if(alpha_digit == 1 && previous != '\n') /* tisk textovych udaju */
      {
	{
        if(w==4)
	  putchar(previous);
	}
	
        if(isspace(aktual)) /* oddentrovani celeho textoveho udaje */
	{
	  if(w==4)
	    printf("\n");
	}
      }
      
      if (previous == '-') /* priznak pro zaporne cislo */
	sign = -1;
	    
      if(isdigit(previous)) /* tvoreni ciselneho udaje */
        number = (number*10)+(previous-48);
	
      /* zpracovani ciselnych udaju */
      if((isdigit(previous) && (isspace(aktual) || aktual == EOF)) && alpha_digit != 1) 
      {
	if((number*sign) > max) /* urcuje maximum */
	  max = number*sign;
	
	if((number*sign) < min) /*urcuje minimum */
	  min = number*sign;
	
	if(w==4)
	  printf("%.10g\n",number*sign);
	
	sum = sum+(number*sign);
	number = 0; /* vynuluje cislo */
	sign = 1; /* vynuluje priznak pro zaporne cislo */
	number_counter++;
	
      }
    }
    
    /*pocita pocet sloupcu */
    if((isalpha(previous) || isdigit(previous) || ispunct(previous)) && isspace(aktual))
    {
      col_counter++;
      alpha_digit = 0; /* vynulovani priznaku pro textovy udaj */    
    } 
    
    if (aktual == '\n' && aktual != EOF){/* pocita pocet radku */
  
/* pokud nesedi cisla nesedi format v jednotlivych sloupcich vyhodnoti error */

    if(row_counter == 1)check2 = check1;
      row_counter++;
    
    if(check2 != check1 && ((col_end >= (col_counter-1)) || (row_end >= (row_counter-1))))
      print_error(NOK_VSTUPNI_DATA);
      
    
      check2 = check1;
      check1 = 0;
      
    
      if (col_end > (col_counter-1)) /* kontrola jestli nebyl zadan vetsi rozsah nez je pocet sloupcu */
	print_error(NOK_ROZSAH);
      
      col_counter = 1; /* vynuluje pocitadlo sloupcu */
      char_counter = 1; /* vynuluje pocitani znaku na radku */
      
    }
    
    previous = aktual;
    
  }
  
  if (row_end > (row_counter-1)) /* kontrola jestli nebyl zadan vetsi rozsah nez je pocet radku */
    print_error(NOK_ROZSAH);
  
   if (w == 1 && aktual == EOF)
      printf("%.10g\n", min);
    if (w == 0 && aktual == EOF)
      printf("%.10g\n", max);
    if (w == 2 && aktual == EOF)
      printf("%.10g\n", sum);
    if (w == 3 && aktual == EOF)
      printf("%.10g\n", sum/number_counter);
}

/*funkce pro tisk erroru*/

void print_error(int kod) 
{ 
  switch(kod) 
  { 
    case 1: 
      fprintf(stderr, "\nChybny 1. parametr prikazove radky! Program bude ukoncen.\n");
      exit(1);
      break;
    case 2: 
      fprintf(stderr, "\nChybny 2. parametr prikazove radky! Program bude ukoncen.\n");
      exit(1);
      break; 
    case 3: 
      fprintf(stderr, "\nChybne zadane parametry rozsahu! Program bude ukoncen.\n");
      exit(1);
      break; 
    case 4: 
      fprintf(stderr, "\nPocet znaku na radku prekrocil 1024! Program bude ukoncen.\n");
      exit(1);
      break; 
    case 5: 
      fprintf(stderr, "\nSpatna vstupni data! Program bude ukoncen.\n");
      exit(1);
      break;
    case 6: 
      fprintf(stderr, "\nSpatny pocet parametru! Program bude ukoncen.\n");
      exit(1);
      break; 
    default: 
      break; 
    
  }
}

/*funkce pro tisk napovedy*/

void print_help(){
	printf( "\n______________________________________________________________________\n"
			"Program na tisk, soucet, prumer, vyber maxima a minima v danem rozsahu\n\n"
			
			"Autor: Jan Pavlica, 1BIB, skupina 32\n\n"
			
			"Spusteni programu: 1) ./proj1 --help\n"
			"                   2) ./proj1 operace vyber_bunek\n\n"
			
			"--help - spusti napovedu\n\n"
			
			"Vycet operaci:\n"
			"select - vytiskne bunky v danem rozsahu\n"
			"sum - secte ciselne bunky v danem rozsahu\n"
			"avg - zprumeruje ciselne bunky v danem rozsahu\n"
			"max - nejvetsi ciselna bunka v danem rozsahu\n"
			"min - nejmensi ciselna bunka v danem rozsahu\n\n"
			
			"Jak vybrat rozsah:\n"			
            "row X vyber vsech bunek na radku X (X > 0)\n"
            "col X vyber vsech bunek ve sloupci X (X > 0)\n"
            "rows X Y vyber vsech bunek od radku X az po Y. 0 < X <= Y\n"
            "cols X Y vyber vsech bunek od sloupce X az po Y. 0 < X <= Y\n"
            "range A B X Y vyber vsech bunek od radku A po radek B a od sloupce X po sloupec Y.\n"
            "0 < A <= B, 0 < X <= Y\n\n"
            
            "Vstupni text lze zadavat: 1) z klavesnice\n"
            "                          2) ze souboru např. < data.txt\n\n"
            
            "Data musi mit konstantni pocet sloupcu!\n"
            "______________________________________________________________________\n");
            exit(0);
}
