#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


int smithyCard(int player, struct gameState *state,int handPos,int trashFlag){

    //+3 Cards
    for (int i = 0; i <= 3; i++)
    {
    drawCard(player, state);
    }

    //discard card from hand
    discardCard(handPos, player, state, trashFlag);
    return 0;

}

int adventurerCard(struct gameState *state, int currentPlayer){
    int drawntreasure=0;
    int cardDrawn;
    int temphand[MAX_HAND];// moved above the if statement
    int z = 0;// this is the counter for the temp hand

    while(drawntreasure<=2){
        if (state->deckCount[currentPlayer] <1){//if the deck is empty we need to shuffle discard and add to deck
            shuffle(currentPlayer, state);
        }
        drawCard(currentPlayer, state);
        cardDrawn = state->hand[currentPlayer][state->handCount[currentPlayer]-1];//top card of hand is most recently drawn card.
        if (cardDrawn == copper || cardDrawn == silver || cardDrawn == gold){
            drawntreasure++;
        } else{
            temphand[z]=cardDrawn;
            state->handCount[currentPlayer]--; //this should just remove the top card (the most recently drawn one).
            z++;
        }
    }
    while(z-1>=0){
        state->discard[currentPlayer][state->discardCount[currentPlayer]++]=temphand[z-1]; // discard all cards in play that have been drawn
        z=z-1;
    }

    return 0;
}


int villageCard(int player, struct gameState *state,int handPos,int trashFlag){

    return greatHallCard(player, state, handPos, trashFlag);


}


int greatHallCard(int player, struct gameState *state,int handPos,int trashFlag){
    //+1 Card
    drawCard(player, state);

    //+1 Actions
    state->numActions++;

    //discard card from hand
    discardCard(handPos, player, state, trashFlag);
    return 0;
}

int embargoCard(int player, int choice1, struct gameState *state,int handPos,int trashFlag){

    //+2 Coins
    state->coins = state->coins + 2;

    //see if selected pile is in play
    if ( state->supplyCount[choice1] == -1 )
    {
    return -1;
    }

    //add embargo token to selected supply pile
    state->embargoTokens[choice1]++;

    //trash card
    discardCard(handPos, player, state, trashFlag);
    return 0;
}
