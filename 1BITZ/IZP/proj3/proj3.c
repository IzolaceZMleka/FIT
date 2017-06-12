/*
 * Soubor:  proj3.c
 * Datum:   13. 12. 2014
 * Autor:   Jan Pavlica, xpavli78@stud.fit.vutbr.cz
 * Projekt: IZP c. 3 - Pruchod bludistem
 * Popis:   Program pro pruchod bludistem
 */

#include <stdio.h> /* vstup a vystup */ 
#include <stdlib.h> /* obecne funkce */ 
#include <stdbool.h> /* datovy typ bool */
#include <ctype.h> /* funkce pro testovani znaku - isdigit, isprint atd. */ 
#include <string.h> /* funkce strcmp */ 

#define leva_hrana 1 // oznaceni leve strany
#define dolni_hrana 2 // oznaceni dolni strany
#define prava_hrana 3 // oznaceni prave strany
#define horni_hrana 4 // oznaceni horni strany

#define levy_smer 2 //oznaceni leveho smeru
#define pravy_smer 1 //oznaceni praveho smeru

#define start 0 //nulty sloupec nebo radek

#define leva_hrana_plna 1 //pro testovani leve hrany na pruchodnost
#define prava_hrana_plna 2 //pro testovani prave hrany na pruchodnost
#define horni_dolni_hrana_plna 4 //pro testovani horni/dolni hrany na pruchodnost

#define HELP 1 //pro spusteni napovedy
#define TEST 2 //pro spusteni testu
#define PATH 3 //pro spusteni hledani cesty

/*konstanty pro errory */

#define NOK_1_PARAMETR 1 
#define NOK_VSTUP 2
#define NOK_ROZSAH 3
#define NOK_ZNAK 4
#define NOK_VSTUPNI_SOURADNICE 5



typedef struct{
  unsigned rows; //pocet radku v mape
  unsigned cols; //pocet sloupcu v mape
  unsigned char *cells; //ukazatel na zacatek pole znaku
} Map;


void map_alokuj(Map * map, unsigned rows, unsigned cols); //funkce pro alokaci mapy
void map_zrus(Map * map); //funkce pro zruseni mapy
void map_set(Map * map, unsigned i, unsigned j, unsigned char a); //funce pro ulozeni znaku do mapy
int map_vytvor(Map * map, char *nazev); // funkce pro vytvoreni mapy
bool isborder(Map *map, int r, int c, int border); //funkce pro kontrolu pruchodnosti
int start_border(Map *map, int r, int c, int leftright); //funkce pro nalezeni startovace hrany
void pruchod(Map *map, int r, int c, int smer); //funkce pro pruchod bludistem
void print_help(void); //tisk napovedy
void print_error(int kod); //tisk erroru
void call_f(int smer, int funce, char *nazev, Map *map, int r, int c); //zavolani funkce
int isvalid(Map *map, char *nazev); //kontrola validity
int kontrola_znaku(char *nazev); //kontrola spravnych znaku v souboru
void udelej_parametry(int argc, char * argv[], int *smer, int *funkce, char **nazev, int *r, int *c); //vytvoreni parametru

int main (int argc, char * argv[])
{
  Map map; //struktura mapy
  
  int smer; //promenna udavajici smer
  int funkce; //promenna pro uchovani hodnoty funkce
  int r, c; //promenne pro vstupni souradnice
  char *nazev; //nazev souboru
  
  udelej_parametry(argc, argv, &smer, &funkce, &nazev, &r, &c);
  call_f(smer, funkce, nazev, &map, r, c);
  
  
  map_zrus(&map);
  
  return 0;
	
}

