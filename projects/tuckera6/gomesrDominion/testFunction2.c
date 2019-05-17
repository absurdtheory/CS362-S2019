/* -----------------------------------------------------------------------
 * Program name: testFunction2.c
 * Name: Alexis Tucker
 * Class: CS 362
 * Email: tuckera@oregonstate.edu
 * Description: Tests the numHandCards function in dominion.c
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
#include <limits.h>

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
	

    printf ("TESTING numHandCards():\n");
    for (p = 0; p < numPlayer; p++)
    {
		int printPass = 0;
#if (NOISY_TEST == 1)
		/*TEST 1: Returns hand count of the current player when both players have the same number of cards in hand*/
		printf("\nTesting player %d adding card to hand.\n", p+1);
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

		int functionHandCount = numHandCards(&G);							//calls draw card

		i = p + 1;
		while (i != p) {						 //fills out array
			otherHandsCurr[i] = G.handCount[i];
			i++;
			if (i >= numPlayer) {
				i = 0;
			}
		}

		int currentHandCount = G.handCount[p];				//holds current player's hand count after function call
		if ((currentHandCount == previousHandCount) & (currentHandCount == functionHandCount)) {		//if there was no difference in hand count
			i = p + 1;
			int netChange = 0;
			while (i != p) {					//check if other player's hands were affected
				netChange = otherHandsCurr[i] - otherHandsPrev[i];
				if (netChange != 0) {
#if (NOISY_TEST == 1)
					printf("numHandCards(): FAIL added/removed card to other player's hand\n"); //if so, test fails
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
		else {								//else if hand count changes, the test fails
#if (NOISY_TEST == 1)
			printf("numHandCards(): FAIL did not return current player's hand count\n");
#endif
			printPass = 1;
		}
#if (NOISY_TEST == 1)
		if (printPass == 0) {						//otherwise, test passes
			printf("numHandCards(): PASS returned current player's hand count when both players have same hand count\n");
		}

		/*TEST 2: Returns current player's hand count when it is 0*/
		printf("Testing player hand count when it is zero %d\n", p+1);
#endif
		G.handCount[p] = 0;			//set current player's hand count to 0
		previousHandCount = G.handCount[p]; //holds current players hand count

		i = p + 1;
		while (i != p) {						 //fills out array
			otherHandsPrev[i] = G.handCount[i];
			i++;
			if (i >= numPlayer) {
				i = 0;
			}
		}

		functionHandCount = numHandCards(&G);							//calls draw card

		i = p + 1;
		while (i != p) {						 //fills out array
			otherHandsCurr[i] = G.handCount[i];
			i++;
			if (i >= numPlayer) {
				i = 0;
			}
		}

		currentHandCount = G.handCount[p];				//holds current player's hand count after function call
		if ((currentHandCount == previousHandCount) & (currentHandCount == functionHandCount)) {		//if there was no difference in hand count
			i = p + 1;
			int netChange = 0;
			while (i != p) {					//check if other player's hands were affected
				netChange = otherHandsCurr[i] - otherHandsPrev[i];
				if (netChange != 0) {
#if (NOISY_TEST == 1)
					printf("numHandCards(): FAIL added/removed card to other player's hand\n"); //if so, test fails
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
		else {								//else if hand count changes, the test fails
#if (NOISY_TEST == 1)
			printf("numHandCards(): FAIL did not return current player's hand count\n");
#endif
			printPass = 1;
			allTestPass = 1;
		}
#if (NOISY_TEST == 1)
		if (printPass == 0) {						//otherwise, test passes
			printf("numHandCards(): PASS returned current player's hand count when count is 0\n");
		}

		/*TEST 3: Returns current player's hand count when it is negative*/
		printf("Testing negative hand value for player %d\n", p + 1);
#endif
		G.handCount[p] = -1;			//set current player's hand count to -1
		previousHandCount = G.handCount[p]; //holds current players hand count

		i = p + 1;
		while (i != p) {						 //fills out array
			otherHandsPrev[i] = G.handCount[i];
			i++;
			if (i >= numPlayer) {
				i = 0;
			}
		}

		functionHandCount = numHandCards(&G);							//calls draw card

		i = p + 1;
		while (i != p) {						 //fills out array
			otherHandsCurr[i] = G.handCount[i];
			i++;
			if (i >= numPlayer) {
				i = 0;
			}
		}

		currentHandCount = G.handCount[p];				//holds current player's hand count after function call
		if ((currentHandCount == previousHandCount) & (currentHandCount == functionHandCount)) {		//if there was no difference in hand count
			i = p + 1;
			int netChange = 0;
			while (i != p) {					//check if other player's hands were affected
				netChange = otherHandsCurr[i] - otherHandsPrev[i];
				if (netChange != 0) {
#if (NOISY_TEST == 1)
					printf("numHandCards(): FAIL added/removed card to other player's hand\n"); //if so, test fails
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
		else {								//else if hand count changes, the test fails
#if (NOISY_TEST == 1)
			printf("numHandCards(): FAIL did not return current player's hand count\n");
#endif
			printPass = 1;
			allTestPass = 1;
		}
#if (NOISY_TEST == 1)
		if (printPass == 0) {						//otherwise, test passes
			printf("numHandCards(): PASS returned current player's hand count when count is negative\n");
		}

		/*TEST 4: Return player's hand count when it is max hand size*/
		printf("Testing max hand size value for player %d\n", p + 1);
#endif	
		G.handCount[p] = MAX_HAND;			//set current player's hand count to max hand size
		previousHandCount = G.handCount[p]; //holds current players hand count

		i = p + 1;
		while (i != p) {						 //fills out array
			otherHandsPrev[i] = G.handCount[i];
			i++;
			if (i >= numPlayer) {
				i = 0;
			}
		}

		functionHandCount = numHandCards(&G);							//calls draw card

		i = p + 1;
		while (i != p) {						 //fills out array
			otherHandsCurr[i] = G.handCount[i];
			i++;
			if (i >= numPlayer) {
				i = 0;
			}
		}

		currentHandCount = G.handCount[p];				//holds current player's hand count after function call
		if ((currentHandCount == previousHandCount) & (currentHandCount == functionHandCount)) {		//if there was no difference in hand count
			i = p + 1;
			int netChange = 0;
			while (i != p) {					//check if other player's hands were affected
				netChange = otherHandsCurr[i] - otherHandsPrev[i];
				if (netChange != 0) {
#if (NOISY_TEST == 1)
					printf("numHandCards(): FAIL added/removed card to other player's hand\n"); //if so, test fails
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
		else {								//else if hand count changes, the test fails
#if (NOISY_TEST == 1)
			printf("numHandCards(): FAIL did not return current player's hand count\n");
#endif
			printPass = 1;
			allTestPass = 1;
		}
#if (NOISY_TEST == 1)
		if (printPass == 0) {						//otherwise, test passes
			printf("numHandCards(): PASS returned current player's hand count when count is max hand size\n");
		}

		/*TEST 5: Return's player's handcount when it is max int size*/
		printf("Testing max int hand value for player %d\n", p + 1);
#endif

		G.handCount[p] = INT_MAX;			//set current player's hand count to max int  size
		previousHandCount = G.handCount[p]; //holds current players hand count

		i = p + 1;
		while (i != p) {						 //fills out array
			otherHandsPrev[i] = G.handCount[i];
			i++;
			if (i >= numPlayer) {
				i = 0;
			}
		}

		functionHandCount = numHandCards(&G);							//calls draw card

		i = p + 1;
		while (i != p) {						 //fills out array
			otherHandsCurr[i] = G.handCount[i];
			i++;
			if (i >= numPlayer) {
				i = 0;
			}
		}

		currentHandCount = G.handCount[p];				//holds current player's hand count after function call
		if ((currentHandCount == previousHandCount) & (currentHandCount == functionHandCount)) {		//if there was no difference in hand count
			i = p + 1;
			int netChange = 0;
			while (i != p) {					//check if other player's hands were affected
				netChange = otherHandsCurr[i] - otherHandsPrev[i];
				if (netChange != 0) {
#if (NOISY_TEST == 1)
					printf("numHandCards(): FAIL added/removed card to other player's hand\n"); //if so, test fails
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
		else {								//else if hand count changes, the test fails
#if (NOISY_TEST == 1)
			printf("numHandCards(): FAIL did not return current player's hand count\n");
#endif
			printPass = 1;
			allTestPass = 1;
		}
#if (NOISY_TEST == 1)
		if (printPass == 0) {						//otherwise, test passes
			printf("numHandCards(): PASS returned current player's hand count when count is max int size\n");
		}

		/*TEST 6: Return's player's handcount when it is max int size*/
		printf("Testing min int hand value for player %d\n", p + 1);
#endif
		G.handCount[p] = INT_MIN;  //sets current player's hand count to min int size
		previousHandCount = G.handCount[p]; //holds current players hand count

		i = p + 1;
		while (i != p) {						 //fills out array
			otherHandsPrev[i] = G.handCount[i];
			i++;
			if (i >= numPlayer) {
				i = 0;
			}
		}

		functionHandCount = numHandCards(&G);							//calls draw card

		i = p + 1;
		while (i != p) {						 //fills out array
			otherHandsCurr[i] = G.handCount[i];
			i++;
			if (i >= numPlayer) {
				i = 0;
			}
		}

		currentHandCount = G.handCount[p];				//holds current player's hand count after function call
		if ((currentHandCount == previousHandCount) & (currentHandCount == functionHandCount)) {		//if there was no difference in hand count
			i = p + 1;
			int netChange = 0;
			while (i != p) {					//check if other player's hands were affected
				netChange = otherHandsCurr[i] - otherHandsPrev[i];
				if (netChange != 0) {
#if (NOISY_TEST == 1)
					printf("numHandCards(): FAIL added/removed card to other player's hand\n"); //if so, test fails
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
		else {								//else if hand count changes, the test fails
#if (NOISY_TEST == 1)
			printf("numHandCards(): FAIL did not return current player's hand count\n");
#endif
			printPass = 1;
			allTestPass = 1;
		}
#if (NOISY_TEST == 1)
		if (printPass == 0) {						//otherwise, test passes
			printf("numHandCards(): PASS returned current player's hand count when count is min int size\n");
		}

#endif

		G.whoseTurn = p + 1;				//increment player's turn
	}

	if (allTestPass == 0) {
		printf("TEST SUCCESSFULLY COMPLETED\n");	//if all tests were successful, print success
	}
	else {
		printf("TEST FAILED\n");					//if all tests were not successfuly, print failure
	}


    return 0;
}
