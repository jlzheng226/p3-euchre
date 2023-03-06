// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Player.h"
#include "Card.h"
#include "unit_test_framework.h"

#include <iostream>

using namespace std;

TEST(test_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
}

TEST(test_make_trump){
    Player* alice = Player_factory("Alice", "Simple");
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
    Suit order_up_a = SPADES;
    Suit order_up_b = SPADES;
    Suit order_up_d = SPADES;

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

    //Alice has two face-or-Ace trumps for the first round, one is left bower
    bool make_trump_1 = (*alice).make_trump(upcard, false, round_one, order_up_a);
    ASSERT_EQUAL(make_trump_1, true);
    ASSERT_EQUAL(order_up_a, DIAMONDS);
    //whether Alice is dealer or not, she shall always make trump
    bool make_trump_2 = (*alice).make_trump(upcard, true, round_one, order_up_a);
    ASSERT_EQUAL(make_trump_2, true);
    ASSERT_EQUAL(order_up_a, DIAMONDS);
    //Bob has only one face-or-Ace trump for first round, so return false, order up doesn't change
    bool make_trump_3 = (*bob).make_trump(upcard, false, round_one, order_up_b);
    ASSERT_EQUAL(make_trump_3, false);
    ASSERT_EQUAL(order_up_b, SPADES);
    //for the second round, Bob has enough face-or-ace trump, so return true, order up changes
    bool make_trump_4 = (*bob).make_trump(upcard, false, round_two, order_up_b);
    ASSERT_EQUAL(make_trump_4, true);
    ASSERT_EQUAL(order_up_b, HEARTS);
    //for the second round, Dan is dealer, and even if he does't have enough HEARTS, he has to accept the upcard
    bool make_trump_5 = (*Dan).make_trump(upcard, true, round_two, order_up_d);
    ASSERT_EQUAL(make_trump_5, true);
    ASSERT_EQUAL(order_up_d, HEARTS);
    delete alice;
    delete bob;
    delete Dan;
}

TEST(test_add_and_discard_lead_card){
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
    //after add and discard, Alice has four trumps and one non-trump

    //she first lead the largest non-trump, then the largest trump...
    Card lead_card_a = (*alice).lead_card(trump);
    ASSERT_EQUAL(lead_card_a, ace_hearts);
    lead_card_a = (*alice).lead_card(trump);
    ASSERT_EQUAL(lead_card_a, jack_hearts);
    lead_card_a = (*alice).lead_card(trump);
    ASSERT_EQUAL(lead_card_a, king_diamonds);
    lead_card_a = (*alice).lead_card(trump);
    ASSERT_EQUAL(lead_card_a, upcard_a);
    lead_card_a = (*alice).lead_card(trump);
    ASSERT_EQUAL(lead_card_a, nine_diamonds);
    delete alice;

    (*bob).add_card(ten_diamonds);
    (*bob).add_card(king_diamonds);
    (*bob).add_card(jack_hearts);
    (*bob).add_card(queen_diamonds);
    (*bob).add_card(nine_diamonds);
    (*bob).add_and_discard(upcard_b);
    //Bob has five trumps before add and discard, so after it he still has five trumps

    //starting from jack_hearts, he leads the largest trump
    Card lead_card_b = (*bob).lead_card(trump);
    ASSERT_EQUAL(lead_card_b, jack_hearts);
    lead_card_b = (*bob).lead_card(trump);
    ASSERT_EQUAL(lead_card_b, upcard_b);
    lead_card_b = (*bob).lead_card(trump);
    ASSERT_EQUAL(lead_card_b, king_diamonds);
    lead_card_b = (*bob).lead_card(trump);
    ASSERT_EQUAL(lead_card_b, queen_diamonds);
    lead_card_b = (*bob).lead_card(trump);
    ASSERT_EQUAL(lead_card_b, ten_diamonds);
    delete bob;
}

TEST(test_play_card){
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

    //led card is trump, so Alice plays the largest trump
    Card play_card = (*alice).play_card(led_card_a, trump);
    ASSERT_EQUAL(play_card, jack_hearts);
    play_card = (*alice).play_card(led_card_a, trump);
    ASSERT_EQUAL(play_card, king_diamonds);
    play_card = (*alice).play_card(led_card_a, trump);
    ASSERT_EQUAL(play_card, nine_diamonds);
    play_card = (*alice).play_card(led_card_a, trump);
    //she cannot follow suit now, so play the lowest card
    ASSERT_EQUAL(play_card, ten_spades);
    play_card = (*alice).play_card(led_card_a, trump);
    ASSERT_EQUAL(play_card, king_clubs);
    delete alice;

    //follow the suit, which is spades
    play_card = (*bob).play_card(led_card_b, trump);
    ASSERT_EQUAL(play_card, ten_spades);
    //does not have spades, so play the lowest card
    play_card = (*bob).play_card(led_card_b, trump);
    ASSERT_EQUAL(play_card, king_clubs);
    play_card = (*bob).play_card(led_card_b, trump);
    ASSERT_EQUAL(play_card, nine_diamonds);
    play_card = (*bob).play_card(led_card_b, trump);
    ASSERT_EQUAL(play_card, king_diamonds);
    play_card = (*bob).play_card(led_card_b, trump);
    ASSERT_EQUAL(play_card, jack_hearts);
    delete bob;
    
}

TEST(test_lead_card) {
    Player* alice = Player_factory("Alice", "Simple");
    const Card ace_hearts = Card(ACE, HEARTS);
    const Card king_hearts = Card(KING, HEARTS);
    const Card jack_spades = Card(JACK, SPADES);
    const Card ten_spades = Card(TEN, SPADES);
    const Card nine_spades = Card(NINE, SPADES);
    Suit trump = HEARTS;

    alice->add_card(jack_spades);
    alice->add_card(nine_spades);
    alice->add_card(ten_spades);
    alice->add_card(ace_hearts);
    alice->add_card(king_hearts);

    Card play_card = alice->lead_card(trump);
    ASSERT_EQUAL(play_card, jack_spades);
    play_card = alice->play_card(Card(KING, CLUBS), trump);
    ASSERT_EQUAL(play_card, nine_spades);
    play_card = alice->play_card(Card(TEN, DIAMONDS), trump);
    ASSERT_EQUAL(play_card, ten_spades);
    play_card = alice->play_card(Card(JACK, HEARTS), trump);
    ASSERT_EQUAL(play_card, ace_hearts);
    play_card = alice->play_card(Card(QUEEN, HEARTS), trump);
    ASSERT_EQUAL(play_card, king_hearts);

    delete alice;
}


TEST_MAIN()
