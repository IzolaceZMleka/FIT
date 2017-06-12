// Mereni rychlosti strcpy vs. memcpy

#include "../main-files/builtin_func.h"
#include <stdio.h>
#include <string.h>

#define DEFAULT_ITERATIONS 20000000

// #define PIDI_DELKA // 5 B
// #define KRATKA_DELKA // 27 B
#define STREDNI_DELKA // 105 B

#ifdef PIDI_DELKA
	Tstring testString1 = {
		4,
		"zyxa"
	};

	Tstring testString2 = {
		4,
		"abcd"
	};

	Tstring testString3 = {
		4,
		"defg" // je jinsi
	};
#elif defined(KRATKA_DELKA)
	Tstring testString1 = {
		26,
		"zyxwvutsrqponmlkjihgfedcba"
	};

	Tstring testString2 = {
		26,
		"abcdefghijklmnopqrstuvwxyz"
	};

	Tstring testString3 = {
		26,
		"hijklmnopqrstuvwxyzabcdefg" // je jinsi
	};
#elif defined(STREDNI_DELKA)
	Tstring testString1 = {
		26*4,
		"zyxwvutsrqponmlkjihgfedcbazyxwvutsrqponmlkjihgfedcbazyxwvutsrqponmlkjihgfedcbazyxwvutsrqponmlkjihgfedcba"
	};

	Tstring testString2 = {
		26*4,
		"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
	};

	Tstring testString3 = {
		26*4,
		"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzzyxwvutsrqponmlkjihgfedcba" // je jinsi
	};
#endif

inline void benchmarkedCode(void) {
	if(1) {
		memcpy(testString3.str, testString1.str, testString1.len + 1);
		memcpy(testString1.str, testString2.str, testString2.len + 1);
		memcpy(testString2.str, testString3.str, testString3.len + 1);
	} else {
		strcpy(testString3.str, testString1.str);
		strcpy(testString1.str, testString2.str);
		strcpy(testString2.str, testString3.str);
	}
}

// Vysledky Velda:
// pidi string memcpy 0,50 s, strcpy 0,50 s
// kratky string memcpy 0,75 s, strcpy 0,93 s
// stredni string memcpy 1,55 s, strcpy 2,75 s
//
// zapnuti optimalizaci nevyrazne vylepsi strcpy pro pidi string a take memcpy pro stredni string