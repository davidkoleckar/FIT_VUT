/*************************************************************
 * Projekt: IVS - projekt 2
 * Soubor: math_library.cpp
 * Cas vytvoreni: 4.4.2017 19:50
 * Cas posledni zmeny: 5.4.2017 20:00
 *
 * Autor: Janota Ondrej xjanot02@stud.fit.vutbr.cz
 *
 * Popis: Implementace matematicke knihovny.
 *
 ***********************************************************/
/**
 * @file math_library.cpp
 *
 * @author Ondrej Janota <xjanot02@stud.fit.vutbr.cz>
 *
 * @date 4.4.2017
 *
 * @brief Implementace matematicke knihovny.
 *
 * Knihovna pouziva funkce z knihovny cmath.
 */

#include "math_library.h"

/**
 * Konstruktor
 *
 * Nastavuje hodnotu vysledku na 0.
 *
 */
Calculator::Calculator()
{
	result = 0.0;
    first_int = 0;
    first_double = 0.0;
    second_int = 0;
    second_double = 0.0;
    first = Calculator::none_;
    second = Calculator::none_;
    operation = Calculator::none;
}

/**
 * Destruktor
 *
 */
Calculator::~Calculator()
{

}

/**
 * Funkce pro soucet 2 celych cisel.
 *
 * @param first Prvni scitanec - cele cislo.
 * @param second Druhy scitanec - cele cislo.
 *
 * @return Vraci soucet zadanych cisel.
 */
double Calculator::addition(int first, int second)
{
	return result = first + second;
}

/**
 * Funkce pro soucet celeho a realneho cisla.
 *
 * @param first Prvni scitanec - cele cislo.
 * @param second Druhy scitanec - realne cislo.
 *
 * @return Vraci soucet zadanych cisel.
 */
double Calculator::addition(int first, double second)
{
	return result = first + second;
}

/**
 * Funkce pro soucet realneho a celeho cisla.
 *
 * @param first Prvni scitanec - realne cislo.
 * @param second Druhy scitanec - cele cislo.
 *
 * @return Vraci soucet zadanych cisel.
 */
double Calculator::addition(double first, int second)
{
	return result = first + second;
}

/**
 * Funkce pro soucet 2 realnehych cisel.
 *
 * @param first Prvni scitanec - realne cislo.
 * @param second Druhy scitanec - realne cislo.
 *
 * @return Vraci soucet zadanych cisel.
 */
double Calculator::addition(double first, double second)
{
	return result = first + second;
}


/**
 * Funkce pro rozdil 2 celych cisel.
 *
 * @param first Mensenec - cele cislo.
 * @param second Mensitel - cele cislo.
 *
 * @return Vraci rozdil zadanych cisel.
 */
double Calculator::subtraction(int first, int second)
{
	return result = first - second;
}

/**
 * Funkce pro rozdil celeho a realneho cisla.
 *
 * @param first Mensenec - cele cislo.
 * @param second Mensitel - realne cislo.
 *
 * @return Vraci rozdil zadanych cisel.
 */
double Calculator::subtraction(int first, double second)
{
	return result = first - second;
}

/**
 * Funkce pro rozdil realneho a celeho cisla.
 *
 * @param first Mensenec - realne cislo.
 * @param second Mensitel - cele cislo.
 *
 * @return Vraci rozdil zadanych cisel.
 */
double Calculator::subtraction(double first, int second)
{
	return result = first - second;
}

/**
 * Funkce pro rozdil 2 realnehych cisel.
 *
 * @param first Mensenec - realne cislo.
 * @param second Mensitel - realne cislo.
 *
 * @return Vraci rozdil zadanych cisel.
 */
double Calculator::subtraction(double first, double second)
{
	return result = first - second;
}


/**
 * Funkce pro soucin 2 celych cisel.
 *
 * @param first Prvni cinitel - cele cislo.
 * @param second Druhy cinitel - cele cislo.
 *
 * @return Vraci soucin zadanych cisel.
 */
double Calculator::multiplication(int first, int second)
{
	return result = first * second;
}

/**
 * Funkce pro soucin celeho a realneho cisla.
 *
 * @param first Prvni cinitel - cele cislo.
 * @param second Druhy cinitel - realne cislo.
 *
 * @return Vraci soucin zadanych cisel.
 */
double Calculator::multiplication(int first, double second)
{
	return result = first * second;
}

/**
 * Funkce pro soucin realneho a celeho cisla.
 *
 * @param first Prvni cinitel - realne cislo.
 * @param second Druhy cinitel - cele cislo.
 *
 * @return Vraci soucin zadanych cisel.
 */
