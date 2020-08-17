/*************************************************************
 * Projekt: IVS - projekt 2
 * Soubor: math_library_tests.cpp
 * Cas vytvoreni: 4.4.2017 19:50
 * Cas posledni zmeny: 5.4.2017 20:00
 *
 * Autor: Janota Ondrej xjanot02@stud.fit.vutbr.cz
 *
 * Popis: Testy pro matematickou knihovnu.
 *
 ***********************************************************/
/**
 * @file math_library_tests.cpp
 *
 * @author Ondrej Janota <xjanot02@stud.fit.vutbr.cz>
 *
 * @date 4.4.2017
 *
 * @brief Testy pro matematickou knihovnu.
 *
 * Testy jsou realizovany pomoci google-test.
 */

#include "math_library.h"
#include "gtest/gtest.h"

/**
 * @class calculate_int
 *
 * Testovaci trida pro cela cisla.
 *
 */
class calculate_int : public ::testing::Test
{
	void SetUp()
	{
		x = 1;
		y = 2;
	}

	protected:
		/**
		 * Objekt kalkulacky.
		 */
		Calculator calc;
		/**
		 * Prvni operand.
		 */
		int x;
		/**
		 * Druhy operand.
		 */
		int y;
};


/**
 * @class calculate_double
 *
 * Testovaci trida pro realna cisla.
 *
 */
class calculate_double : public ::testing::Test
{
	void SetUp()
	{
		x = 1.5;
		y = 2.5;
	}

	protected:
		/**
		 * Objekt kalkulacky.
		 */
		Calculator calc;
		/**
		 * Prvni operand.
		 */
		double x;
		/**
		 * Druhy operand.
		 */
		double y;
};


/**
 * @class calculate_int_double
 *
 * Testovaci trida pro cele a realne cislo.
 *
 */
class calculate_int_double : public ::testing::Test
{
	void SetUp()
	{
		x = 1;
		y = 2.5	;
	}
	
	protected:
		/**
		 * Objekt kalkulacky.
		 */
		Calculator calc;
		/**
		 * Prvni operand.
		 */
		int x;
		/**
		 * Druhy operand.
		 */
		double y;
};

/**
 * Test pro soucet celych cisel.
 */
TEST_F(calculate_int, OpAdd)
{
	EXPECT_EQ(calc.addition(x, y), 3);

	EXPECT_EQ(calc.get_result(), 3);
}

/**
 * Test pro soucet realnych cisel.
 */
TEST_F(calculate_double, OpAdd)
{
	EXPECT_EQ(calc.addition(x, y), 4);

	EXPECT_EQ(calc.get_result(), 4);
}

/**
 * Test pro soucet celeho a realneho cisla.
 */
TEST_F(calculate_int_double, OpAdd)
{
	EXPECT_EQ(calc.addition(x, y), 3.5);

	EXPECT_EQ(calc.get_result(), 3.5);

	EXPECT_EQ(calc.addition(y, x), 3.5);

	EXPECT_EQ(calc.get_result(), 3.5);
}

/**
 * Test pro rozdil celych cisel.
 */
TEST_F(calculate_int, OpSub)
{
	EXPECT_EQ(calc.subtraction(x, y), -1);

	EXPECT_EQ(calc.get_result(), -1);
}

/**
 * Test pro rozdil realnych cisel.
 */
TEST_F(calculate_double, OpSub)
{
	EXPECT_EQ(calc.subtraction(x, y), -1);

	EXPECT_EQ(calc.get_result(), -1);
}

/**
 * Test pro rozdil celeho a realneho cisla.
 */
TEST_F(calculate_int_double, OpSub)
{
	EXPECT_EQ(calc.subtraction(x, y), -1.5);

	EXPECT_EQ(calc.get_result(), -1.5);

	EXPECT_EQ(calc.subtraction(y, x), 1.5);

	EXPECT_EQ(calc.get_result(), 1.5);	
}


/**
 * Test pro soucin celych cisel.
 */
TEST_F(calculate_int, OpMul)
{
	EXPECT_EQ(calc.multiplication(x, y), 2);

	EXPECT_EQ(calc.get_result(), 2);
}

/**
 * Test pro soucin realnych cisel.
 */
TEST_F(calculate_double, OpMul)
{
	EXPECT_EQ(calc.multiplication(x, y), 3.75);

	EXPECT_EQ(calc.get_result(), 3.75);
}

/**
 * Test pro soucin celeho a realneho cisla.
 */
TEST_F(calculate_int_double, OpMul)
{
	EXPECT_EQ(calc.multiplication(x, y), 2.5);

	EXPECT_EQ(calc.get_result(), 2.5);

	EXPECT_EQ(calc.multiplication(y, x), 2.5);

	EXPECT_EQ(calc.get_result(), 2.5);
}


/**
 * Test pro podil celych cisel.
 */
