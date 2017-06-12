#!/usr/bin/env bash

make clean
make

HOST='eva.fit.vutbr.cz'

echo "-----SPUSTENI SERVERU S NAHODNYM PORTEM-----"
RUN=true
PORT=$(jot -r 1 2000 65000)
WPORT=$(jot -r 1 2000 65000)
cd server_dir
./server -p $PORT & SERVERID=$! 
pgrep "server" | grep "$SERVERID" >/dev/null
echo "Server je spusten. ID = $SERVERID"

cd ../client_dir

echo -e "\n\e[34m-----TEST 1-----" 
echo -e "NAHRANI MALEHO SOUBORU\e[0m"
TESTFILE="test1"
echo "TOTO JE TESTOVACI SOUBOR CISLO1" >> $TESTFILE

./client -h $HOST -p $PORT -u $TESTFILE
if [ $? = 0 ]; then
	echo -e "\e[32mPrenos byl uspesny.\e[0m Nasleduje kontrola diffem."
	diff $TESTFILE ../server_dir/$TESTFILE
	if [ $? = 0 ]; then
		echo -e "\e[32mSoubory jsou stejne.\e[0m"
	else 
		echo -e "\e[31mSoubory se lisi.\e[0m"
	fi
else 
	echo -e "\e[31mPrenos se nezdaril.\e[0m"
fi


rm $TESTFILE
rm ../server_dir/$TESTFILE

echo -e "\n\e[34m-----TEST 2-----" 
echo "STAZENI MALEHO SOUBORU"
TESTFILE="test2"
echo "TOTO JE TESTOVACI SOUBOR CISLO2" >> ../server_dir/$TESTFILE

./client -h $HOST -p $PORT -d $TESTFILE
if [ $? = 0 ]; then
	echo -e "\e[32mPrenos byl uspesny.\e[0m Nasleduje kontrola diffem."
	diff $TESTFILE ../server_dir/$TESTFILE
	if [ $? = 0 ]; then
		echo -e "\e[32mSoubory jsou stejne.\e[0m"
	else 
		echo -e "\e[31mSoubory se lisi.\e[0m"
	fi
else 
	echo -e "\e[31mPrenos se nezdaril.\e[0m"
fi

rm $TESTFILE
rm ../server_dir/$TESTFILE


echo -e "\n\e[34m-----TEST 3-----" 
echo "STAZENI SOUBORU VE FORMATU PNG"
TESTFILE="download.png"

./client -h $HOST -p $PORT -d $TESTFILE
if [ $? = 0 ]; then
	echo -e "\e[32mPrenos byl uspesny.\e[0m Nasleduje kontrola diffem."
	diff $TESTFILE ../server_dir/$TESTFILE
	if [ $? = 0 ]; then
		echo -e "\e[32mSoubory jsou stejne.\e[0m"
	else 
		echo -e "\e[31mSoubory se lisi.\e[0m"
	fi
else 
	echo -e "\e[31mPrenos se nezdaril.\e[0m"
fi

rm $TESTFILE

echo -e "\n\e[34m-----TEST 4-----" 
echo "NAHRANI SOUBORU VE FORMATU JPG"
TESTFILE="upload.jpg"

./client -h $HOST -p $PORT -u $TESTFILE
if [ $? = 0 ]; then
	echo -e "\e[32mPrenos byl uspesny.\e[0m Nasleduje kontrola diffem."
	diff $TESTFILE ../server_dir/$TESTFILE
	if [ $? = 0 ]; then
		echo -e "\e[32mSoubory jsou stejne.\e[0m"
	else 
		echo -e "\e[31mSoubory se lisi.\e[0m"
	fi
else 
	echo -e "\e[31mPrenos se nezdaril.\e[0m"
fi

rm ../server_dir/$TESTFILE


echo -e "\n\e[34m-----TEST 5-----" 
echo -e "STAZENI VELKEHO SOUBORU\e[0m"
TESTFILE="large_file_to_download"
dd if=/dev/urandom of=../server_dir/$TESTFILE bs=1 count=0 seek=10M

./client -h $HOST -p $PORT -d $TESTFILE
if [ $? = 0 ]; then
	echo -e "\e[32mPrenos byl uspesny.\e[0m Nasleduje kontrola diffem."
	diff $TESTFILE ../server_dir/$TESTFILE
	if [ $? = 0 ]; then
		echo -e "\e[32mSoubory jsou stejne.\e[0m"
	else 
		echo -e "\e[31mSoubory se lisi.\e[0m"
	fi
