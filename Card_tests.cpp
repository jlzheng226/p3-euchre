// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Card.h"
#include "unit_test_framework.h"
#include <iostream>

using namespace std;


TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}

TEST(test_ostream_operator) {
    ostringstream os1;
    ostringstream os2;
    ostringstream os3;

    Card jack_diamonds = Card(JACK, DIAMONDS);
    Card nine_hearts = Card(NINE, HEARTS);
    Card ace_clubs = Card(ACE, CLUBS);

    os1 << jack_diamonds;
    os2 << nine_hearts;
    os3 << ace_clubs;

    ASSERT_EQUAL(os1.str(), "Jack of Diamonds");
    ASSERT_EQUAL(os2.str(), "Nine of Hearts");
    ASSERT_EQUAL(os3.str(), "Ace of Clubs");
}

TEST(test_istream_operator) {
    istringstream iss("Jack of Spades");
    Card c;
    iss >> c;
    ASSERT_EQUAL(c, Card(JACK, SPADES));
}

TEST(test_operator_comparison) {
    Card jack_diamonds = Card(JACK, DIAMONDS);
    Card jack_hearts = Card(JACK, HEARTS);
    Card nine_hearts = Card(NINE, HEARTS);

    ASSERT_TRUE(jack_hearts < jack_diamonds);
    ASSERT_TRUE(jack_hearts <= jack_diamonds);
    ASSERT_TRUE(jack_diamonds > jack_hearts);
    ASSERT_TRUE(jack_diamonds >= jack_hearts);
    ASSERT_TRUE(nine_hearts < jack_diamonds);
    ASSERT_TRUE(jack_diamonds == jack_diamonds);
    ASSERT_TRUE(jack_hearts != jack_diamonds);
}

TEST(test_get_suit_trump) {
    // test trump card
    Card jack_diamonds = Card(JACK, DIAMONDS);
    Card jack_hearts = Card(JACK, HEARTS);
    Card nine_hearts = Card(NINE, HEARTS);
    Suit trump = HEARTS;
    ASSERT_EQUAL(jack_diamonds.get_suit(trump), trump);
    ASSERT_EQUAL(jack_hearts.get_suit(trump), trump);
    ASSERT_EQUAL(nine_hearts.get_suit(trump), trump);

    // test normal card
    Card ace_diamonds = Card(ACE, DIAMONDS);
    ASSERT_EQUAL(ace_diamonds.get_suit(trump), DIAMONDS);
}

TEST(test_is_face_or_ace) {
    Card nine_hearts = Card(NINE, HEARTS);
    Card jack_hearts = Card(JACK, HEARTS);
    Card queen_hearts = Card(JACK, HEARTS);
    Card king_hearts = Card(JACK, HEARTS);
    Card ace_hearts = Card(JACK, HEARTS);

    ASSERT_FALSE(nine_hearts.is_face_or_ace());
    ASSERT_TRUE(jack_hearts.is_face_or_ace());
    ASSERT_TRUE(queen_hearts.is_face_or_ace());
    ASSERT_TRUE(king_hearts.is_face_or_ace());
    ASSERT_TRUE(ace_hearts.is_face_or_ace());
}

TEST(test_is_left_or_right_bower) {
    Card jack_diamonds = Card(JACK, DIAMONDS);
    Card jack_hearts = Card(JACK, HEARTS);
    Suit trump = HEARTS;
    ASSERT_TRUE(jack_diamonds.is_left_bower(trump));
    ASSERT_TRUE(jack_hearts.is_right_bower(trump));
}

TEST(test_is_trump) {
    Card jack_diamonds = Card(JACK, DIAMONDS);
    Card jack_hearts = Card(JACK, HEARTS);
    Suit trump = HEARTS;
    ASSERT_TRUE(jack_diamonds.is_trump(trump));
    ASSERT_TRUE(jack_hearts.is_trump(trump));
}

TEST(test_suit_next) {
    ASSERT_EQUAL(Suit_next(CLUBS), SPADES);
    ASSERT_EQUAL(Suit_next(SPADES), CLUBS);
    ASSERT_EQUAL(Suit_next(HEARTS), DIAMONDS);
    ASSERT_EQUAL(Suit_next(DIAMONDS), HEARTS);
}

TEST(test_right_bowel_value) {
    Suit trump_suit = HEARTS;
    Card led_card = Card(KING, CLUBS);

    // test right and left bower
    Card left_bower = Card(JACK, DIAMONDS);
    Card right_bower = Card(JACK, HEARTS);
    ASSERT_TRUE(Card_less(left_bower, right_bower, trump_suit));

    // test right bower with other trumps
    Card ace_hearts = Card(ACE, HEARTS);
    Card ten_hearts = Card(TEN, HEARTS);
    ASSERT_TRUE(Card_less(ace_hearts, right_bower, trump_suit));
    ASSERT_FALSE(Card_less(right_bower, ten_hearts, trump_suit));

    // test right bower with led card and normal card
    Card ace_clubs = Card(ACE, CLUBS);
    Card ten_diamonds = Card(TEN, DIAMONDS);
    ASSERT_TRUE(Card_less(ace_clubs, right_bower, led_card, trump_suit));
    ASSERT_TRUE(Card_less(ten_diamonds, right_bower, led_card, trump_suit));
}

