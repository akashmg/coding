/**
 * File name:   Calculations.c
 * Description: This file provides the user interface to get command line arguments 
 *              and to print out the result of the card shuffling quiz.
 * Author:      Akash Gujarati
 * Date:        Aug 1st, 2015
 */
/********* I N C L U D E S ***********/ 
#include "Calculations.h"
#include <stdint.h>
#include <stdbool.h>

/********* D E F I N E S ***********/ 

/********* T Y P E D E F S ***********/ 
typedef struct cardList {
    uint16_t          value;
    struct cardList * next;
    struct cardList * origNext;
} CardList_S;

typedef struct {
    uint16_t numCards;
    uint16_t result;
} CalculationVars_S;

/********* PUBLIC DATA ***********/ 

/********* PRIVATE DATA ***********/
CalculationVars_S CalculationVars; 
CardList_S * head = NULL;

/********* PRIVATE FUNCTION DEFINITIONS ***********/
static void createCardList(uint16_t numCards); 
static void invertList(void);

/********* PUBLIC FUNCTIONS ***********/ 
uint16_t calcNumIterations(uint16_t numCards)
{
    createCardList(numCards);

    return numCards;
}

/********* PRIVATE FUNCTIONS ***********/ 
static void createCardList(uint16_t numCards)
{

    CardList_S * newCard;

    for (int i=1; i<=numCards; i++)
    {   
        newCard = (CardList_S *)malloc(sizeof(CardList_S));
        newCard->value = i;
        newCard->origNext = head;
        newCard->next = head;
        head = newCard;
    }

    invertList();

}

static void invertList(void)
{
    CardList_S * tempNext = NULL;
    CardList_S * prevNext = NULL;
    while(head->origNext != NULL)
    {
        prevNext = head->origNext;
        head->origNext = tempNext;
        tempNext = head;                
        head = prevNext;
    }
    // Invert the next for the last (first) value
    head->origNext = tempNext;

}