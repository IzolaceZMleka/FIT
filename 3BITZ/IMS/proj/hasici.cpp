/* Autori: Jan Pavlica, Adam Piecek
 * Projekt: Modelovani a simulace - 5. hasicske stanice
 * Datum: 6.12.2016
 * Popis: Cilem projektu je simulovat vyskyt pozaru a jejich haseni/horeni.
 */

#include <iostream>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include "simlib.h"
#include <limits.h>
#include <math.h>  
#include <float.h>
#include <algorithm>
#include <vector>

using namespace std;

#define VELIKOST_MAPY 73668 // vychozi velikost mapy
#define POCET_STANIC 25 // vychoozi pocet stanic
#define ROK * 356 * 24 * 60 // konstanta pro prepocet casu z roku na minuty
#define KOLIK 1 // vychozi pocer roku behu

unsigned long max_skody[] = {0,0,0}; // pole pro uchovani maximalnich skod pro jednotlive typy
unsigned long min_skody[] = {ULONG_MAX,ULONG_MAX,ULONG_MAX};// pole pro uchovani minimalnich skod pro jednotlive typy
unsigned long celkem[] = {0,0,0}; // pole pro uchovani celkovych skod pro jednotlive typy
unsigned int cnt[] = {0,0,0}; // pole pro pocitani vyskytu pozaru daneho typu
	
unsigned int mintime[] = {UINT_MAX,UINT_MAX,UINT_MAX}; // pole pro uchovani maximalnich casu haseni pro jednotlive typy
unsigned int maxtime[] = {0,0,0}; // pole pro uchovani minimalnich casu haseni pro jednotlive typy
double maxtime_dojezd[] = {0,0,0}; // pole pro uchovani maximalnich casu prijezdu pro jednotlive typy
unsigned long celkemtime[] = {0,0,0}; // pole pro uchovani vsech casu haseni pro jednotlive typy

unsigned int mintime2[] = {UINT_MAX,UINT_MAX,UINT_MAX}; // pole pro uchovani maximalnich casu trvani pozaru pro jednotlive typy
unsigned int maxtime2[] = {0,0,0}; // pole pro uchovani minimalni casu trvani pozaru pro jednotlive typy
unsigned long celkemtime2[] = {0,0,0}; // pole pro uchovani vsech casu trvani pozaru pro jednotlive typy

unsigned long dobazasahu[] = {0,0,0}; // pole pro uchovani celkove doby zasahu pro jednotlive typy

unsigned long skody_celkem = 0; // pocitadlo celkovych skod
unsigned long naklady_celkem = 0; // pocitadlo nakladu za vsechny stanice
int cc = 0; // pocitadlo pro prijezdy trvajici vice nez 20 minut

int vyuzito_jine = 0; // pocitadlo pro pocet vyuziti jinych nez nejblizsich stanic
int totalni_zniceni = 0; // pocitadlo pozaru, kde bylo zniceno vse

int pocet_stanic = POCET_STANIC; // pocet stanic

class hasici : public Facility // trida pro hasici stanice
{
public:
	int x;
	int y;
	int vydaje;
	
	hasici(int N, int posunx, int posuny)
	{
		x = (rand()%N)+posunx;
		y = (rand()%N)+posuny;
		
		vydaje = (rand()%2500000) + 10000000;
	}
};

vector<hasici *> seznam_stanic(POCET_STANIC);

class jina_udalost : public Process // trida pro beh pozaru
{
public:
	int x; // souradnice
	int y; // souradnice
	
	jina_udalost(int N)
	{
		x = rand()%N;
		y = rand()%N;
	}
	
	void Behavior()
	{
		double minimalni_vzdalenost = DBL_MAX;  // promenna pro zjisteni minimalni vzdalenosti
		int index = -1;
		
		/* zjisteni nejblizsi stanice */
		
		while(minimalni_vzdalenost == DBL_MAX) // pokud jsou vsechny stanice zabrane
		{
			for(int i = 0; i < pocet_stanic; i++) // projdu vsechny pro nejblizsi
			{
				if(seznam_stanic[i]->Busy())
				{
					continue;
				}
				else
				{
					double vzdalenost = (sqrt(pow((double)(seznam_stanic[i]->x - x),2.0) + pow((double)(seznam_stanic[i]->y - y),2.0))); // zjisteni vzdalenosti
					
					if(vzdalenost < minimalni_vzdalenost)
					{
						index = i;
						minimalni_vzdalenost = vzdalenost;
					}
				}
			}
			Wait(1);
		}
		
		// auto je na vyjezdu
		Seize(*seznam_stanic[index]);
		Wait((random()%90)+30);
		Release(*seznam_stanic[index]);
	}
};
	
