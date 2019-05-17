/* -----------------------------------------------------------------------
 * Program name: cardtest3.c
 * Name: Alexis Tucker
 * Class: CS 362
 * Email: tuckera@oregonstate.edu
 * Description: Tests the village card in dominion.c
 * Last-modified: 5/1/19
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

    printf ("TESTING village:\n");
    for (p = 0; p < numPlayer; p++)
    {
		printPass = 0;
#if (NOISY_TEST == 1)
		/************************************************/
		/*TEST 1: Player's hand gains one card*/
		/************************************************/
		printf("\nTesting player %d's hand gains one card\n", p+1);
#endif
		int otherHandsPrev[numPlayer];	//holds hand count of players besides current before buyCard
		int otherHandsCurr[numPlayer];	//holds hand count of players beside current after buyCard
		int otherDecksPrev[numPlayer];	//holds deck count of players besides current before buyCard
		int otherDecksCurr[numPlayer];	//holds deck count of players besides current after buyCard
		int otherDiscardPrev[numPlayer];//holds discard count of players besides current before buycard
		int otherDiscardCurr[numPlayer];//holds discard count of players besides current after buycard

		G.hand[p][G.handCount[p]] = village;	//add village card to hand
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

		int prevActions = G.numActions;

		cardEffect(village, choice1, choice2, choice3, &G, handpos, &bonus); 			//calls Adventurer card

		int currActions = G.numActions;

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
		if (currentHandCount - previousHandCount == 0) {		//if there is a gain of 1 cards minus one to represent smithy card discarded
			i = p + 1;
			netChange = 0;
			while (i != p) {					//check if other player's hands were affected
				netChange = otherHandsCurr[i] - otherHandsPrev[i];
				if (netChange != 0) {
#if (NOISY_TEST == 1)
					printf("villageCard(): FAIL added card to other player's hand\n"); //if so, test fails
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
		else {								//if player did not gain 1 cards to the hand, test fails
#if (NOISY_TEST == 1)
			printf("villageCard(): FAIL did not add one card to current player's hand\n");
#endif
			printPass = 1;
			allTestPass = 1;
		}
		if (printPass == 0) {
#if (NOISY_TEST == 1)
					//otherwise, test passes
			printf("villageCard(): PASS added one card to current player's hand and one card removed\n");
#endif
		}
#if (NOISY_TEST == 1)
			/*****************************************************************/
			/*TEST 2: 1 card removed from deck*/
			/****************************************************************/
			printf("Testing card removed from deck\n");
#endif

		if (prevDeck - currDeck == 1) {		//if one card removed from deck
			i = p + 1;
			netChange = 0;
			while (i != p) {					//check if other player's decks piles were affected
				netChange = otherDecksCurr[i] - otherDecksPrev[i];
				if (netChange != 0) {
#if (NOISY_TEST == 1)
					printf("villageCard(): FAIL affected other players' decks \n"); //if so, test fails
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
		else {								//if number of cards removed from deck was not two or greater
#if (NOISY_TEST == 1)
			printf("villageCard(): FAIL deck did not change correctly\n");
#endif
			printPass = 1;
			allTestPass = 1;
		}
#if (NOISY_TEST == 1)
		if (printPass == 0) {						//otherwise, test passes
			printf("villageCard(): PASS deck changed correctly\n");
		}
		/*********************************************************
		*TEST 3: cards added to discard pile correctly
		*********************************************************/
		printf("Testing discard pile for player %d\n", p + 1);
#endif

		if (currDiscard - prevDiscard == 1) {		//if the discard pile has a gain of one card
			i = p + 1;
			netChange = 0;
			while (i != p) {					//check if other player's discard piles were affected
				netChange = otherDiscardCurr[i] - otherDiscardPrev[i];
				if (netChange != 0) {
#if (NOISY_TEST == 1)
					printf("villageCard(): FAIL affected other player's discard piles\n"); //if so, test fails
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
		else {								//if number of cards removed from deck - 2 + 1 was not moved to discard pile
#if (NOISY_TEST == 1)
			printf("villageCard(): FAIL discard pile did not change correctly\n");
#endif
			printPass = 1;
			allTestPass = 1;
		}
#if (NOISY_TEST == 1)
		if (printPass == 0) {						//otherwise, test passes
			printf("villageCard(): PASS discard pile changed correctly\n");
		}
		/*********************************************************
		*TEST 3: Village  card discarded
		*********************************************************/
		printf("Testing discarded village for player %d\n", p + 1);
#endif

		printPass = 0;
		if (G.hand[p][previousHandCount - 1] == village) {
#if (NOISY_TEST == 1)
			printf("villageCard(): FAIL did not discard adventurer card\n");
#endif	
			allTestPass = 1;
		}
#if (NOISY_TEST == 1)
		else {
			printf("villageCard(): PASS discard adventurer card\n");
		}
#endif
		/********************************************************/
		/*TEST 4: Deck is empty*/
		/********************************************************/
		//empty discard pile
		int discardCount = G.discardCount[p];
		int deckTracker = G.deckCount[p];
		for (i = 0; i < discardCount; i++) {
			G.deck[p][deckTracker] = G.discard[p][i];
			deckTracker++;
			G.discardCount[p]--;
		}

		//move deck to discard pile
		int deckCount = G.deckCount[p];
		//Move deck to discard
		for (i = 0; i < deckCount;i++) {
			G.discard[p][i] = G.deck[p][i];
			G.deck[p][i] = -1;
			G.deckCount[p]--;
			G.discardCount[p]++;
		}

		prevDeck = G.deckCount[p];			//previous deck count

		cardEffect(village, choice1, choice2, choice3, &G, handpos, &bonus);

		currDeck = G.deckCount[p];			//current deck count

		if ((prevDeck == 0) & (currDeck > 0)) {
#if (NOISY_TEST == 1)
			printf("villageCard(): PASS empty deck moved discard pile\n");
#endif	
			printPass = 0;
		}
		else {
#if (NOISY_TEST == 1)
			printf("villageCard(): FAIL empty deck did not move discard pile\n");
#endif	
			printPass = 1;
			allTestPass = 1;
		}

		/********************************************************/
		/*TEST 4: Number of actions increased by two*/
		/********************************************************/
		if (currActions - prevActions == 2) {
#if (NOISY_TEST == 1)
			printf("villageCard(): PASS player gained two actions\n");
#endif	
		}
		else {
#if (NOISY_TEST == 1)
			printf("villageCard(): FAIL player did not gain two actions\n");
#endif	
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
