/*
 * Soubor:  proj2.c
 * Datum:   30. 11. 2014
 * Autor:   Jan Pavlica, xpavli78@stud.fit.vutbr.cz
 * Projekt: IZP c. 2 - Iteracni vypocty
 * Popis:   Program pro vypocet uhlu tangens a nasledneho vypoctu vzdalenosti a vysky
 */

#include <stdio.h> /* vstup a vystup */ 
#include <stdlib.h> /* obecne funkce */ 
#include <ctype.h> /* funkce pro testovani znaku - isdigit, isprint atd. */ 
#include <string.h> /* funkce strcmp */ 
#include <math.h> /* funkce tan */

/* konstanty pro errory */
#define NOK_1_PARAMETR 1
#define NOK_RADIAN 2
#define NOK_DOLNI_ROZSAH 3
#define NOK_HORNI_ROZSAH 4
#define NOK_POCET_ARGUMENTU 5
#define NOK_VYSKA 6

#define MAX_RADIAN 1.4 /* maximalni uhel */
#define TAYLOR_CLENY 13 /* pocet clenu Taylorova polynomu */
#define MAX_ITERACI 13 /* maximalni pocet iteraci */
#define DEFAULT_HIGH 1.5 /* implicitni  vyska */
#define ANGLE_ITERATIONS 11 /* pocet potrebnych iteracich pro cfrac_tan */

void print_help(void); /* funkce pro napovedu */
void print_error(int kod); /* funkce pro tisk erroru */
void print_tan(double radians, int start, int end); /* funkce pro tisk ke srovnani podle iteraci */
int control(int argc, char *argv[]); /* kontrola argumentu */
double taylor_tan(double x, unsigned int n); /* funkce pro vypocet Taylorova polynomu */
double cfrac_tan(double x, unsigned int n); /* funkce pro vypocet pomoci zretezenych zlomku */
double abso(double x); /* funkce pro absolutni hodnotu */
double distance(double alfa, double v); /* funkce pro vypocet vzdalenosti */
double high(double alfa, double beta, double v); /* funkce pro vypocet vysky */
void call_f(int argc, char *argv[]); /* funkce pro zavolani funkce */
int iterations(double x); /* funkce pro vypocet potrebneho poctu iteraci */

int main(int argc, char *argv[])
{
  if(control(argc, argv)) /* kontrola argumentu */
    return EXIT_FAILURE;
  
  call_f(argc, argv); /* zavolani funkce */

  return EXIT_SUCCESS;
}

int iterations(double x)
{
  double angle_iterations[ANGLE_ITERATIONS] = {0.0, 0.00006695000, 0.00537825000, /* intervaly pro rozdeleni iteraci, viz. dokumentace */
    0.03976726000, 0.12882882997, 0.28243970989, 0.49621669978, 0.75592270106, 1.03872845205, 1.30776122042, 1.4};
    
  int i; /* pocitadlo pro iterace */
  
  for(i = 0; i < ANGLE_ITERATIONS; i++) 
  {
    if(angle_iterations[i] < x && x <= angle_iterations[i+1]) /* podminka vyhodnocujici kolik iteraci bude potreba */
    {
      break;
    }
  }
  return i+1;
}