double Calculator::multiplication(double first, int second)
{
	return result = first * second;
}

/**
 * Funkce pro soucin 2 realnehych cisel.
 *
 * @param first Prvni cinitel - realne cislo.
 * @param second Druhy cinitel - realne cislo.
 *
 * @return Vraci soucin zadanych cisel.
 */
double Calculator::multiplication(double first, double second)
{
	return result = first * second;
}


/**
 * Funkce pro podil 2 celych cisel.
 *
 * @param first Delenec - cele cislo.
 * @param second Delitel - cele cislo.
 *
 * @return Vraci podil zadanych cisel.
 */
double Calculator::division(int first, int second)
{
	if (!second)
		throw new std::runtime_error("Nelze delit nulou!");

	return result = (double)first / second;
}

/**
 * Funkce pro podil celeho a realneho cisla.
 *
 * @param first Delenec - cele cislo.
 * @param second Delitel - realne cislo.
 *
 * @return Vraci podil zadanych cisel.
 */
double Calculator::division(int first, double second)
{
	if (!second)
		throw new std::runtime_error("Nelze delit nulou!");

	return result = first / second;
}

/**
 * Funkce pro podil realneho a celeho cisla.
 *
 * @param first Delenec - realne cislo.
 * @param second Delitel - cele cislo.
 *
 * @return Vraci podil zadanych cisel.
 */
double Calculator::division(double first, int second)
{
	if (!second)
		throw new std::runtime_error("Nelze delit nulou!");

	return result = first / second;
}

/**
 * Funkce pro podil 2 realnehych cisel.
 *
 * @param first Delenec - realne cislo.
 * @param second Delitel - realne cislo.
 *
 * @return Vraci podil zadanych cisel.
 */
double Calculator::division(double first, double second)
{
	if (!second)
		throw new std::runtime_error("Nelze delit nulou!");

	return result = first / second;
}


/**
 * Funkce pro ziskani zbytku po deleni 2 celych cisel.
 *
 * @param first Delenec - cele cislo.
 * @param second Delitel - cele cislo.
 *
 * @return Vraci zbytek po deleni zadanych cisel.
 */
double Calculator::modulation(int first, int second)
{
	if (!second)
		throw new std::runtime_error("Nelze delit nulou!");

	return result = first % second;
}

/**
 * Funkce pro ziskani zbytku po deleni celeho a realneho cisla.
 *
 * @param first Delenec - cele cislo.
 * @param second Delitel - realne cislo.
 *
 * @return Vraci zbytek po deleni zadanych cisel.
 */
double Calculator::modulation(int first, double second)
{
	if (!second)
		throw new std::runtime_error("Nelze delit nulou!");

	return result = fmod(first, second);
}

/**
 * Funkce pro ziskani zbytku po deleni realneho a celeho cisla.
 *
 * @param first Delenec - realne cislo.
 * @param second Delitel - cele cislo.
 *
 * @return Vraci zbytek po deleni zadanych cisel.
 */
double Calculator::modulation(double first, int second)
{
	if (!second)
		throw new std::runtime_error("Nelze delit nulou!");

	return result = fmod(first, second);
}

/**
 * Funkce pro ziskani zbytku po deleni 2 realnehych cisel.
 *
 * @param first Delenec - realne cislo.
 * @param second Delitel - realne cislo.
 *
 * @return Vraci zbytek po deleni zadanych cisel.
 */
double Calculator::modulation(double first, double second)
{
	if (!second)
		throw new std::runtime_error("Nelze delit nulou!");

	return result = fmod(first, second);
}


/**
 * Funkce pro zaklad na vypocet faktorialu.
 *
 * Vola rekurzivni funkci pro vypocet faktorialu.
 *
 * @param x Cislo, z nehoz se faktorial pocita.
 *
 * @return Vraci x faktorial (x!).
 */
double Calculator::factorial(int x)
{
	return result = fact(x);
}

/**
 * Funkce pro zaklad na vypocet faktorialu.
 *
 * Funkce pouze vyvola vyjimku, protoze faktorial
 * z realneho cisla neni implementovan.
 *
 * @param x Cislo, z nehoz se faktorial pocita.
 */
double Calculator::factorial(double x)
{
	throw new std::runtime_error("Faktorial nelze tvorit z realneho cisla!");
}

/**
 * Funkce pro vypocet faktorialu.
 *
 * Rekurzivni funkce pro vypocet faktorialu.
 * V pripade zaporneho cisla vyvola vyjimku.
 *
 * @param x Cislo, z nehoz se faktorial pocita.
 *
 * @return Vraci x faktorial (x!).
 */
