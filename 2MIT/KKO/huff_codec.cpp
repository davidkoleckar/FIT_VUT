/**
 * Projekt KKO
 * @file huff_codec.cpp
 *
 * @author David Koleckar <xkolec07@stud.fit.vutbr.cz>
 *
 * @date 27.2.2019
 *
 * @brief Komprese obrazových dat s využitím statického a adaptivního Huffmanova kódování
 */

#include "huff_codec.hpp"

using namespace std;

const char *ERRORMSG[] = {
	"Chyba parametru programu",
	"Neznama chyba",
	"Chyba pri cteni souboru",
	"Chyba pri zapisu souboru"
};
//tisk napovedy
void help(){
	printf("KKO projekt\n");
	printf("Komprese obrazovych dat s vyuzitim statickeho a adaptivniho Huffmanova kodovani\n");
	printf("Parametry:\n");
	printf("\t -c |komprimace\n");
	printf("\t -d |dekomprimace\n");
	printf("\t -h static |staticke huffmanovo kodovani\n");
	printf("\t -h adaptive |adaptivni huffmanovo kodovani\n");
	printf("\t -i <value> |nazev vstupniho souboru\n");
	printf("\t -o <value> |nazev vystupniho souboru\n");
	printf("\t -w <value> |sirka obrazku > 1\n");
	printf("\t -h |napoveda\n");
}
//tisk chybove hlasky
void printEcode(int ecode){
	cerr << ERRORMSG[ecode] << endl;
	exit(ecode);
}
//vytvoreni histogramu
void makeHistogram(vector<int> &rgb, vector<int> &freq, vector<int> &data){
	rgb.clear();
	freq.clear();
	map<int, uint> his;
	//spocitani cetnosti
	for(auto value: data)
		his[value] = his[value] + 1;
	//rozdeleni do vectoru
	for (auto const& p : his){
	    rgb.push_back(p.first);
        freq.push_back(p.second);
	}
    //serazeni od nejmensi frekvence
	sortPair(freq.data(), rgb.data(), rgb.size());
}
//aplikace modelu diference sousednich pixelu
void encodeModel(int width, vector<int> &rgb, vector<int> &freq, vector<int> &data){
	vector<int> tmp(data.size());
	int l = 0;
	for (uint y = 0; y < data.size()/width; y++){
		for (int x = 0; x < width; x++){
			if(x == 0)
				tmp.at(l) = data.at(l);
			else
				tmp.at(l) = data.at(l)-data.at(l-1);
			l++;
		}  
	}
	data = tmp;
	makeHistogram(rgb,freq,data);
}
//reversni operace modelu
void decodeModel(int width, vector<int> &data){
	vector<int> tmp(data.size());
	int l = 0;
	for (uint y = 0; y < data.size()/width; y++){
		for (int x = 0; x < width; x++){
			if(x == 0)
				tmp.at(l) = data.at(l);
			else
				tmp.at(l) = data.at(l)+tmp.at(l-1);
			l++;
	   }
	}
	data = tmp;
}

