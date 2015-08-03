/**
 * File name:   AppleQuiz.c
 * Description: This file provides the user interface to get command line arguments 
 *              and to print out the result of the card shuffling quiz.
 * Author:      Akash Gujarati
 * Date:        Aug 1st, 2015
 */
/********* I N C L U D E S ***********/ 
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "Calculations.h"

/********* D E F I N E S ***********/ 
#define EXIT_SUCCESS     0
#define EXIT_INPUT_ERROR 1
#define EXIT_RETRY_ERROR 2
#define MAX_INTEGER_DIGITS 5

/********* PRIVATE FUNCTION DEFINITIONS ***********/ 
static void progIntro(void);
static uint16_t getUserInput(void);
static void retryInput(void);

/********** MAIN FUNCTION *********/
int main(int argc, char *argv[])
{
    uint16_t numCards;
    uint16_t result;

    // Explain the program to the user
    progIntro();

    while (1)
    {
        // Get user input
        numCards = getUserInput();
        // Calculate values
        result = calcNumIterations(numCards);
        // Print result
        if (result > 0U)
        {  
            printf("%hu iterations are needed to return to starting pattern.\n", result);
        }
        // Check if user wants to try again
        retryInput();
    }

    return 0;
}



/********* PRIVATE FUNCTIONS ***********/
/**
 * This function prints a short description of the program.
 */
static void progIntro(void)
{
    printf("Hello! This program computes the number of iterations needed to get back to square one!\n");
    printf("Given a deck of cards, a special shuffling pattern is implemented - the top card is alternated between,"
            "first, being put on a table (assuming you are sitting at a table) and, second, placed at the bottom of the deck.\n");
    printf("Once all cards are on the table, the same process is repeated until the original sequence of cards is returned.\n");
    printf("The goal of this code is to count the number of repetitions needed to complete this cycle.\n");
    printf("The user gets to input the number of cards present in the deck.\n");
    printf("The first 5 characters of your input will be accepted and an integer will be extracted upto a valid character.\n");
    printf("If the computation takes longer than 1 minute then I will quit.\n");

}

/**
 * [getUserInput This function gets the user input]
 * @return  [Unsigned integer entered by user.]
 */
static uint16_t getUserInput(void)
{
    uint16_t checkInput;           // check result of fgets
    char     inputString[(MAX_INTEGER_DIGITS + 1)];    
    int16_t inputNumber;           // input number
    bool     inputSuccess = false; // Poll user until we get a successful input
    uint16_t exitCounter = 3U;     // count down to zero how many input errors user has made.

    // Give user multiple chances to enter valid command
    while (!inputSuccess)
    {
        // Prompt user for input
        printf("\nPlease choose how many cards this deck will have and then hit the return key (Enter)\n");
        printf("Number: ");
        checkInput = (fgets(inputString, (MAX_INTEGER_DIGITS + 1), stdin) != NULL); // Get input number

        if (checkInput && (inputString[0] != '\n'))
        {
            // if user input is longer than what we expect
            if (inputString[strlen(inputString)-1] != '\n')
            {
                while(getchar() != '\n');       // Get all overflow characters
            }
            inputNumber = atoi(inputString);
        }
        else
        {
            printf("No input was received!\n");
            continue;
        }
        // If valid then exit loop
        if (inputNumber > 0)
        {
            inputSuccess = true;
            printf("Calculating for %d cards...\n", inputNumber);
        }
        else    // Retry implementation
        {
            // Exit loop if too many incorrect attempts
            if (exitCounter == 0U)
            {
                printf("Too many bad tries. Exiting program.\n");
                exit(EXIT_RETRY_ERROR);
            }

            if (inputNumber == 0)
            {
                printf("I'm not falling for that! Please enter a positive integer.\n");
            }
            else if (inputNumber < 0)
            {
                printf("Positive integers only!\n");
            }
            else
            {
                printf("Invalid input!\n");
            }

            // Check if user would like to retry
            retryInput();

            exitCounter--;
        } 
    }

    // if valid input, then return
    return (uint16_t)inputNumber;
}

/**
 * retryInput This function prompts the user to retry, gets feedback and proceeds accordingly.
 *             If the user input is incorrect, then it exits with a fault.
 */
static void retryInput(void)
{
    char     repeatDecision;       // record user's decision to try again

    printf("Try again? (y/n): ");
    scanf("%1c", &repeatDecision);
    if (getchar() != '\n')
    {
        printf("Only the first input character is read.\n");
        while (getchar() != '\n');
    }

    // User chose to quit
    if ((repeatDecision == 'n') || (repeatDecision == 'N'))
    {
        printf("Bye!\n");
        exit(EXIT_SUCCESS);
    }
    // Program chose to quit
    if ((repeatDecision != 'y') && (repeatDecision != 'Y'))
    {
        printf("I QUIT!\n");
        exit(EXIT_INPUT_ERROR);
    }

}