/* -----------------------------------------------------------------------
 * Program name: unittest4.c
 * Name: Alexis Tucker
 * Class: CS 362
 * Email: tuckera@oregonstate.edu
 * Description: Tests the sea hag card in dominion.c
 * Last-modified: 4/30/19
 * Sources: testUpdateCoins.c, cardTest4.c
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 0

int main() {
    int i;
	int netChange;
    int seed = 1000;
    int numPlayer = 2;
    int p;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G;
	int allTestPass = 0;
	int printPass = 0;
	memset(&G, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &G); // initialize a new game
	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;

    printf ("TESTING sea hag:\n");
    for (p = 0; p < numPlayer; p++)
    {
		printPass = 0;
#if (NOISY_TEST == 1)
		/***************************************************************/
		/*TEST 1: Top card in every other player's deck is now a curse */
		/***************************************************************/
		printf("\nTesting player change top card to curse of all player's besides player %d\n", p+1);
#endif
		int otherHandsPrev[numPlayer];	//holds hand count of players besides current before buyCard
		int otherHandsCurr[numPlayer];	//holds hand count of players beside current after buyCard
		int otherDecksPrev[numPlayer];	//holds deck count of players besides current before buyCard
		int otherDecksCurr[numPlayer];	//holds deck count of players besides current after buyCard
		int otherDiscardPrev[numPlayer];//holds discard count of players besides current before buycard
		int otherDiscardCurr[numPlayer];//holds discard count of players besides current after buycard

		G.hand[p][G.handCount[p]] = sea_hag;	//add adventurer card to hand
		G.handCount[p]++;
		handpos = G.handCount[p] - 1;			//set hand position to the position of adventurer

		int previousHandCount = G.handCount[p]; //holds current players hand count
		int prevDeck = G.deckCount[p];			//previous deck count
		int prevDiscard = G.discardCount[p];	//previous discard count

		i = p + 1;
		while (i != p) {						 //fills out array
			otherHandsPrev[i] = G.handCount[i];
			otherDecksPrev[i] = G.deckCount[i];
			otherDiscardPrev[i] = G.discardCount[i];
			i++;
			if (i >= numPlayer) {
				i = 0;
			}
		}

		cardEffect(sea_hag, choice1, choice2, choice3, &G, handpos, &bonus); 			//calls Adventurer card

		i = p + 1;
		while (i != p) {						 //fills out array
			otherHandsCurr[i] = G.handCount[i];
			otherDecksCurr[i] = G.deckCount[i];
			otherDiscardCurr[i] = G.discardCount[i];
			i++;
			if (i >= numPlayer) {
				i = 0;
			}
		}

		int currentHandCount = G.handCount[p];				//holds current player's new hand count
		int currDeck = G.deckCount[p];						//current deck count
		int currDiscard = G.discardCount[p];				//current discard count

		if (G.deck[p][G.deckCount[i]-1] == curse) {
#if (NOISY_TEST == 1)
			printf("seaHagCard(): FAIL current player's top deck in card was changed to curse\n");
#endif
		}

		int currTopDeck[numPlayer];		//holds current top card of deck
		for (i = 0; i < numPlayer; i++) {
			currTopDeck[i] = G.deck[i][G.deckCount[i]-1];
			if ((currTopDeck[i] != curse) & (i != p)) {
#if (NOISY_TEST == 1)
				printf("seaHagCard(): FAIL did not change other players' top card in deck to curse\n");
#endif
				allTestPass = 1;
				break;
			}
#if (NOISY_TEST == 1)
			else {
				printf("seaHagCard(): PASS only other player's top card in deck was changed to curse\n");
				break;
			}
#endif
		}

	

#if (NOISY_TEST == 1)
		/*****************************************************************/
		/*TEST 2: Deck Size unchanged*/
		/****************************************************************/
		printf("Testing deck sizes unchanged\n");
