/* -----------------------------------------------------------------------
 * Program name: randomtestcard1.c
 * Name: Alexis Tucker
 * Class: CS 362
 * Email: tuckera@oregonstate.edu
 * Description: Randomly tests the smithy function in dominion.c
 * Last-modified: 5/13/19
 * Sources: betterTestDrawCard.c
 * -----------------------------------------------------------------------
 */
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "rngs.h"

int allTestPass = 0;	//global variable, 0 if all test's have passed, otherwise 1

#define DEBUG 0
#define NOISY_TEST 1	//set to 1 for printing all results, 0 to only print overall result
#define assertTrue(expr) if (!(expr)) {allTestPass = 1;}

void checkSmithyCard(int p, int choice1, int choice2, int choice3, struct gameState *G, int handpos, int *bonus) {
  int previousHandCount = G->handCount[p];	//player's hand count before cardEffect
  int prevDeck = G->deckCount[p];			//deck count before cardEffect
  int prevDiscard = G->discardCount[p];		//discard count before cardEffect
  int i;

  
  int otherHandsPrev[G->numPlayers];
  int otherHandsCurr[G->numPlayers];
  int otherDecksPrev[G->numPlayers];
  int otherDecksCurr[G->numPlayers];
  int otherDiscardPrev[G->numPlayers];
  int otherDiscardCurr[G->numPlayers];
  
  
  //get other players' deck count, discard count, and hand count before cardEffect
  for (i = 0; i < G->numPlayers; i++) {
	  if (i != p) {
		  otherDecksPrev[i] = G->deckCount[i];
		  otherDiscardPrev[i] = G->discardCount[i];
		  otherHandsPrev[i] = G->handCount[i];
	  }
  }
  

  //call cardEffect with smithy
  cardEffect(smithy, choice1, choice2, choice3, G, handpos, bonus); 

  int currentHandCount = G->handCount[p];		//player's hand count after cardEffect
  int currDeck = G->deckCount[p];				//deck count after cardEffect
  int currDiscard = G->discardCount[p];			//discard count after cardEffect
  
  
//get other players' deck count, discard count, and hand count after cardEffect
  for (i = 0; i < G->numPlayers; i++) {
	  if (i != p) {
		  otherDecksCurr[i] = G->deckCount[i];
		  otherDiscardCurr[i] = G->discardCount[i];
		  otherHandsCurr[i] = G->handCount[i];
	  }
  }
  

  //check two net cards added to hand
  assertTrue(previousHandCount + 2 == currentHandCount);
#if NOISY_TEST == 1
  if (previousHandCount + 2 == currentHandCount) {
	  printf("smithyCard(): PASS net gain of 2 cards to hand\n");
  }
  else {
	  printf("smithyCard(): FAIL did not gain 2 cards to hand\n");
  }
#endif

  //check smithy removed from hand
  assertTrue(G->hand[p][handpos] != smithy);
#if NOISY_TEST == 1
  if (G->hand[p][handpos] != smithy) {
	  printf("smithyCard(): PASS smithy removed from hand\n");
  }
  else {
	  printf("smithyCard(): FAIL smithy still in hand\n");
  }
#endif

  //check discard pile gained 1 card
  assertTrue(currDiscard - prevDiscard == 1);
#if NOISY_TEST == 1
  if (currDiscard - prevDiscard == 1) {
	  printf("smithyCard(): PASS discard pile gained 1 card\n");
  }
  else {
	  printf("smithyCard(): FAIL discard pile did not gain 1 card\n");
  }
#endif

  //check deck removed 3 cards
  assertTrue(prevDeck - currDeck == 3);
#if NOISY_TEST == 1
  if (prevDeck - currDeck == 3) {
	  printf("smithyCard(): PASS 3 cards removed from deck\n");
  }
  else {
	  printf("smithyCard(): FAIL 3 cards not removed from deck\n");
  }
#endif
  
  for (i = 0; i < G->numPlayers; i++) {
	  if (i != p) {
		  assertTrue(otherHandsCurr[i] == otherHandsPrev[i]);   //check other players' hands unchanged
#if NOISY_TEST == 1
		  if (otherHandsCurr[i] == otherHandsPrev[i]) {
			  printf("smithyCard(): PASS other players' hands unchanged\n");
		  }
		  else {
			  printf("smithyCard(): FAIL other players' hands changed\n");
		  }
#endif
		  assertTrue(otherDecksPrev[i] == otherDecksCurr[i]);   //check other players' deck did not change
#if NOISY_TEST == 1
		  if (otherDecksPrev[i] == otherDecksCurr[i]) {
			  printf("smithyCard(): PASS other players' decks unchanged\n");
		  }
		  else {
			  printf("smithyCard(): FAIL other players' decks changed\n");
		  }
#endif
		  assertTrue(otherDiscardCurr[i] == otherDiscardPrev[i]);   //check other players' discard pile doesn't change
#if NOISY_TEST == 1
		  if (otherDiscardCurr[i] == otherDiscardPrev[i]) {
			  printf("smithyCard(): PASS other players' discard unchanged\n\n");
		  }
		  else {
			  printf("smithyCard(): FAIL other players' discard changed\n\n");
		  }
#endif
	  }
  }
  
}



