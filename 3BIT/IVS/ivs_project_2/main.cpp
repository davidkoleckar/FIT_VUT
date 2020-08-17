#include "math_library.h"
#include <iostream>

int main(int argc, char const *argv[])
{	
	Calculator calc = Calculator();

	std::cout << "Addition:" << std::endl;
	std::cout << "5 + 2 = " << calc.addition(5, 2) << std::endl;
	std::cout << "7.2 + 6 = " << calc.addition(7.2, 6) << std::endl;
	std::cout << "7 + 6.2 = " << calc.addition(7, 6.2) << std::endl;
	std::cout << "7.2 + 6.2 = " << calc.addition(7.2, 6.2) << std::endl;
		
	std::cout << std::endl << "subtraction:" << std::endl;
	std::cout << "5 - 2 = " << calc.subtraction(5, 2) << std::endl;
	std::cout << "7.2 - 6 = " << calc.subtraction(7.2, 6) << std::endl;
	std::cout << "7 - 6.2 = " << calc.subtraction(7, 6.2) << std::endl;
	std::cout << "7.2 - 6.2 = " << calc.subtraction(7.2, 6.2) << std::endl;

	try
	{
		std::cout << std::endl << "Multiplication:" << std::endl;
		std::cout << "5 * 2 = " << calc.multiplication(5, 2) << std::endl;
		std::cout << "7.2 * 6 = " << calc.multiplication(7.2, 6) << std::endl;
		std::cout << "7 * 6.2 = " << calc.multiplication(7, 6.2) << std::endl;
		std::cout << "7.2 * 6.2 = " << calc.multiplication(7.2, 6.2) << std::endl;

		std::cout << std::endl << "Division:" << std::endl;
		std::cout << "5 / 2 = " << calc.division(5, 2) << std::endl;
		std::cout << "7.2 / 6 = " << calc.division(7.2, 6) << std::endl;
		std::cout << "7 / 6.2 = " << calc.division(7, 6.2) << std::endl;
		std::cout << "7.2 / 6.2 = " << calc.division(7.2, 6.2) << std::endl;

		std::cout << std::endl << "Modulation:" << std::endl;
		std::cout << "5 % 2 = " << calc.modulation(5, 2) << std::endl;
		std::cout << "7.2 % 6 = " << calc.modulation(7.2, 6) << std::endl;
		std::cout << "7 % 6.2 = " << calc.modulation(7, 6.2) << std::endl;
		std::cout << "7.2 % 6.2 = " << calc.modulation(7.2, 6.2) << std::endl;
	}
	catch(std::runtime_error* e)
	{
		std::cerr << e->what() << std::endl;
	}
	
	try
	{
		std::cout << std::endl << "Factorial:" << std::endl;
		std::cout << "5! = " << calc.factorial(5) << std::endl;
		calc.factorial(5.2);
		calc.factorial(-5);
	}
	catch (std::runtime_error* e)
	{
		std::cerr << e->what() << std::endl;
	}

	try
	{
		std::cout << std::endl << "Power:" << std::endl;
		std::cout << "5 ^ 2 = " << calc.power(5, 2) << std::endl;
		std::cout << "7.2 ^ 6 = " << calc.power(7.2, 6) << std::endl;
		std::cout << "7 ^ 6.2 = " << calc.power(7, 6.2) << std::endl;
		std::cout << "7.2 ^ 6.2 = " << calc.power(7.2, 6.2) << std::endl;
	}
	catch(std::runtime_error* e)
	{
		std::cerr << e->what() << std::endl;
	}

	try
	{
		std::cout << std::endl << "Root:" << std::endl;
		std::cout << "square root from 5 = " << calc.root(5, 2) << std::endl;
		std::cout << "sixth root from 7.2 = " << calc.root(7.2, 6) << std::endl;
		std::cout << "6.2 root from 7 = " << calc.root(7, 6.2) << std::endl;
		std::cout << "6.2 root from 7.2 = " << calc.root(7.2, 6.2) << std::endl;
	}
	catch (std::runtime_error* e)
	{
		std::cerr << e->what() << std::endl;
	}
	
	std::cout << std::endl << "Standard deviation:" << std::endl;
	try
	{
		double number = 0;
		std::vector<double> numbers;

		while (std::cin >> number)
			numbers.push_back(number);

		std::cout << "SD from your input = " << calc.standard_deviation(numbers) << std::endl;
	}
	catch (std::runtime_error* e)
	{
		std::cerr << e->what() << std::endl;
	}

	return 0;
}