int control(int argc, char *argv[])
{
  char* p_end;
  
  if(argc < 2 || argc > 6) /* kontrola poctu argumentu */
  {
    print_error(NOK_POCET_ARGUMENTU);
    return 1;
  }
  
  if (strcmp("--help",argv[1])==0 && argc == 2) /* kontrola zavolani funkce print_help */
  {
   return 0;
  }
  else if (strcmp("--tan",argv[1])==0 && argc == 5) /* kontrola zavolani funkce print_tan */
  {
   double radians = strtod(argv [2], &p_end); /* velikost uhlu */
   
   if(radians <= 0 || radians > MAX_RADIAN || *p_end != '\0') /* kontrola velikosti uhlu */
      {
	print_error(NOK_RADIAN);
	return 1;
      }
   
   int start = strtol (argv[3],&p_end,10); /* pocatecni iterace */
   
    if(start <= 0 || start > MAX_ITERACI || *p_end != '\0') /* kontrola spodni hranice iteraci */
      {
	print_error(NOK_DOLNI_ROZSAH);
	return 1;
      }
      
   int end = strtol (argv [4],&p_end,10); /* konecna iterace */     
      
      if(end < start || end > MAX_ITERACI || *p_end != '\0') /* kontrola horni hranice iteraci */
      {
	print_error(NOK_HORNI_ROZSAH);
	return 1;
      }
      return 0;
  }
  else if ((strcmp("-c",argv[1])==0 && strcmp("-m",argv[3])==0 && (argc == 5 || argc == 6)) 
	    || (strcmp("-m",argv[1])==0 && (argc == 3 || argc == 4))) /* kontrola argumentu pro vypocet vzdalenosti a vysky */
  {
    double v; /* promenna pro vysku */
    double radians; /* promenna pro uhel alfa */
    
    if(argc == 5 || argc == 6) /* prirazeni hodnot do promennych */
    {
      v = strtod(argv [2], &p_end);
	if(*p_end != '\0' || v <= 0)
	{
	  print_error(NOK_VYSKA);
	  return 1;
	}
      radians = strtod(argv [4], &p_end);
    }
    
    if(argc == 3 || argc == 4) /* prirazeni hodnot do promennych */
    {
      v = DEFAULT_HIGH;
      radians = strtod(argv [2], &p_end);;
    }
    
    if(radians < 0 || radians >= MAX_RADIAN || *p_end != '\0') /* kontrola uhlu alfa */
    {
      print_error(NOK_RADIAN);
      return 1;
    }
    
    if (argc == 6 || argc == 4) /* prirazeni uhlu beta */
    {
      double b = strtod (argv[argc-1], &p_end); /* promenna pro uhel beta */
      if(b <= 0 || b > MAX_RADIAN || *p_end != '\0') /* kontrola uhlu beta */
      {
	print_error(NOK_RADIAN);
	return 1;
      }
    }
    return 0;
  }
  else
  {
    print_error(NOK_1_PARAMETR);
    return 1;
  }
}

void print_tan(double radians, int start, int end)
{
  
  double taylor; /* promenna pro ulozeni vysledku z funkce taylor_tan */
  double cfrac; /* promenna pro ulozeni vysledku z funkce cfrac_tan */
  double tangens; /* promenna pro ulozeni vysledku z funkce tan z knihovny math.h */
  
  for(unsigned int i = (unsigned)start; i<= (unsigned)end; i++) /* cyklus pro tisknuti vysledku jednotlivych iteraci */
  {
    taylor = taylor_tan(radians,i);
    cfrac = cfrac_tan(radians,i);
    tangens = tan(radians);
    
    printf("%d %e %e %e %e %e\n", i, tangens, taylor,abso(tangens - taylor), cfrac, abso(cfrac - tangens));
  }
  
}

void call_f(int argc, char *argv[])
{
  
  
  if (strcmp("--help",argv[1])==0 && argc == 2) /* zavolani funkce help */
  {
    print_help(); 
    return;
  }
  
  else if (strcmp("--tan",argv[1])==0 && argc == 5) /* zavolani fuknce print_tan */
    {
      double radians = atof (argv [2]); /* velikost uhlu */
      int start = atof (argv[3]); /* pocatecni iterace */
      int end = atof (argv [4]); /* konecna iterace */
      
      print_tan(radians, start, end);
      return;
    }
	    
  else if ((strcmp("-c",argv[1])==0 && strcmp("-m",argv[3])==0 && (argc == 5 || argc == 6))
	    || (strcmp("-m",argv[1])==0 && (argc == 3 || argc == 4)))
  {
    double v; /* promenna pro ulozeni vysky */
    double radians; /* promenna pro uhel alfa */
    
    if(argc == 5 || argc == 6) /* prirazeni hodnot do promennych */
    {
      v = atof(argv[2]);
      radians = atof(argv[4]);
    }
    
    if(argc == 3 || argc == 4) /* prirazeni hodnot do promennych */
    {
      v = DEFAULT_HIGH;
      radians = atof(argv[2]);
    }
    
    if(argc == 5 || argc == 3) /* tisk vzdalenosti */
    {
      printf("%.10e\n", distance(radians,v));
      return;
    }
    else if (argc == 6 || argc == 4) /* tisk vysky a vzdalenosti */
    {
      double b = atof(argv[argc-1]); /* promenna pro uhel beta */
      printf("%.10e\n", distance(radians,v));
      printf("%.10e\n", high(radians,b,v));
      return;
    }
  }
  
  return;
}

double abso(double x)
{
  if(x < 0.00)
    x = x * (-1.00);
  
  return x;
  
}

double high(double alfa, double beta, double v)
{
  double dist = distance(alfa,v); /* promenna pro ulozeni vzdalenosti */
  double h = dist * cfrac_tan(beta,iterations(beta)) + v; /* vypocet vysky */
  
  return h;
}

