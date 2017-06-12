#include "../main-files/builtin_func.h"
#include <stdio.h>
#include <string.h>

#define DEFAULT_ITERATIONS 200

Tstring testString = {
	26*4,
	"zyxwvutsrqponmlkjihgfedcbazyxwvutsrqponmlkjihgfedcbazyxwvutsrqponmlkjihgfedcbazyxwvutsrqponmlkjihgfedcba"
};

inline void benchmarkedCode(void) {
	Tstring * str2 = sort(&testString);
	// puts(str2->str);
}