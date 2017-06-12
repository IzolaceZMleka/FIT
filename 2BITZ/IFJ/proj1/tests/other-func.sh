#!/bin/bash
make clean
make tests

printf "\n--------------------------\n"
printf "\n--------TEST error--------\n"
for i in {1..2}
do
	./other-func $i
done

printf "\n--------TEST konec error--------\n"
