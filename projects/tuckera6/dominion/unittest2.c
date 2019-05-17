/* -----------------------------------------------------------------------
 * Program name: unittest2.c
 * Name: Alexis Tucker
 * Class: CS 362
 * Email: tuckera@oregonstate.edu
 * Description: Tests the council room card in dominion.c
 * Last-modified: 4/26/19
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
    int numPlayer = 4;
    int p;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G;
	int allTestPass = 0;
	int printPass = 0;
	memset(&G, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &G); // initialize a new game
	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;

    printf ("TESTING council room:\n");
    for (p = 0; p < numPlayer; p++)
    {
		printPass = 0;
#if (NOISY_TEST == 1)
		/************************************************/
		/*TEST 1: Player's hand has net gain of three cards */
		/************************************************/
		printf("\nTesting player %d's hand net gain of three cards\n", p+1);
#endif
		int otherHandsPrev[numPlayer];	//holds hand count of players besides current before buyCard
		int otherHandsCurr[numPlayer];	//holds hand count of players beside current after buyCard
		int otherDecksPrev[numPlayer];	//holds deck count of players besides current before buyCard
		int otherDecksCurr[numPlayer];	//holds deck count of players besides current after buyCard
		int otherDiscardPrev[numPlayer];//holds discard count of players besides current before buycard
		int otherDiscardCurr[numPlayer];//holds discard count of players besides current after buycard

		G.hand[p][G.handCount[p]] = council_room;	//add adventurer card to hand
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
		int prevBuys = G.numBuys;

		cardEffect(council_room, choice1, choice2, choice3, &G, handpos, &bonus); 			//calls Adventurer card

		int currBuys = G.numBuys;

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
		if (currentHandCount - previousHandCount == 3) {		//if there is an increase of 3 cards (assumes 4 cards gained, council room discarded)
			i = p + 1;
			netChange = 0;
			while (i != p) {					//check if other player's hands were affected
				netChange = otherHandsCurr[i] - otherHandsPrev[i];
				if (netChange != 1) {
#if (NOISY_TEST == 1)
					printf("councilRoomCard(): FAIL did not add 1 card to other players' hands\n"); //if so, test fails
#endif
					allTestPass = 1;
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
		
		if(currentHandCount - previousHandCount != 3){								//if player did not gain 3 cards to the hand, test fails
#if (NOISY_TEST == 1)
			printf("councilRoomCard(): FAIL did not add four cards to current player's hand\n");
#endif
			printPass = 1;
			allTestPass = 1;
		}
#if (NOISY_TEST == 1)
		if (printPass == 0) {						//otherwise, test passes
			printf("councilRoomCard(): PASS net gain of three cards to current player\n");
		}

		/*****************************************************************/
		/*TEST 2: cards removed from deck*/
		/****************************************************************/
		printf("Testing 4 cards removed from deck of player %d\n", p+1);
#endif
		printPass = 0;
		if (prevDeck - currDeck == 4) {		//if 4 cards were removed from the player's deck
			i = p + 1;
			netChange = 0;
			while (i != p) {					//check if other player's decks piles were affected
				netChange = otherDecksCurr[i] - otherDecksPrev[i];
				if (netChange != 1) {
#if (NOISY_TEST == 1)
					printf("councilRoomCard(): FAIL did not remove 1 card from other players' decks\n"); //if so, test fails
#endif
					allTestPass = 1;
					break;
				}
				i++;
				if (i >= numPlayer) {
					i = 0;
				}
			}

		}
		if (prevDeck - currDeck != 4) {								//if number of cards removed from deck was not two or greater
#if (NOISY_TEST == 1)
			printf("councilRoomCard(): FAIL deck did not change correctly\n");
#endif
			printPass = 1;
			allTestPass = 1;
		}
#if (NOISY_TEST == 1)
		if (printPass == 0) {						//otherwise, test passes
			printf("councilRoomCard(): PASS deck lost 4 cards\n");
		}
		/*********************************************************
		*TEST 3: Card added to discard
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
					printf("councilRoomCard(): FAIL affected other player's discard piles\n"); //if so, test fails
#endif
					allTestPass = 1;
					break;
				}
				i++;
				if (i >= numPlayer) {
					i = 0;
				}
			}
		}
		if (currDiscard - prevDiscard != 1){					//if card was not moved to discard pile
#if (NOISY_TEST == 1)
			printf("councilRoomCard(): FAIL discard pile did not change correctly\n");
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
		*TEST 3: council room card discarded
		*********************************************************/
		printf("Testing discard council room for player %d\n", p + 1);
#endif

		printPass = 0;
		if (G.hand[p][previousHandCount - 1] == council_room) {
#if (NOISY_TEST == 1)
			printf("councilRoomCard(): FAIL did not discard council room card\n");
#endif	
			allTestPass = 1;
		}
#if (NOISY_TEST == 1)
		else {
			printf("councilRoomCard(): PASS discarded council room card\n");
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
	

		cardEffect(smithy, choice1, choice2, choice3, &G, handpos, &bonus);

		currDeck = G.deckCount[p];			//current deck count


		if ((prevDeck == 0) & (currDeck > 0)) {
#if (NOISY_TEST == 1)
			printf("councilRoomCard(): PASS empty deck moved discard pile\n");
#endif	
			printPass = 0;
		}
		else {
#if (NOISY_TEST == 1)
			printf("councilRoomCard(): FAIL empty deck did not move discard pile\n");
#endif	
			printPass = 1;
			allTestPass = 1;
		}

		/******************************************************
		* TEST 5: number of buys increased by 1
		*******************************************************/
		if (currBuys - prevBuys != 1) {
#if (NOISY_TEST == 1)
			printf("councilRoomCard(): FAIL buys did not increase by 1\n");
#endif	
			printPass = 1;
			allTestPass = 1;
		}
#if (NOISY_TEST == 1)
		else {
			printf("councilRoomCard(): PASS buys increased by 1\n");
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
