//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - Tests suite
//
// $NoKeywords: $ivs_project_1 $white_box_code.cpp
// $Author:     David Koleckar <xkolec07@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author David Koleckar - xkolec07
 * 
 * @brief Implementace testu prace s maticemi.
 */

#include "gtest/gtest.h"
#include "white_box_code.h"


//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy operaci nad maticemi. Cilem testovani je:
// 1. Dosahnout maximalniho pokryti kodu (white_box_code.cpp) testy.
// 2. Overit spravne chovani operaci nad maticemi v zavislosti na rozmerech 
//    matic.
//============================================================================//

class EmptyMatrix : public ::testing::Test
{

  protected:

    Matrix SmallSquare; //1x1
    Matrix LargeSquare = Matrix(5, 5);
    Matrix Rectangle1 = Matrix(3, 9); 
    Matrix Rectangle2 = Matrix(9, 3);
};

class NonEmptyMatrix : public ::testing::Test
{
  protected:
    virtual void SetUp() {
      std::vector< std::vector<double> > values(5, std::vector<double>(5, 5));
      LargeSquare.set(values);
                
      values = std::vector< std::vector<double> > (3, std::vector<double>(9, 5));
      Rectangle1.set(values);

      values = std::vector< std::vector<double> > (9, std::vector<double>(3, 1));
      Rectangle2.set(values);

      SmallSquare.set(0,0,1);

  }
  Matrix SmallSquare; //1x1
  Matrix LargeSquare = Matrix(5, 5);
  Matrix Rectangle1 = Matrix(3, 9);
  Matrix Rectangle2 = Matrix(9, 3);
};

//0.Test constructor 
TEST_F(EmptyMatrix, ConstructorMatrix)
{
  EXPECT_ANY_THROW(Matrix(0,0));
}
 
//1.Test nastaveni hodnoty na urcitou pozici
TEST_F(EmptyMatrix, SetEmpty)
{
  EXPECT_FALSE(SmallSquare.set(2, 2, 2));
  EXPECT_TRUE(SmallSquare.set(0, 0, 2));
  EXPECT_TRUE(LargeSquare.set(2, 3, 0.2));
  EXPECT_FALSE(LargeSquare.set(7, 5, 0.2));
}

TEST_F(EmptyMatrix, SetArrayEmpty)
{
  std::vector< std::vector<double> > values(1, std::vector<double>(1, 2));
  EXPECT_TRUE(SmallSquare.set(values));
  EXPECT_EQ(SmallSquare.get(0, 0), 2);

  values = std::vector< std::vector<double> > (6, std::vector<double>(6, 3));
  Matrix s = Matrix(5, 5);
  EXPECT_TRUE(s.set(values));
  EXPECT_EQ(s.get(2,3), 3);

  values = std::vector< std::vector<double> > (2, std::vector<double>(2, 3));
  EXPECT_FALSE(s.set(values));

 	values = std::vector< std::vector<double> > (3, std::vector<double>(9, 3));
 	EXPECT_TRUE(Rectangle1.set(values));
}

TEST_F(NonEmptyMatrix, SetNonEmpty)
{
  EXPECT_TRUE(LargeSquare.set(2, 2, 2));
  EXPECT_EQ(LargeSquare.get(2, 2), 2);
}

//2. Test GET 
TEST_F(NonEmptyMatrix, GetNonEmpty)
 {
  EXPECT_EQ(SmallSquare.get(0,0), 1);
  EXPECT_EQ(LargeSquare.get(3,3),5);
  std::string s0 ("nan");
  EXPECT_EQ( std::to_string(LargeSquare.get(10,10)),s0);
  EXPECT_EQ(Rectangle1.get(2, 2), 3);
  EXPECT_EQ(Rectangle2.get(1, 1), 1);
}

TEST_F(EmptyMatrix, GetEmpty)
 {
  EXPECT_EQ(SmallSquare.get(0,0), 0);
  EXPECT_EQ(LargeSquare.get(3,3),0);
  EXPECT_EQ(Rectangle1.get(2, 2), 0);
  EXPECT_EQ(Rectangle2.get(1, 1), 0);
}