void udelej_parametry(int argc, char * argv[], int *smer, int *funkce, char **nazev, int *r, int *c)
{
  
  char* p_end;
  
  if (strcmp("--help",argv[1])==0 && argc == 2)
  {
    *funkce = HELP;
    return;
  }
  
  if (strcmp("--test",argv[1])==0 && argc == 3)
  {
    *funkce = TEST;
    *nazev = argv[2];
    return;
  }
  
  if (strcmp("--rpath",argv[1])==0 && argc == 5)
  {
    *funkce = PATH;
    *nazev = argv[4];
    *smer = pravy_smer;
    *r = strtol(argv[2], &p_end, 10); //zjisteni souradnice 
    if(*r < 1 || *p_end != '\0') //kontrola spravnosti
      print_error(NOK_VSTUPNI_SOURADNICE); //zjisteni souradnice
    *c = strtol(argv[3], &p_end, 10); //kontrola spravnosti
    if(*r < 1 || *p_end != '\0')
      print_error(NOK_VSTUPNI_SOURADNICE);
    
    return;
  }
  
  if (strcmp("--lpath",argv[1])==0 && argc == 5)
  {
    *funkce = PATH;
    *nazev = argv[4];
    *smer = levy_smer;
    *r = strtol(argv[2], &p_end, 10); //zjisteni souradnice 
    if(*r < 1 || *p_end != '\0') //kontrola spravnosti
      print_error(NOK_VSTUPNI_SOURADNICE); //zjisteni souradnice
    *c = strtol(argv[3], &p_end, 10); //kontrola spravnosti
    if(*r < 1 || *p_end != '\0')
      print_error(NOK_VSTUPNI_SOURADNICE);
    return;
  }
  
  print_error(NOK_1_PARAMETR);
  return;
}

int kontrola_znaku(char *nazev)
{
  FILE *soubor;
  int c; //promenna pro znak
  int rows, cols; //promenne pro radky a sloupce
  int pocitadlo_cisel = 0; //pocitadlo cisel
  int cislo_pred = 0; //priznak zda nejsou 2 cisla po sobe
  
  if((soubor = fopen(nazev,"r")) == NULL)
  {
    fprintf(stderr, "Chyba otevření souboru %s\n",nazev);
    return 1;
  }
  
  fscanf(soubor, "%d %d", &rows, &cols);
  
  while(pocitadlo_cisel != rows*cols) //dokud nebude pocet cisel dostatecny pro dosazeni do velikosti mapy
  {
    c = fgetc (soubor);

    if((isspace(c) || isdigit(c)) != 1) //kontrola zda jsou jenom cisla nebo bile znaky
    {
      print_error(NOK_ZNAK);
      return 1;
    }
    
    if(isdigit(c)) //pokud je cislo
    {
      if(c>57 || c<48) //kontrola rozsahu
      {
	print_error(NOK_ROZSAH);
	return 1;
      }
      
      if(cislo_pred == 1) //kontrola zda neni viceciferne
      {
	print_error(NOK_ROZSAH);
	return 1;
      }
      
      cislo_pred = 1; //nastaveni priznaku
      
      pocitadlo_cisel++;
    }
    
    if(isspace(c))
    {
      cislo_pred = 0; //vymazani priznaku
    }
  }
  
  fclose(soubor);
  return 0;
  
}


int isvalid(Map *map, char *nazev)
{
  
  int vstup = 0; //priznak pro alespon jeden vstup do bludiste
  
  if(kontrola_znaku(nazev))
    return 1;
    
    
    for (unsigned i = 0; i < map->rows; i++)
    {
      for (unsigned j = 0; j < map->cols; j++)
      {
	
	  if(j != 0)
	  {
	    if(((map->cells[map->cols*i + j] & leva_hrana_plna) == leva_hrana_plna)
		 != ((map->cells[map->cols*i + j-1] & prava_hrana_plna) == prava_hrana_plna))
	      return 1;
	  }
	  else
	  {
	    if((map->cells[map->cols*i + j] & leva_hrana_plna) == 0)
	      vstup++;
	  }
	  
	  if(j != map->cols - 1)
	  {
	    if(((map->cells[map->cols*i + j] & prava_hrana_plna) == prava_hrana_plna)
		  != ((map->cells[map->cols*i + j+1] & leva_hrana_plna) == leva_hrana_plna))
	      return 1;
	  }
	  else
	  {
	    if((map->cells[map->cols*i + j] & prava_hrana_plna) == 0)
	      vstup++;
	  }
	  
	if(i%2 != j%2)
	{
	  if(i != map->rows - 1)
	  {
	    if((map->cells[map->cols*i + j] & horni_dolni_hrana_plna) != (map->cells[map->cols*(i+1) + j] & horni_dolni_hrana_plna))
	      return 1;
	  }
	  else
	  {
	    if((map->cells[map->cols*i + j] & horni_dolni_hrana_plna) == 0)
	      vstup++;
	  }
	}
	if(i%2 == j%2)
	{
	  if(i != 0)
	  {
	    if((map->cells[map->cols*i + j] & horni_dolni_hrana_plna) != (map->cells[map->cols*(i-1) + j] & horni_dolni_hrana_plna))
	      return 1;
	    else
	    {
	      if((map->cells[map->cols*i + j] & horni_dolni_hrana_plna) == 0)
	      vstup++;
	    }
	  }
	}
      }
    }
    
    if(vstup == 0)
    {
      print_error(NOK_VSTUP);
      return 1;
    }
    
    return 0;
}

