#include <stdio.h>
#include <math.h>
#include "../main-files/ial.h"
#define VARIANTS 7 // vsechny kombinace pro pole velikosti 1-VARIANTS

int main() {
	// základní test øazení
	puts("Pole: 5 7 8 5 1 2 5");
	char arr[] = {5, 7, 8, 5, 1, 2, 5};
	QuickSort(arr, 0, sizeof(arr) - 1);
	for(unsigned int l = 0; l < sizeof(arr); l++)
		printf("%d ", arr[l]);
	puts("");
	
	for(int i = 1; i <= VARIANTS; i++) {
		char arr[i], arr2[i];
		
		for(int j = 0; j < pow(i, i); j++) {
			for(int k = i - 1, n = j; k >= 0; k--) {
				arr[k] = arr2[k] = n % i;
				n /= i;
			}
			
			QuickSort(arr, 0, i-1);
			
			for(int l = 0; l < i - 1; l++)
				if(arr[l] > arr[l+1]) {
					puts("Spatne serazene pole: ");
					for(int l = 0; l < i; l++)
						printf("%d ", arr[l]);
					puts("");
					for(int l = 0; l < i; l++)
						printf("%d ", arr2[l]);
					puts("");
				}
		}
	}
	
	puts("\nTesty razeni OK!");
}