int main () {

  int i, n, p, choice1, choice2, choice3, handpos, pickCard, bonus=0;

  struct gameState G;

  printf ("Testing smithy.\n");

  printf ("RANDOM TESTS.\n");

  SelectStream(2);
  PutSeed(3);

  for (n = 0; n < 5; n++) {
    for (i = 0; i < sizeof(struct gameState); i++) {
      ((char*)&G)[i] = floor(Random() * 256);
    }
	G.numPlayers = 2;
    p = floor(Random() * 2);						//randomize player
	G.whoseTurn = p;								//set whose's turn to player
    G.deckCount[p] = floor(Random() * MAX_DECK);	//randomize deck size
    G.discardCount[p] = floor(Random() * MAX_DECK);	//randomize discard pile size
    G.handCount[p] = floor(Random() * MAX_HAND);	//randomize hand size
	handpos = floor(Random() * G.handCount[p]);		//randomize position of Smithy in hand
	choice1 = floor(Random() * 26);					//randomize choice1
	choice2 = floor(Random() * 26);					//randomize choice2
	choice3 = floor(Random() * 26);					//randomize choice3
	G.playedCardCount = floor(Random() * MAX_DECK); //randomize playedCardCount

	
	//randomize other players' hand, deck, and discard
	for (i = 0; i < G.numPlayers; i++) {
		if (i != p) {
			G.deckCount[i] = floor(Random() * MAX_DECK);	//randomize deck size
			G.discardCount[i] = floor(Random() * MAX_DECK);	//randomize discard pile size
			G.handCount[i] = floor(Random() * MAX_HAND);	//randomize hand size
		}
	}
	
	
	//randomize card supply
	for (i = 0; i < treasure_map+1; i++) {
		G.supplyCount[i] = floor(Random() * 40);
	}

	//randomize deck contents
	for (i = 0; i < G.deckCount[p]; i++) {
		pickCard = floor(Random() * 26);
		G.deck[p][i] = pickCard;
	}

	//randomize discard pile contents
	for (i = 0; i < G.discardCount[p]; i++) {
		pickCard = floor(Random() * 26);
		G.discard[p][i] = pickCard;
	}

	//randomize hand contents
	for (i = 0; i < G.handCount[p]; i++) {
		pickCard = floor(Random() * 26);
		G.hand[p][i] = pickCard;
	}

	G.hand[p][handpos] = smithy;				//put smithy card at handpos

	checkSmithyCard(p, choice1, choice2, choice3, &G, handpos, &bonus);	//call test				
  }

  if (allTestPass == 0) {
	  printf("ALL TESTS OK\n");
  }
  else {
	  printf("TESTS FAILED.\n");
  }


  return 0;
}