void call_f(int smer, int funkce, char *nazev, Map *map, int r, int c)
{
  if(funkce == HELP) //zavolani funkce pro tisk napovedy
  {
    print_help();
    return;
  }
  
  if(map_vytvor(map, nazev))
    return;
  
  if(funkce == TEST) //zavolani funkce pro test
  {
    if(isvalid(map, nazev))
      printf("Invalid\n");
    else
      printf("Valid\n");
    
    return;
  }
  
  if(funkce == PATH) //zavolani funkce pro pruchod
  {
    if(isvalid(map, nazev))
      return;
    
    pruchod(map, r, c, smer);
  }
  
  return;
}

void pruchod(Map *map, int r, int c, int smer)
{
  int border = start_border(map, r, c, smer); //urceni prvni hrany
  
  if(border == 0)
    return;
  
  r = r - 1;
  c = c - 1;
  
  /* dokud nevyjdeme z mapy */
  while((unsigned)r < map->rows && (unsigned)c < map->cols && r != -1 && c != -1){
    if(isborder(map, r, c, border)) //pokud je hrana nepruchozi, posune na dalsi ranu
    {
      if(((((r%2) == 1) && (c%2)==1)) || (((r%2) == 0) && ((c%2)==0))) //bunka ma horni hranu
      {
	if(border == leva_hrana)
	{
	  if(smer == pravy_smer)
	    border += 2;
	  if(smer == levy_smer)
	    border += 3;
	}
	else if(border == prava_hrana)
	{
	  if(smer == pravy_smer)
	    border += 1;
	  if(smer == levy_smer)
	    border -= 2;
	}
	else if(border == horni_hrana)
	{
	  if(smer == pravy_smer)
	    border -= 3;
	  if(smer == levy_smer)
	    border -= 1;
	}
      }
      else if(((((r%2) == 1) && (c%2)==0)) || (((r%2) == 0) && ((c%2)==1))) //bunka ma dolni hranu
      {
	if(border == leva_hrana)
	{
	  if(smer == pravy_smer)
	    border += 1;
	  if(smer == levy_smer)
	    border += 2;
	}
	else if(border == dolni_hrana)
	{
	  if(smer == pravy_smer)
	    border += 1;
	  if(smer == levy_smer)
	    border -= 1;
	}
	else if(border == prava_hrana)
	{
	  if(smer == pravy_smer)
	    border -= 2;
	  if(smer == levy_smer)
	    border -= 1;
	}
      }
    }
      else //pokud je pruchozi posune se do dalsiho policka a zmeni hranu
      {
	printf("%d,%d\n",r+1,c+1); //tisk aktualni souradnice
	if(((((r%2) == 1) && (c%2)==1)) || (((r%2) == 0) && ((c%2)==0))) //bunka ma horni hranu
	{
	  if (border == leva_hrana)
	  {
	    if(smer == pravy_smer)
	      border = leva_hrana;
	    if(smer == levy_smer)
	      border = dolni_hrana;
	    --c;
	  }
	  else if (border == prava_hrana)
	  {
	    if(smer == pravy_smer)
	      border = dolni_hrana;
	    if(smer == levy_smer)
	      border = prava_hrana;
	    ++c;
	  }
	  else if (border == horni_hrana)
	  {
	    if(smer == pravy_smer)
	      border = prava_hrana;
	    if(smer == levy_smer)
	      border = leva_hrana;
	    --r;
	  }
	}
	else if(((((r%2) == 1) && (c%2)==0)) || (((r%2) == 0) && ((c%2)==1))) //bunka ma dolni hranu
	{
	  if (border == leva_hrana)
	  {
	    if(smer == pravy_smer)
	      border = horni_hrana;
	    if(smer == levy_smer)
	      border = leva_hrana;
	    --c;
	  }
	  else if (border == prava_hrana)
	  {
	    if(smer == pravy_smer)
	      border = prava_hrana;
	    if(smer == levy_smer)
	      border = horni_hrana;
	    ++c;
	  }
	  else if (border == dolni_hrana)
	  {
	    if(smer == pravy_smer)
	      border = leva_hrana;
	    if(smer == levy_smer)
	      border = prava_hrana;
	    ++r;
	  }
	}
      }
  }
  return;
}