TEST_F(calculate_int, OpDiv)
{
	EXPECT_EQ(calc.division(x, y), 0.5);

	EXPECT_EQ(calc.get_result(), 0.5);
}

/**
 * Test pro podil realnych cisel.
 */
TEST_F(calculate_double, OpDiv)
{
	EXPECT_EQ(calc.division(x, y), 0.6);

	EXPECT_EQ(calc.get_result(), 0.6);
}

/**
 * Test pro podil celeho a realneho cisla.
 */
TEST_F(calculate_int_double, OpDiv)
{
	EXPECT_EQ(calc.division(x, y), 0.4);

	EXPECT_EQ(calc.get_result(), 0.4);

	EXPECT_EQ(calc.division(y, x), 2.5);

	EXPECT_EQ(calc.get_result(), 2.5);
}


/**
 * Test pro zbytek po deleni celych cisel.
 */
TEST_F(calculate_int, OpMod)
{
	EXPECT_EQ(calc.modulation(x, y), 1);

	EXPECT_EQ(calc.get_result(), 1);
}

/**
 * Test pro zbytek po deleni realnych cisel.
 */
TEST_F(calculate_double, OpMod)
{
	EXPECT_EQ(calc.modulation(x, y), 1.5);

	EXPECT_EQ(calc.get_result(), 1.5);
}

/**
 * Test pro zbytek po deleni celeho a realneho cisla.
 */
TEST_F(calculate_int_double, OpMod)
{
	EXPECT_EQ(calc.modulation(x, y), 1);

	EXPECT_EQ(calc.get_result(), 1);

	EXPECT_EQ(calc.modulation(y, x), 0.5);

	EXPECT_EQ(calc.get_result(), 0.5);
}


/**
 * Test pro faktorial celych cisel.
 */
TEST_F(calculate_int, OpFact)
{
	EXPECT_EQ(calc.factorial(x), 1);

	EXPECT_EQ(calc.get_result(), 1);

	EXPECT_EQ(calc.factorial(y), 2);

	EXPECT_EQ(calc.get_result(), 2);

	EXPECT_EQ(calc.factorial(5), 120);

	EXPECT_EQ(calc.get_result(), 120);

	EXPECT_DOUBLE_EQ(calc.factorial(50), 3.0414093201713376e+64);
}

/**
 * Test pro faktorial realnych cisel.
 */
TEST_F(calculate_double, OpFact)
{
	EXPECT_ANY_THROW(calc.factorial(x));
}


/**
 * Test pro umocneni celych cisel.
 */
TEST_F(calculate_int, opPow)
{
	EXPECT_EQ(calc.power(x, y), 1);

	EXPECT_EQ(calc.get_result(), 1);

	EXPECT_EQ(calc.power(x, -1), 1);
}

/**
 * Test pro umocneni realnych cisel.
 */
TEST_F(calculate_double, opPow)
{
	EXPECT_DOUBLE_EQ(calc.power(x, y), 2.7556759606310753);

	EXPECT_DOUBLE_EQ(calc.get_result(), 2.7556759606310753);
}

/**
 * Test pro umocneniodmocneni celeho a realneho cisla.
 */
TEST_F(calculate_int_double, opPow)
{
	EXPECT_EQ(calc.power(x, y), 1);

	EXPECT_EQ(calc.get_result(), 1);

	EXPECT_EQ(calc.power(y, x), 2.5);

	EXPECT_EQ(calc.get_result(), 2.5);

	EXPECT_EQ(calc.power(x, -1), 1);

	EXPECT_EQ(calc.get_result(), 1);

	EXPECT_EQ(calc.power(y, -1), 0.4);

	EXPECT_EQ(calc.get_result(), 0.4);
}


/**
 * Test pro odmocneni celych cisel.
 */
TEST_F(calculate_int, opRoot)
{
	EXPECT_EQ(calc.root(x, y), 1);

	EXPECT_EQ(calc.get_result(), 1);

	EXPECT_ANY_THROW(calc.root(x, (int)0));
}

/**
 * Test pro odmocneni realnych cisel.
 */
TEST_F(calculate_double, opRoot)
{
	EXPECT_DOUBLE_EQ(calc.root(x, y), 1.1760790225246736);

	EXPECT_DOUBLE_EQ(calc.get_result(), 1.1760790225246736);

	EXPECT_ANY_THROW(calc.root(x, 0));
}

/**
 * Test pro odmocneni celeho a realneho cisla.
 */
TEST_F(calculate_int_double, opRoot)
{
	EXPECT_EQ(calc.root(x, y), 1);

	EXPECT_EQ(calc.get_result(), 1);

	EXPECT_EQ(calc.root(y, x), y);

	EXPECT_EQ(calc.get_result(), y);

	EXPECT_ANY_THROW(calc.root(y, 0));
}


/**
 * Test pro smerodatnou odchylku.
 */