double Calculator::fact(int x)
{
	if (x < 0)
		throw new std::runtime_error("Faktorial nelze z cisla " + std::to_string(x) + " tvorit!");

	return x <= 1 ? 1 : x * factorial(x - 1);
}


/**
 * Funkce pro umocneni celeho cisla celym cisel.
 *
 * @param x Zaklad mocniny - mocnenec.
 * @param y Exponent - mocnitel.
 *
 * @return Vraci y-tou mocninu cisla x.
 */
double Calculator::power(int x, int y)
{
	return result = (double)pow(x, y);
}

/**
 * Funkce pro umocneni celeho cisla realnym cislem.
 *
 * @param x Zaklad mocniny - mocnenec.
 * @param y Exponent - mocnitel.
 *
 * @return Vraci y-tou mocninu cisla x.
 */
double Calculator::power(int x, double y)
{
	return result = pow(x, y);
}

/**
 * Funkce pro umocneni realneho cisla celym cislem.
 *
 * @param x Zaklad mocniny - mocnenec.
 * @param y Exponent - mocnitel.
 *
 * @return Vraci y-tou mocninu cisla x.
 */
double Calculator::power(double x, int y)
{
	return result = (double)pow(x, y);
}

/**
 * Funkce pro umocneni realneho cisla realnym cislem.
 *
 * @param x Zaklad mocniny - mocnenec.
 * @param y Exponent - mocnitel.
 *
 * @return Vraci y-tou mocninu cisla x.
 */
double Calculator::power(double x, double y)
{
	return result = pow(x, y);
}


/**
 * Funkce pro odmocneni celeho cisla celym cislem.
 *
 * Funkce pouziva operaci umocneni,
 * kdy je odmocnitel invertovan.
 *
 * @param x Zaklad odmocniny.
 * @param y Odmocnitel.
 *
 * @return Vraci y-tou odmocninu cisla x.
 */
double Calculator::root(int x, int y)
{
	if (!y)
		throw new std::runtime_error("Nelze provadet nultou odmocninu!");		

	return result = pow(x, 1.0/y);
}

/**
 * Funkce pro odmocneni celeho cisla realnym cislem.
 *
 * Funkce pouziva operaci umocneni,
 * kdy je odmocnitel invertovan.
 *
 * @param x Zaklad odmocniny.
 * @param y Odmocnitel.
 *
 * @return Vraci y-tou odmocninu cisla x.
 */
double Calculator::root(int x, double y)
{
	if (!y)
		throw new std::runtime_error("Nelze provadet nultou odmocninu!");

	return result = pow(x, 1.0/y);
}

/**
 * Funkce pro odmocneni realneho cisla celym cislem.
 *
 * Funkce pouziva operaci umocneni,
 * kdy je odmocnitel invertovan.
 *
 * @param x Zaklad odmocniny.
 * @param y Odmocnitel.
 *
 * @return Vraci y-tou odmocninu cisla x.
 */
double Calculator::root(double x, int y)
{
	if (!y)
		throw new std::runtime_error("Nelze provadet nultou odmocninu!");		

	return result = pow(x, 1.0/y);
}

/**
 * Funkce pro odmocneni realneho cisla realnym cislem.
 *
 * Funkce pouziva operaci umocneni,
 * kdy je odmocnitel invertovan.
 *
 * @param x Zaklad odmocniny.
 * @param y Odmocnitel.
 *
 * @return Vraci y-tou odmocninu cisla x.
 */
double Calculator::root(double x, double y)
{
	if (!y)
		throw new std::runtime_error("Nelze provadet nultou odmocninu!");

	return result = pow(x, 1.0/y);	
}


/**
 * Funkce pro vypocet aritmetickeho prumeru.
 *
 * Scita jednotliva cisla z vektoru a nakonec je podeli
 * poctem cisel.
 *
 * @param numbers Cisla, z nichz se prumer pocita.
 *
 * @return Vraci aritmeticky prumer zadanych cisel.
 */
double Calculator::arithmetic_mean(std::vector<double> numbers)
{
	double sum = 0.0; // Suma zadanych cisel.

	for (auto i : numbers)
		sum = addition(i, sum); // Soucet zadanych cisel.

	return result = division(1, (int)numbers.size()) * sum;
}


/**
 * Funkce pro vypocet smerodatne odchylky.
 *
 * Vola vsechny zakladni funkce matematicke knihovny.
 *
 * @param numbers Cisla, z nichz se odchylka pocita.
 *
 * @return Vraci smerodatnou odchylku zadanych cisel.
 */
