/**
 * @file huff_codec.hpp
 *
 * @author David Koleckar <xkolec07@stud.fit.vutbr.cz>
 *
 * @date 27.2.2019
 *
 * @brief Hlavickovy soubor pro HUFF_CODEC.
 *
 */

#ifndef HUFF_CODEC_HPP
#define HUFF_CODEC_HPP

//knihovny
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <bits/stdc++.h> 
#include <string>
#include <chrono>

//konstanta
#define TAG 500

//chybove kody
enum ecodes{
	EARG=0,     // Chyba argumentu
	EUNKNOWN,   // Neznama chyba
	EREAD,  // Chyba cteni
	EWRITE  // Chyba pri zapisu
};
//struktura minheapu (huffman strom)
struct hufNode{ 
    int data; 
    unsigned freq; 
    hufNode *left, *right; 
    hufNode(int data, unsigned freq){ 
        left = right = NULL; 
        this->data = data; 
        this->freq = freq; 
    } 
}; 
  
//Porovnani dvou heap uzlu
struct compare{ 
    bool operator()(hufNode* l, hufNode* r){ 
        return (l->freq > r->freq); 
    } 
}; 

//Vytvoreni minheapu a vlozeni vsech dat
std::priority_queue<hufNode*, std::vector<hufNode*>, compare> minHeap;
//Mapovani hodnoty RGB na huffman kod
std::map<int, std::string> codes;

/**
 * Funkce pro vypis chyby
 *
 * @param ecode Typ chyby.
 */
void printEcode(int ecode);

/**
 * Funkce pro vypocet histogramu obrazku
 *
 * @param rgb Vektor s hodnotami RGB barev
 * @param freq Vektor s frekvenci jednotlivych RGB barev
 * @param data Vektor se vstupnim obrazkem
 */
void makeHistogram(std::vector<int> &rgb, std::vector<int> &freq, std::vector<int> &data);

/**
 * Funkce pro vypocet modelu 
 * Model = diference sousednich pixelu
 *
 * @param width Sirka obrazku
 * @param rgb Vektor s hodnotami RGB barev
 * @param freq Vektor s frekvenci jednotlivych RGB barev
 * @param data Vektor se vstupnim obrazkem
 */
void encodeModel(int width, std::vector<int> &rgb, std::vector<int> &freq, std::vector<int> &data);

/**
 * Funkce pro vypocet reversniho modelu 
 * Model = diference sousednich pixelu
 *
 * @param width Sirka obrazku
 * @param data Vektor se vstupnim obrazkem
 */
void decodeModel(int width, std::vector<int> &data);

/**
 * Funkce pro ulozeni huffmanova kodu
 *
 * @param root Koren huffmanova stromu 
 * @param str Retezec reprezentujici huffmanuv kod
 */
void storeCodes(struct hufNode* root, std::string str);

/**
 * Funkce pro vytvoreni huffmanova stromu
 *
 * @param data Pole s RGB hodnotami v obraze
 * @param freq Pole s frekvenci jednotlivych RGB barev v obraze
 * @param size Velikost Pole s RGB hodnotami
 */
void makeHuffTree(int *data, int *freq, int size);

/**
 * Funkce pro dekodovani huffmanova kodu 
 * Vysledek uklada do vektoru data
 *
 * @param root Koren huffmanova stromu 
 * @param s Retezec reprezentujici huffmanuv kod
 * @param data Vektor pro vysledek, ukladaji se zde jednotlive RGB hodnoty obrazy
 */
void decodeHuffString(struct hufNode* root, std::string s, std::vector<int> &data);

/**
 * Funkce pro serazeni frekvence vyskytu jednotlivych RGB barev
 * Od nejmensi frekvence po nejvetsi
 * Funkce vyuzita pri tvorbe histogramu
 * https://www.geeksforgeeks.org/sorting-vector-of-pairs-in-c-set-1-sort-by-first-and-second/
 *
 * @param a Pole s cetnosti jednotlivych barev
 * @param b Pole s jednotlivymi barvami v obraze
 * @param n Velikost pole a
 */
void sortPair(int *a, int *b, int n);

/**
 * Funkce pro cteni souboru ve formatu .RAW
 * Funkce spocita histogram a ulozi data do vektoru
 *
 * @param width Sirka obrazku
 * @param filename Nazev vstupniho souboru
 * @param rgb Vektor s hodnotami RGB barev
 * @param freq Vektor s frekvenci jednotlivych RGB barev
 * @param data Vektor se vstupnim obrazkem
 */
void readRawFile(int width, std::string filename, std::vector<int> &rgb, std::vector<int> &freq, std::vector<int> &data);

/**
 * Funkce pro ulozeni kodovaneho obrazku
 * Ulozi dvojici RGB:FREKVENCE + cely huffmanuv kod obrazku
 *
 * @param filename Nazev vystupniho souboru
 * @param encodedString Huffmanovu kod obrazu
 * @param rgb Vektor s hodnotami RGB barev
 * @param freq Vektor s frekvenci jednotlivych RGB barev
 */
void saveEncode(std::string filename, std::string encodedString, std::vector<int> rgb, std::vector<int> freq, int width);

/**
 * Funkce pro ulozeni dekodovaneho obrazku
 * Ulozi jednotlive hodnoty RGB (8bit hodnota)
 *
 * @param filename Nazev vystupniho souboru
 * @param data Vektor s jednotlivymi dekodovanymi hodnotami obrazu
 */
void saveDecode(std::string filename, std::vector<int> data);

/**
 * Funkce pro cteni kodovaneho souboru
 * Ulozi jednotlive hodnoty RGB a ferkvenci vyskytu do vektoru
 *
 * @return Retezec s huffmanovym kodem obrazu
 * @param filename Nazev vstupniho zakodovaneho souboru
 * @param rgb Vektor s hodnotami RGB barev
 * @param freq Vektor s frekvenci jednotlivych RGB barev
 */
std::string readEncode(std::string filename, std::vector<int> &rgb, std::vector<int> &freq, uint *width);

#endif