class pozar : public Process
{
public:
	int x;
	int y;
	int intenzita; 
	int typ;
	int prirazenych_aut;
	
	pozar(int N)
	{
		x = rand()%N;
		y = rand()%N;
		
		int typrand = rand() % 100; // generovani typu
		
		// generovani vychozi intenzity

		if(typrand < 1)
			intenzita = (rand() % 10000)+8000;
		else if(typrand < 25)
			intenzita = (rand() % 700)+700;
		else
			intenzita = (rand() % 150)+200;
		
		typ = 3;
		
		if(intenzita < 350)
		{
			typ = 1;
		}
		else if(intenzita < 1400)
		{
			typ = 2;
		}
		
		prirazenych_aut = 0;
	}
	
	int vrat_nejblizsi_stanici() // metoda pro zjisteni nejblizsi nezabrane stanice
	{
		int index = -1;
		double nejblizsi_cesta = DBL_MAX;
		double vzdalenost_vyuzite = DBL_MAX;
		
		for(int i = 0; i < pocet_stanic; i++)
		{
			double cesta = sqrt(pow((double)(seznam_stanic[i]->x - x),2.0) + pow((double)(seznam_stanic[i]->y - y),2.0));
			if(seznam_stanic[i]->Busy())
			{
				if(cesta < nejblizsi_cesta) 
					vzdalenost_vyuzite = cesta;
				
				continue;
			}
			else
			{
				if(cesta < nejblizsi_cesta)
				{
					index = i;
					nejblizsi_cesta = cesta;
				}
			}
		}
		
		if(index != -1)
		{
			if((vzdalenost_vyuzite < nejblizsi_cesta) && (prirazenych_aut == 0)) // v pripade prvni stanice zjistujeme zda nebyla jina blizsi zabrana
				vyuzito_jine++;
			
			prirazenych_aut++;
			Seize(*seznam_stanic[index]);
		}
			
		
		return index;
	}
	
	double doba_jizdy(hasici * pozadovana_stanice) // metoda pro vypocet dobu jizdy v minutach
	{
		return (((sqrt(pow((double)(pozadovana_stanice->x - x),2.0) + pow((double)(pozadovana_stanice->y - y),2.0))) / 1000)/60)*(50);
	}
	
	double vrat_cas_zacatku_haseni(hasici * pozadovana_stanice) // metoda pro zjisteni skutecneho casu zacatku haseni
	{
		double prijezd = doba_jizdy(pozadovana_stanice);
		prijezd += 2; // vyjezd jednotky
		
		if(prirazenych_aut==1) // porovnavani prijezdu nad 20 minut
		{
			if(maxtime_dojezd[typ-1] < prijezd)
				maxtime_dojezd[typ-1] = prijezd;
			
			if(prijezd > 20)
				cc++;
		}
		
		prijezd += 5; // doba bojoveho rozvinuti
		prijezd += 3; // oznameni dispecink
		
		return prijezd;
	}
	