bool isborder(Map *map, int r, int c, int border)
{
  
  /*kontrola pruchodnosti logickym soucinem */

  if(border == leva_hrana)
  {
    if((map->cells[map->cols*r + c] & leva_hrana_plna) == 1)
    return true;
  }
  else if (border == prava_hrana)
  {
    if((map->cells[map->cols*r + c] & prava_hrana_plna) == 2)
    return true;
  }
  else if (border == horni_hrana || border == dolni_hrana)
  {
    if((map->cells[map->cols*r + c] & horni_dolni_hrana_plna) == 4)
    return true;
  }
  
  return false;
}

int start_border(Map *map, int r, int c, int leftright)
{
  char vstup = map->cells[map->cols*(r-1)+ (c-1)];
  int hrana = 0;
  
  if(c != 1 && (unsigned)c != map->cols) //kontrola zda vstup zespodu nebo shora ma vnejsi hranici
  {
    if(r == 1 && ((c%2)==0))
    {
      print_error(NOK_VSTUP);
      return 0;
    }
    
    if((unsigned)r == map->rows && ((((map->rows%2) == 1) && ((c%2) == 1)) || (((map->rows%2) == 0) && ((c%2) == 0))))
    {
      print_error(NOK_VSTUP);
      return 0;
    }
  }

  if(c==1)
  {
    if((vstup&leva_hrana) == 0)
      hrana = leva_hrana;
  }
  else if((unsigned) c == map->cols)
  {
    if((vstup&prava_hrana) == 0)
      hrana = prava_hrana;
  }
  
  if(leftright == pravy_smer)
  {
    if((c == 1) && ((r%2) == 1) && hrana == leva_hrana)
      return prava_hrana;
    else if ((c == 1) && ((r%2) == 0) && hrana == leva_hrana)
      return dolni_hrana;
    else if ((unsigned) c == map->cols && ((r%2) == 1) && ((c%2)==1) && hrana == prava_hrana)
      return horni_hrana;
    else if ((unsigned) c == map->cols && ((r%2) == 0) && ((c%2)==0) && hrana == prava_hrana)
      return horni_hrana;
    else if ((unsigned) c == map->cols && ((r%2) == 1) && ((c%2)==0) && hrana == prava_hrana)
      return leva_hrana;
    else if ((unsigned) c == map->cols && ((r%2) == 0) && ((c%2)==1) && hrana == prava_hrana)
      return leva_hrana;
    else if (r == 1)
      return leva_hrana;
    else if ((unsigned) r == map->rows)
      return prava_hrana;
    
  }
  else if(leftright == levy_smer)
  {
    if((c == 1) && ((r%2) == 1) && hrana == leva_hrana)
      return horni_hrana;
    else if ((c == 1) && ((r%2) == 0) && hrana == leva_hrana)
      return prava_hrana;
    else if ((unsigned) c == map->cols && ((r%2) == 1) && ((c%2)==1) && hrana == prava_hrana)
      return leva_hrana;
    else if ((unsigned) c == map->cols && ((r%2) == 0) && ((c%2)==0) && hrana == prava_hrana)
      return leva_hrana;
    else if ((unsigned) c == map->cols && ((r%2) == 1) && ((c%2)==0) && hrana == prava_hrana)
      return dolni_hrana;
    else if ((unsigned) c == map->cols && ((r%2) == 0) && ((c%2)==1) && hrana == prava_hrana)
      return dolni_hrana;
    else if (r == 1)
      return prava_hrana;
    else if ((unsigned) r == map->rows)
      return leva_hrana;
  }
  
  return 0; 
}