double Calculator::standard_deviation(std::vector<double> numbers)
{
	double sum = 0.0; // Suma druhych mocnin zadanych cisel.

	int count = numbers.size(); // Pocet zadanych cisel.

	for (int i = 1; i <= count; i++)
		sum = addition(power(numbers[i-1], 2), sum); // Soucet druhych mocnin zadanych cisel.

	try
	{
		// Vzorec podle https://wikimedia.org/api/rest_v1/media/math/render/svg/da053339611acb71c190acf04589c44cb49191fc
		return result = root(multiplication(division(1, subtraction(count, 1)), subtraction(sum, multiplication(count, power(arithmetic_mean(numbers), 2)))), 2);
	}
	catch(std::runtime_error* e)
	{
		// V pripade spatneho vstupu vyvola vyjimku.
		throw new std::runtime_error("Zvolili jste spatne vstupni hodnoty!");
	}
}


/**
 * Funkce pro prevod decimalniho cisla na binarni.
 *
 * Prevadi cislo postupnym delenim 2.
 * Vyvola vyjimku, pokud je decimalni cislo zaporne.
 *
 * @param decimal Decimalni cislo, ktere je prevadeno.
 *
 * @return Vraci binarni cislo.
 */
long long int Calculator::decimal_to_binary(long long int decimal)
{
	if (decimal < 0)
		throw new std::runtime_error("Nelze prevadet zaporna cisla!");

	std::string binary("");

	while (decimal != 0)
	{
		binary = (decimal % 2 ? "1" : "0") + binary;

		decimal /= 2;
	}

    return result = std::strtoll(binary.c_str(), NULL, 10);
}

/**
 * Funkce pro prevod binarniho cisla na decimalni.
 *
 * Prevadi cislo postupnym delenim 10.
 * Vyvola vyjimku, pokud je binarni cislo zaporne.
 *
 * @param binary Binarni cislo, ktere je prevadeno.
 *
 * @return Vraci decimalni cislo.
 */
long long int Calculator::binary_to_decimal(long long int binary)
{
	if (binary < 0)
		throw new std::runtime_error("Nelze prevadet zaporna cisla!");

	double decimal = 0;

	int order = 0;

	while(binary)
	{
		if (binary % 10 > 1)
			throw new std::runtime_error("Zadane cislo neni binarni!");

		decimal += pow(2, order++) * (binary % 10);
		binary /= 10;
	}

	return result = decimal;
}


/**
 * Funkce pro prevod decimalniho cisla na oktalove.
 *
 * Prevadi cislo postupnym delenim 8.
 * Vyvola vyjimku, pokud je decimalni cislo zaporne.
 *
 * @param decimal Decimalni cislo, ktere je prevadeno.
 *
 * @return Vraci oktalove cislo.
 */
long long int Calculator::decimal_to_octal(long long int decimal)
{
	if (decimal < 0)
		throw new std::runtime_error("Nelze prevadet zaporna cisla!");

	std::string octal("");

	while (decimal)
	{
		octal = std::to_string(decimal % 8) + octal;
		decimal /= 8;
	}

    return result = std::strtoll(octal.c_str(), NULL, 10);
}

/**
 * Funkce pro prevod oktaloveho cisla na decimalni.
 *
 * Prevadi cislo postupnym delenim 10.
 * Vyvola vyjimku, pokud je oktalove cislo zaporne.
 *
 * @param octal Oktalove cislo, ktere je prevadeno.
 *
 * @return Vraci decimalni cislo.
 */
long long int Calculator::octal_to_decimal(long long int octal)
{
	if (octal < 0)
		throw std::runtime_error("Nelze prevadet zaporna cisla!");

	double decimal = 0;

	int order = 0;

	while (octal)
	{
		decimal += pow(8, order++) * (octal % 10);
		octal /= 10;
	}

	return result = decimal;
}


/**
 * Funkce pro prevod decimalniho cisla na hexadecimalni.
 *
 * Prevadi cislo postupnym delenim 16 a prevodem na znak.
 * Vyvola vyjimku, pokud je decimalni cislo zaporne.
 *
 * @param decimal Decimalni cislo, ktere je prevadeno.
 *
 * @return Vraci hexadecimalni cislo v retezci.
 */
std::string Calculator::decimal_to_hex(long long int decimal)
{
	if (decimal < 0)
		throw new std::runtime_error("Nelze prevadet zaporna cisla!");

	std::string hex_number("");

	std::vector<int> hex;

	while (decimal)
	{
		hex.push_back(decimal % 16);
		decimal /= 16;
	}

	for (auto pair : hex)
	{
		switch (pair)
		{
			case 10:
				hex_number = "A" + hex_number;
				break;
			case 11:
				hex_number = "B" + hex_number;
				break;
			case 12:
				hex_number = "C" + hex_number;
				break;
			case 13:
				hex_number = "D" + hex_number;
				break;
			case 14:
				hex_number = "E" + hex_number;
				break;
			case 15:
				hex_number = "F" + hex_number;
				break;
			default:
				hex_number = std::to_string(pair) + hex_number;
		}
	}

	if (hex_number.size() == 0)
		hex_number += "0";

	return hex_number;
}