//3.Test Operator == 
TEST_F(NonEmptyMatrix, EqualMatrixNonEmpty)
{
  Matrix CompareMatrix = Matrix(5, 5);
  std::vector< std::vector<double> > values(5, std::vector<double>(5, 2));
  CompareMatrix.set(values);

  EXPECT_FALSE(LargeSquare.operator==(CompareMatrix));
  values = std::vector< std::vector<double> > (5, std::vector<double>(5, 5));
  CompareMatrix.set(values);
  EXPECT_TRUE(LargeSquare.operator==(CompareMatrix));

  EXPECT_ANY_THROW(SmallSquare.operator==(CompareMatrix));
}

TEST_F(EmptyMatrix, EqualMatrixEmpty)
{
	Matrix CompareMatrix = Matrix(5, 5);
	EXPECT_TRUE(LargeSquare.operator==(CompareMatrix));

	CompareMatrix = Matrix(1, 1);
	EXPECT_TRUE(SmallSquare.operator==(CompareMatrix));
	CompareMatrix = Matrix(3, 9);
	EXPECT_TRUE(Rectangle1.operator==(CompareMatrix));
	CompareMatrix = Matrix(9, 3);
	EXPECT_TRUE(Rectangle2.operator==(CompareMatrix));

}

//4.Test Operator +=
TEST_F(NonEmptyMatrix, AddMatrixNonEmpty)
{
  Matrix AddMatrix = Matrix(5, 5);
  std::vector< std::vector<double> > values(5, std::vector<double>(5, 2));
  AddMatrix.set(values);
  Matrix result = LargeSquare.operator+(AddMatrix);

  for (int i = 0; i < 5; i++)
    for (int j = 0; j < 5; j++)
      EXPECT_EQ(result.get(i, j), 7);
  EXPECT_ANY_THROW(SmallSquare.operator+(LargeSquare));

  AddMatrix = Matrix(1, 1);
  AddMatrix.set(0,0,1);
  result = SmallSquare.operator+(AddMatrix);
  EXPECT_EQ(result.get(0, 0), 2);

  AddMatrix = Matrix(9, 3);
  values = std::vector< std::vector<double> > (9, std::vector<double>(3, 1));
  AddMatrix.set(values);
  result = Rectangle2.operator+(AddMatrix);
  for (int i = 0; i < 9; i++)
    for (int j = 0; j < 3; j++)
      EXPECT_EQ(result.get(i, j), 2);

}

TEST_F(EmptyMatrix, AddMatrixEmpty)
{
  Matrix AddMatrix = Matrix(5, 5);
  std::vector< std::vector<double> > values(5, std::vector<double>(5, 2));
  AddMatrix.set(values);
  Matrix result = LargeSquare.operator+(AddMatrix);

  for (int i = 0; i < 5; i++)
    for (int j = 0; j < 5; j++)
      EXPECT_EQ(result.get(i, j), 2);
  EXPECT_ANY_THROW(SmallSquare.operator+(LargeSquare));

  AddMatrix = Matrix(1, 1);
  AddMatrix.set(0,0,1);
  result = SmallSquare.operator+(AddMatrix);
  EXPECT_EQ(result.get(0, 0), 1);

  AddMatrix = Matrix(9, 3);
  values = std::vector< std::vector<double> > (9, std::vector<double>(3, 1));
  AddMatrix.set(values);
  result = Rectangle2.operator+(AddMatrix);
  for (int i = 0; i < 9; i++)
    for (int j = 0; j < 3; j++)
      EXPECT_EQ(result.get(i, j), 1);
}

//5.Test Operator *=
TEST_F(NonEmptyMatrix, MultiplyMatrixNonEmpty)
{
  Matrix MultiplyMatrix = Matrix(5, 5);
  std::vector< std::vector<double> > values(5, std::vector<double>(5, 2));
  MultiplyMatrix.set(values);
  Matrix result = LargeSquare.operator*(MultiplyMatrix);

  for (int i = 0; i < 5; i++)
    for (int j = 0; j < 5; j++)
    EXPECT_EQ(result.get(i, j), 50);

  EXPECT_ANY_THROW(SmallSquare.operator*(LargeSquare));
}