//Ulozeni kodu do stringu
void storeCodes(struct hufNode* root, string str){ 
	if (root==NULL)
		return;
	if (root->data != TAG) 
		codes[root->data]=str;
	storeCodes(root->right, str + "1");
	storeCodes(root->left, str + "0");  
}

 
//Vytvoreni huffmanova stromu
void makeHuffTree(int *data, int *freq, int size){ 
    struct hufNode *left, *right, *top; 
  
    for (int i = 0; i < size; ++i) 
        minHeap.push(new hufNode(data[i], freq[i])); 

    if(minHeap.size() == 1){
    	storeCodes(minHeap.top(), "0");
    	return;
    }
  
    while (minHeap.size() != 1){
    	//nalezeni dvou listu s nejnizsi frekvenci a spojeni do noveho uzlu
		left = minHeap.top(); 
		minHeap.pop(); 
  
		right = minHeap.top(); 
		minHeap.pop(); 

		top = new hufNode(TAG, left->freq + right->freq); 
  
		top->left = left; 
		top->right = right; 
  
		minHeap.push(top); 
	} 
  
    storeCodes(minHeap.top(), "");
} 
//dekodavani retezce s huffmanovym kodem
void decodeHuffString(struct hufNode* root, string s, vector<int> &data){ 
	struct hufNode* curr = root;
	if (root->left==NULL and root->right==NULL) {
		for(uint i=0;i<s.size();i++)
			data.push_back(root->data);
		return;
	} 
	for (uint i=0;i<s.size();i++) { 
		if (s[i] == '0') 
			curr = curr->left; 
		else
			curr = curr->right; 
  
		if (curr->left==NULL and curr->right==NULL) { 
			data.push_back(curr->data);
			curr = root; 
		} 
	} 
} 
//serazeni 
void sortPair(int *a, int *b, int n){ 
    pair<int, int> pairt[n];
  
    for (int i = 0; i < n; i++){
        pairt[i].first = a[i];
        pairt[i].second = b[i];
    }

    sort(pairt, pairt + n); 

    for (int i = 0; i < n; i++){
        a[i] = pairt[i].first;
        b[i] = pairt[i].second;
    }
}
//cteni RAW souboru
void readRawFile(int width, string filename, vector<int> &rgb, vector<int> &freq, vector<int> &data){
	//cteni souboru
	FILE * pFile;
	pFile = fopen(filename.c_str(), "rb");
	if (pFile==NULL){
		printEcode(EREAD);
	}
	//vypocitani height obrazku
	fseek(pFile,0,SEEK_END);
    int height = int(ftell(pFile)/width);
    fclose (pFile);
    //buffer pro obrazek
	const size_t sizevec = width*height;
	vector<unsigned char> myvector(sizevec, 0);
	ifstream input(filename.c_str(), std::ios::in | std::ifstream::binary);
	if(!input){
        printEcode(EREAD);
    } 
	input.read(reinterpret_cast<char*>(&myvector[0]), myvector.size() * sizeof(uint8_t));
	input.close();

	for (int i = 0; i < height*width; i++){
		data.push_back((unsigned int)myvector.at(i));
	}
	makeHistogram(rgb,freq,data);
}
//ulozeni kodovanych dat
void saveEncode(string filename, string encodedString, vector<int> rgb, vector<int> freq, int width){
	ofstream fout;
	fout.open(filename.c_str(), ios::out | ios::binary);
	if(!fout){
        printEcode(EWRITE);
    } 
	int16_t codeToWriteInt;
	uint32_t codeToWriteUInt32;
	uint16_t codeTowriteUInt16;
	//zapis  velikosti
	codeTowriteUInt16 = rgb.size();
	fout.write(reinterpret_cast<char*>(&codeTowriteUInt16), sizeof(codeTowriteUInt16));
	//zapis dvojice znak/frekvence
	for (uint i = 0; i < rgb.size(); ++i) {
		codeToWriteInt = rgb.at(i);
		fout.write(reinterpret_cast<char*>(&codeToWriteInt), sizeof(codeToWriteInt));
		codeToWriteUInt32 = freq.at(i);
		fout.write(reinterpret_cast<char*>(&codeToWriteUInt32), sizeof(codeToWriteUInt32));
	}
	//append 
	uint16_t append = 16 - (encodedString.length()%16);
	if(append == 16)
		append = 0;
	for (int i = 0; i < append; ++i){
		encodedString.append(to_string(0));
	}
	//zapis appendu
	fout.write(reinterpret_cast<char*>(&append), sizeof(append));
	codeToWriteUInt32 = encodedString.length()/16;
	fout.write(reinterpret_cast<char*>(&codeToWriteUInt32), sizeof(codeToWriteUInt32));
	//zapis encode stringu
	for (uint i = 0; i < encodedString.length(); i+=16) {
		codeTowriteUInt16 = (uint16_t) std::bitset<16>(encodedString.substr(i,i+16)).to_ulong();
		fout.write(reinterpret_cast<char*>(&codeTowriteUInt16), sizeof(codeTowriteUInt16));
	}
	//zapis sirky obrazku kvuli dekodovani
	append = width;
	fout.write(reinterpret_cast<char*>(&append), sizeof(append));
	fout.close();
}
//ulozeni 8bit hodnot do souboru po dekompresy
void saveDecode(string filename, vector<int> data){
	ofstream fout;
	fout.open(filename.c_str(), ios::out | ios::binary);
	if(!fout){
        printEcode(EWRITE);
    } 

    uint8_t value;
    for (uint i = 0; i < data.size(); ++i) {
		value = data.at(i);
		fout.write(reinterpret_cast<char*>(&value), sizeof(value));
	}

	fout.close();
}
//cteni kodovaneho souboru
string readEncode(string filename, vector<int> &rgb, vector<int> &freq, uint *width){
	ifstream fin;
	int16_t valInt16 = 0;
	uint32_t valUint32 = 0;
	uint16_t inSize = 0;
	fin.open(filename.c_str(), ios::binary);
	if(!fin){
        printEcode(EREAD);
    } 
	fin.read(reinterpret_cast<char *>(&inSize), sizeof(inSize));

	for (int i = 0; i < inSize; ++i) {
		fin.read(reinterpret_cast<char *>(&valInt16), sizeof(valInt16));
	    rgb.push_back(valInt16);
	    fin.read(reinterpret_cast<char *>(&valUint32), sizeof(valUint32));
	    freq.push_back(valUint32);
	}
	//nacteni velikosti appendu
	fin.read(reinterpret_cast<char *>(&inSize), sizeof(inSize));
	fin.read(reinterpret_cast<char *>(&valUint32), sizeof(valUint32));
	uint16_t tmp;
	string data = "";
	for (uint i = 0; i < valUint32; ++i) {
		fin.read(reinterpret_cast<char *>(&tmp), sizeof(tmp));
	   	data.append(std::bitset<16>(tmp).to_string());
	}
	
	data = data.substr(0,data.length()-inSize);
	//cteni
	fin.read(reinterpret_cast<char *>(&inSize), sizeof(inSize));
	*width = (uint)inSize;
	fin.close();
	return data;
}

