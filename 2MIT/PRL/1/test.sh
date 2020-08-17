#!/bin/bash

numproc=1
m=0
sizebucket=0
input=1

#funkce pro pocitani logaritmu pri zakladu dva
function log2() { 
	local x=0 
	for (( y=$1-1 ; $y > 0; y >>= 1 )) ; do 
		let x=$x+1 
	done 
	echo $x
}

#kontrola parametru
if [ "$1" = "0" ]; then #pokud parametr je 0
	echo "Chyba parametr musi byt vetsi jak 0"
	exit 1
elif [ "$#" -eq 1 ]; then #jeden parametr
	input=$1
elif [ "$#" -gt 1 ]; then #vice jak jeden parametr
	echo "Chyba mnoho parametru"
	exit 1
else #zadny parametr - default hodnota
	input=32
fi
#pokud je pocet cisel vetsi jak 1 dopocitam pocet procesoru
if [ $input -ne 1 ]; then
	loginput=$(log2 $input)
	#zaokrouhleni m na nejblizsi mocninu dvojky
	m=`python -c "print 2**($loginput-1).bit_length();"`
	#echo "m " $m
	#vypocet poctu procesoru
	numproc=$((2*$m-1))
	#echo "p "$numproc
fi

#vyrobeni souboru s random cisly
dd if=/dev/random bs=1 count=$input of=numbers 2>/dev/null

#preklad cpp zdrojaku
mpic++ --prefix /usr/local/share/OpenMPI -o bks bks.cpp

#spusteni
mpirun --prefix /usr/local/share/OpenMPI -np $numproc bks

#uklid
rm -f bks numbers