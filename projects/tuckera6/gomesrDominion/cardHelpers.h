#ifndef _CARDHELPERS_H
#define _CARDHELPERS_H

#include "dominion.h"

int smithyCard(int player, struct gameState *state,
			int handPos, int trashFlag);
int adventurerCard(struct gameState *state, int currentPlayer);

int villageCard(int player, struct gameState *state,
			int handPos, int trashFlag);

int greatHallCard(int player, struct gameState *state,
			int handPos,int trashFlag);

int embargoCard(int player, int choice1, struct gameState *state,int handPos,int trashFlag);
#endif
