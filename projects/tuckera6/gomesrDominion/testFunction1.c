/* -----------------------------------------------------------------------
 * Program name: testFunction1.c
 * Name: Alexis Tucker
 * Class: CS 362
 * Email: tuckera@oregonstate.edu
 * Description: Tests the drawCard function in dominion.c
 * Last-modified: 4/23/19
 * Source: testUpdateCoins.c
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
    int seed = 1000;
    int numPlayer = 2;
    int p;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G;
	int allTestPass = 0;

	memset(&G, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &G); // initialize a new game

    printf ("TESTING drawCard():\n");
    for (p = 0; p < numPlayer; p++)
    {
		int printPass = 0;
#if (NOISY_TEST == 1)
		/*TEST 1: Adding card to current player's hand only*/
		printf("Testing player %d adding card to hand.\n", p+1);
#endif
		int otherHandsPrev[numPlayer];	//holds hand count of players besides current before drawCard
		int otherHandsCurr[numPlayer];	//holds hand count of players beside current after drawCard

		int previousHandCount = G.handCount[p]; //holds current players hand count
		
		i = p + 1;
		while (i != p) {						 //fills out array
			otherHandsPrev[i] = G.handCount[i];
			i++;
			if (i >= numPlayer) {
				i = 0;
			}
		}

		drawCard(p, &G);							//calls draw card

		i = p + 1;
		while (i != p) {						 //fills out array
			otherHandsCurr[i] = G.handCount[i];
			i++;
			if (i >= numPlayer) {
				i = 0;
			}
		}

		int currentHandCount = G.handCount[p];				//holds current player's new hand count
		if (currentHandCount - previousHandCount == 1) {		//if there is a gain of 1 card
			i = p + 1;
			int netChange = 0;
			while (i != p) {					//check if other player's hands were affected
				netChange = otherHandsCurr[i] - otherHandsPrev[i];
				if (netChange != 0) {
#if (NOISY_TEST == 1)
					printf("drawCard(): FAIL added card to other player's hand\n"); //if so, test fails
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
		else {								//if player did not gain 1 card to the hand, test fails
#if (NOISY_TEST == 1)
			printf("drawCard(): FAIL did not add one card to current player's hand\n");
#endif
			printPass = 1;
		}
#if (NOISY_TEST == 1)
		if (printPass == 0) {						//otherwise, test passes
			printf("drawCard(): PASS added one card to current player's hand\n");
			printf("currenthand: %d\n", G.handCount[p]);
		}

		/*TEST 2: Removing card from current player's hand only*/
		printf("Testing removing card from deck of player %d\n", p+1);
#endif
		printPass = 0;
		int otherDecksPrev[numPlayer];	//holds deck count of players besides current before drawCard
		int otherDecksCurr[numPlayer];	//holds deck count of players beside current after drawCard
		int previousDeckCount = G.deckCount[p]; //holds current players deck count

		i = p + 1;
		while (i != p) {						 //fills out array
			otherDecksPrev[i] = G.deckCount[i];
			i++;
			if (i >= numPlayer) {
				i = 0;
			}
		}

		drawCard(p, &G);							//calls draw card

		i = p + 1;
		while (i != p) {						 //fills out array
			otherDecksCurr[i] = G.deckCount[i];
			i++;
			if (i >= numPlayer) {
				i = 0;
			}
		}

		int currentDeckCount = G.deckCount[p];			// holds current player's new deck count
		if (currentDeckCount - previousDeckCount == -1) {	//if a card was successfully removed from current player's deck
			i = p + 1;
			int netChange = 0;
			while (i != p) {								//check that no cards were removed from other players' decks
				netChange = otherDecksCurr[i] - otherDecksPrev[i];
				if (netChange != 0) {
#if (NOISY_TEST == 1)
					printf("drawCard(): FAILED removed card from other player's deck \n"); //if a card was removed, test fails
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
		else {							//if a card was not removed, test fails
#if (NOISY_TEST == 1)
			printf("drawCard(): FAILED did not remove 1 card from current player's deck\n");
#endif			
			printPass = 1;
			allTestPass = 1;
		}
#if (NOISY_TEST == 1)
		if (printPass == 0) {		//if successful test
			printf("drawCard(): PASS removed 1 card from current player's deck\n");
			printf("currenthand: %d\n", G.handCount[p]);
		}

		/*TEST 3: If deck is empty, current player's discard pile is shuffled and moved to their deck. Discard pile should then be empty*/
		printf("Testing empty deck for player %d\n", p+1);
#endif
		printPass = 0;
		//empty current player's deck
		int i;
		int deckCount = G.deckCount[p];
		//Move deck to discard
		for (i = 0; i < deckCount;i++) {
			G.discard[p][i] = G.deck[p][i];
			G.deck[p][i] = -1;
			G.deckCount[p]--;
			G.discardCount[p]++;
		}

		int otherDiscardPrev[numPlayer]; //holds discard pile count of other players before drawCard
		int otherDiscardCurr[numPlayer]; //holds discard pile count of other players after drawCard

		previousDeckCount = G.deckCount[p]; //holds current players deck count
		int previousDiscardCount = G.discardCount[p]; //holds current player's discard count
		previousHandCount = G.handCount[p];	

		i = p + 1;
		while (i != p) {						 //fills out array
			otherDecksPrev[i] = G.deckCount[i];
			otherDiscardPrev[i] = G.discardCount[i];
			i++;
			if (i >= numPlayer) {
				i = 0;
			}
		}

		drawCard(p, &G);							//calls draw card

		i = p + 1;
		while (i != p) {						 //fills out array
			otherDecksCurr[i] = G.deckCount[i];
			otherDiscardCurr[i] = G.discardCount[i];
			i++;
			if (i >= numPlayer) {
				i = 0;
			}
		}

		currentDeckCount = G.deckCount[p];			//holds current players new deck count
		int currentDiscardCount = G.discardCount[p];		//holds current players new discard pile count
		currentHandCount = G.handCount[p];
	


		if ((currentDeckCount+1 == previousDiscardCount) & (currentDiscardCount == previousDeckCount)) {	//if cards were successfully moved from discard to deck, and 1 card drawn from new deck
			i = p + 1;
			int netChange = 0;
			int netChangeDiscard = 0;
			while (i != p) {								//check that other player's discard piles and decks are untouched
				netChange = otherDecksCurr[i] - otherDecksPrev[i];
				netChangeDiscard = otherDiscardCurr[i] - otherDiscardPrev[i];
				if (netChange != 0 || netChangeDiscard != 0) {
#if (NOISY_TEST == 1)
					printf("drawCard(): FAILED other players' decks and/or discard piles affected\n"); //if they were affected, test fails
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
			if (netChange != 0 || netChangeDiscard != 0) {
				break;
			}
		}
		else{						//else cards were not successfully swapped between discard pile and deck and test fails
#if (NOISY_TEST == 1)
			printf("drawCard(): FAILED deck and discard piles not properly swapped\n");
			printf("currentDeck: %d, PreviousDeck: %d, CurrentDiscard %d, PreviousDiscard: %d\n",currentDeckCount, previousDeckCount, currentDiscardCount, previousDiscardCount);
#endif
			allTestPass = 1;	
			printPass = 1;
		}
#if (NOISY_TEST == 1)
		if (printPass == 0) {				//otherwise test succeeeds
			printf("drawCard(): PASS deck and discard piles swapped successfully\n");
		}
		printf("\n");
		
#endif
	
	}

	if (allTestPass == 0) {
		printf("TEST SUCCESSFULLY COMPLETED\n");	//if all tests were successful, print success
	}
	else {
		printf("TEST FAILED\n");					//if all tests were not successfuly, print failure
	}


    return 0;
}
