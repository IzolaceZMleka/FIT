#include <stdio.h>
#include <string.h>
#include "../main-files/read.h"

int main(int argc, char * argv[]){
	//printf("Provizorni test pro odhaleni chyby double. Napis cislo:\n");
	//printf("Funkce readDouble vratila: %g\n", readDouble());
	int Ires;
	double Dres;                                                         
	char * Sres;

	if(argc == 2){
		if(!strcmp(argv[1], "int")){                                         
			Ires = readInteger();                                               
			printf("%d\n", Ires);                                             
		}                                                                    
		else if(!strcmp(argv[1], "double")){                                 
			Dres = readDouble();
			printf("%g\n", Dres);                                             
		}                                                                    
		else if(!strcmp(argv[1], "string"))
			printf("%s\n", readString()->str);                            
		else                                                                 
			printf("spatne argumenty\n");    
	}

	if(argc == 3){ // kombinace
		if((!strcmp(argv[1], "int")) && (!strcmp(argv[2], "string"))){ // jestli int nebere znak
			Ires = readInteger();
			printf("%d%s\n", Ires, readString()->str);
		}
		else if((!strcmp(argv[1], "double")) && (!strcmp(argv[2], "string"))){ // double nebere
			Dres = readDouble();
			printf("%g%s\n", Dres, readString()->str);
		}
		else if((!strcmp(argv[1], "string")) && (!strcmp(argv[2], "int"))){ // string nebere
			Sres = readString()->str;
			printf("%s%d\n", Sres, readInteger());
		}
		else
			printf("spatne argumenty\n");
	}
		
}


