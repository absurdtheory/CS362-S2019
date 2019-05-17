/* -----------------------------------------------------------------------
 * Program name: cardtest2.c
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

    printf ("TESTING smithy:\n");
    for (p = 0; p < numPlayer; p++)
    {
		printPass = 0;
#if (NOISY_TEST == 1)
		/*************************************************************/
		/*TEST 1: Deck and hand are as initialized by initializeGame */
		/*************************************************************/
		printf("\nTesting player %d's with intialized deck and hand\n", p+1);
#endif
		int otherHandsPrev[numPlayer];	//holds hand count of players besides current before buyCard
		int otherHandsCurr[numPlayer];	//holds hand count of players beside current after buyCard
		int otherDecksPrev[numPlayer];	//holds deck count of players besides current before buyCard
		int otherDecksCurr[numPlayer];	//holds deck count of players besides current after buyCard


		G.hand[p][G.handCount[p]] = smithy;	//add adventurer card to hand
		G.handCount[p]++;
		handpos = G.handCount[p] - 1;			//set hand position to the position of adventurer

		int previousHandCount = G.handCount[p]; //holds current players hand count
		int prevDeck = G.deckCount[p];			//previous deck count

		i = p + 1;
		while (i != p) {						 //fills out array
			otherHandsPrev[i] = G.handCount[i];
			otherDecksPrev[i] = G.deckCount[i];
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
			i++;
			if (i >= numPlayer) {
				i = 0;
			}
		}

		int currentHandCount = G.handCount[p];				//holds current player's new hand count
		int currDeck = G.deckCount[p];						//current deck count


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

		/*TEST 1b: player's deck looses 3 cards*/
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
#endif

#if (NOISY_TEST == 1)
		/********************************************************/
		/*TEST 2: Deck is empty*/
		/********************************************************/
		printf("Testing player %d with empty deck\n", p + 1);
#endif	
		//empty discard pile
		int discardCount = G.discardCount[p];
		int deckTracker = G.deckCount[p];
		for (i = 0; i < discardCount; i++) {
			G.deck[p][deckTracker] = G.discard[p][i];
			deckTracker++;
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

		cardEffect(smithy, choice1, choice2, choice3, &G, handpos, &bonus);

		currDeck = G.deckCount[p];			//current deck count

		if ((prevDeck == 0) & (currDeck > 0)) {
#if (NOISY_TEST == 1)
			printf("smithyCard(): PASS empty deck moved discard pile\n");
#endif	
			printPass = 0;
		}
		else {
#if (NOISY_TEST == 1)
			printf("smithyCard(): FAIL empty deck did not move discard pile\n");
#endif	
			printPass = 1;
			allTestPass = 1;
		}

		/********************************************************/
		/*TEST 3: Deck is max size*/
		/********************************************************/
#if (NOISY_TEST == 1)
		printf("Testing player %d with full deck\n", p + 1);
#endif	
		//increase deck to max size
		for (i = G.deckCount[p]; i < MAX_DECK;i++) {
			G.deck[p][i] = copper;
			G.deckCount[p]++;
		}

		prevDeck = G.deckCount[p];			//previous deck count

		cardEffect(smithy, choice1, choice2, choice3, &G, handpos, &bonus);

		currDeck = G.deckCount[p];			//current deck count

		if ((prevDeck == MAX_DECK) & (currDeck == MAX_DECK-3)) {
#if (NOISY_TEST == 1)
			printf("smithyCard(): PASS deck size reduced by 3 \n");
#endif	
			printPass = 0;
		}
		else {
#if (NOISY_TEST == 1)
			printf("smithyCard(): FAIL deck size not reduced by 3\n");
#endif	
			printPass = 1;
			allTestPass = 1;
		}


		/********************************************************/
		/*TEST 4: Hand is max size*/
		/********************************************************/
#if (NOISY_TEST == 1)
		printf("Testing player %d with full hand\n",p+1);
#endif	
		//increase hand to max size
		for (i = G.handCount[p]; i < MAX_HAND; i++) {
			G.hand[p][i] = copper;
			G.handCount[p]++;
		}

		previousHandCount = G.handCount[p];			//previous deck count

		cardEffect(smithy, choice1, choice2, choice3, &G, handpos, &bonus);

		currentHandCount = G.handCount[p];			//current deck count

		if ((previousHandCount == MAX_HAND) & (currentHandCount == MAX_DECK + 2)) {
#if (NOISY_TEST == 1)
			printf("smithyCard(): FAIL hand size increased by 2 but player had no more room for cards in hand\n");
#endif	
			printPass = 1;
			allTestPass = 1;
		}
		else if(currentHandCount == previousHandCount) {
#if (NOISY_TEST == 1)
			printf("smithyCard(): PASS hand size did not change\n");
#endif	
			printPass = 0;
		}
		else {
#if (NOISY_TEST == 1)
			printf("smithyCard(): FAIL hand size changed\n");
#endif	
			printPass = 1;
			allTestPass = 1;
		}

		/********************************************************/
		/*TEST 5: Hand is empty*/
		/********************************************************/
#if (NOISY_TEST == 1)
		printf("Testing player %d with empty hand\n", p + 1);
#endif	
		for (i = G.handCount[p]; i > 0; i--) {
			G.hand[p][i] = -1;
			G.handCount[p]--;
		}

		previousHandCount = G.handCount[p];			//previous deck count

		cardEffect(smithy, choice1, choice2, choice3, &G, handpos, &bonus);

		currentHandCount = G.handCount[p];			//current deck count

		if ((previousHandCount == 0) & (currentHandCount == 2)) {
#if (NOISY_TEST == 1)
			printf("smithyCard(): FAIL hand size increased by 2 even though no smithy card in hand\n");
#endif	
			printPass = 1;
			allTestPass = 1;
		}
		else if(previousHandCount == currentHandCount){
#if (NOISY_TEST == 1)
			printf("smithyCard(): PASS hand size did not increase by 2\n");
#endif	
			printPass = 0;
		}
		else {
#if (NOISY_TEST == 1)
			printf("smithyCard(): FAIL hand size changed with no smithy card in hand\n");
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