double taylor_tan(double x, unsigned int n)
{
  long long dividend[TAYLOR_CLENY] = {1, 1, 2, 17, 62, 1382, 21844, 929569, 6404582,
				      443861162, 18888466084, 113927491862, 58870668456604};
  long long divisor[TAYLOR_CLENY] = {1, 3, 15, 315, 2835, 155925, 6081075, 638512875, 10854718875,
				      1856156927625, 194896477400625, 49308808782358125, 3698160658676859375};
				      
  double sum = 0.00; /* promenna pro ulozeni vysledku */
  double pow_x = x; /* promenna pro ulozeni mocniny x */
  
  for (unsigned int i = 0; i < n; i++)
  {
    sum += dividend[i] * pow_x / divisor[i]; /* vypocet Taylorova polynomu */
    pow_x = pow_x * x * x; /* vypocet pozadovane mocniny */
  }
  
  return sum;
  
}

double distance(double x, double v)
{
  double dist = v/cfrac_tan(x,iterations(x)); /* vypocet vzdalenosti */
  return dist; 
}

double cfrac_tan(double x, unsigned int n)
{
  double sum = ((n*2)-1)/x; /*vypocet posledniho clenu zretezeneho zlomku */
  
  if (n == 1)
    return 1.00/sum; /* provedeni prvni iterace */
  else
  {
    for ( int i = n-1; i > 0; i--)
    {
      sum = ((i*2)-1)/x - 1/sum; /* vypocet clenu podle iterace */
    }
  }
  
  return 1.00/sum; /* provedeni prvni iterace */
  
}

/*funkce pro tisk erroru*/

void print_error(int kod) 
{ 
	switch(kod) { 
		case NOK_1_PARAMETR: 
		fprintf(stderr, "\n\nChybny 1. parametr prikazove radky! Program bude ukoncen. \n\n");  
		break; 
		case NOK_RADIAN: 
		fprintf(stderr, "\n\nChybna velikost uhlu v radianech! Program bude ukoncen. \n\n"); 
		break; 
		case NOK_DOLNI_ROZSAH: 
		fprintf(stderr, "\n\nChybny dolni parametr rozsahu! Program bude ukoncen. \n\n"); 
		break; 
		case NOK_HORNI_ROZSAH: 
		fprintf(stderr, "\n\nChybny horni parametr rozsahu! Program bude ukoncen. \n\n"); 
		break; 
		case NOK_POCET_ARGUMENTU: 
		fprintf(stderr, "\n\nChybny pocet argumentu! Program bude ukoncen. \n\n"); 
		break; 
		case NOK_VYSKA: 
		fprintf(stderr, "\n\nChybna vyska! Program bude ukoncen. \n\n"); 
		break; 
		default: 
		break; 
	} 
}

/*funkce pro tisk napovedy*/

void print_help(){
	printf( "\n____________________________________________________________________________________________________________\n"
		"Program pro vypocet uhlu tangens a naslednemu vypoctu vzdalenosti a vysky pomoci nej.\n\n"
		
		"Autor: Jan Pavlica, 1BIB, skupina 32\n\n"
		
		"Spusteni programu: 1) ./proj2 --help\n"
		"                   2) ./proj2 --tan A N M\n"
		"                   3) ./proj2 [-c -X] -m A [B]\n\n"
		
		"--help - spusti napovedu\n\n"
		
		"--tan A N M - Vytiskne tangens vypocitany pomoci Taylorova polynomu, zretezenych zlomku a knihovny z math.h.\n"
		"              Dale pak absolutni odchylku a pocet iteraci uzitych k vypoctu. Pocet je zadan uzivatelem.  \n\n"
		"A - velikost uhlu v radianech (0 < A <= 1.4)\n"
		"N - dolni rozsah iteraci (0 < N <= M)\n"
		"M - horni rozsah iteraci (N <= M < 14)\n\n"
		
		"[-c -X] -m A [B] - Vypocita vzdalenost a pripadnou vysku objektu.\n"
		"A - velikost uhlu v radianech (0 < A <= 1.4)\n"
		"B - velikost uhlu v radianech, volitelny argument pro vypocet vysky objektu.\n"
		"-c - s argumentem X nastavi pozadovanou vysku, volitelny argument, implicitni vyska 1.5.\n"
		
            "_________________________________________________________________________________________________________________\n");
            return;
}
