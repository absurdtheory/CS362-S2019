/* -----------------------------------------------------------------------
 * Program name: testFunction4.c
 * Name: Alexis Tucker
 * Class: CS 362
 * Email: tuckera@oregonstate.edu
 * Description: Tests the isGameOver function in dominion.c
 * Last-modified: 4/24/19
 * Source: testUpdateCoins.c
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <limits.h>

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 1

int main() {
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
	

    printf ("TESTING isGameOver():\n");

#if (NOISY_TEST == 1)
	/*TEST 1: No supplies are empty*/
	printf("Testing no empty supplies \n");
#endif
	int numSupplyCards = sizeof(G.supplyCount)/sizeof(int)-1;

	int prevSupply[numSupplyCards];

	for (p = 0; p < numSupplyCards; p++) {
		prevSupply[p] = G.supplyCount[p];
	}

	int retValue = isGameOver(&G);

	/*TEST 1a: No change in supply piles examined*/
	int currSupply[numSupplyCards];

	for (p = 0; p < numSupplyCards; p++) {
		currSupply[p] = G.supplyCount[p];
	}

	for (p = 0; p < numSupplyCards; p++) {
		if (prevSupply[p] != currSupply[p]) {
#if (NOISY_TEST == 1)
			printf("isGameOver(): FAIL supply piles were changed\n");
#endif
		}
	}

	/*TEST 1b: Function Returns 0 for game not over*/
	if (retValue != 0) {
#if (NOISY_TEST == 1)
		printf("isGameOver(): FAIL returned game is over when it is not\n");
#endif
		allTestPass = 1;
		printPass = 1;
	}

#if (NOISY_TEST == 1)
	if (printPass == 0) {
		printf("isGameOver(): PASS no change in supply piles and game returned not over\n");
	}
#endif
	printPass = 0;

#if (NOISY_TEST == 1)
	/*TEST 2: Province pile is empty*/
	printf("Testing empty province pile\n");
#endif

	int tempProvince = G.supplyCount[province];
	G.supplyCount[province] = 0;
	for (p = 0; p < numSupplyCards; p++) {
		prevSupply[p] = G.supplyCount[p];
	}

	retValue = isGameOver(&G);

	/*TEST 2a: No change in supply piles examined*/

	for (p = 0; p < numSupplyCards; p++) {
		currSupply[p] = G.supplyCount[p];
	}

	for (p = 0; p < numSupplyCards; p++) {
		if (prevSupply[p] != currSupply[p]) {
#if (NOISY_TEST == 1)
			printf("isGameOver(): FAIL supply piles were changed\n");
#endif
		}
	}

	/*TEST 2b: Function Returns 1 for game over*/
	if (retValue != 1) {
#if (NOISY_TEST == 1)
		printf("isGameOver(): FAIL returned game is not over\n");
#endif
		allTestPass = 1;
		printPass = 1;
	}

#if (NOISY_TEST == 1)
	if (printPass == 0) {
		printf("isGameOver(): PASS no change in supply piles and game returned over\n");
	}
#endif
	printPass = 0;

#if (NOISY_TEST == 1)
	/*TEST 3: Three supplies (other than province) are empty*/
	printf("Testing three empty supplies\n");
#endif
	G.supplyCount[province] = tempProvince;

	//save supply counts
	int tempCopper = G.supplyCount[copper];
	int tempSilver = G.supplyCount[silver];
	int tempGold = G.supplyCount[gold];

	//set supply counts to zero
	G.supplyCount[copper] = 0;
	G.supplyCount[silver] = 0;
	G.supplyCount[gold] = 0;

	for (p = 0; p < numSupplyCards; p++) {
		prevSupply[p] = G.supplyCount[p];
	}

	retValue = isGameOver(&G);

	/*TEST 3a: No change in supply piles examined*/
	for (p = 0; p < numSupplyCards; p++) {
		currSupply[p] = G.supplyCount[p];
	}

	for (p = 0; p < numSupplyCards; p++) {
		if (prevSupply[p] != currSupply[p]) {
#if (NOISY_TEST == 1)
			printf("isGameOver(): FAIL supply piles were changed\n");
#endif
		}
	}

	/*TEST 3b: Function Returns 1 for game*/
	if (retValue != 1) {
#if (NOISY_TEST == 1)
		printf("isGameOver(): FAIL returned game is not over\n");
#endif
		allTestPass = 1;
		printPass = 1;
	}

#if (NOISY_TEST == 1)
	if (printPass == 0) {
		printf("isGameOver(): PASS no change in supply piles and game returned over\n");
	}
#endif

	//restore supply counts
	G.supplyCount[copper] = tempCopper;
	G.supplyCount[silver] = tempSilver;
	G.supplyCount[gold] = tempGold;
	printPass = 0;

#if (NOISY_TEST == 1)
   /*TEST 5: All supplies in play are examined*/
	printf("Testing all supplies examined\n");
#endif 
	int tempSupply1;

	for (p = 0; p < numSupplyCards-1; p++) {
		if (p == 1) {
			p += 2;
		}
		else if (p == 2) {
			p++;
		}

		while ((p < numSupplyCards) & (G.supplyCount[p] == -1)) {
			p++;
		}
		if (G.supplyCount[p] == -1) {
			break;
		}

		tempSupply1 = G.supplyCount[p];
		G.supplyCount[p] = 0;
		G.supplyCount[1] = 0;
		G.supplyCount[2] = 0;


		retValue = isGameOver(&G);
		if (retValue == 0) {
#if (NOISY_TEST == 1)
			printf("isGameOver(): FAIL not all supply cards in game examined for game over\n");
#endif 
			printPass = 1;
			allTestPass = 1;
			break;
		}

		G.supplyCount[p] = tempSupply1;

	}
#if (NOISY_TEST == 1)
	if (printPass == 0) {
		printf("isGameOver(): PASS all supply cards examined for game over\n");
	}
#endif 

	if (allTestPass == 0) {
		printf("TEST SUCCESSFULLY COMPLETED\n");	//if all tests were successful, print success
	}
	else {
		printf("TEST FAILED\n");					//if all tests were not successfuly, print failure
	}


    return 0;
}
