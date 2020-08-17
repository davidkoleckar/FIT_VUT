/*************************************************************
 * Projekt: IVS - projekt 2
 * Soubor: profiling.cpp
 * Cas vytvoreni: 14.4.2017 14:00
 * Cas posledni zmeny: 14.4.2017 20:00
 *
 * Autor: Marek Jan xmarek62@stud.fit.vutbr.cz
 *
 * Popis: Implementace matematicke knihovny.
 *
 ***********************************************************/
/**
 * @file math_library.cpp
 *
 * @author Jan Marek <xmarek62@stud.fit.vutbr.cz>
 *
 * @date 14.4.2017
 *
 * @brief Program pro výpočet standardní odchylky
 *
 * Očekává na vstupu čísla oddělená konci řádků, ukončená znakem EOF. Na stdout vypíše standardní odchylku.
 */

#include "math_library.h"
#include <iostream>

int main()
{
    Calculator calc = Calculator();
    try
    {
		double number = 0;
		std::vector<double> numbers;
		numbers.reserve(1000);//reserve space for numbers, vector extensions wasted a lot of time

		while (std::cin >> number) //load all numbers from stdin
			numbers.push_back(number);

		std::cout << calc.standard_deviation(numbers) << std::endl; //compute and print std. deviation
        return 0;
	}
	catch (std::runtime_error* e)
	{
		std::cerr << e->what() << std::endl;
        return 1;
	}
}
	