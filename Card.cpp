// Project UID 1d9f47bfc76643019cfbf037641defe1

#include <cassert>
#include <iostream>
#include <array>
#include "Card.h"

using namespace std;

/////////////// Rank operator implementations - DO NOT CHANGE ///////////////

constexpr const char *const RANK_NAMES[] = {
  "Two",   // TWO
  "Three", // THREE
  "Four",  // FOUR
  "Five",  // FIVE
  "Six",   // SIX
  "Seven", // SEVEN
  "Eight", // EIGHT
  "Nine",  // NINE
  "Ten",   // TEN
  "Jack",  // JACK
  "Queen", // QUEEN
  "King",  // KING
  "Ace"    // ACE
};

//REQUIRES str represents a valid rank ("Two", "Three", ..., "Ace")
Rank string_to_rank(const std::string &str) {
  for(int r = TWO; r <= ACE; ++r) {
    if (str == RANK_NAMES[r]) {
      return static_cast<Rank>(r);
    }
  }
  assert(false); // Input string didn't match any rank
}

//EFFECTS Prints Rank to stream, for example "Two"
std::ostream & operator<<(std::ostream &os, Rank rank) {
  os << RANK_NAMES[rank];
  return os;
}

//REQUIRES If any input is read, it must be a valid rank
//EFFECTS Reads a Rank from a stream, for example "Two" -> TWO
std::istream & operator>>(std::istream &is, Rank &rank) {
  string str;
  if(is >> str) {
    rank = string_to_rank(str);
  }
  return is;
}



/////////////// Suit operator implementations - DO NOT CHANGE ///////////////

constexpr const char *const SUIT_NAMES[] = {
  "Spades",   // SPADES
  "Hearts",   // HEARTS
  "Clubs",    // CLUBS
  "Diamonds", // DIAMONDS
};

//REQUIRES str represents a valid suit ("Spades", "Hearts", "Clubs", or "Diamonds")
Suit string_to_suit(const std::string &str) {
  for(int s = SPADES; s <= DIAMONDS; ++s) {
    if (str == SUIT_NAMES[s]) {
      return static_cast<Suit>(s);
    }
  }
  assert(false); // Input string didn't match any suit
}

//EFFECTS Prints Suit to stream, for example "Spades"
std::ostream & operator<<(std::ostream &os, Suit suit) {
  os << SUIT_NAMES[suit];
  return os;
}

//REQUIRES If any input is read, it must be a valid suit
//EFFECTS Reads a Suit from a stream, for example "Spades" -> SPADES
std::istream & operator>>(std::istream &is, Suit &suit) {
  string str;
  if (is >> str) {
    suit = string_to_suit(str);
  }
  return is;
}


/////////////// Write your implementation for Card below ///////////////

std::ostream& operator<<(std::ostream& os, const Card& card) {
    Rank card_rank = card.get_rank();
    Suit card_suit = card.get_suit();
    os << card_rank << " of " << card_suit;
    return os;
}

std::istream& operator>>(std::istream& is, Card& card) {
    string seperated;
    Rank card_rank;
    Suit card_suit;
    is >> card_rank >> seperated >> card_suit;
    card.rank = card_rank;
    card.suit = card_suit;
    return is;
}