TEST_F(calculate_int, opStandardDeviation)
{
	std::vector<double> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	EXPECT_DOUBLE_EQ(calc.standard_deviation(numbers), 3.0276503540974917);

	EXPECT_DOUBLE_EQ(calc.get_result(), 3.0276503540974917);

	numbers = {1.1, 2.5, 3.4, 4.8, 5.1, 6.7, 7.2, 8.6, 9.0, 10.9};

	EXPECT_DOUBLE_EQ(calc.standard_deviation(numbers), 3.1077144155937);

	EXPECT_DOUBLE_EQ(calc.get_result(), 3.1077144155937);

	std::vector<double> bad_numbers;

	EXPECT_ANY_THROW(calc.standard_deviation(bad_numbers));

	bad_numbers.push_back(1);

	EXPECT_ANY_THROW(calc.standard_deviation(bad_numbers));
}


/**
 * Test pro prevod decimalniho na binarni cislo.
 */
TEST_F(calculate_int, opDecToBin)
{
	EXPECT_EQ(1, calc.decimal_to_binary(x));

	EXPECT_EQ(1, calc.get_result());

	EXPECT_EQ(10, calc.decimal_to_binary(y));

	EXPECT_EQ(10, calc.get_result());

	EXPECT_EQ(101, calc.decimal_to_binary(5));

	EXPECT_EQ(101, calc.get_result());

	EXPECT_ANY_THROW(calc.decimal_to_binary(-2));
}

/**
 * Test pro prevod binarniho na decimalni cislo.
 */
TEST_F(calculate_int, opBinToDec)
{
	EXPECT_EQ(calc.binary_to_decimal((long long)x), 1);

	EXPECT_EQ(calc.get_result(), 1);

	EXPECT_EQ(calc.binary_to_decimal((long long)10101010), 170);

	EXPECT_EQ(calc.get_result(), 170);

	EXPECT_ANY_THROW(calc.binary_to_decimal((long long)y));
}


/**
 * Test pro prevod decimalniho na oktalove cislo.
 */
TEST_F(calculate_int, opDecToOct)
{
	EXPECT_EQ(calc.decimal_to_octal(10245), 24005);

	EXPECT_ANY_THROW(calc.decimal_to_octal(-12));
}

/**
 * Test pro prevod oktaloveho na decimalni cislo.
 */
TEST_F(calculate_int, opOctToDec)
{
	EXPECT_EQ(calc.octal_to_decimal(24005), 10245);

	EXPECT_ANY_THROW(calc.octal_to_decimal(-24005));
}


/**
 * Test pro prevod decimalniho na hexadecimalni cislo.
 */
TEST_F(calculate_int, opDecToHex)
{
	EXPECT_EQ(calc.decimal_to_hex(10245), std::to_string(2805));

	EXPECT_ANY_THROW(calc.decimal_to_hex(-10245));
}

/**
 * Test pro prevod hexadecimalniho na decimalni cislo.
 */
TEST_F(calculate_int, opHexToDec)
{
	EXPECT_EQ(calc.hex_to_decimal(std::to_string(2805)), 10245);

	EXPECT_ANY_THROW(calc.hex_to_decimal(std::to_string(-2805)));
}

/**
 * Test pro prevod binarniho na oktalove cislo.
 */
TEST_F(calculate_int, opBinToOct)
{
	EXPECT_EQ(calc.binary_to_octal(101010), 52);

	EXPECT_ANY_THROW(calc.binary_to_octal(-101010));
}

/**
 * Test pro prevod oktaloveho na binarni cislo.
 */
TEST_F(calculate_int, opOctToBin)
{
	EXPECT_EQ(calc.octal_to_binary(52), (int)101010);

	EXPECT_ANY_THROW(calc.octal_to_binary(-52));
}


/**
 * Test pro prevod binarniho na hexadecimalni cislo.
 */
TEST_F(calculate_int, opBinToHex)
{
	EXPECT_EQ(calc.binary_to_hex(101010), std::string("2A"));

	EXPECT_ANY_THROW(calc.binary_to_hex(-101010));
}

/**
 * Test pro prevod hexadecimalniho na binarni cislo.
 */
TEST_F(calculate_int, opHexToBin)
{
	EXPECT_EQ(calc.hex_to_binary(std::string("2A")), 101010);

	EXPECT_ANY_THROW(calc.hex_to_binary(std::string("-2A")));
}


/**
 * Test pro prevod oktaloveho na hexadecimalni cislo.
 */
TEST_F(calculate_int, opOctToHex)
{
	EXPECT_EQ(calc.octal_to_hex(52), std::string("2A"));

	EXPECT_ANY_THROW(calc.octal_to_hex(-52));	
}

/**
 * Test pro prevod hexadecimalniho na oktalove cislo.
 */
TEST_F(calculate_int, opHexToOct)
{
	EXPECT_EQ(calc.hex_to_octal(std::string("2A")), 52);

	EXPECT_ANY_THROW(calc.hex_to_octal(std::string("-2A")));
}

/*** Konec souboru math_library_tests.cpp ***/