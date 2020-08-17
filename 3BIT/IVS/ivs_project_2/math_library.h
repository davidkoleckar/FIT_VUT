/*************************************************************
 * Projekt: IVS - projekt 2
 * Soubor: math_library.h
 * Cas vytvoreni: 4.4.2017 19:50
 * Cas posledni zmeny: 5.4.2017 20:00
 *
 * Autor: Janota Ondrej xjanot02@stud.fit.vutbr.cz
 *
 * Popis: Hlavickovy soubor matematicke knihovny.
 *
 ***********************************************************/
/**
 * @file math_library.h
 *
 * @author Ondrej Janota <xjanot02@stud.fit.vutbr.cz>
 *
 * @date 5.4.2017
 *
 * @brief Hlavickovy soubor matematicke knihovny.
 *
 * Knihovna pouziva funkce z knihovny cmath.
 */

#ifndef H_MATH_LIBRARY
#define H_MATH_LIBRARY

#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>
#include "string.h"


/**
 * @class Calculator
 *
 * Trida obsahujici zakladni matematicke funkce.
 *
 * Obsahuje take funkci pro vypocet smerodatne odchylky. 
 *
 */
class Calculator
{
	public:
		Calculator();

		~Calculator();

		double addition(int first, int second);

		double addition(int first, double second);

		double addition(double first, int second);

		double addition(double first, double second);

		double subtraction(int first, int second);

		double subtraction(int first, double second);

		double subtraction(double first, int second);

		double subtraction(double first, double second);

		double multiplication(int first, int second);

		double multiplication(int first, double second);

		double multiplication(double first, int second);

		double multiplication(double first, double second);

		double division(int first, int second);

		double division(int first, double second);

		double division(double first, int second);

		double division(double first, double second);

		double modulation(int first, int second);

		double modulation(int first, double second);

		double modulation(double first, int second);

		double modulation(double first, double second);

		double factorial(int x);

		double factorial(double x);

		double power(int x, int y);

		double power(double x, int y);

		double power(int x, double y);

		double power(double x, double y);

		double root(int x, int y);

		double root(int x, double y);

		double root(double x, int y);

		double root(double x, double y);

		double standard_deviation(std::vector<double> numbers);

		long long int decimal_to_binary(long long int decimal);

		long long int binary_to_decimal(long long int binary);

		long long int decimal_to_octal(long long int decimal);

		long long int octal_to_decimal(long long int octal);

		std::string decimal_to_hex(long long int decimal);

		long long int hex_to_decimal(std::string hex);

		long long int binary_to_octal(long long int binary);
		
		long long int octal_to_binary(long long int octal);
		
		std::string binary_to_hex(long long int binary);
		
		long long int hex_to_binary(std::string hex);
		
		std::string octal_to_hex(long long int octal);
		
		long long int hex_to_octal(std::string hex);

		double get_result();

		 /**
         * Prvni operand - cele cislo.
         */
        int first_int;
        /**
         * Druhy operand - cele cislo.
         */
        int second_int;
        /**
         * Prvni operand - desetinne cislo.
         */
        double first_double;
        /**
         * Druhy operand - desetinne cislo.
         */
        double second_double;

        /**
         * Operace.
         */
        enum ops
        {
            none,   /**< Zadna operace. */
            add,    /**< Operace +. */
            sub,    /**< Operace -. */
            mul,    /**< Operace *. */
            div,    /**< Operace /. */
            pow_,   /**< Operace x^2. */
            ypow_,  /**< Operace x^y.*/
            root_,  /**< Operace sqrt(x). */
            yroot,  /**< Operace x yroot z. */
            fact_,  /**< Operace x!. */
            bin_oct,/**< Operace bin->oct. */
            bin_dec,/**< Operace bin->dec. */
            bin_hex,/**< Operace bin->hex. */
            oct_bin,/**< Operace oct->bin. */
            oct_dec,/**< Operace oct->dec. */
            oct_hex,/**< Operace oct->hex. */
            dec_bin,/**< Operace dec->bin. */
            dec_oct,/**< Operace dec->oct. */
            dec_hex,/**< Operace dec->hex. */
            hex_bin,/**< Operace hex->bin. */
            hex_oct,/**< Operace hex->oct. */
            hex_dec,/**< Operace hex->dec. */
            bracket /**< Operace (. */
        };

        /**
         * Typ
         */
        enum type
        {
            none_,  /**< Zadny typ. */
            int_,   /**< Cele cislo. */
            double_ /**< Desetinne cislo. */
        };

        /**
         * Typ prvniho operandu.
         */
        type first;
        /**
         * Typ druheho operandu.
         */
        type second;

        /**
         * Typ operace.
         */
        ops operation;

	protected:
		/**
		 * Vysledek operace.
		 */
		double result;
		double fact(int x);
		double arithmetic_mean(std::vector<double> numbers);
};

#endif
