/**
 * @file vuv.cpp
 *
 * @author David Koleckar <xkolec07@stud.fit.vutbr.cz>
 *
 * @date 5.4.2019
 *
 * @brief Paralelni algoritmus pro vypocet urovne vrcholu stromu
 */
#include <mpi.h>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>

using namespace std;

//Struktura AdjencyListu
struct AdjList{
    int node;
    int edge;
    int reverse;
    int next;
};

/**
 * Funkce urci zda je hrana dopredna ci zpetna
 * @param i - index hrany, n - pocet uzlu
 * @return pravdivostni hodnota zda je hrana dopredna
 */
bool isForward(int i, int n){
    return i < n;
}

/**
 * Nalezeni reverse hrany v adjencylistu
 * @param index, vector<AdjList>
 * @return index reversni hrany v adjency listu
 */
int findReverse(int index, std::vector<AdjList> v){
    for(std::size_t i=0; i<v.size(); ++i) {
        if (v[i].edge == index){
            return i;
        }
    }
}

/**
 * Hlavni funkce programu
 * @param argc, argv
 * @return EXIT_SUCCESS
 */
int main(int argc, char *argv[]){
    //pocet procesu, id procesu
    int numprocs, myid;
    //pocet uzlu
    int n = strlen(argv[1]);
    //pocet hran
    int edgeSize = n*2-2;

    //Vytvoreni Adjency list 
    vector<AdjList> alist;
    for (int i = 1; i <= n; ++i){
        int parent = i/2;
        int leftChild = 2*i;
        int rightChild = 2*i+1;
        bool hasleft = leftChild <= n;
        bool hasright = rightChild <= n;
        bool hasparent = parent >= 1 && parent <= n;
        if(hasleft){
            AdjList item = {i,leftChild-1,leftChild+n-2,hasright || hasparent ? 1 : 0};
            alist.push_back(item);
            if(hasright){
                AdjList item = {i,rightChild-1,rightChild+n-2,hasparent ? 1 : 0};
                alist.push_back(item);
            }
            if (hasparent){
                AdjList item = {i,i+n-2,i-1,0};
                alist.push_back(item);
            }
        }
        if(!hasright && !hasleft){
            AdjList item = {i,i+n-2,i-1,0};
            alist.push_back(item);
        }
    }

    MPI_Status stat;
    MPI_Init(&argc,&argv); // inicializace MPI 
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs); // zjistíme, kolik procesů běží
    MPI_Comm_rank(MPI_COMM_WORLD,&myid); // zjistíme id svého procesu

    if(n == 2){
        if(myid == 0)
            printf("%c:%d,%c:%d\n", argv[1][0],0,argv[1][1],1);
        MPI_Finalize();
        return EXIT_SUCCESS;
    }
    else if(n == 1){
        if(myid == 0)
            printf("%c:%d\n", argv[1][0],0);
        MPI_Finalize(); 
        return EXIT_SUCCESS;      
    }

    //pole nasledniku
    int *euler = (int *) malloc(edgeSize * sizeof(int));
    //pole weights
    int *weights = (int *) malloc(edgeSize * sizeof(int));
    //pole result
    int *result = (int *) malloc(edgeSize * sizeof(int));
     
    //pocitani hran od 1 
    myid++;

    //Vypocet naslednika pro danou hranu pomoci adjency listu
    int reverse;
    if(isForward(myid,n))
        reverse = myid + n - 1;
    else
        reverse = myid - n + 1;
    int value;
    int in = findReverse(reverse,alist);
    if(alist.at(in).next != 0){
        value = alist.at(in+1).edge;
    }
    else{
        int from = alist.at(in).node;
        size_t tmp;
        for(std::size_t i=0; i<alist.size(); ++i) {
            if(alist[i].node == from){
                tmp = i;
                break;
            }
        }
        value = alist.at(tmp).edge;
    }
    if (myid == n+1)
        value = myid;
    MPI_Allgather(&value, 1, MPI_INT, euler, 1, MPI_INT, MPI_COMM_WORLD);

    //rozmisteni vah hranam
    int weight;
    if (isForward(myid,n))
        weight = -1;
    else
        weight = 1;
    MPI_Allgather(&weight, 1, MPI_INT, weights, 1, MPI_INT, MPI_COMM_WORLD);

    //Suffix sum
    int i = myid - 1;
    int sufSum = 0;
    while (euler[i] != i + 1){
        sufSum += weights[i];
        i = euler[i] - 1;
    }
    MPI_Allgather(&sufSum, 1, MPI_INT, result, 1, MPI_INT, MPI_COMM_WORLD);

    //Vypis
    if(myid == 1){
        printf("%c:%d", argv[1][myid-1],0);
        for (int i = 0; i < edgeSize/2; ++i){
            //vrchol
            int v = i + 2;
            //uroven
            int level = result[i] + 2;
            printf(",%c:%d", argv[1][v-1],level);
            if(v == n)
                printf("\n");
        }
    }
    //Smazani poli
    free(euler);
    free(weights);
    free(result);
    MPI_Finalize(); 
    return EXIT_SUCCESS;
}//end main

