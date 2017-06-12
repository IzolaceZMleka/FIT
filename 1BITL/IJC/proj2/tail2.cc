/*Projekt c.2 pro predmet IJC - Jazyk C
*Autor: Jan Pavlica - 1BIT, VUT FIT
*       xpavli78@stud.fit.vutbr.cz
*Datum: 27.4.2015
*Prelozeno: g++ 4.5.2
*Popis: Implementace POSIX programu tail v jazyce C++, cast a)
*/


#include <iostream>
#include <fstream> 
#include <deque>
#include <string>
#include <sstream>
#include <cstdlib> 

#define LINES_DEFAULT 10 //zakladni pocet znaku

using namespace std;

#define OK 1
#define ERROR_WRONG_PARAMS 2
#define ERROR_MORE_CHARS 3
#define ERROR_WRONG_NUMBER 4
#define ERROR_WRONG_FILE_INPUT 5
#define ERROR_MORE_PARAMS 6

typedef struct Params
{
  int c_lines; //pocet radku
  istream *input_file; //vstupni soubor
  bool from; // zda je prepinac -n
  int errors; // cislo erroru
} params_t;

params_t do_params(int argc, char **argv);// funkce pro zpracovani parametru
void tail(int c_line, istream &input_file, bool from);
void error(int error_code);//fuknce pro tisk erroru


int main(int argc, char **argv)
{
  params_t my_params;
  my_params = do_params(argc, argv);
  if(my_params.errors != OK)//kontrola erroru
  {
    error(my_params.errors);
    return EXIT_FAILURE;
  }
  tail(my_params.c_lines, *my_params.input_file, my_params.from);
  return EXIT_SUCCESS;
}

params_t do_params(int argc, char **argv)
{
  params_t my_params;
  fstream file_stream; //stream pro soubor
  string argument; //string pro zpracovani argumentu
  
  my_params.c_lines = LINES_DEFAULT;
  my_params.from = false;
  my_params.errors = OK;
  my_params.input_file = &cin; //zkladni vstup stdin
  
  if(argc == 4 || argc == 3) // kontrola zda je prepinas -n
  {
    argument = argv[1];
  }
  
  if (argc > 4) //kontrola poctu parametru
  {
    my_params.errors = ERROR_MORE_PARAMS;
  }
  else if (argc == 2) //otevreni soubou
  {
    file_stream.open(argv[1], ios::in);
    my_params.input_file = &file_stream;
    if (my_params.input_file == NULL) //kontrola spravenho otevreni
    {
      my_params.errors = ERROR_WRONG_FILE_INPUT;
    }
  }
  else if (argc == 1)
  {
    my_params.input_file = &cin; //stdin
  }
  else if (argument.compare("-n") == 0) //porovnani prepinace -n
  {
    if (argc == 4)
    {
      file_stream.open(argv[3], ios::in);
      my_params.input_file = &file_stream;
      if (my_params.input_file == NULL)
      {
	my_params.errors = ERROR_WRONG_FILE_INPUT;
      }
    }
    else
    {
      my_params.input_file = &cin;
    }
    
    my_params.c_lines = atoi(argv[2]); //zpracovani poctu radku pro tisk
    
    
    if (my_params.c_lines <= 0)
    {
      my_params.errors = ERROR_WRONG_NUMBER;
    }
    
    if (argv[2][0] == '+')
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

void tail(int c_line, istream &input_file, bool from)
{
  int line_counter = 0;
  deque<string> buffer; //kontejner pro ukladani znaku
  string s; //string pro nacitani radku
  
  while(getline(input_file, s)) //dokud se nacitaji radky ze vstupu
  {
    line_counter++;
    
    if(from == true && line_counter >= c_line) //pokud je zadano tisk do konce
    {
      cout << s << endl; //tisk radku a pridani konce radky
    }
    else
    {
      buffer.push_front(s); //jinak se string ulozi na vrchol kontejneru
      if(line_counter > c_line) //pokud je prekrocen pocet radku, tak se zahodi posledni string v kontejneru
      {
	buffer.pop_back();
      }
    }
  }
  if (from == false)
  {
    for(int i = 0; i < c_line; i++)
    {
      cout << buffer.back() << endl; //tisk radku z kontejneru
      buffer.pop_back(); //zahozeni vytisknuteho radku
    }
  }
  return;
}

void error(int error_code) 
{ 
  switch(error_code) 
  { 
    case ERROR_WRONG_PARAMS: 
      cerr << "\n\nChybne parametr prikazove radky! Program bude ukoncen. \n\n";  
      break; 
    case ERROR_MORE_CHARS: 
      cerr << "\n\nNa radku je vice znaku nez je povoleno (512). Zbytek radku bude orezan a zahozen. \n\n"; 
      break; 
    case ERROR_WRONG_NUMBER: 
      cerr << "\n\nChybne zadane cislo! Program bude ukoncen. \n\n"; 
      break; 
    case ERROR_WRONG_FILE_INPUT: 
      cerr << "\n\nZdrojovy soubor nelze otevrit! Program bude ukoncen. \n\n"; 
      break; 
    case ERROR_MORE_PARAMS: 
      cerr << "\n\nZadano vice parametru, nez je povoleno! Program bude ukoncen. \n\n"; 
      break; 
    default: 
      break; 
  } 
  return;
}
