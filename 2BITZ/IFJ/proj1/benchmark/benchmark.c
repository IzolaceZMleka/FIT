#include <stdio.h>
#include <time.h>

// direktiva prekladace v niz je ulozen "soubor" obsahujici 'benchmarkedCode' - mereny kod a pocet DEFAULT_ITERATIONS
#include BENC_TYPE

int main() {
	int i, iterCount = DEFAULT_ITERATIONS;
	
	printf("Zadejte cislo poctu iteraci. Enter pro pouziti vychozi hodnoty %d: ", iterCount);
	if((i = getc(stdin)) != '\n' /* ENTER */)
		ungetc(i, stdin),
		scanf("%d", &iterCount);
	puts("Start benchmarku:");
	
	clock_t time = clock();
	
	while(i++ < iterCount)
		benchmarkedCode();
	
	time = clock() - time;
	puts("Benchmark dokoncen!");
	printf("Doba trvani: %d OPS, %.2f s\n", time, time / (float) CLOCKS_PER_SEC);
	
	return 0;
 }