bool operator<(const Card& lhs, const Card& rhs) {
    Rank lhs_rank = lhs.get_rank();
    Rank rhs_rank = rhs.get_rank();
    //compare rank
    if (lhs_rank < rhs_rank) {
        return true;
    }
    else if (lhs_rank == rhs_rank) {
        //if rank values are the same, then compare suit
        Suit lhs_suit = lhs.get_suit();
        Suit rhs_suit = rhs.get_suit();
        if (lhs_suit < rhs_suit) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

bool operator<=(const Card& lhs, const Card& rhs) {
    return ((lhs < rhs) || (lhs == rhs));
}

bool operator>(const Card& lhs, const Card& rhs) {
    return !(lhs <= rhs);
}

bool operator>=(const Card & lhs, const Card & rhs) {
    return !(lhs < rhs);
}

bool operator==(const Card& lhs, const Card& rhs) {
    Rank lhs_rank = lhs.get_rank();
    Rank rhs_rank = rhs.get_rank();
    Suit lhs_suit = lhs.get_suit();
    Suit rhs_suit = rhs.get_suit();
    if (lhs_rank == rhs_rank && lhs_suit == rhs_suit) {
        return true;
    }
    else {
        return false;
    }
}

bool operator!=(const Card& lhs, const Card& rhs) {
    return !(lhs == rhs);
}

Card::Card() {
    rank = TWO;
    suit = SPADES;
}

Card::Card(Rank rank_in, Suit suit_in) {
    rank = rank_in;
    suit = suit_in;
}

Rank Card::get_rank() const {
    return rank;
}

Suit Card::get_suit() const {
    return suit;
}

Suit Card::get_suit(Suit trump) const {
    if (is_left_bower(trump) || suit == trump) {
        return trump;
    }
    else {
        return suit;
    }
}

bool Card::is_face_or_ace() const {
    if (rank == JACK || rank == QUEEN || rank == KING || rank == ACE) {
        return true;
    }
    else {
        return false;
    }
}

bool Card::is_right_bower(Suit trump) const {
    if (suit == trump && rank == JACK) {
        return true;
    }
    else {
        return false;
    }
}

bool Card::is_left_bower(Suit trump) const {
    if (abs(suit - trump) == 2 && rank == 9) {
        return true;
    }
    else {
        return false;
    }
}

bool Card::is_trump(Suit trump) const {
    if (get_suit(trump) == trump) {
        return true;
    }
    else {
        return false;
    }
}

Suit Suit_next(Suit suit) {
    Suit next_suit;
    for (int s = SPADES; s <= DIAMONDS; s++) {
        if (s - suit == 2 || suit - s == 2) {
            next_suit = static_cast<Suit>(s);
        }
    }
    return next_suit;
}

bool Card_less(const Card& a, const Card& b, Suit trump) {
    if (a.get_suit(trump) == trump && b.get_suit(trump) == trump) {
        //both cards are bowers
        if (a.get_rank() == JACK && b.get_rank() == JACK) {
            //when a is the right-bower, return false
            if (a.is_right_bower(trump)) {
                return false;
            }
            // when a is the left-bower, return true
            else {
                return true;
            }
        }
        //a is left or right bower, and b is other trump card
        else if (a.get_rank() == JACK) {
            return false;
        }
        //b is left or right bower, and a is other trump card
        else if (b.get_rank() == JACK) {
            return true;
        }
        //both cards are trump cards but not bowers
        else {
            return a < b;
        }
    }
    //a is trump but not b
    else if (a.get_suit(trump) == trump) {
        return false;
    }
    //b is trump but not a
    else if (b.get_suit(trump) == trump) {
        return true;
    }
    //both are not trump cards
    else {
        return (a < b);
    }
}

bool Card_less(const Card& a, const Card& b, const Card& led_card, Suit trump) {
    Suit led = led_card.get_suit(trump);
    if (led == trump) {
        return(Card_less(a, b, trump));
    }
    else {
        //three situations: a being trump, led or normal cards
        if (a.get_suit(trump) == trump) {
            //when both cards are trump, just call the first card-less function
            if (b.get_suit(trump) == trump) {
                //becasue of jack, cannot use < directly
                return (Card_less(a, b, trump));
            }
            else {
                return false;
            }
        }
        // case of a being led
        else if (a.get_suit(trump) == led) {
            if (b.get_suit(trump) == trump) {
                return true;
            }
            else if (b.get_suit(trump) == led) {
                return (a < b);
            }
            else {
                return false;
            }
        }
        // case of a being normal cards
        else {
            if (b.get_suit(trump) == trump || b.get_suit(trump) == led) {
                return true;
            }
            else {
                return (a < b);
            }
        }
    }
}

// NOTE: We HIGHLY recommend you check out the operator overloading
// tutorial in the project spec before implementing
// the following operator overload functions:
//   operator<<
//   operator>>
//   operator<
//   operator<=
//   operator>
//   operator>=
//   operator==
//   operator!=
