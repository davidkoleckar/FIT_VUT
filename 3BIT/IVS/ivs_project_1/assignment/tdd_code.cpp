//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     David Koleckar <xkolec07@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author David Koleckar - xkolec07
 * 
 * @brief Implementace metod tridy prioritni fronty.
 */

#include <stdlib.h>
#include <stdio.h>

#include "tdd_code.h"

//============================================================================//
// ** ZDE DOPLNTE IMPLEMENTACI **
//
// Zde doplnte implementaci verejneho rozhrani prioritni fronty (Priority Queue)
// 1. Verejne rozhrani fronty specifikovane v: tdd_code.h (sekce "public:")
//    - Konstruktor (PriorityQueue()), Destruktor (~PriorityQueue())
//    - Metody Insert/Remove/Find a GetHead
//    - Pripadne vase metody definovane v tdd_code.h (sekce "protected:")
//
// Cilem je dosahnout plne funkcni implementace prioritni fronty implementovane
// pomoci tzv. "double-linked list", ktera bude splnovat dodane testy 
// (tdd_tests.cpp).
//============================================================================//

PriorityQueue::PriorityQueue()
{
	head = NULL;
}

PriorityQueue::~PriorityQueue()
{
	Element_t *temp;
	while (head != NULL) {
		temp = head->pNext;
		delete head;
		head = temp;
	}
}

void PriorityQueue::Insert(int value)
{
	Element_t *temp;
	Element_t *n = new Element_t;
	n->pNext = NULL;
	n->pPrev = NULL;
	n->value = value;
	temp = head;

	if (head == NULL) {
		head = n;
	}
	else {
		while (temp != NULL) {
			if (temp->value < value && temp->pNext != NULL) {
				temp = temp->pNext;
			}
			else {
				if (temp->pPrev == NULL && temp->value > value) {
					n->pNext = temp;
					temp->pPrev = n;	
					head = n;
					break;
				}
				else if (temp->pNext == NULL && temp->value < value) {
					temp->pNext = n;
					n->pPrev = temp;
					break;
				}
				else {
					n->pNext = temp;
					n->pPrev = temp->pPrev;
					temp->pPrev->pNext = n;
					temp->pPrev = n;
					break;
				}
			}
		}
	}

}

bool PriorityQueue::Remove(int value)
{

	Element_t *temp = head;
	Element_t *current = head;
	Element_t *del = NULL;

	while (current != NULL && current->value != value) {
		temp = current;
		current = current->pNext;
	}

	if (current == NULL) {
		return false;
	}
	
	else {
		del = current;
		current = current->pNext;
		temp->pNext = current;
		if (del == head) {
			head = head->pNext;
		}
		delete del;
		return true;
	}
}

PriorityQueue::Element_t *PriorityQueue::Find(int value)
{

	Element_t *temp = head;
	Element_t *current = head;

	while (current != NULL && current->value != value) {
		temp = current;
		current = current->pNext;
	}
	if (current != NULL) {
		return current;
	}
	else {
		return NULL;
	}
}

PriorityQueue::Element_t *PriorityQueue::GetHead()
{
	return head;  
}

/*** Konec souboru tdd_code.cpp ***/
