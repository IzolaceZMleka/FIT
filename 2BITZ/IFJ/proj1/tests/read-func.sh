#!/bin/bash
#make clean
#make tests
#make

allOk=true

printf "\n--------TEST readInteger--------\n"
function testos {
	output="$(echo $2 |./read-func $1)"
	if [ "$output" == "$3" ]; then
		return;
	else
		echo "Chyba: vstup = $2, vystup = $output";
		allOk=false;
	fi

}

function errorson {
	echo $2 |./read-func $1 &> /dev/null
	if [ $? == 7 ]; then
		return;
	else
		echo "testovano $2: Ocekava se chybny vystup, ale neprisel";
		allOk=false;
	fi
}

function multi {
	output="$(echo $3 |./read-func $1 $2)"
	if [ "$output" == "$4" ]; then
		return;
	else
		echo "Chyba: vstup = $3, vystup = $output";
		allOk=false;
	fi

}

testos		"int"	"45"		"45"		# obycejny vstup
testos		"int"	"   45"		"45"		# mezery na zacatku
testos		"int"	"45   "		"45"		# mezezery na konci
testos		"int"	"5 4 4"		"5"			# vyber prvni
testos		"int"	"0"			"0"			# overeni 0
testos		"int"	"0005"		"5"			# overeni 000..
testos		"int"	"45a"		"45"		# pismeno
testos		"int"	"3.5"		"3"			# desetinne cislo

# chyby
errorson	"int"	""		 				# zadny vstup
errorson	"int"	"ahoj jak"				# zadne cislo
errorson	"int"	"ahoj3.5"
errorson	"int"	"asd5"
errorson	"int"	"-5"					# zaporne cislo


if [ "$allOk" = true ] ; then
	echo "int is Ok"
fi
printf "\n--------TEST konec readInteger--------\n"
printf "\n--------TEST readDouble--------\n"
testos		"double"	"0.1"			"0.1"			# .
testos		"double"	"0.0e0"			"0"			
testos		"double"	"0.1e0"			"0.1"			
testos		"double"	"0.1e-1"		"0.01"			
testos		"double"	"0.1e+1"		"1"			
testos		"double"	"0.1e1"			"1"			
testos		"double"	"5e+1"			"50"			
testos		"double"	"5e1"			"50"
testos		"double"	"5E-2"			"0.05"
testos		"double"	"1234.4E-2"		"12.344"
testos		"double"	"	 1234.4E-2 "		"12.344"
testos		"double"	"1234.4E-2  124"		"12.344"
testos		"double"	"0004.0E2"			"400"
testos		"double"	"5.3g"		"5.3"
testos		"double"	"1.5-1"		"1.5"			

errorson	"double"	"123"				# musi byt desetinne
errorson	"double"	"1.5e+-1"			
errorson	"double"	"1.5E--1"			
errorson	"double"	"1.5E-+1"			
errorson	"double"	"ahoj"			
errorson	"double"	"1.5ee1"			
errorson	"double"	""			
errorson	"double"	"eE5E0"	
errorson	"double"	".5e+1"						
errorson	"double"	"ahoj5.3g"
errorson	"double"	"-5.1"		

if [ "$allOk" = true ] ; then
	echo "double is Ok"
fi

printf "\n--------TEST konec readDouble--------\n"

printf "\n--------TEST readString--------\n"
testos		"string"	""				"" #prazdny retezec je validni 
testos		"string"	"   ahoj"		"ahoj" 
testos		"string"	"+-lol"			"+-lol" 
testos		"string"	"	@#$%^"		"@#$%^" 
testos		"string"	"lol ahoj"		"lol"
testos		"string"	"agent47"		"agent47"
testos		"string"	"ahoj   "		"ahoj"
testos		"string"	"1234567890123456789012"	"1234567890123456789012" #realloc

if [ "$allOk" = true ] ; then
	echo "string is Ok"
fi
printf "\n--------TEST konec readString--------\n"
printf "\n--------TEST kombinace--------\n"
multi		"int"		"string"	"45ahoj"		"45ahoj"
multi		"double"	"string"	"45.5ahoj"		"45.5ahoj"	
multi		"double"	"string"	"45.5E1ahoj"	"455ahoj"
multi		"double"	"string"	"45.5E+1ahoj"	"455ahoj"
multi		"double"	"string"	"45E+1ahoj"		"450ahoj"
multi		"string"	"int"		"ahoj 45"		"ahoj45"
if [ "$allOk" = true ] ; then
	echo "kombinace is Ok"
fi
printf "\n--------TEST konec kombinace--------\n"
