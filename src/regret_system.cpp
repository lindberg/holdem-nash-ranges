#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "regret_system.h"
#include "helper_functions.h"

RegretSystem::RegretSystem() : HR(32487834) {
	srand(time(NULL));

	// Load the HandRanks.DAT file and map it into the HR array
	printf("Loading HandRanks.DAT file...");

	FILE* fin = fopen("HandRanks.dat", "rb");
	if (!fin) {
		printf("fail.\n\n");
		exit(EXIT_FAILURE);
	}
	size_t bytesread = fread(HR.data(), HR.size()*sizeof(int), 1, fin);	// get the HandRank Array

	printf("complete.\n");
}

static unsigned int g_seed;

inline int FastRand() {
	g_seed = (214013 * g_seed + 2531011);
	return (g_seed >> 16) & 0x7FFF;
}

void RegretSystem::RunIterations(int iterations, int stack_in_small_blinds) {
	const char* file_name = ("regret_tree" + std::to_string(stack_in_small_blinds) + ".dat").c_str();
	if (!regret_tree_.OpenRegretTree(file_name)) {
		regret_tree_.SaveRegretTree(file_name);
	}

	// 2998800 hands
	for (int ite = 0; ite < iterations; ite++) {
		for (int c1 = 0; c1 < 48; c1++) {
			for (int c2 = c1 + 1; c2 < 49; c2++) {
				for (int c3 = 0; c3 < 50; c3++) {
					for (int c4 = c3 + 1; c4 < 51; c4++) {
						Work(c1, c2, c3, c4, stack_in_small_blinds);
						Work(c3, c4, c1, c2, stack_in_small_blinds);
					}
				}
			}
		}
	}

	regret_tree_.SaveRegretTree(file_name);
}

void RegretSystem::Work(int c1, int c2, int c3, int c4, int stack_in_small_blinds) {
	int* sb_push, * bb_call;
	int sb_hand_strength, bb_hand_strength;
	bool draw, sb_won;

	draw = false, sb_won = false;

	int cards[9];

	cards[0] = c1;
	cards[1] = c2;
	cards[2] = c3;
	cards[3] = c4;

	DealCards(cards);

	int sb_cards[7] = { cards[0], cards[1], cards[4], cards[5], cards[6], cards[7], cards[8] };
	int bb_cards[7] = { cards[2], cards[3], cards[4], cards[5], cards[6], cards[7], cards[8] };

	sb_push = regret_tree_.GetNodePointer(sb_cards, true);
	bb_call = regret_tree_.GetNodePointer(bb_cards, false);

	sb_hand_strength = LookupHand(sb_cards);
	bb_hand_strength = LookupHand(bb_cards);

	if (sb_hand_strength > bb_hand_strength) sb_won = true;
	else if (sb_hand_strength == bb_hand_strength) draw = true;

	// BB regret system
	if (*sb_push >= 0) {
		if (sb_won)* bb_call -= stack_in_small_blinds - 2;
		else if (draw)* bb_call += 2;
		else *bb_call += stack_in_small_blinds + 2;
	}

	// SB regret system
	if (*bb_call >= 0) {
		if (sb_won)* sb_push += stack_in_small_blinds + 1;
		else if (draw)* sb_push += 1;
		else *sb_push -= stack_in_small_blinds - 1;
	}
	else *sb_push += 3;
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

	deck[cards[0]] = deck[51];
	deck[cards[1]] = deck[50];
	deck[cards[2]] = deck[49];
	deck[cards[3]] = deck[48];

	// Deal random cards
	int i;
	for (i = 4; i < 9-1; i++) {
		random_card = FastMod(FastRand(), (52 - i));
		cards[i] = deck[random_card];
		deck[random_card] = deck[52 - 1 - i];
	}

	cards[i] = deck[FastMod(FastRand(), (52 - 1 - i))];
}