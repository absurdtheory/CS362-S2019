/* -----------------------------------------------------------------------
 * Program name: unittest3.c
 * Name: Alexis Tucker
 * Class: CS 362
 * Email: tuckera@oregonstate.edu
 * Description: Tests the remodel card in dominion.c
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
#define NOISY_TEST 1 

int main() {
	int i;
	int netChange;
	int seed = 1000;
	int numPlayer = 2;
	int p;
	int k[10] = { adventurer, council_room, feast, gardens, mine
			   , remodel, smithy, village, baron, great_hall };
	struct gameState G;
	int allTestPass = 0;
	int printPass = 0;
	memset(&G, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &G); // initialize a new game
	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;

	printf("TESTING remodel:\n");
	for (p = 0; p < numPlayer; p++)
	{
		printPass = 0;
#if (NOISY_TEST == 1)
		/***********************************************************************************/
		/*TEST 1: If Player Discards card with cost n and Selects Cards that costs = n + 2*/
		/*********************************************************************************/
		//TEST 1a: Play discards card at choice 1 and gains card choice2
		printf("\nTesting player %d's remodel with choice1: %d, choice2:%d\n", p + 1, getCost(gardens), getCost(gold));
#endif
		choice1 = gardens;
		choice2 = gold;

		//initialize player's hand to all copper
		for (i = 0; i < G.handCount[p]; i++) {
			G.hand[p][i] = copper;
		}

		int otherHandsPrev[numPlayer];	//holds hand count of players besides current before buyCard
		int otherHandsCurr[numPlayer];	//holds hand count of players beside current after buyCard
		int otherDecksPrev[numPlayer];	//holds deck count of players besides current before buyCard
		int otherDecksCurr[numPlayer];	//holds deck count of players besides current after buyCard
		int otherDiscardPrev[numPlayer];//holds discard count of players besides current before buycard
		int otherDiscardCurr[numPlayer];//holds discard count of players besides current after buycard

		G.hand[p][G.handCount[p]] = remodel;	//add remodel card to hand
		handpos = G.handCount[p];				//set hand position of remodel
		G.handCount[p]++;

		G.hand[p][G.handCount[p]] = gardens;	//add card to hand
		G.handCount[p]++;

		G.hand[p][G.handCount[p]] = council_room;	//add card to hand
		G.handCount[p]++;

		G.hand[p][G.handCount[p]] = village;	//add card to hand
		G.handCount[p]++;

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

		int prevGold = G.supplyCount[gold];		//get initial supply count of gold

		cardEffect(remodel, choice1, choice2, choice3, &G, handpos, &bonus); 			//calls remodel card

		int currGold = G.supplyCount[gold];		//get new supply count of gold


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


		/*TEST 1a: card added to player's hand and not other players*/
		if (currentHandCount - previousHandCount == -1) {		//if there is a one decrease in hand
			i = p + 1;
			netChange = 0;
			while (i != p) {					//check if other player's hands were affected
				netChange = otherHandsCurr[i] - otherHandsPrev[i];
				if (netChange != 0) {
#if (NOISY_TEST == 1)
					printf("remodelCard(): FAIL affected other players' hands\n"); //if so, test fails
#endif
					allTestPass = 1;
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

		printPass = 1;
		if (currentHandCount - previousHandCount == 0) {								//if no change in hand count
			for (i = 0; i < currentHandCount; i++) {								//if gold card added
				if (G.hand[p][i] == gold) {
					printPass = 0;
					G.hand[p][i] = -1;												//remove gold card
					break;
				}
			}
		}

		if (printPass == 0) {						//otherwise, test passes
#if (NOISY_TEST == 1)
			printf("remodelCard(): PASS card choice2 gained\n");
#endif
		}
		else {
#if (NOISY_TEST == 1)
			printf("remodelCard(): FAIL did not gain choice2 card\n");
#endif
			printPass = 1;
			allTestPass = 1;
		}
		printPass = 0;
#if (NOISY_TEST == 1)

		//TEST 1b: Remodel card removed from hand
		printf("Testing discard remodel for player %d\n", p + 1);
#endif
		if (G.hand[p][handpos] == remodel) {
			printPass = 1;
		}

		if (printPass == 1) {
#if (NOISY_TEST == 1)
			printf("remodelCard(): FAIL did not discard remodel card\n");
#endif	
			allTestPass = 1;
		}
#if (NOISY_TEST == 1)
		else {
			printf("remodelCard(): PASS discarded remodel card\n");
		}

		//TEST 1c: gardens card removed from hand
		printf("Testing discard gardens for player %d\n", p + 1);
#endif

		for (i = 0; i < G.handCount[p]; i++) {
			if (G.hand[p][i] == gardens)
				printPass = 1;
			break;
		}

	
		if (printPass == 1) {
#if (NOISY_TEST == 1)
			printf("remodelCard(): FAIL did not discard garden\n");
#endif	
				allTestPass = 1;
		}
#if (NOISY_TEST == 1)
		else {
			printf("remodelCard(): PASS discarded garden card\n");
		}
	//TEST 1d: gold supply decreased by 1
		printf("Testing gold supply decreased by 1\n");
#endif
		printPass = 0;
		if (prevGold - currGold != 1) {
#if (NOISY_TEST == 1)
			printf("remodelCard(): FAIL did not decrease gold supply by 1\n");
#endif	
			allTestPass = 1;
		}
#if (NOISY_TEST == 1)
		else {
			printf("remodelCard(): PASS decreased gold supply by 1\n");
		}

	//TEST 1e: discard pile increased by 2
		printf("Testing player %d discard pile increase by 2\n", p + 1);
#endif
		printPass = 0;
		if (currDiscard - prevDiscard == 2) {		//if the discard pile has gained 2 card
			i = p + 1;
			netChange = 0;
			while (i != p) {					//check if other player's discard piles were affected
				netChange = otherDiscardCurr[i] - otherDiscardPrev[i];
				if (netChange != 1) {
	#if (NOISY_TEST == 1)
					printf("remodelCard(): FAIL affected other player's discard piles\n"); //if so, test fails
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
		if (currDiscard - prevDiscard != 2) {					//if card was not moved to discard pile
	#if (NOISY_TEST == 1)
			printf("remodelCard(): FAIL discard pile did not change correctly\n");
	#endif
			printPass = 1;
			allTestPass = 1;
		}
	#if (NOISY_TEST == 1)
		if (printPass == 0) {						//otherwise, test passes
			printf("remodelCard(): PASS discard pile gained 2 card\n");
		}

		//TEST 1f: Deck count not changed
		printf("Testing no deck change for player %d\n", p + 1);
	#endif
		printPass = 0;
		if (prevDeck - currDeck == 0) {		//if 4 cards were removed from the player's deck
			i = p + 1;
			netChange = 0;
			while (i != p) {					//check if other player's decks piles were affected
				netChange = otherDecksCurr[i] - otherDecksPrev[i];
				if (netChange != 0) {
	#if (NOISY_TEST == 1)
					printf("councilRoomCard(): FAIL affected other players' decks\n"); //if so, test fails
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
		if (prevDeck - currDeck != 0) {								//if number of cards removed from deck was not two or greater
	#if (NOISY_TEST == 1)
			printf("councilRoomCard(): FAIL deck changed\n");
	#endif
			printPass = 1;
			allTestPass = 1;
		}
	#if (NOISY_TEST == 1)
		if (printPass == 0) {						//otherwise, test passes
			printf("councilRoomCard(): PASS deck did not change\n");
		}
		/***********************************************************************************/
		/*TEST 2: If Player Discards card with cost n and Selects Cards that costs < n + 2*/
		/*********************************************************************************/
		G.hand[p][handpos] = remodel;	//add remodel card to hand if it was removed
	
		printf("\nTesting player %d's remodel with choice1: %d, choice2:%d\n", p + 1, getCost(adventurer), getCost(gold));
	#endif

		choice1 = adventurer;
		choice2 = gold;

		previousHandCount = G.handCount[p]; //holds current players hand count
		prevDeck = G.deckCount[p];			//previous deck count
		prevDiscard = G.discardCount[p];	//previous discard count

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

		prevGold = G.supplyCount[gold];		//get initial supply count of gold

		cardEffect(remodel, choice1, choice2, choice3, &G, handpos, &bonus); 			//calls remodel card

		currGold = G.supplyCount[gold];		//get new supply count of gold

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

		currentHandCount = G.handCount[p];				//holds current player's new hand count
		currDeck = G.deckCount[p];						//current deck count
		currDiscard = G.discardCount[p];				//current discard count


		/*TEST 2a: card added to player's hand and not other players*/
		if (currentHandCount - previousHandCount == 0) {		//if there is no increase in cards in hand
			i = p + 1;
			netChange = 0;
			while (i != p) {					//check if other player's hands were affected
				netChange = otherHandsCurr[i] - otherHandsPrev[i];
				if (netChange != 0) {
	#if (NOISY_TEST == 1)
					printf("remodelCard(): FAIL affected other players' hands\n"); //if so, test fails
	#endif
					allTestPass = 1;
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


		printPass = 1;
		if (currentHandCount - previousHandCount == 0) {								//if no change in hand count
			for (i = 0; i < currentHandCount; i++) {								//if gold card added
				if (G.hand[p][i] == gold) {
					printPass = 0;
					G.hand[p][i] = -1;
					break;
				}
			}
		}

		if (printPass == 0) {						//otherwise, test passes
#if (NOISY_TEST == 1)
			printf("remodelCard(): PASS card choice2 gained\n");
#endif
		}
		else {
	#if (NOISY_TEST == 1)
			printf("remodelCard(): FAIL did not gain choice2 card\n");
	#endif
			printPass = 1;
			allTestPass = 1;
		}
		printPass = 0;
	#if (NOISY_TEST == 1)

		//TEST 2b: Remodel card removed from hand
		printf("Testing discard remodel for player %d\n", p + 1);
	#endif
		if (G.hand[p][handpos] == remodel) {
			printPass = 1;
		}

		if (printPass == 1) {
	#if (NOISY_TEST == 1)
			printf("remodelCard(): FAIL did not discard remodel card\n");
	#endif	
			allTestPass = 1;
		}
	#if (NOISY_TEST == 1)
		else {
			printf("remodelCard(): PASS discarded remodel card\n");
		}

		//TEST 2c: adventurer card removed from hand
		printf("Testing discard adventurer for player %d\n", p + 1);
	#endif
		if (currentHandCount - previousHandCount == 0) {								//if no change in hand count
			for (i = 0; i < currentHandCount; i++) {								//if council room not discarded
				if (G.hand[p][i] == adventurer)
					printPass = 1;
					break;
			}
		}
		if (printPass == 1) {
	#if (NOISY_TEST == 1)
			printf("remodelCard(): FAIL did not discard adventurer\n");
	#endif	
				allTestPass = 1;
		}
	#if (NOISY_TEST == 1)
		else {
			printf("remodelCard(): PASS discarded adventurer card\n");
		}

		//TEST 2d: gold supply decreased by 1
		printf("Testing gold supply decreased by 1\n");
	#endif

		if (prevGold - currGold != 1) {
	#if (NOISY_TEST == 1)
			printf("remodelCard(): FAIL did not decrease gold supply by 1\n");
	#endif	
			allTestPass = 1;
		}
	#if (NOISY_TEST == 1)
		else {
			printf("remodelCard(): PASS decreased gold supply by 1\n");
		}

		//TEST 2e: discard pile increased by 2
		printf("Testing player %d discard pile increase by 2\n", p + 1);
	#endif
		printPass = 0;
		if (currDiscard - prevDiscard == 2) {		//if the discard pile has gained 2 cards
			i = p + 1;
			netChange = 0;
			while (i != p) {					//check if other player's discard piles were affected
				netChange = otherDiscardCurr[i] - otherDiscardPrev[i];
				if (netChange != 1) {
	#if (NOISY_TEST == 1)
					printf("remodelCard(): FAIL affected other player's discard piles\n"); //if so, test fails
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
		if (currDiscard - prevDiscard != 2) {					//if card was not moved to discard pile
#if (NOISY_TEST == 1)
			printf("remodelCard(): FAIL discard pile did not change correctly\n");
#endif
			printPass = 1;
			allTestPass = 1;
		}
#if (NOISY_TEST == 1)
		if (printPass == 0) {						//otherwise, test passes
			printf("remodelCard(): PASS discard pile gained 2 card\n");
		}

		//TEST 2f: Deck count not changed
		printf("Testing no deck change for player %d\n", p + 1);
#endif
		printPass = 0;
		if (prevDeck - currDeck == 0) {
			i = p + 1;
			netChange = 0;
			while (i != p) {					//check if other player's decks piles were affected
				netChange = otherDecksCurr[i] - otherDecksPrev[i];
				if (netChange != 0) {
#if (NOISY_TEST == 1)
					printf("remodelCard(): FAIL changed other players' decks\n"); //if so, test fails
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
		if (prevDeck - currDeck != 0) {
#if (NOISY_TEST == 1)
			printf("remodelCard(): FAIL deck changed\n");
#endif
			printPass = 1;
			allTestPass = 1;
		}
#if (NOISY_TEST == 1)
		else  {						//otherwise, test passes
			printf("remodelCard(): PASSS deck unchanged\n");
		}
		/***********************************************************************************/
		/*TEST 3: If Player Discards card with cost n and Selects Cards that costs > n + 2*/
		/*********************************************************************************/
		G.hand[p][handpos] = remodel;	//add remodel card to hand if it was removed
		printf("\nTesting player %d's remodel with choice1: %d, choice2:%d\n", p + 1, getCost(village), getCost(gold));
#endif
		

		choice1 = village;
		choice2 = gold;

		previousHandCount = G.handCount[p]; //holds current players hand count
		prevDeck = G.deckCount[p];			//previous deck count
		prevDiscard = G.discardCount[p];	//previous discard count

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

		prevGold = G.supplyCount[gold];		//get initial supply count of gold

		cardEffect(remodel, choice1, choice2, choice3, &G, handpos, &bonus); 			//calls remodel card

		currGold = G.supplyCount[gold];		//get new supply count of gold

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

		currentHandCount = G.handCount[p];				//holds current player's new hand count
		currDeck = G.deckCount[p];						//current deck count
		currDiscard = G.discardCount[p];				//current discard count


		//TEST 3a: card not added to player's hand or other players
		if (currentHandCount - previousHandCount == 0) {		//if there is no increase in cards in hand
			i = p + 1;
			netChange = 0;
			while (i != p) {					//check if other player's hands were affected
				netChange = otherHandsCurr[i] - otherHandsPrev[i];
				if (netChange != 0) {
#if (NOISY_TEST == 1)
					printf("remodelCard(): FAIL affected other players' hands\n"); //if so, test fails
#endif
					allTestPass = 1;
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


		printPass = 0;
	
		for (i = 0; i < currentHandCount; i++) {								//if gold card added

			if (G.hand[p][i] == gold) {
				printPass = 0;
				G.hand[p][i] = -1;
				break;
			}
		}
		
		if (printPass == 0) {						//otherwise, test passes
#if (NOISY_TEST == 1)
			printf("remodelCard(): PASS no card gained\n");
#endif
		}
		else {
#if (NOISY_TEST == 1)
			printf("remodelCard(): FAIL choice2 card gained\n");
#endif
			printPass = 1;
			allTestPass = 1;
		}
		printPass = 0;
#if (NOISY_TEST == 1)

		//TEST 3b: Remodel card removed from hand
		printf("Testing discard remodel for player %d\n", p + 1);
#endif

		if (G.hand[p][handpos] == remodel) {
#if (NOISY_TEST == 1)
			printf("remodelCard(): PASS did not discard remodel card\n");
#endif	
			allTestPass = 1;
		}
#if (NOISY_TEST == 1)
		else {
			printf("remodelCard(): FAIL discarded remodel card\n");
		}

		//TEST 3c: village card not removed from hand
		printf("Testing discard village for player %d\n", p + 1);
#endif
		if (currentHandCount - previousHandCount == 0) {								//if no change in hand count
			for (i = 0; i < currentHandCount; i++) {								//if council room not discarded
				if (G.hand[p][i] == village)
					printPass = 0;
					break;
			}
		}
		else {
			printPass = 1;
		}
		if (printPass == 1) {
#if (NOISY_TEST == 1)
			printf("remodelCard(): FAIL discarded village\n");
#endif	
				allTestPass = 1;
		}
#if (NOISY_TEST == 1)
		else {
			printf("remodelCard(): PASS did not discard village card\n");
		}

		//TEST 3d: gold supply no increase
		printf("Testing gold supply decreased by 0\n");
#endif

		if (prevGold - currGold != 0) {
#if (NOISY_TEST == 1)
			printf("remodelCard(): FAIL changed gold supply\n");
#endif	
			allTestPass = 1;
		}
#if (NOISY_TEST == 1)
		else {
			printf("remodelCard(): PASS gold supply unchanged\n");
		}

		//TEST 3e: discard pile increased by 2
		printf("Testing player %d discard pile unchanged\n", p + 1);
#endif
		printPass = 0;
		if (currDiscard - prevDiscard == 0) {		//if the discard pile has not changed
			i = p + 1;
			netChange = 0;
			while (i != p) {					//check if other player's discard piles were affected
				netChange = otherDiscardCurr[i] - otherDiscardPrev[i];
				if (netChange != 1) {
#if (NOISY_TEST == 1)
					printf("remodelCard(): FAIL affected other player's discard piles\n"); //if so, test fails
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
		if (currDiscard - prevDiscard != 0) {
#if (NOISY_TEST == 1)
			printf("remodelCard(): FAIL discard changed\n");
#endif
			printPass = 1;
			allTestPass = 1;
		}
#if (NOISY_TEST == 1)
		if (printPass == 0) {						//otherwise, test passes
			printf("remodelCard(): PASS discard did not change\n");
		}

		//TEST 3f: Deck count not changed
		printf("Testing no deck change for player %d\n", p + 1);
#endif
		printPass = 0;
		if (prevDeck - currDeck == 0) {
			i = p + 1;
			netChange = 0;
			while (i != p) {					//check if other player's decks piles were affected
				netChange = otherDecksCurr[i] - otherDecksPrev[i];
				if (netChange != 0) {
#if (NOISY_TEST == 1)
					printf("remodelCard(): FAIL changed other player's decks\n"); //if so, test fails
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
		if (prevDeck - currDeck != 0) {
#if (NOISY_TEST == 1)
			printf("remodelCard(): FAIL deck changed\n");
#endif
			printPass = 1;
			allTestPass = 1;
		}
#if (NOISY_TEST == 1)
		if (printPass == 0) {						//otherwise, test passes
			printf("remodelCard(): PASS deck did not change\n");
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
