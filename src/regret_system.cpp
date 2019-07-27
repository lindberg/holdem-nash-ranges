#include <iostream>
#include <cstdlib>
#include <ctime>

#include "regret_system.h"

#define PLAYERS 2
#define CARDS PLAYERS*2+5

RegretSystem::RegretSystem() : HR(32487834) {
	srand(time(NULL));

	// Load the HandRanks.DAT file and map it into the HR array
	printf("Loading HandRanks.DAT file...");
	//memset(HR, 0, sizeof(HR));
	FILE* fin = fopen("HandRanks.dat", "rb");
	if (!fin)
		exit(EXIT_FAILURE);
	size_t bytesread = fread(HR.data(), HR.size()*sizeof(int), 1, fin);	// get the HandRank Array
	fclose(fin);
	printf("complete.\n\n");

	if (!regret_tree_.OpenRegretTree("regret_tree.dat")) {
		regret_tree_.SaveRegretTree("regret_tree.dat");
	}
}

void RegretSystem::RunIterations(int iterations) {
	int cards[CARDS];

	int* sb_push, * bb_call;
	int sb_hand_strength, bb_hand_strength;
	bool draw, sb_won;

	for (int ite = 0; ite < iterations; ite++) {
		draw = false, sb_won = false;

		DealCards(cards);

		int sb_cards[7] = { cards[0], cards[1], cards[4], cards[5], cards[6], cards[7], cards[8] };
		int bb_cards[7] = { cards[2], cards[3], cards[4], cards[5], cards[6], cards[7], cards[8] };

		int inde = regret_tree_.RegretIndex(sb_cards, true);
		int indebb = regret_tree_.RegretIndex(bb_cards, false);


		sb_push = regret_tree_.GetNodePointer(sb_cards, true);
		bb_call = regret_tree_.GetNodePointer(bb_cards, false);
		*sb_push += 2;
		
		sb_hand_strength = LookupHand(sb_cards);
		bb_hand_strength = LookupHand(bb_cards);

		if (sb_hand_strength > bb_hand_strength) sb_won = true;
		else if (sb_hand_strength == bb_hand_strength) draw = true;

		// BB regret system
		if (*sb_push >= 0) {
			if (sb_won) *bb_call -= 18;
			else if (draw) *bb_call += 2;
			else *bb_call += 22;
		}

		// SB regret system
		if (*bb_call >= 0) {
			if (sb_won) *sb_push += 21;
			else if (draw) *sb_push += 1;
			else *sb_push -= 19;
		}
	}

	regret_tree_.SaveRegretTree("regret_tree.dat");
}

int RegretSystem::LookupHand(int* cards)
{
	long long p = HR[(long long)53 + *cards++];
	p = HR[p + *cards++];
	p = HR[p + *cards++];
	p = HR[p + *cards++];
	p = HR[p + *cards++];
	p = HR[p + *cards++];
	return HR[p + *cards++];
}

void RegretSystem::DealCards(int * cards) {
	int deck[52];
	int random_card;

	// Reset deck
	for (int i = 1; i <= 52; i++) {
		deck[i - 1] = i;
	}

	// Deal random cards
	int i;
	for (i = 0; i < CARDS-1; i++) {
		random_card = rand() % (52 - i);
		cards[i] = deck[random_card];
		deck[random_card] = deck[52 - 1 - i];
	}

	cards[i] = deck[rand() % (52 - 1 - i)];
}