else 
	echo -e "\e[31mPrenos se nezdaril.\e[0m"
fi

rm $TESTFILE
rm ../server_dir/$TESTFILE

echo -e "\n\e[34m-----TEST 6-----" 
echo -e "NAHRANI VELKEHO SOUBORU\e[0m"
TESTFILE="large_file_to_upload"
dd if=/dev/urandom of=$TESTFILE bs=1 count=0 seek=10M

./client -h $HOST -p $PORT -u $TESTFILE
if [ $? = 0 ]; then
	echo -e "\e[32mPrenos byl uspesny.\e[0m Nasleduje kontrola diffem."
	diff $TESTFILE ../server_dir/$TESTFILE
	if [ $? = 0 ]; then
		echo -e "\e[32mSoubory jsou stejne.\e[0m"
	else 
		echo -e "\e[31mSoubory se lisi.\e[0m"
	fi
else 
	echo -e "\e[31mPrenos se nezdaril.\e[0m"
fi

rm $TESTFILE
rm ../server_dir/$TESTFILE

echo -e "\n\e[34m-----TEST 7-----" 
echo "NAHRANI SOUBORU MIMO SLOZKU, KDE SE NACHAZI KLIENT"
TESTFILE="upload_from_dif_dir"
echo "TOTO JE TESTOVACI SOUBOR PRO UPLOAD Z JINE SLOZKY" >> ../$TESTFILE

./client -h $HOST -p $PORT -u ../$TESTFILE
if [ $? = 0 ]; then
	echo -e "\e[32mPrenos byl uspesny.\e[0m Nasleduje kontrola diffem."
	diff ../$TESTFILE ../server_dir/$TESTFILE
	if [ $? = 0 ]; then
		echo -e "\e[32mSoubory jsou stejne.\e[0m"
	else 
		echo -e "\e[31mSoubory se lisi.\e[0m"
	fi
else 
	echo -e "\e[31mPrenos se nezdaril.\e[0m"
fi

rm ../$TESTFILE
rm ../server_dir/$TESTFILE

echo -e "\n\e[34m-----TEST 8-----" 
echo "PRACE SE SYMBOLICKYMI ODKAZY NAHRANI SOBORU PREDANEHO SYMBOLICKYM ODKAZEM"
TESTFILE="symbolic_file"
echo "TOTO JE TESTOVACI SOUBOR PRO SYMBOLICKY ODKAZ" >> ../$TESTFILE
ln -s ../$TESTFILE symbolic_link

./client -h $HOST -p $PORT -u symbolic_link
if [ $? = 0 ]; then
	echo -e "\e[32mPrenos byl uspesny.\e[0m Nasleduje kontrola diffem."
	diff ../$TESTFILE ../server_dir/$TESTFILE
	if [ $? = 0 ]; then
		echo -e "\e[32mSoubory jsou stejne.\e[0m"
	else 
		echo -e "\e[31mSoubory se lisi.\e[0m"
	fi
else 
	echo -e "\e[31mPrenos se nezdaril.\e[0m"
fi

rm ../$TESTFILE
rm ../server_dir/$TESTFILE
rm symbolic_link

echo -e "\n\e[34m-----TEST 9-----" 
echo -e "SPATNE ZADANE PARAMETRY\e[0m"

./client spatne_parametry
if [ $? = 0 ]; then
	echo -e "\e[31mChyba nebyla detekovana."
else 
	echo -e "\e[32mSpatne zadane parametry. \e[32mTest probehl uspesne.\e[0m"
fi

echo -e "\n\e[34m-----TEST 10-----" 
echo -e "PRIPOJENI K SPATNEMU PORTU\e[0m"

./client -h $HOST -p $WPORT -u random_file
if [ $? = 0 ]; then
	echo -e "\e[31mChyba nebyla detekovana."
else 
	echo -e "\e[32mSpatny port. \e[32mTest probehl uspesne.\e[0m"
fi

echo -e "\n\e[34m-----TEST 11-----" 
echo -e "ZADANY HOST NEEXISTUJE\e[0m"

./client -h random_host -p $WPORT -u random_file
if [ $? = 0 ]; then
	echo -e "\e[31mChyba nebyla detekovana."
else 
	echo -e "\e[32mSpatny host. \e[32mTest probehl uspesne.\e[0m"
fi

sleep 1
kill -9 $SERVERID
