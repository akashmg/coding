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
#include <stdlib.h>
#include <stdio.h>

/********* D E F I N E S ***********/ 
 #define TURN_TABLE_DECK_OVER_FOR_RESHUFFLE 0   // Set to 1 if you turn the deck over when you pick it up from the table.
 #define START_CARDS_SHUFFLE_IN_REVERSE     0   // Set to 1 if the card order should be reversed for first shuffle

/********* T Y P E D E F S ***********/ 
// This is the linked list structure for each element.
typedef struct cardList {
    uint16_t          value;    // Holds the value of the card
    struct cardList * next;     // Pointer to the next element in the modified list
    struct cardList * origNext; // Pointer to the next element in the original list
} CardList_S;

// Structure for a local copy of the result and the nubmer of cards
typedef struct {
    uint16_t numCards;          // Number of cards in the deck
    uint16_t result;            // Number of iterations it took to shuffle back to the original pattern
} CalculationVars_S;

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
    createCardList(numCards);   // Create a linked list to implement the task
    beginCalculation();         // simulate the shuffling and calculate required result
    deleteCardList(head);       // free any allocated memory

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
    // Insert n elements in linked list where n is the number of Cards
    for (int i=1; i<=numCards; i++)
    {   
        newCard = (CardList_S *)malloc(sizeof(CardList_S));
        newCard->value = i;
        newCard->origNext = head;
        newCard->next = head;
        head = newCard;
    }
#if (START_CARDS_SHUFFLE_IN_REVERSE)
    head = invertList(head); // Uncomment if starting order should be ascending
#endif

    CardList_S *temp = head;
    // Find last element of list (before the NULL element) and assign to tail
    while (temp != NULL)
    {
        tail = temp;
        temp->origNext = temp->next;    // update original list order - only here!
        temp = temp->next;
    }

}


/**
 * invertList This function takes a pointer to the head of a list and
 *            returns a pointer to the inverted list.
 * @param  listHead The head of the linked list
 * @return Pointer to the head of the inverted linked list.
 */
static CardList_S * invertList(CardList_S * listHead)
{
    CardList_S * tempNext = NULL;
    CardList_S * prevNext = NULL;
    // While the end of list is not reached
    while(listHead->next != NULL)
    {
        prevNext = listHead->next;  // Store the next element
        listHead->next = tempNext;  // Update the next element
        tempNext = listHead;        // Move the temp next back
        listHead = prevNext;        // Update the current head
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
    // Loop while the original pattern is not achieved
    while (!backToSquareOne)
    {
        loopCounter++;      // Increment loop counter to keep track of number of iterations
        iterateShuffle();   // Repeat shuffling of the deck
        // Check if current order matches original
        if (compareOriginal())
        {
            backToSquareOne = true;
        }

    }

    CalculationVars.result = loopCounter;   // Update the result
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
            if (head != tail)       // If it is the last card then just add it to the table pile.
            {
                cardPopAndAppend();
            }
            removeCard = true;
        }
    }

#if (TURN_TABLE_DECK_OVER_FOR_RESHUFFLE)
    head = invertList(tableHead); 
#else
    head = tableHead;
#endif

    CardList_S *temp = head;
    // Find last element of list (before the NULL element)
    while (temp != NULL)
    {
        tail = temp;
        temp = temp->next;
    }
 
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

/**
 * cardPop This function pops the head of the list and returns the popped value
 * @return  [list element that was popped]
 */
static CardList_S * cardPop(void)
{
    CardList_S * temp;
    temp = head;                // Assign head to a temporary pointer
    head = temp->next;          // Pop the first element and update head
    return temp;                // Return popped element
}

/**
 * cardPopAndAppend This function pops the head and appends it to the tail of the list
 */
static void cardPopAndAppend(void)
{
    CardList_S * temp = head;   // Assign head to a temporary pointer

    head = temp->next;          // Pop the head element
    temp->next = tail->next;    // Assign the NULL pointer to temp->next
    tail->next = temp;          // Append popped element to tail
    tail = temp;                // Update tail

}

/**
 * deleteCardList This function frees any allocated memory
 * @param listHead [Linked list head]
 */
static void deleteCardList(CardList_S * listHead)
{
    CardList_S * temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}