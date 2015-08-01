/**
 * File name:   Calculations.c
 * Description: This file provides the user interface to get command line arguments 
 *              and to print out the result of the card shuffling quiz.
 * Author:      Akash Gujarati
 * Date:        Aug 1st, 2015
 */
/********* I N C L U D E S ***********/ 
#include "Calculations.h"
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
CardList_S * tail = NULL;

/********* PRIVATE FUNCTION DEFINITIONS ***********/
static void createCardList(uint16_t numCards); 
static CardList_S * invertList(CardList_S * listHead);
static void beginCalculation(void);
static void iterateShuffle(void);
static bool compareOriginal(void);
static CardList_S * cardPop(void);
static void cardPopAndAppend(void);
static void deleteCardList(CardList_S * listHead);

/********* PUBLIC FUNCTIONS ***********/ 
uint16_t calcNumIterations(uint16_t numCards)
{
    createCardList(numCards);
    beginCalculation();
    return CalculationVars.result;
}

/********* PRIVATE FUNCTIONS ***********/ 
/**
 * createCardList This function creates a linked list of n elements where
 *                 n is the number of cards argument passed to it.
 * 
 * @param numCards unsigned short integer value of the number of
 *                 cards in the deck.
 */
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

    head = invertList(head);

    CardList_S *temp = head;
    // Find last element of list (before the NULL element)
    while (temp != NULL)
    {
        tail = temp;
        temp->origNext = temp->next;    // update original list order
        temp = temp->next;
    }
}


/**
 * This function takes a pointer to the head of a list and
 * returns a pointer to the inverted list.
 * @param  listHead The head of the linked list
 * @return          Pointer to the head of the inverted linked list.
 */
static CardList_S * invertList(CardList_S * listHead)
{
    CardList_S * tempNext = NULL;
    CardList_S * prevNext = NULL;
    while(listHead->next != NULL)
    {
        prevNext = listHead->next;
        listHead->next = tempNext;
        tempNext = listHead;                
        listHead = prevNext;
    }
    // Invert the next for the last (now first) value
    listHead->next = tempNext;

    return listHead;
}

/**
 * This function is the container of the function calls to the 
 * shuffling iterations and the check to see if original list order
 * has been achieved.
 */
static void beginCalculation(void)
{
    bool backToSquareOne = false;
    uint16_t loopCounter = 0U;
    while (!backToSquareOne)
    {
        iterateShuffle();

        if (compareOriginal())
        {
            backToSquareOne = true;
        }
    }

    CalculationVars.result = loopCounter;
}

/**
 * This function does the iterations for the special shuffle pattern.
 */
static void iterateShuffle(void)
{
    CardList_S *tableHead = NULL;
    CardList_S *newTableCard;
    bool removeCard = true;

    while (head != NULL)
    {
        if (removeCard || (head == tail))
        {
            newTableCard = cardPop();
            newTableCard->next = tableHead;
            tableHead = newTableCard;
            removeCard = false;
        }
        else
        {
            cardPopAndAppend();
            removeCard = true;
        }
    }

    head = invertList(tableHead);

}

/**
 * This function compares the original order to the current order of 
 * the list.
 * @return  [If the original list order is achieved then it returns true.]
 */
static bool compareOriginal(void)
{
    bool match;
    CardList_S * temp = head;
    // Check while the end of the list is not reached
    while(temp != NULL)
    {
        // If order matches original
        if (temp->next == temp->origNext)
        {
            match = true;
        }
        else
        {
            match = false;
            break;
        }
        temp = temp->next;
    }
    return match;
}

static CardList_S * cardPop(void)
{
    CardList_S * temp;
    temp = head;                // Assign head to a temporary pointer
    head = temp->next;          // Pop the first element and update head
    return temp;                // Return popped element
}

static void cardPopAndAppend(void)
{
    CardList_S * temp = head;   // Assign head to a temporary pointer

    head = temp->next;          // Pop the head element
    temp->next = tail->next;    // Assign the NULL pointer to temp->next
    tail->next = temp;          // Append popped element to tail
    tail = temp;                // Update tail

    temp = head;

}