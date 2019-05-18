/* -----------------------------------------------------------------------
 * Program name: unittest1.c
 * Name: Alexis Tucker
 * Class: CS 362
 * Email: tuckera@oregonstate.edu
 * Description: Tests the smithy card in dominion.c
 * Last-modified: 4/25/19
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
#define NOISY_TEST 1

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

    printf ("TESTING smithy:\n");
    for (p = 0; p < numPlayer; p++)
    {
		printPass = 0;
#if (NOISY_TEST == 1)
		/************************************************/
		/*TEST 1: Player's hand has net gain of two cards */
		/************************************************/
		printf("\nTesting player %d's hand net gain of two cards\n", p+1);
#endif
		int otherHandsPrev[numPlayer];	//holds hand count of players besides current before buyCard
		int otherHandsCurr[numPlayer];	//holds hand count of players beside current after buyCard
		int otherDecksPrev[numPlayer];	//holds deck count of players besides current before buyCard
		int otherDecksCurr[numPlayer];	//holds deck count of players besides current after buyCard
		int otherDiscardPrev[numPlayer];//holds discard count of players besides current before buycard
		int otherDiscardCurr[numPlayer];//holds discard count of players besides current after buycard

		G.hand[p][G.handCount[p]] = smithy;	//add adventurer card to hand
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

		cardEffect(smithy, choice1, choice2, choice3, &G, handpos, &bonus); 			//calls Adventurer card

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


		/*TEST 1a: cards added to player's hand*/
		if (currentHandCount - previousHandCount == 2) {		//if there is an increase of 2 cards (assumes 3 cards gained, smithy discarded)
			i = p + 1;
			netChange = 0;
			while (i != p) {					//check if other player's hands were affected
				netChange = otherHandsCurr[i] - otherHandsPrev[i];
				if (netChange != 0) {
#if (NOISY_TEST == 1)
					printf("smithyCard(): FAIL added card to other player's hand\n"); //if so, test fails
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
			if (netChange != 0){
				break;
			}
		}
		else {								//if player did not gain 2 cards to the hand, test fails
#if (NOISY_TEST == 1)
			printf("smithyCard(): FAIL did not add three cards to current player's hand\n");
#endif
			printPass = 1;
			allTestPass = 1;
		}
		if (printPass == 0) {						//otherwise, test passes
#if (NOISY_TEST == 1)
			printf("smithyCard(): PASS net gain of two cards\n");
#endif
		}
#if (NOISY_TEST == 1)
		/*****************************************************************/
		/*TEST 2: cards removed from deck*/
		/****************************************************************/
		printf("Testing 3 cards removed from deck\n");
#endif

		if (prevDeck - currDeck == 3) {		//if 3 cards were removed from the player's deck
			i = p + 1;
			netChange = 0;
			while (i != p) {					//check if other player's decks piles were affected
				netChange = otherDecksCurr[i] - otherDecksPrev[i];
				if (netChange != 0) {
#if (NOISY_TEST == 1)
					printf("smithyCard(): FAIL affected other players' decks \n"); //if so, test fails
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
			printf("smithyCard(): FAIL deck did not change correctly\n");
#endif
			printPass = 1;
			allTestPass = 1;
		}
#if (NOISY_TEST == 1)
		if (printPass == 0) {						//otherwise, test passes
			printf("smithyCard(): PASS deck lost 3 cards\n");
		}
		/*********************************************************
		*TEST 3: Card added to discard
		*********************************************************/
		printf("Testing discard pile for player %d\n", p + 1);
#endif

		if (currDiscard - prevDiscard == 1) {		//if the discard pile has gained 1 card
			i = p + 1;
			netChange = 0;
			while (i != p) {					//check if other player's discard piles were affected
				netChange = otherDiscardCurr[i] - otherDiscardPrev[i];
				if (netChange != 0) {
#if (NOISY_TEST == 1)
					printf("smithyCard(): FAIL affected other player's discard piles\n"); //if so, test fails
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
		else {								//if number of cards removed from deck - 2 + 1 was not moved to discard pile
#if (NOISY_TEST == 1)
			printf("smithyCard(): FAIL discard pile did not change correctly\n");
#endif
			printPass = 1;
			allTestPass = 1;
		}
#if (NOISY_TEST == 1)
		if (printPass == 0) {						//otherwise, test passes
			printf("smithyCard(): PASS discard pile gained 1 card\n");
		}
		/*********************************************************
		*TEST 3: smithy card discarded
		*********************************************************/
		printf("Testing discard smithy for player %d\n", p + 1);
#endif

		printPass = 0;
		if (G.hand[p][previousHandCount - 1] == smithy) {
#if (NOISY_TEST == 1)
			printf("smithyCard(): FAIL did not discard smithy card\n");
#endif	
			allTestPass = 1;
		}
#if (NOISY_TEST == 1)
		else {
			printf("smithyCard(): PASS discard smithy card\n");
		}
#endif
	


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
