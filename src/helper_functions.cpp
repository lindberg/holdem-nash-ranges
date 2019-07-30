#include "helper_functions.h"

int FastMod(const uint32_t input, const int ceil) {
	return input >= ceil ? input % ceil : input;
	// NB: the assumption here is that the numbers are positive.
}

std::string HandToString(int* cards) {
	std::string output = "";
	int index = 0;
	int cards_suit[2], cards_val[2];

	// Make cards[0] the smallest card.
	if (cards[0] > cards[1]) {
		int temp = cards[1];
		cards[1] = cards[0];
		cards[0] = temp;
	}

	cards_suit[0] = FastMod(cards[0], 4);
	cards_suit[1] = FastMod(cards[1], 4);
	cards_val[0] = cards[0] / 4.01 + 2;
	cards_val[1] = cards[1] / 4.01 + 2;

	for (int i = 1; i >= 0; i--) {
		int card = cards_val[i];
		if (card == 10) output += "T";
		else if (card == 11) output += "J";
		else if (card == 12) output += "Q";
		else if (card == 13) output += "K";
		else if (card == 14) output += "A";
		else output += std::to_string(card);
	}

	if (cards_suit[0] == cards_suit[1]) output += "s";
	else if (cards_suit[0] != cards_suit[1] && cards_val[0] != cards_val[1]) output += "o";
}

std::string IndexToString(int index) {
	bool suited = false;
	std::string output = "";
	if (index >= 169) index -= 169;

	if (index > 12) index -= 13;
	else {
		int card = index + 2;
		if (card == 10) output += "TT";
		else if (card == 11) output += "JJ";
		else if (card == 12) output += "QQ";
		else if (card == 13) output += "KK";
		else if (card == 14) output += "AA";
		else output += std::to_string(card) + std::to_string(card);
		return output;
	}

	if (index >= 78) {
		index -= 78;
		suited = true;
	}

	int cards[2];

	for (int m = 0; m < 12; m++) {
		cards[0] = 2 + m;
		for (int i = m; i < 12; i++) {
			cards[1] = 3 + i;
			if (index == 0) {
				for (int j = 1; j >= 0; j--) {
					int card = cards[j];

					if (card == 10) output += "T";
					else if (card == 11) output += "J";
					else if (card == 12) output += "Q";
					else if (card == 13) output += "K";
					else if (card == 14) output += "A";
					else output += std::to_string(card);
				}
			}
			index--;
		}
	}

	if (suited) output += "s";
	else output += "o";

	return output;
}

// Fast random integers in chunks of 4
void srand_sse(unsigned int seed) {
	cur_seed = _mm_set_epi32(seed, seed + 1, seed, seed + 1);
}

void rand_sse(unsigned int* result) {
	__declspec(align(16)) __m128i cur_seed_split;
	__declspec(align(16)) __m128i multiplier;
	__declspec(align(16)) __m128i adder;
	__declspec(align(16)) __m128i mod_mask;
	__declspec(align(16)) __m128i sra_mask;
	__declspec(align(16)) __m128i sseresult;

	__declspec(align(16)) static const unsigned int mult[4] =
	{ 214013, 17405, 214013, 69069 };

	__declspec(align(16)) static const unsigned int gadd[4] =
	{ 2531011, 10395331, 13737667, 1 };

	__declspec(align(16)) static const unsigned int mask[4] =
	{ 0xFFFFFFFF, 0, 0xFFFFFFFF, 0 };

	__declspec(align(16)) static const unsigned int masklo[4] =
	{ 0x00007FFF, 0x00007FFF, 0x00007FFF, 0x00007FFF };

	adder = _mm_load_si128((__m128i*) gadd);
	multiplier = _mm_load_si128((__m128i*) mult);
	mod_mask = _mm_load_si128((__m128i*) mask);
	sra_mask = _mm_load_si128((__m128i*) masklo);
	cur_seed_split = _mm_shuffle_epi32(cur_seed, _MM_SHUFFLE(2, 3, 0, 1));

	cur_seed = _mm_mul_epu32(cur_seed, multiplier);
	multiplier = _mm_shuffle_epi32(multiplier, _MM_SHUFFLE(2, 3, 0, 1));
	cur_seed_split = _mm_mul_epu32(cur_seed_split, multiplier);

	cur_seed = _mm_and_si128(cur_seed, mod_mask);
	cur_seed_split = _mm_and_si128(cur_seed_split, mod_mask);
	cur_seed_split = _mm_shuffle_epi32(cur_seed_split, _MM_SHUFFLE(2, 3, 0, 1));
	cur_seed = _mm_or_si128(cur_seed, cur_seed_split);
	cur_seed = _mm_add_epi32(cur_seed, adder);

	_mm_storeu_si128((__m128i*) result, cur_seed);

	return;
}