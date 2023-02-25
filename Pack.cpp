// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Pack.h"

Pack::Pack() {
	next = 0;
	const int number_per_suit = 6;
	for (int suit = SPADES; suit <= DIAMONDS; suit++) {
		for (int rank = 7; rank <= 12; rank++) {
			cards[suit * number_per_suit + rank - 7] = Card(static_cast<Rank>(rank), 
														static_cast<Suit>(suit));
		}
	}
}

Pack::Pack(std::istream& pack_input) {
	next = 0;
	Rank rank;
	Suit suit;
	std::string seperated;
	for (int i = 0; i < 24; i++) {
		pack_input >> rank >> seperated >> suit;
		cards[i] = Card(rank, suit);
	}
}

Card Pack::deal_one(){
	return cards[next++];
}

void Pack::reset() {
	next = 0;
}

void Pack::shuffle() {
	next = 0;
	int half_pack = 12;
	std::array<Card, PACK_SIZE> shuffled_pack;
	for (int shuffle_times = 0; shuffle_times < 7; shuffle_times++) {
		for (int i = 0; i < half_pack; i++) {
			shuffled_pack[2 * i + 1] = cards[i];
		}
		for (int i = half_pack; i < PACK_SIZE; i++) {
			shuffled_pack[2 * (i - half_pack)] = cards[i];
		}
		for (int i = 0; i < PACK_SIZE; i++) {
			cards[i] = shuffled_pack[i];
		}
	}
}

bool Pack::empty() const {
	if (next == PACK_SIZE) {
		return true;
	}
	else {
		return false;
	}
}
