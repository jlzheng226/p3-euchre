// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Player.h"
#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class SimplePlayer : public Player {
public:
    SimplePlayer(string name_in) {
        name = name_in;
    }
    virtual const std::string& get_name() const override {
        return name;
    }
    virtual void add_card(const Card& c) override {
        assert(static_cast<int>(handCard.size()) < MAX_HAND_SIZE);
        handCard.push_back(c);
        sort(handCard.begin(), handCard.end());
    }

    virtual bool make_trump(const Card& upcard, bool is_dealer,
        int round, Suit& order_up_suit) const override {
        assert(round == 1 || round == 2);

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
        assert(static_cast<int>(handCard.size()) >= 1);
        handCard.push_back(upcard);
        Suit trump = upcard.get_suit();
        Card lowest = handCard[0];
        int lowest_index = 0;
        for (size_t card = 1; card < handCard.size(); card++) {
            if (Card_less(handCard[card], lowest, trump)) {
                lowest = handCard[card];
                // update the index of the lowest card
                lowest_index = card;
            }
        }
        handCard.erase(handCard.begin() + lowest_index);
        cout << endl;
    }

    virtual Card lead_card(Suit trump) override {
        assert(static_cast<int>(handCard.size()) >= 1);
        Card largestCard = handCard[0];
        int largest_index = 0;
        for (size_t index = 0; index < handCard.size(); index++) {
            // When a Simple Player leads a trick, 
            // they play the highest non-trump card in their hand.
            if (handCard[index].is_trump(trump) == false) {
                largestCard = handCard[index];
                largest_index = index;
                for (size_t remain_index = index + 1;
                    remain_index < handCard.size();
                    remain_index++) {
                    if (handCard[remain_index].is_trump(trump) == false
                        && handCard[remain_index] > largestCard) {
                        // update the largest non-trump card
                        largestCard = handCard[remain_index];
                        // update the index of the largest card
                        largest_index = remain_index;
                    }
                }
                // erase the card from the vector
                handCard.erase(handCard.begin() + largest_index);
                return largestCard;
            }
        }
        // If they have only trump cards, 
        // they play the highest trump card in their hand.
        for (size_t index = 1; index < handCard.size(); index++) {
            if (Card_less(largestCard, handCard[index], trump)) {
                largestCard = handCard[index];
                // track the index of the largest card
                largest_index = index;
            }
        }
        // erase the card from the vector
        handCard.erase(handCard.begin() + largest_index);
        return largestCard;
    }

    virtual Card play_card(const Card& led_card, Suit trump) override {
        assert(static_cast<int>(handCard.size()) >= 1);
        Suit ledSuit = led_card.get_suit(trump);
        Card largestSuitCard = handCard[0];
        int largestSuit_index = 0;

        // if a simple player can follow suit
        // they play the highest card that follows suit
        for (size_t index = 0; index < handCard.size(); index++) {
            // first, find the first same suit card
            if (handCard[index].get_suit(trump) == ledSuit) {
                largestSuitCard = handCard[index];
                largestSuit_index = index;
                // find the highest card that follows suit
                for (size_t remain_index = index + 1;
                    remain_index < handCard.size();
                    remain_index++) {
                    if (handCard[remain_index].get_suit(trump) == ledSuit &&
                        Card_less(largestSuitCard, handCard[remain_index],
                            led_card, trump)) {
                        // update largest follow suit card
                        largestSuitCard = handCard[remain_index];
                        // update the largest follow suit index
                        largestSuit_index = remain_index;
                    }
                }
                handCard.erase(handCard.begin() + largestSuit_index);
                return largestSuitCard;
            }
        }

        // if the player does not have cards that follow suit,
        // they play the lowest card in their hand
        Card lowestCard = handCard[0];
        int lowest_index = 0;
        for (size_t index = 1; index < handCard.size(); index++) {
            if (Card_less(handCard[index], lowestCard, trump)) {
                lowestCard = handCard[index];
                lowest_index = index;
            }
        }
        handCard.erase(handCard.begin() + lowest_index);
        return lowestCard;
    }


private:
    string name;
    vector<Card>handCard;
};


class HumanPlayer : public Player {
public:
    HumanPlayer(string name_in) {
        name = name_in;
    }

    virtual const std::string& get_name() const override {
        return name;
    }

    virtual void add_card(const Card& c) override {
        assert(static_cast<int>(handCard.size()) < MAX_HAND_SIZE);
        handCard.push_back(c);
        sort(handCard.begin(), handCard.end());
    }

    virtual bool make_trump(const Card& upcard, bool is_dealer,
        int round, Suit& order_up_suit) const override {
        assert(round == 1 || round == 2);
        // print the player's hand card
        print_hand();
        cout << "Human player " << name << ", please enter a suit, or \"pass\":\n";

        // ask the human player for decision
        string decision;
        cin >> decision;
        if (decision != "pass") {
            order_up_suit = string_to_suit(decision);
            return true;
        }
        else {
            return false;
        }
    }

    virtual void add_and_discard(const Card& upcard) override {
        assert(static_cast<int>(handCard.size()) >= 1);
        print_hand();
        cout << "Discard upcard: [-1]\n";
        cout << "Human player " << name << ", please select a card to discard:\n";

        int decision;
        cin >> decision;
        if (decision == -1) {
            return;
        }
        handCard.erase(handCard.begin() + decision);
        handCard.push_back(upcard);
        sort(handCard.begin(), handCard.end());
    }

    virtual Card lead_card(Suit trump) override {
        assert(static_cast<int>(handCard.size()) >= 1);

        print_hand();
        cout << "Human player " << name << ", please select a card:\n";
        int decision;
        cin >> decision;
        Card played = handCard[decision];
        handCard.erase(handCard.begin() + decision);
        return played;
    }

    virtual Card play_card(const Card& led_card, Suit trump) override {
        assert(static_cast<int>(handCard.size()) >= 1);
        print_hand();
        cout << "Human player " << name << ", please select a card:\n";
        int decision;
        cin >> decision;
        Card played = handCard[decision];
        handCard.erase(handCard.begin() + decision);
        return played;
    }

private:
    string name;
    vector<Card>handCard;

    void print_hand() const {
        for (size_t i = 0; i < handCard.size(); ++i) {
            cout << "Human player " << name << "'s hand: "
                << "[" << i << "] " << handCard[i] << "\n";
        }
    }

};

Player* Player_factory(const std::string& name,
    const std::string& strategy) {
    // We need to check the value of strategy and return 
    // the corresponding player type.
    if (strategy == "Simple") {
        // The "new" keyword dynamically allocates an object.
        return new SimplePlayer(name);
    }
    else if (strategy == "Human") {
        return new HumanPlayer(name);
    }

    // Invalid strategy if we get here
    return nullptr;
}

std::ostream& operator<<(std::ostream& os, const Player& p) {
    return os << p.get_name();
}