TEST(test_left_bowel_value) {
    Suit trump_suit = HEARTS;
    Card led_card = Card(KING, CLUBS);
    Card left_bower = Card(JACK, DIAMONDS);
    Card right_bower = Card(JACK, HEARTS);

    // test right and left bower
    ASSERT_FALSE(Card_less(right_bower, left_bower, trump_suit));

    // test left bower with other trumps
    Card ace_hearts = Card(ACE, HEARTS);
    Card ten_hearts = Card(TEN, HEARTS);
    ASSERT_TRUE(Card_less(ace_hearts, left_bower, trump_suit));
    ASSERT_FALSE(Card_less(left_bower, ten_hearts, trump_suit));

    // test left bower with led card and normal card
    Card ace_clubs = Card(ACE, CLUBS);
    Card ten_diamonds = Card(TEN, DIAMONDS);
    ASSERT_TRUE(Card_less(ace_clubs, left_bower, led_card, trump_suit));
    ASSERT_TRUE(Card_less(ten_diamonds, left_bower, led_card, trump_suit));
}

TEST(test_trump_value) {
    Suit trump_suit = HEARTS;
    Card led_card = Card(KING, CLUBS);
    Card left_bower = Card(JACK, DIAMONDS);
    Card right_bower = Card(JACK, HEARTS);

    // test normal trump with right and left bower
    Card ace_hearts = Card(ACE, HEARTS);
    ASSERT_TRUE(Card_less(ace_hearts, right_bower, trump_suit));
    ASSERT_TRUE(Card_less(ace_hearts, left_bower, trump_suit));

    // test normal trump with led suit card
    Card nine_hearts = Card(NINE, HEARTS);
    Card ace_clubs = Card(ACE, CLUBS);
    ASSERT_TRUE(Card_less(ace_clubs, ace_hearts, led_card, trump_suit));
    ASSERT_FALSE(Card_less(nine_hearts, ace_clubs, led_card, trump_suit));

    // test normal trump with normal card
    Card ace_diamonds = Card(ACE, DIAMONDS);
    ASSERT_TRUE(Card_less(ace_diamonds, ace_hearts, trump_suit));
    ASSERT_TRUE(Card_less(ace_diamonds, nine_hearts, trump_suit));
}


TEST(test_led_suit_card_value) {
    Suit trump_suit = HEARTS;
    Card left_bower = Card(JACK, DIAMONDS);
    Card right_bower = Card(JACK, HEARTS);

    // ** Case 1: led suit is same as trump suit
    Card led_card1 = Card(KING, HEARTS);

    // test led suit card with bowers
    Card ace_hearts = Card(ACE, HEARTS);
    ASSERT_TRUE(Card_less(ace_hearts, right_bower, led_card1, trump_suit));
    ASSERT_TRUE(Card_less(ace_hearts, left_bower, led_card1, trump_suit));

    // test led suit card with led suit card
    ASSERT_FALSE(Card_less(ace_hearts, led_card1, led_card1, trump_suit));

    // test led suit card with normal card
    Card ace_diamonds = Card(ACE, DIAMONDS);
    Card nine_hearts = Card(NINE, HEARTS);
    ASSERT_TRUE(Card_less(ace_diamonds, nine_hearts, led_card1, trump_suit));

    // ** Case 2: led suit is different from trump suit
    Card led_card2 = Card(KING, CLUBS);
    Card ace_clubs = Card(ACE, CLUBS);
    Card nine_clubs = Card(QUEEN,CLUBS);

    // test led suit card with led suit card
    ASSERT_FALSE(Card_less(ace_clubs, led_card2, led_card2, trump_suit));
    ASSERT_TRUE(Card_less(nine_clubs, led_card2, led_card2, trump_suit));

    // test led suit card with normal card
    ASSERT_TRUE(Card_less(ace_diamonds, nine_hearts, led_card2, trump_suit));
}

TEST(test_normal_card_value) {
    Suit trump_suit = SPADES;
    Card ace_clubs = Card(ACE, CLUBS);
    Card ace_diamonds = Card(ACE, DIAMONDS);
    Card ten_clubs = Card(TEN, CLUBS);
    Card ten_hearts = Card(TEN, HEARTS);

    // test normal cards with different rank
    ASSERT_TRUE(Card_less(ten_hearts, ace_clubs, trump_suit));
    ASSERT_TRUE(Card_less(ten_clubs, ace_clubs, trump_suit));

    // test normal cards with same rank
    ASSERT_TRUE(Card_less(ace_clubs, ace_diamonds, trump_suit));
    ASSERT_TRUE(Card_less(ten_hearts, ten_clubs, trump_suit));
}





// Add more test cases here

TEST_MAIN()