#endif

		if (prevDeck - currDeck == 0) {		
			i = p + 1;
			netChange = 0;
			while (i != p) {					//check if other player's decks piles were affected
				netChange = otherDecksCurr[i] - otherDecksPrev[i];
				if (netChange != 0) {
#if (NOISY_TEST == 1)
					printf("seaHagCard(): FAIL affected other players' deck sizes \n"); //if so, test fails
#endif
					allTestPass = 1;
					printPass = 1;
					break;
				}
				i++;
				if (i >= numPlayer) {
					i = 0;
				}
			}
			if (netChange != 0) {
				break;
			}
		}
		else {								//if number of cards removed from deck was not two or greater
#if (NOISY_TEST == 1)
			printf("seaHagCard(): FAIL deck changed\n");
#endif
			printPass = 1;
			allTestPass = 1;
		}
		if (printPass == 0) {						//otherwise, test passes
#if (NOISY_TEST == 1)
			printf("councilRoomCard(): PASS discard pile gained 1 card\n");
#endif
		}
#if (NOISY_TEST == 1)
		/*********************************************************
		*TEST 3: Discard pile gains 1 card for all players (current sea hag, other's top card of deck)
		*********************************************************/
		printf("Testing discard pile for player %d\n", p + 1);
#endif
		printPass = 0;
		if (currDiscard - prevDiscard == 1) {		//if the discard pile has gained 1 card
			i = p + 1;
			netChange = 0;
			while (i != p) {					//check if other player's discard piles were affected
				netChange = otherDiscardCurr[i] - otherDiscardPrev[i];
				if (netChange != 1) {
#if (NOISY_TEST == 1)
					printf("seaHagCard(): FAIL other players did not discard a card\n"); //if so, test fails
#endif
					allTestPass = 1;
					printPass = 1;
					break;
				}
				i++;
				if (i >= numPlayer) {
					i = 0;
				}
			}
		}
		else {								
#if (NOISY_TEST == 1)
			printf("seaHagCard(): FAIL discard piles did not change correctly\n");
#endif
			printPass = 1;
			allTestPass = 1;
		}
#if (NOISY_TEST == 1)
		if (printPass == 0) {						//otherwise, test passes
			printf("seaHagCard(): PASS discard piles gained 1 card\n");
		}
		/*********************************************************
		*TEST 3: sea hag card discarded
		*********************************************************/
		printf("Testing for discarded sea hag for player %d\n", p + 1);
#endif

		printPass = 0;
		if (G.hand[p][previousHandCount - 1] == sea_hag) {
#if (NOISY_TEST == 1)
			printf("seaHagCard(): FAIL did not discard sea hag card card\n");
#endif	
			allTestPass = 1;
		}
#if (NOISY_TEST == 1)
		else {
			printf("seaHagCard(): PASS discard sea hag card\n");
		}
		printf("Testing hand size decreased by 1\n");
#endif
		/********************************************************/
		/*TEST 4: Hand size secreases by 1*/
		/********************************************************/

		if (currentHandCount - previousHandCount == -1) {		
			i = p + 1;
			netChange = 0;
			while (i != p) {					//check if other player's hands were affected
				netChange = otherHandsCurr[i] - otherHandsPrev[i];
				if (netChange != 0) {
#if (NOISY_TEST == 1)
					printf("seaHagCard(): FAIL changed other player's hand\n"); //if so, test fails
#endif
					allTestPass = 1;
					printPass = 1;
					break;
				}
				i++;
				if (i >= numPlayer) {
					i = 0;
				}
			}
			if (netChange != 0) {
				break;
			}
		}
		else {								
#if (NOISY_TEST == 1)
			printf("seaHagCard(): FAIL hand size did not decrease by 1\n");
#endif
			printPass = 1;
			allTestPass = 1;
		}



		//simulate next turn
		if (p + 1 != numPlayer) {
			G.whoseTurn++;
			updateCoins(p + 1, &G, 0);
			G.numBuys = 1;
		}

	}

	if (allTestPass == 0) {
		printf("TEST SUCCESSFULLY COMPLETED\n");	//if all tests were successful, print success
	}
	else {
		printf("TEST FAILED\n");					//if all tests were not successfuly, print failure
	}


    return 0;
}
