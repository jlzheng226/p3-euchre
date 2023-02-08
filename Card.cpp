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

//EFFECTS Reads a Rank from a stream, for example "Two" -> TWO
std::istream & operator>>(std::istream &is, Rank &rank) {
  string str;
  is >> str;
  rank = string_to_rank(str);
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

//EFFECTS Reads a Suit from a stream, for example "Spades" -> SPADES
std::istream & operator>>(std::istream &is, Suit &suit) {
  string str;
  is >> str;
  suit = string_to_suit(str);
  return is;
}


/////////////// Write your implementation for Card below ///////////////


// NOTE: We HIGHLY recommend you check out the operator overloading
// tutorial in the project spec before implementing
// the following operator overload functions:
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
    return ((lhs< rhs) || (lhs == rhs));
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
    if (suit - trump == 2 || trump - suit == 2) {
        if (rank == 9) {
            return trump;
        }
    }
    return suit;
}

bool Card::is_face_or_ace() const {
    if (rank == 9 || rank == 10 || rank == 11 || rank == 12) {
        return true;
    }
    else {
        return false;
    }
}

bool Card::is_right_bower(Suit trump) const {
    if (suit == trump) {
        if (rank == 9) {
            return true;
        }
    }
    return false;
}

bool Card::is_left_bower(Suit trump) const {
    if (suit - trump == 2 || trump - suit == 2) {
        if (rank == 9) {
            return true;
        }
    }
    return false;
}

bool Card::is_trump(Suit trump) const {
    if (get_suit(trump) == trump) {
        return true;
    }
    return false;
}

Suit Suit_next(Suit suit) {
    for (int s = SPADES; s <= DIAMONDS; s++) {
        if (s - suit == 2 || suit - s == 2) {
            return static_cast<Suit>(s);
        }
    }
}

bool Card_less(const Card& a, const Card& b, Suit trump) {
    if (a.get_suit(trump) == trump && b.get_suit(trump) == trump) {
        //both cards are bowers
        if (a.get_rank() == 9 && b.get_rank() == 9) {
            //when a is the right-bower and b is the left-bower
            if (a.get_suit() == trump) {
                return false;
            }
            //a is the left-bower and b is the right-bower
            else {
                return true;
            }
        }
        //a is left or right bower, and b is other trump card
        else if (a.get_rank() == 9) {
            return false;
        }
        //b is left or right bower, and a is other trump card
        else if (b.get_rank() == 9) {
            return true;
        }
        //both cards are trump cards but not bowers
        else {
            return (a < b);
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