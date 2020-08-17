#!/bin/bash

input=""
numproc=0
#kontrola parametru
if [ "$#" -eq 1 ]; then #jeden parametr
	input=$1
elif [ "$#" -gt 1 ]; then #vice jak jeden parametr
	echo "Chyba mnoho parametru"
	exit 1
else
	echo "Chyba test.sh PARAMETER"
	exit 1	
fi

#dopocitani parametru
len=${#input}
numproc=$((2*len-2))

#preklad cpp zdrojaku
mpic++ --prefix /usr/local/share/OpenMPI -o vuv vuv.cpp

#spusteni
mpirun --prefix /usr/local/share/OpenMPI -np $numproc vuv $input

#uklid
rm -f vuv