/**
 * Funkce pro prevod hexadecimalniho cisla na decimalni.
 *
 * Prevadi cislo postupnym zpracovanim posledniho znaku.
 * Vyvola vyjimku, pokud je hexadecimalni cislo zaporne.
 *
 * @param hex Hexadecimalni cislo, ktere je prevadeno.
 *
 * @return Vraci decimalni cislo.
 */
long long int Calculator::hex_to_decimal(std::string hex)
{
	if (hex.at(0) == '-')
		throw new std::runtime_error("Nelze prevadet zaporna cisla!");

	double decimal = 0;

	int len = hex.length() - 1;

	int order = 0;

	for (int i = len; i >= 0; i--)
	{
		switch (hex.at(i))
		{
			case 'A':
				decimal += pow(16, order++) * 10;
				break;

			case 'B':
				decimal += pow(16, order++) * 11;
				break;

			case 'C':
				decimal += pow(16, order++) * 12;
				break;

			case 'D':
				decimal += pow(16, order++) * 13;
				break;

			case 'E':
				decimal += pow(16, order++) * 14;
				break;

			case 'F':
				decimal += pow(16, order++) * 15;
				break;

			default:
				decimal += pow(16, order++) * (hex.at(i)-'0');
		}
	}

	return result = decimal;
}


/**
 * Funkce pro prevod binarniho cisla na oktalove.
 *
 * Prevadi cislo ziskanim decimalniho cisla, pote cisla oktaloveho.
 *
 * @param binary Binarni cislo, ktere je prevadeno.
 *
 * @return Vraci oktalove cislo.
 */
long long int Calculator::binary_to_octal(long long int binary)
{
	return decimal_to_octal(binary_to_decimal(binary));
}

/**
 * Funkce pro prevod oktaloveho cisla na binarni.
 *
 * Prevadi cislo ziskanim decimalniho cisla, pote cisla binarniho.
 *
 * @param octal Oktalove cislo, ktere je prevadeno.
 *
 * @return Vraci binarni cislo.
 */
long long int Calculator::octal_to_binary(long long int octal)
{
	return decimal_to_binary(octal_to_decimal(octal));
}


/**
 * Funkce pro prevod binarniho cisla na hexadecimalni.
 *
 * Prevadi cislo ziskanim decimalniho cisla, pote cisla hexadecimalniho.
 *
 * @param binary Binarni cislo, ktere je prevadeno.
 *
 * @return Vraci hexadecimalni cislo.
 */
std::string Calculator::binary_to_hex(long long int binary)
{
	return decimal_to_hex(binary_to_decimal(binary));
}

/**
 * Funkce pro prevod hexadecimalniho cisla na binarni.
 *
 * Prevadi cislo ziskanim decimalniho cisla, pote cisla birarniho.
 *
 * @param hex Hexadecimalni cislo, ktere je prevadeno.
 *
 * @return Vraci binarni cislo.
 */
long long int Calculator::hex_to_binary(std::string hex)
{
	return decimal_to_binary(hex_to_decimal(hex));
}


/**
 * Funkce pro prevod oktaloveho cisla na hexadecimalni.
 *
 * Prevadi cislo ziskanim decimalniho cisla, pote cisla hexadecimalniho.
 *
 * @param octal Oktalove cislo, ktere je prevadeno.
 *
 * @return Vraci hexadecimalni cislo.
 */
std::string Calculator::octal_to_hex(long long int octal)
{
	return decimal_to_hex(octal_to_decimal(octal));
}

/**
 * Funkce pro prevod hexadecimalniho cisla na oktalove.
 *
 * Prevadi cislo ziskanim decimalniho cisla, pote cisla oktaloveho.
 *
 * @param hex Hexadecimalni cislo, ktere je prevadeno.
 *
 * @return Vraci oktalove cislo.
 */
long long int Calculator::hex_to_octal(std::string hex)
{
	return decimal_to_octal(hex_to_decimal(hex));
}

/**
 * Funkce pro ziskani vysledku posledni provedene operace.
 *
 * Vrati hodnotu private promenne result.
 *
 * @return Vraci vysledek posledni provedene operace.
 */
double Calculator::get_result()
{
	return result;
}

/*** Konec souboru math_library.cpp ***/