//6.TEST Operator *= skalarni hodnotou
TEST_F(NonEmptyMatrix, MultiplyScalarMatrixNonEmpty)
{
  Matrix result = LargeSquare.operator*(2);
  for (int i = 0; i < 5; i++)
   for (int j = 0; j < 5; j++)
     EXPECT_EQ(result.get(i, j), 10);

   result = SmallSquare.operator*(2);
   EXPECT_EQ(result.get(0, 0), 2);


   result = Rectangle2.operator*(2);
   for (int i = 0; i < 9; i++)
   	for (int j = 0; j < 3; j++)
     EXPECT_EQ(result.get(i, j), 2);

}

TEST_F(EmptyMatrix, MultiplyScalarMatrixEmpty)
{
  Matrix result = LargeSquare.operator*(2);
  for (int i = 0; i < 5; i++)
   for (int j = 0; j < 5; j++)
     EXPECT_EQ(result.get(i, j), 0);

   result = SmallSquare.operator*(2);
   EXPECT_EQ(result.get(0, 0), 0);

   result = Rectangle1.operator*(2);
   for (int i = 0; i < 3; i++)
   	for (int j = 0; j < 9; j++)
     EXPECT_EQ(result.get(i, j), 0);

   result = Rectangle2.operator*(2);
   for (int i = 0; i < 9; i++)
   	for (int j = 0; j < 3; j++)
     EXPECT_EQ(result.get(i, j), 0);

}

//7.TEST Solve Equation
TEST_F(NonEmptyMatrix, SolveEquationMatrixNonEmpty)
{
  std::vector<double> b = {5,15};
  //singularni det=0
  Matrix singular = Matrix(2, 2);
  singular.set(0, 0,1);
  singular.set(0, 1,-2);
  singular.set(1, 0,-2);
  singular.set(1, 1,4);
  EXPECT_ANY_THROW(singular.solveEquation(b));
  //Pocet prvku prave strany rovnice musi odpovidat poctu radku matice.
  EXPECT_ANY_THROW(LargeSquare.solveEquation(b));
  //checkSquare
  std::vector<double> bb = {5,15,3,4,5,6,7,8,9};
  EXPECT_ANY_THROW(Rectangle2.solveEquation(bb));

  //2x2
  Matrix test = Matrix(2, 2);
  std::vector<double> result; 
  test.set(0, 0,1);
  test.set(0, 1,2);
  test.set(1, 0,4);
  test.set(1, 1,3);
  result = test.solveEquation(b);
  auto it = result.begin();
  EXPECT_EQ(*it, 3);
  it++;
  EXPECT_EQ(*it, 1);

  //1x1
  b={2};
  result = SmallSquare.solveEquation(b);
  it = result.begin();
  EXPECT_EQ(*it, 2);

  //3x3
  Matrix test3 = Matrix(3, 3);
  test3.set(0, 0,2);test3.set(0, 1,1);test3.set(0, 2,3);
  test3.set(1, 0,1);test3.set(1, 1,-2);test3.set(1, 2,1);
  test3.set(2, 0,3);test3.set(2, 1,2);test3.set(2, 2,2);
  b={9,-2,7};
  result = test3.solveEquation(b);
  it = result.begin();
  EXPECT_EQ(*it, -1);
  it++;
  EXPECT_EQ(*it, 2);
  it++;
  EXPECT_EQ(*it, 3);

  //4x4
  Matrix test4 = Matrix(4, 4);
  test4.set(0, 0,1);test4.set(0, 1,1);test4.set(0, 2,1);test4.set(0, 3,1);
  test4.set(1, 0,1);test4.set(1, 1,1);test4.set(1, 2,-1);test4.set(1, 3,-1);
  test4.set(2, 0,1);test4.set(2, 1,-1);test4.set(2, 2,1);test4.set(2, 3,-1);
  test4.set(3, 0,1);test4.set(3, 1,-1);test4.set(3, 2,-1);test4.set(3, 3,1);
  b={10,4,2,0};
  result = test4.solveEquation(b);
  it = result.begin();
  EXPECT_EQ(*it, 4);
  it++;
  EXPECT_EQ(*it, 3);
  it++;
  EXPECT_EQ(*it, 2);
  it++;
  EXPECT_EQ(*it, 1);
}



//
/*** Konec souboru white_box_tests.cpp ***/