int main(int argc, char *argv[]){
	// prepinace pro getopt()
	int options;
	//komprese = true
	bool encode = false;
	//dekomprese = true
	bool decode = false;
	//prepinac -m
	bool model = false;
	//prepinac -h (false=static, true=adaptive)
	bool huff = false;
	//sirka obrazku
	uint width = 0;
	//nazev vstupniho a vystupniho souboru
	string inFileName = "", outFileName = "", h="";

	while ((options = getopt(argc, argv, ":h:i:w:o:mcd")) != -1){	
		switch (options){
			// prepinac -i
			case 'i':
				inFileName = optarg;
				break;
			// prepinac -o
			case 'o':
				outFileName = optarg;
				break;
			// prepinac -h
			case 'h':
				h = optarg;
				if(h.compare("static") == 0)
					huff = false;
				else if(h.compare("adaptive") == 0)
					huff = true;
				else
					printEcode(EARG);
				break;
			case ':':
				switch(optopt){
					case 'h':
						help();
						exit(EXIT_SUCCESS);
						break;
				}
				break;
			// prepinac -m
			case 'm':
				model = true;
				break;
			// prepinac -c
			case 'c':
				encode = true;
				break;
			// prepinac -d
			case 'd':
				decode = true;
				break;
			// prepinac -w
			case 'w':
				width = atoi(optarg);
				if(width<1)
					printEcode(EARG);
				break;
			// zadny argument, mozna chyba
			case '?':
				// chybejici argument
				break;

			default:
				break;
		}
	}

	if(inFileName.compare("") == 0 || outFileName.compare("") == 0)
		printEcode(EARG);

	//datastruktury
	vector<int> rgb;
	vector<int> freq;
	vector<int> data;
	string encodedString, decodedString;
	//kodovani huffman static
	if(encode && !huff){
		readRawFile(width, inFileName, rgb, freq, data);
		if(model)
			encodeModel(width,rgb,freq,data);
		makeHuffTree(rgb.data(), freq.data(), rgb.size());
		for(auto value: data)
	        encodedString+=codes[value];
	    saveEncode(outFileName, encodedString, rgb, freq, width);
	}
	//dekodovani huffman static
	if (decode && !huff){
		decodedString = readEncode(inFileName, rgb, freq, &width);
		makeHuffTree(rgb.data(), freq.data(), rgb.size());
    	decodeHuffString(minHeap.top(), decodedString, data);
    	if(model)
			decodeModel(width,data);
    	saveDecode(outFileName,data);
	}

	return EXIT_SUCCESS;
}