int map_vytvor(Map * map, char *nazev)
{
  FILE *soubor;
  unsigned char a;
  unsigned rows, cols;
  
  if((soubor = fopen(nazev,"r")) == NULL)
  {
    fprintf(stderr, "Chyba otevření souboru %s\n",nazev);
    return 1;
  }
  
  fscanf(soubor, "%u %u", &rows, &cols);
  map_alokuj(map, rows, cols);
  
  for (unsigned i = 0; i < rows; i++)
  {
    for (unsigned j = 0; j < cols; j++)
    {
      fscanf(soubor, "%hhu ", &a);
      map_set(map,i,j,a);
    }
  }
  
  fclose(soubor);
  return 0;
  
}


void map_alokuj(Map * map, unsigned rows, unsigned cols)
{ 
  map->rows = rows;
  map->cols = cols;

  map->cells = (unsigned char*) malloc(rows*cols*sizeof(unsigned char));
}

void map_zrus(Map * map)
{
  free(map->cells);
}

void map_set(Map * map, unsigned i, unsigned j, unsigned char a)
{
	map->cells[map->cols*i + j] = a;
}

/*funkce pro tisk erroru*/

void print_error(int kod) 
{ 
	switch(kod) { 
		case NOK_1_PARAMETR: 
		fprintf(stderr, "\n\nChybny 1. parametr prikazove radky! Program bude ukoncen. \n\n");  
		break; 
		case NOK_VSTUP: 
		fprintf(stderr, "\n\nDane bludiste nema platny vstup! Program bude ukoncen. \n\n"); 
		break; 
		case NOK_ROZSAH: 
		fprintf(stderr, "\n\nData mimo rozsah 0-7! Program bude ukoncen. \n\n"); 
		break; 
		case NOK_ZNAK: 
		fprintf(stderr, "\n\nJiny nez bily nebo ciselny znak! Program bude ukoncen. \n\n"); 
		break; 
		case NOK_VSTUPNI_SOURADNICE: 
		fprintf(stderr, "\n\nChybne vstupni souradnice! Program bude ukoncen. \n\n"); 
		break; 
		default: 
		break; 
	} 
}

/*funkce pro tisk napovedy*/

void print_help(){
	printf( "Napoveda:\n"
		"Autor: Jan Pavlica, 1BIB, skupina 32\n\n"
		
		"Program pro pruchod bludistem.\n\n"
		"Spusteni programu: 1) ./proj2 --help\n"
		"                   2) ./proj2 --test soubor.txt\n"
		"                   3) ./proj2 --lpath R C soubor.txt\n"
		"                   4) ./proj2 --rpath R C soubor.txt\n\n"
		
		"--help - spusti napovedu\n\n"
		
		"--test soubor.txt - Overi validitu mapy nactene ze zadaneho souboru.\n\n"

		
		"./proj2 --lpath R C soubor.txt - Projde bludistem podle pravidla leve ruky.\n"
		"                               - Vytiskne souradnice, kterymi prosel.\n"
		"R - vstup na radku R, (R>0)\n"
		"C - vstup na sloupci C, (C>0)\n\n"
		"./proj2 --rpath R C soubor.txt - Projde bludistem podle pravidla prave ruky.\n"
	        "                               - Vytiskne souradnice, kterymi prosel.\n"
		"R - vstup na radku R, (R>0)\n"
		"C - vstup na sloupci C, (C>0)\n\n");
            return;
}