	void Behavior()
	{
		unsigned long skody = 0;
		int cas = 0;
		int sila_haseni = 0;
		int faktor = typ;
		vector<int> index_pritomne_stanice(typ); // vektor pritomnych stanic
		vector<double> doba_prijezdu(typ); // vektor prijezdu jednotlivych stanic
		int c_pritomna_auta = 0;
		double vznik_pozaru = Time;	// zacatek pozaru
		
		for(int i = 0; i < typ; i++) // hledam nejblizsi stanice
		{
			index_pritomne_stanice[i] = vrat_nejblizsi_stanici();
			
			if(index_pritomne_stanice[i] != -1) // v pripade, ze je stanice dostupna spocitam cas
				doba_prijezdu[i] = vrat_cas_zacatku_haseni(seznam_stanic[index_pritomne_stanice[i]]);
		}
		
		sort(doba_prijezdu.begin(), doba_prijezdu.end()); // seradim casy prijezdu
		
		while(intenzita > 0) // dokud neni pozar uhasen
		{
			
			if((skody < 60000 && typ == 1) || (skody < 1000000 && typ == 2) || (skody < 50000000 && typ == 3)) // zarazka pro pripad, ze uz neni mozno zachranit majetek
				skody += intenzita; // pripisuji skody
			else
			{
				totalni_zniceni++;
				faktor = -1; // pozar se uz nema kam sirit, vse je zniceno, sam slabne
			}
			
			if(c_pritomna_auta < typ && prirazenych_aut > c_pritomna_auta)
			{
				if(doba_prijezdu[c_pritomna_auta] < cas) // v pripade, ze dojelo auto
				{
					c_pritomna_auta++; 
					
					// generovani sily haseni
					if(c_pritomna_auta == 1)
					{
						sila_haseni = (rand()%40)+20;
					}
					else if(c_pritomna_auta == 2)
					{
						sila_haseni = (rand()%50)+50;
					}
					else if(c_pritomna_auta == 3)
					{
						sila_haseni = (rand()%400)+300;
					}
					
					if(faktor > 0)
						faktor--;
					
				}
				
			}
			
			intenzita = (intenzita + (intenzita * ((double)faktor/40))) - sila_haseni; // sireni/haseni pozaru
			cas++;
			Wait(1); // cekani minutu
			
			if(prirazenych_aut < typ) // v pripade, ze nebylo mozno priradit pozadovany pocet stanic
			{
				// zkouska zda se neuvolnily nove stanice
				for(int i = 0; i < typ; i++)
				{
					if(index_pritomne_stanice[i] == -1)
					{
						index_pritomne_stanice[i] = vrat_nejblizsi_stanici();
						
						if(index_pritomne_stanice[i] != -1)
							doba_prijezdu[i] = vrat_cas_zacatku_haseni(seznam_stanic[index_pritomne_stanice[i]]) + cas;
					}
				}
			}
			
		}
		// zapis do statistik
		if(min_skody[typ-1] > skody)
			min_skody[typ-1] = skody;
		if(max_skody[typ-1] < skody)
			max_skody[typ-1] = skody;
		celkem[typ-1] += skody;
		cnt[typ-1]++;
		
		if(mintime[typ-1] > cas - doba_prijezdu[0])
			mintime[typ-1] = cas - doba_prijezdu[0];
		if(maxtime[typ-1] < cas - doba_prijezdu[0])
			maxtime[typ-1] = cas - doba_prijezdu[0];
		celkemtime[typ-1] += cas - doba_prijezdu[0];
		
		if(mintime2[typ-1] > (Time - vznik_pozaru))
			mintime2[typ-1] = (Time - vznik_pozaru);
		if(maxtime2[typ-1] < (Time - vznik_pozaru))
			maxtime2[typ-1] = (Time - vznik_pozaru);
		celkemtime2[typ-1] += (Time - vznik_pozaru);
		
		skody_celkem += skody;
		
		
		// odjezd aut
		double cesta_predchoziho_auta = 0.0;
		double cesta_aktualniho_auta = Time;
		
		for(int j = 0; j < prirazenych_aut; j++)
		{
			cesta_aktualniho_auta = doba_jizdy(seznam_stanic[index_pritomne_stanice[j]]) + 20; // pridano 20 minut pro sbaleni/pripravu na dalsi vyjezd
			Wait(cesta_aktualniho_auta - cesta_predchoziho_auta);
			Release(*seznam_stanic[index_pritomne_stanice[j]]);
			
			cesta_predchoziho_auta = cesta_aktualniho_auta;
		}
		
		dobazasahu[typ-1] += (Time - vznik_pozaru);
	}
};

class Generator : public Event // generovani udalosti
{
	int interval;
public:
	Generator()
	{
		interval = (rand()%3)+27;
	}
	
	void Behavior()
	{
		if(rand()%100 < 12) // vznik pozaru 
		{
			(new pozar(VELIKOST_MAPY))->Activate();
			Activate(Time + Exponential(interval));
		}
		else // vznik jine udalosti
		{
			(new jina_udalost(VELIKOST_MAPY))->Activate();
			Activate(Time + Exponential(interval));
		}
	}
};

