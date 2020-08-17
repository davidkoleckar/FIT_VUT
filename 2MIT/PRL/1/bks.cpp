/**
 * @file bks.cpp
 *
 * @author David Koleckar <xkolec07@stud.fit.vutbr.cz>
 *
 * @date 27.3.2019
 *
 * @brief Paralelni radici algoritmus Bucket sort
 */
#include <mpi.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h> 
#include <algorithm>
#include <math.h>

using namespace std;
#define SENDLEAF 0
#define SENDNODE 1
const string FILENAME = "numbers";

/**
 * Funkce pro zaokrouhleni na nejblizsi mocninu dvojky
 * Funkce prevzata a upravena z https://www.techiedelight.com/round-next-highest-power-2/
 * @param n Hodnota pro zaokrouhleni
 * @return Zaokrouhlenou hodnotu
 */
unsigned int nextPowerOf2(int n){
	// decrement n (to handle the case when n itself 
	// is a power of 2)
	if (n == 1)
		return 2;
	n = n - 1;
	// do till only one bit is left
	while (n & n - 1)
	    n = n & n - 1;  // unset rightmost bit
	// n is now a power of two (less than n)
	// return next power of 2
	return n << 1;
}

/**
 * Funkce pro vypocet rodicovskeho uzlu(procesoru)
 * @param id Rank aktualniho uzlu(procesoru)
 * @return Rank rodicovskeho uzlu(procesoru)
 */
unsigned int parentNode(unsigned int id){
	return (id-1)/2;
}

/**
 * Funkce pro vypocet leveho syna uzlu(procesoru)
 * @param id Rank aktualniho uzlu(procesoru)
 * @return Rank leveho syna uzlu(procesoru)
 */
unsigned int leftChild(unsigned int id){
	return (id*2)+1;
}

/**
 * Funkce pro vypocet praveho syna uzlu(procesoru)
 * @param id Rank aktualniho uzlu(procesoru)
 * @return Rank praveho syna uzlu(procesoru)
 */
unsigned int rightChild(unsigned int id){
	return (id*2)+2;
}

/**
 * Hlavni funkce programu
 * @param argc, argv
 * @return EXIT_SUCCESS
 */
int main(int argc, char *argv[]){
	int numprocs, myid, m, sizeleaf;
	int n=0;
	vector<int> numbers, leftTmp, rightTmp, leaf;

	MPI_Status stat;
	MPI_Init(&argc,&argv); // inicializace MPI 
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs); // zjistíme, kolik procesů běží
	MPI_Comm_rank(MPI_COMM_WORLD,&myid); // zjistíme id svého procesu

	//nacteni souboru (inspirace kodu s ukazek odd-even sort)
	if(myid == 0){
	    int number;  //hodnota nacteneho cisla
	    fstream fin; 
	    fin.open(FILENAME.c_str(), ios::in);                   

	    while(fin.good()){
	        number = fin.get();
	        if(!fin.good()) break;
	        numbers.push_back(number);
	        if(n == 0)
	        	printf("%u", number);
	        else
	        	printf(" %u", number);
	        n++;
	    }
	    printf("\n");
	    fin.close();

	    //pokud n je rovno 1 tak jen tisk prvku
	    if(n == 1){
	    	printf("%d\n", numbers.front());
	    	MPI_Finalize(); 
	    	return EXIT_SUCCESS;
	    }
	    //pokud n je rovno 2 tak jen jeden procesor seradi a vypise hodnoty
	    else if(n == 2){
	    	std::sort(numbers.begin(), numbers.end());
	    	for (auto x : numbers) 
				printf("%d\n", x);
	    	MPI_Finalize(); 
	    	return EXIT_SUCCESS;
	    }
	}

	//Vypocet parametru algortimu
	MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
	m = nextPowerOf2(ceil(log2(n)));
	sizeleaf = (int) ceil((float)n/m);

	//ROOT NODE
	if(myid == 0){
		//printf("m %d n %d sizeleaf %d logm %d \n", m, n, sizeleaf, logm);
		//doplneni pole
		for (int i = 0; i < (sizeleaf*m-n); ++i){
			numbers.push_back(-1);
		}
		//double start = MPI_Wtime();
		//odeslani casti vectoru k listum
		int j = 0;
		for(int i=numprocs-m;i<numprocs;i++){
			MPI_Send(&numbers[j*sizeleaf], sizeleaf, MPI_INT, i, SENDLEAF, MPI_COMM_WORLD);
			j++;
		}

		leftTmp.resize((sizeleaf*m)/2);
		rightTmp.resize((sizeleaf*m)/2);
		MPI_Recv(&leftTmp[0], (sizeleaf*m)/2, MPI_INT, leftChild(myid), SENDNODE, MPI_COMM_WORLD, &stat);
		MPI_Recv(&rightTmp[0], (sizeleaf*m)/2, MPI_INT, rightChild(myid), SENDNODE, MPI_COMM_WORLD, &stat);
		numbers.clear();
		std::merge(leftTmp.begin(), leftTmp.end(), rightTmp.begin(), rightTmp.end(), std::back_inserter(numbers));
		//smazani pridanych hodnot
		numbers.erase(numbers.begin() + 0, numbers.begin() + (sizeleaf*m)-n);
		/*double end = MPI_Wtime();
		std::cout << end - start;*/
		//vypis serazene posloupnosti
		for (auto x : numbers) 
			printf("%d\n", x);
    }
    //LEAFS NODE
    else if(myid >= numprocs-m && myid < numprocs){
		//dostaneme zpravu od procesu s rankem 0: 
		leaf.resize(sizeleaf);
		MPI_Recv(&leaf[0], sizeleaf, MPI_INT, 0, SENDLEAF, MPI_COMM_WORLD, &stat);
		//serazeni hodnot
		std::sort(leaf.begin(), leaf.end()); 
		//poslani uzlum
		MPI_Send(&leaf[0], sizeleaf, MPI_INT, parentNode(myid), SENDNODE, MPI_COMM_WORLD);
    }
    //NODES
    else{
		for(int j = 1; j < log2(m); ++j){//urovne stromu
			int sizeNode = (sizeleaf*pow(2, j-1));
			int startproces = pow(2, log2(m)-j)-1;
			int endproces = pow(2,log2(m)-j)*2-1;
			//vsechny procesy na dane urovni 
			if(myid >= startproces && myid < endproces){
				//zmena velikosti vectoru
				leftTmp.resize(sizeNode);
				rightTmp.resize(sizeNode);
				//recv
				MPI_Recv(&leftTmp[0], sizeNode, MPI_INT, leftChild(myid), SENDNODE, MPI_COMM_WORLD, &stat);
				MPI_Recv(&rightTmp[0], sizeNode, MPI_INT, rightChild(myid), SENDNODE, MPI_COMM_WORLD, &stat);
				vector<int> result;
				//merge hodnot
				std::merge(leftTmp.begin(), leftTmp.end(), rightTmp.begin(), rightTmp.end(), std::back_inserter(result));
				//poslani rodici
				MPI_Send(&result[0], sizeNode*2, MPI_INT, parentNode(myid), SENDNODE, MPI_COMM_WORLD);
			}
		} //end for
    } //end else

    MPI_Finalize(); 
    return EXIT_SUCCESS;
}//end main

