// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Player.h"
#include "Card.h"
#include <sstream>
#include "unit_test_framework.h"

#include <iostream>

using namespace std;

TEST(test_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
}


TEST(test_player_make_trump){
    Player * alice = Player_factory("Alice", "Simple");
    Player* bob = Player_factory("Bob", "Simple");
    Player* Dan = Player_factory("Dan", "Simple");
    const int round_one = 1;
    const int round_two = 2;
    const Card nine_diamonds = Card(NINE, DIAMONDS);
    const Card king_diamonds = Card(KING, DIAMONDS);
    const Card jack_hearts = Card(JACK, HEARTS);
    const Card ten_spades = Card(TEN, SPADES);
    const Card ace_hearts = Card(ACE, HEARTS);
    const Card nine_clubs = Card(NINE, CLUBS);
    const Card upcard = Card(TEN, DIAMONDS);
    Suit order_up;
    (*alice).add_card(nine_diamonds);
    (*alice).add_card(king_diamonds);
    (*alice).add_card(jack_hearts);
    (*alice).add_card(ten_spades);
    (*alice).add_card(ace_hearts);
    (*bob).add_card(nine_diamonds);
    (*bob).add_card(king_diamonds);
    (*bob).add_card(ace_hearts);
    (*bob).add_card(ten_spades);
    (*Dan).add_card(nine_diamonds);
    (*Dan).add_card(king_diamonds);
    (*Dan).add_card(ten_spades);
    (*Dan).add_card(nine_clubs);
    bool make_trump_1 = (*alice).make_trump(upcard, false, round_one, order_up);
    ASSERT_EQUAL(make_trump_1, true);
    ASSERT_EQUAL(order_up, DIAMONDS);
    bool make_trump_2 = (*alice).make_trump(upcard, true, round_one, order_up);
    ASSERT_EQUAL(make_trump_2, true);
    ASSERT_EQUAL(order_up, DIAMONDS);
    bool make_trump_3 = (*bob).make_trump(upcard, false, round_one, order_up);
    ASSERT_EQUAL(make_trump_3, false);
    ASSERT_EQUAL(order_up, SPADES);
    bool make_trump_4 = (*bob).make_trump(upcard, false, round_two, order_up);
    ASSERT_EQUAL(make_trump_4, true);
    ASSERT_EQUAL(order_up, HEARTS);
    bool make_trump_5 = (*Dan).make_trump(upcard, true, round_two, order_up);
    ASSERT_EQUAL(make_trump_5, true);
    ASSERT_EQUAL(order_up, HEARTS);
    delete alice;
    delete bob;
    delete Dan;
}

TEST(add_and_discard_lead_card){
    Player* alice = Player_factory("Alice", "Simple");
    Player* bob = Player_factory("Bob", "Simple");

    const Card nine_diamonds = Card(NINE, DIAMONDS);
    const Card king_diamonds = Card(KING, DIAMONDS);
    const Card jack_hearts = Card(JACK, HEARTS);
    const Card ten_spades = Card(TEN, SPADES);
    const Card ace_hearts = Card(ACE, HEARTS);
    const Card ten_diamonds = Card(TEN, DIAMONDS);
    const Card queen_diamonds = Card(QUEEN, DIAMONDS);
    Card upcard_a = Card(TEN, DIAMONDS);
    Card upcard_b = Card(ACE, DIAMONDS);
    Suit trump = DIAMONDS;

    (*alice).add_card(nine_diamonds);
    (*alice).add_card(king_diamonds);
    (*alice).add_card(jack_hearts);
    (*alice).add_card(ten_spades);
    (*alice).add_card(ace_hearts);
    (*alice).add_and_discard(upcard_a);
    ASSERT_EQUAL((*alice).lead_card(trump), ace_hearts);
    ASSERT_EQUAL((*alice).lead_card(trump), jack_hearts);
    ASSERT_EQUAL((*alice).lead_card(trump), king_diamonds);
    ASSERT_EQUAL((*alice).lead_card(trump), upcard_a);
    ASSERT_EQUAL((*alice).lead_card(trump), nine_diamonds);
    delete alice;

    (*bob).add_card(ten_diamonds);
    (*bob).add_card(king_diamonds);
    (*bob).add_card(jack_hearts);
    (*bob).add_card(queen_diamonds);
    (*bob).add_card(nine_diamonds);
    (*bob).add_and_discard(upcard_b);
    ASSERT_EQUAL((*bob).lead_card(trump), jack_hearts);
    ASSERT_EQUAL((*bob).lead_card(trump), upcard_b);
    ASSERT_EQUAL((*bob).lead_card(trump), king_diamonds);
    ASSERT_EQUAL((*bob).lead_card(trump), queen_diamonds);
    ASSERT_EQUAL((*bob).lead_card(trump), ten_diamonds);
    delete bob;
}

TEST(play_cards){
    Player* alice = Player_factory("Alice", "Simple");
    Player* bob = Player_factory("Bob", "Simple");
    const Card nine_diamonds = Card(NINE, DIAMONDS);
    const Card king_diamonds = Card(KING, DIAMONDS);
    const Card jack_hearts = Card(JACK, HEARTS);
    const Card ten_spades = Card(TEN, SPADES);
    const Card king_clubs = Card(KING, CLUBS);

    Card led_card_a = Card(TEN, DIAMONDS);
    Card led_card_b = Card(KING, SPADES);
    Suit trump = DIAMONDS;

    (*alice).add_card(nine_diamonds);
    (*alice).add_card(king_diamonds);
    (*alice).add_card(jack_hearts);
    (*alice).add_card(ten_spades);
    (*alice).add_card(king_clubs);

    (*bob).add_card(nine_diamonds);
    (*bob).add_card(king_diamonds);
    (*bob).add_card(jack_hearts);
    (*bob).add_card(ten_spades);
    (*bob).add_card(king_clubs);

    Card play_card = (*alice).play_card(led_card_a, trump);
    ASSERT_EQUAL(play_card, jack_hearts);
    play_card = (*alice).play_card(led_card_a, trump);
    ASSERT_EQUAL(play_card, king_diamonds);
    play_card = (*alice).play_card(led_card_a, trump);
    ASSERT_EQUAL(play_card, nine_diamonds);
    play_card = (*alice).play_card(led_card_a, trump);
    ASSERT_EQUAL(play_card, ten_spades);
    play_card = (*alice).play_card(led_card_a, trump);
    ASSERT_EQUAL(play_card, king_clubs);
    delete alice;

    play_card = (*bob).play_card(led_card_b, trump);
    ASSERT_EQUAL(play_card, ten_spades);
    ASSERT_EQUAL(play_card, king_clubs);
    ASSERT_EQUAL(play_card, nine_diamonds);
    ASSERT_EQUAL(play_card, king_diamonds);
    ASSERT_EQUAL(play_card, jack_hearts);
    delete bob;
    
}




TEST_MAIN()
