//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     David Koleckar <xkolec07@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author David Koleckar - xkolec07
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//
class EmptyTree : public ::testing::Test
{
protected:
    BinaryTree tree;
};

class NonEmptyTree : public ::testing::Test
{
protected:
    virtual void SetUp() {
        int values[] = { 13,8,17,1,11,15,25,6,22,27 };

        for(int i = 0; i < 10; ++i)
            tree.InsertNode(values[i]);
    }

    BinaryTree tree;
};
//Vlozeni do prazdneho stromu
TEST_F(EmptyTree, InsertEmptyTree)
{
    EXPECT_TRUE(tree.InsertNode(2).first);
    EXPECT_FALSE(tree.InsertNode(2).first);
}
//Vlozeni do neprazdneho stromu
TEST_F(NonEmptyTree, InsertNonEmptyTree)
{
    EXPECT_TRUE(tree.InsertNode(35).first);
    EXPECT_FALSE(tree.InsertNode(22).first);
}
//Smazani prvku v prazdnem stromu
TEST_F(EmptyTree, DeleteEmptyTree)
{
    EXPECT_FALSE(tree.DeleteNode(2));
}
//Smazani prvku v neprazdnem stromu
TEST_F(NonEmptyTree, DeleteNonEmptyTree)
{
    EXPECT_TRUE(tree.DeleteNode(27));
    EXPECT_FALSE(tree.DeleteNode(60));
}
//Hledání prvku v prázdném stromu
TEST_F(EmptyTree, FindEmptyTree)
{
    EXPECT_TRUE(tree.FindNode(2) == NULL);
}

//Hledání prvku v neprázdném stromu
TEST_F(NonEmptyTree, FindNonEmptyTree)
{
    int values[] = { 13,8,17,1,11,15,25,6,22,27 };

    for(int i = 0; i < 10; ++i)
        EXPECT_TRUE(tree.FindNode(values[i]) != NULL);

    EXPECT_TRUE(tree.FindNode(66) == NULL);
}
//Vsechny listove uzly jsou cerne
TEST_F(NonEmptyTree, FirstAxiomNonEmptyTree)
{
    std::vector<BinaryTree::Node_t *> outLeafNodes;
    tree.GetLeafNodes(outLeafNodes);
    for(auto const& value: outLeafNodes) {
        EXPECT_EQ(value->color,1);
    }
}

//Pokud je uzel cerveny, pak jsou jeho oba potomci cerne
TEST_F(NonEmptyTree, SecondAxiomNonEmptyTree)
{
    std::vector<BinaryTree::Node_t *> outAllNodes;
    tree.GetAllNodes(outAllNodes);
    for(auto const& value: outAllNodes) {
        if(value->color == 0){
            EXPECT_EQ(value->pLeft->color,1);
            EXPECT_EQ(value->pRight->color,1);
        }  
    }
}

//Kazda cesta od kazdeho listoveho uzlu ke koreni obsahuje 
//stejny pocet cernych uzlu
TEST_F(NonEmptyTree, ThirdAxiomNonEmptyTree)
{
    std::vector<BinaryTree::Node_t *> outLeafNodes;
    tree.GetLeafNodes(outLeafNodes);
    BinaryTree::Node_t *root = tree.GetRoot();
    BinaryTree::Node_t *n;
    int a;        
    int before;

    for(auto const& value: outLeafNodes) {
       n = value->pParent;
       before=0;
       while(n != root){
        if(n->color == 1) 
            before++;
        n = n->pParent;
       }
    }
    for(auto const& value: outLeafNodes) {
       n = value->pParent;
       a=0;
       while(n != root){
        if(n->color == 1) 
            a++;
        n = n->pParent;
       }
       EXPECT_EQ(a,before);
    }
}
/*** Konec souboru black_box_tests.cpp ***/