int main(int argc, char *argv[])
{	
	
	int detailed = 1;
	string filename = "exp.out";
	int doba_behu = KOLIK;
	// jednoduchy parser
	if(argc >= 2)
	{
			pocet_stanic = atoi(argv[1]);
			if(pocet_stanic < 16)
			{
				cout << "Je nelidske odepirat hasicskou dostupnost danou zakonem. Pro danou oblast je pozadovano, alespon 16 stanic." << endl;
				return 1;
			}
	}
	if(argc >= 3)
	{
		detailed = atoi(argv[2]);
	}
	if(argc >= 4)
	{
			doba_behu = atoi(argv[3]);
	}
	if(argc >= 5)
	{
		filename = argv[4];
	}
	seznam_stanic.resize(pocet_stanic);
	
	srand(time(NULL));
	// incializace stanic
	for(int i = 0; i < pocet_stanic; i++)
	{
		if(i < pow(sqrt(pocet_stanic),2.0))
			seznam_stanic[i] = new hasici(VELIKOST_MAPY/sqrt(pocet_stanic),((i%(int)sqrt(pocet_stanic))*(VELIKOST_MAPY/sqrt(pocet_stanic))),((i/sqrt(pocet_stanic))*(VELIKOST_MAPY/sqrt(pocet_stanic))));
		else
			seznam_stanic[i] = new hasici(VELIKOST_MAPY,0,0);
		
		naklady_celkem += seznam_stanic[i]->vydaje;
	}
	
	Init(0,doba_behu ROK);
	(new Generator)->Activate();
	Run();
	// tisk sledovanych jevu
	if(detailed == 1 || detailed == 2)
		SetOutput(filename.c_str());
	
	if(detailed == 1 || detailed == 2)
	{
		Print("-----------------------------------------------------------------\n");
		Print("| Pocet stanic:\t\t%i\t\t\t\t\t|\n", pocet_stanic);
		Print("| Zkoumane obdobi:\t%i rok/u\t\t\t\t\t|\n", doba_behu);
		Print("-----------------------------------------------------------------\n");
		Print("| Pozaru celkem:\t%i\t\t\t\t\t|\n", (cnt[2]+cnt[0]+cnt[1]));
		Print("| Skody celkem:\t\t%i,- Kc\t\t\t\t|\n", skody_celkem);
		Print("| Vydaje celkem:\t%i,- Kc\t\t\t\t|\n", naklady_celkem*doba_behu);
		Print("| Skody + vydaje:\t%lu,- Kc\t\t\t\t|\n", skody_celkem + naklady_celkem*doba_behu);
		Print("-----------------------------------------------------------------\n");
		Print("| Pozaru rocni prumer:\t%i\t\t\t\t\t|\n", (cnt[2]+cnt[0]+cnt[1])/doba_behu);
		Print("| Skody rocni prumer:\t%i,- Kc\t\t\t\t|\n", skody_celkem/doba_behu);
		Print("| Vydaje rocni prumer:\t%i,- Kc\t\t\t\t|\n", naklady_celkem);
		Print("| Skody + vydaje rocne:\t%i,- Kc\t\t\t\t|\n", (skody_celkem/doba_behu + naklady_celkem));
		Print("-----------------------------------------------------------------\n");
		Print("| Pocet vozidel s dojezdem pres 20 minut:\t%i\t\t|\n", cc);
		Print("| Procento vozidel s dojezdem pres 20 minut:\t%.2f%\t\t|\n", (cc/(double)(cnt[2]+cnt[0]+cnt[1]))*100);
		Print("| Vyuziti jine nez nejblizsi stanice:\t\t%i\t\t|\n", vyuzito_jine);
		Print("| Procento vyuziti jine nez nejblizsi stanice:\t\t%.2f%\t\t|\n", (vyuzito_jine/(double)(cnt[2]+cnt[0]+cnt[1]))*100);
		Print("-----------------------------------------------------------------\n");
	}
	
	if(detailed == 2)
	{
		for(int i = 0; i<3;i++)
		{
			Print("| TYP %i\t\t\t\t\t\t\t\t|\n", i+1);
			Print("| Pocet pozaru:\t\t\t%i\t\t\t\t|\n", cnt[i]);
			Print("| Maximalni skody:\t\t%lu,- Kc\t\t\t|\n", max_skody[i]);
			Print("| Minimalni skody:\t\t%lu,- Kc\t\t\t|\n", min_skody[i]);
			Print("| Prumerne skody:\t\t%lu,- Kc\t\t\t|\n", (celkem[i]/cnt[i]));
			Print("| Celkove skody zpusobene timto typem pozaru:\t%i,- Kc\t|\n", celkem[i]);
			Print("| Podil na celkovych skodach:\t%.2f%\t\t\t\t|\n",100*((double)celkem[i]/(double)skody_celkem));
			Print("| Maximalni doba haseni pozaru:\t%i:%02i\t\t\t\t|\n", maxtime[i]/60, maxtime[i]%60);
			Print("| Minimalni doba haseni pozaru:\t%i:%02i\t\t\t\t|\n", mintime[i]/60, mintime[i]%60);
			Print("| Prumerna doba haseni pozaru:\t%i:%02i\t\t\t\t|\n", (celkemtime[i]/cnt[i])/60, (celkemtime[i]/cnt[i])%60);
			Print("| Maximalni doba trvani pozaru:\t%i:%02i\t\t\t\t|\n", maxtime2[i]/60, maxtime2[i]%60);
			Print("| Minimalni doba trvani pozaru:\t%i:%02i\t\t\t\t|\n", mintime2[i]/60, mintime2[i]%60);
			Print("| Prumerna doba trvani pozaru:\t%i:%02i\t\t\t\t|\n", (celkemtime2[i]/cnt[i])/60, (celkemtime2[i]/cnt[i])%60);
			Print("| Nejdelsi doba prijezdu prvni jednotky:\t%.2f min\t|\n", maxtime_dojezd[i]);
			Print("-----------------------------------------------------------------\n");
		}
	}
	
	if(detailed == 3)
	{
		cout << "-----------------------------------------------------------------" << endl;
		cout << "| Pocet stanic:\t\t" << pocet_stanic << "\t\t\t\t\t|" << endl;
		cout << "| Zkoumane obdobi:\t" << doba_behu << " rok/u\t\t\t\t\t|" << endl;
		cout << "-----------------------------------------------------------------" << endl;
		cout << "| Pozaru celkem:\t" << (cnt[2]+cnt[0]+cnt[1]) << "\t\t\t\t\t|" << endl;
		cout << "| Skody celkem:\t\t" << skody_celkem << ",- Kc\t\t\t\t|" << endl;
		cout << "| Vydaje celkem:\t" << naklady_celkem*doba_behu << ",- Kc\t\t\t\t|" << endl;
		cout << "| Skody + vydaje:\t" << skody_celkem + naklady_celkem*doba_behu << ",- Kc\t\t\t\t|" << endl;
		cout << "-----------------------------------------------------------------" << endl;
		cout << "| Pozaru rocni prumer:\t" << (cnt[2]+cnt[0]+cnt[1])/doba_behu << "\t\t\t\t\t|" << endl;
		cout << "| Skody rocni prumer:\t" << skody_celkem/doba_behu << ",- Kc\t\t\t\t|" << endl;
		cout << "| Vydaje rocni prumer:\t" << naklady_celkem << ",- Kc\t\t\t\t|" << endl;
		cout << "| Skody + vydaje rocne:\t" << (skody_celkem/doba_behu + naklady_celkem) << ",- Kc\t\t\t\t|" << endl;
		cout << "-----------------------------------------------------------------" << endl;
		cout << "| Pocet vozidel s dojezdem pres 20 minut:\t" << cc << "\t\t|" << endl;
		cout << "| Vyuziti jine nez nejblizsi stanice:\t\t" << vyuzito_jine << "\t\t|" << endl;
			
		/*
		cout << "-----------------------------------------------------------------" << endl;
		cout << "| Pocet stanic:\t\t" << pocet_stanic << "\t\t\t\t\t|" << endl;
		cout << "| Zkoumane obdobi:\t" << doba_behu << " rok/u\t\t\t\t\t|" << endl;
		cout << "| Skody rocni prumer:\t" << skody_celkem/doba_behu << ",- Kc\t\t\t\t|" << endl;
		cout << "| Vydaje rocni prumer:\t" << naklady_celkem << ",- Kc\t\t\t\t|" << endl;
		cout << "| Skody + vydaje rocne:\t" << (skody_celkem/doba_behu + naklady_celkem) << ",- Kc\t\t\t\t|" << endl;
		cout << "| Pocet vozidel s dojezdem pres 20 minut:\t" << cc << "\t\t|" << endl;
		cout << "| Vyuziti jine nez nejblizsi stanice:\t\t" << vyuzito_jine << "\t\t|" << endl;
		*/
	}
	
	if(detailed == 4)
	{
		cout << "-----------------------------------------------------------------" << endl;
		cout << "| Pocet stanic:\t\t" << pocet_stanic << "\t\t\t\t\t|" << endl;
		cout << "| Procento vozidel s dojezdem pres 20 minut:\t" << (cc/(double)(cnt[2]+cnt[0]+cnt[1]))*100 << "\t\t|" << endl;
		cout << "| Procento vyuziti jine nez nejblizsi stanice:\t\t" << (vyuzito_jine/(double)(cnt[2]+cnt[0]+cnt[1]))*100 << "\t\t|" << endl;
		cout << "| Prumerna doba zasahu: " << ((dobazasahu[0]+dobazasahu[1]+dobazasahu[2])/(cnt[0]+cnt[1]+cnt[2]))/60 << ":" << ((dobazasahu[0]+dobazasahu[1]+dobazasahu[2])/(cnt[0]+cnt[1]+cnt[2]))%60 << endl;
	}
	
	for(int i = 0; i < pocet_stanic; i++)
	{
		delete seznam_stanic[i];
	}
	
    return 0;
	
}
