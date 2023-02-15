// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Player.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class SimplePlayer : public Player {
public:
    SimplePlayer() {
        name = "";
    }
    SimplePlayer(string name_in) {
        name = name_in;
    }
    virtual const std::string& get_name() const override {
        return name;
    }
    virtual void add_card(const Card& c) override {
        handCard.push_back(c);
    }

    virtual bool make_trump(const Card& upcard, bool is_dealer,
        int round, Suit& order_up_suit) const override {
        Suit upcard_suit = upcard.get_suit();
        // round 1
        if (round == 1) {
            // count the number of face or ace cards of the trump suit
            int numFaceAce = 0;
            for (auto hand : handCard) {
                if (hand.is_trump(upcard_suit) && hand.is_face_or_ace()) {
                    numFaceAce++;
                }
            }
            // simple player will order up if 
            // that would mean they have two 
            // or more cards that are either 
            // face or ace cards of the trump suit
            if (numFaceAce >= 2) {
                order_up_suit = upcard_suit;
                return true;
            }
            else {
                return false;
            }
        }

        // round 2
        else if (round == 2) {
            // If making reaches the dealer during the second round
            // the dealer will always order up the suit with the 
            // same color as the upcard
            if (is_dealer) {
                order_up_suit = Suit_next(upcard_suit);
                return true;
            }

            // count the number of face or ace cards of
            // the suit with the same color as the upcard
            int numFaceAce = 0;
            for (auto hand : handCard) {
                Suit next_suit = Suit_next(upcard_suit);
                if (hand.get_suit() == next_suit && hand.is_face_or_ace()) {
                    numFaceAce++;
                }
            }
            if (numFaceAce >= 1) {
                order_up_suit = Suit_next(upcard_suit);
                return true;
            }
            else {
                return false;
            }
        }
        return false;
    }

    virtual void add_and_discard(const Card& upcard) override {
        add_card(upcard);
        Card lowest = handCard[0];
        int lowest_index = 0;
        for (auto card : handCard) {
            if (card < lowest) {
                lowest = card;
                // track the index of the lowest card
                lowest_index++;
            }
        }
        handCard.erase(handCard.begin() + lowest_index);
    }

    virtual Card lead_card(Suit trump) override {
        Card largestCard = handCard[0];
        int largest_index = 0;
        bool allTrump = true;
        for (auto card : handCard) {
            // When a Simple Player leads a trick, 
            // they play the highest non-trump card in their hand.
            if (card.is_trump(trump) == false && card > largestCard) {
                // update the largest non-trump card
                largestCard = card;
                // track the index of the largest card
                largest_index++;
                // track if the player have cards other than trump cards
                allTrump = false;
            }
        }
        // If they have only trump cards, 
        // they play the highest trump card in their hand.
        if (allTrump) {
            largestCard = handCard[0];
            largest_index = 0;
            for (auto card : handCard) {
                if (card > largestCard) {
                    largestCard = card;
                    // track the index of the largest card
                    largest_index++;
                }
            }
        }
        // erase the card from the vector
        handCard.erase(handCard.begin() + largest_index);
        return largestCard;
    }

    virtual Card play_card(const Card& led_card, Suit trump) override {
        Suit ledSuit = led_card.get_suit(trump);
        Card largestSuitCard = handCard[0];
        int largestSuit_index = 0;
        bool noFollowSuit = true;
        for (auto card : handCard) {
            if (card.get_suit(trump) == ledSuit && card > largestSuitCard) {
                // update largest follow suit
                largestSuitCard = card;
                // track the largest follow suit index
                largestSuit_index++;
                // track if the player can follow the suit
                noFollowSuit = false;
            }
        }
        // if the player does not have cards that follow suit,
        // they play the lowest card in their hand
        if (noFollowSuit) {
            Card lowestCard = handCard[0];
            int lowest_index = 0;
            for (auto card : handCard) {
                if (card < lowestCard) {
                    lowestCard = card;
                    lowest_index++;
                }
            }
            handCard.erase(handCard.begin() + lowest_index);
            return lowestCard;
        }
        else {
            handCard.erase(handCard.begin() + largestSuit_index);
            return largestSuitCard;
        }
    }


private:
    string name;
    vector<Card>handCard;
};


class HumanPlayer : public Player {
public:
    HumanPlayer() {
        name = "";
    }
    HumanPlayer(string name_in) {
        name = name_in;
    }

    virtual const std::string& get_name() const override {
        return name;
    }

    virtual void add_card(const Card& c) override {
        handCard.push_back(c);
    }

  /*  virtual bool make_trump(const Card& upcard, bool is_dealer,
        int round, Suit& order_up_suit) const override {
        assert(false);
    }

    virtual void add_and_discard(const Card& upcard) override {
        assert(false);
    }

    virtual Card lead_card(Suit trump) override {
        assert(false);
    }

    virtual Card play_card(const Card& led_card, Suit trump) override {
        assert(false);
    }
*/
private:
    string name;
    vector<Card>handCard;
};

Player* Player_factory(const std::string& name,
    const std::string& strategy) {
    // We need to check the value of strategy and return 
    // the corresponding player type.
    if (strategy == "Simple") {
        // The "new" keyword dynamically allocates an object.
        return new SimplePlayer(name);
    }
    /*else if (strategy == "Human") {
        return new HumanPlayer(name);
    }*/

    // Invalid strategy if we get here
    return nullptr;
}

std::ostream& operator<<(std::ostream& os, const Player& p) {
    return os << p.get_name();
}