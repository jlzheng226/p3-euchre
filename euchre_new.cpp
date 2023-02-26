// Project UID 1d9f47bfc76643019cfbf037641defe1

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include "Card.h"
#include "Pack.h"
#include "Player.h"


using namespace std;

class Game {
public:
    Game(vector<Player*> players, Pack cards, string shuffle) {
        euchre_players = players;
        pack = cards;
        shuffle_dec = shuffle;
    }

    void play() {
        shuffle();
        deal();
        make_trump();
        play_hand();
        hands++;
    }

    int get_points_team1() {
        return (points_t1);
    }

    int get_points_team2() {
        return (points_t2);
    }

    string get_player(int index) {
        return (*euchre_players[index]).get_name();
    }

private:
    vector<Player*> euchre_players;
    string shuffle_dec;
    int points_to_win;
    Pack pack;
    int hands = 0;
    int tricks_per_player[4] = { 0,0,0,0 };
    int dealer_index = 0;
    int order_index;
    int points_t1 = 0;
    int points_t2 = 0;
    Card upcard;
    Suit trump;

    void shuffle() {
        cout << "Hand " << hands << endl;
        if (shuffle_dec == "noshuffle") {
            pack.reset();
        }
        else {
            pack.shuffle();
        }
    }

    void deal() {
        dealer_index = hands % 4;
        cout << (*euchre_players[dealer_index]).get_name() << " deals" << endl;
        for (int i = 0; i < 3; i++) {
            (*euchre_players[(dealer_index + 1) % 4]).add_card(pack.deal_one());
        }
        for (int i = 0; i < 2; i++) {
            (*euchre_players[(dealer_index + 2) % 4]).add_card(pack.deal_one());
        }
        for (int i = 0; i < 3; i++) {
            (*euchre_players[(dealer_index + 3) % 4]).add_card(pack.deal_one());
        }
        for (int i = 0; i < 2; i++) {
            (*euchre_players[dealer_index]).add_card(pack.deal_one());
        }
        for (int i = 0; i < 2; i++) {
            (*euchre_players[(dealer_index + 1) % 4]).add_card(pack.deal_one());
        }
        for (int i = 0; i < 3; i++) {
            (*euchre_players[(dealer_index + 2) % 4]).add_card(pack.deal_one());
        }
        for (int i = 0; i < 2; i++) {
            (*euchre_players[(dealer_index + 3) % 4]).add_card(pack.deal_one());
        }
        for (int i = 0; i < 3; i++) {
            (*euchre_players[dealer_index]).add_card(pack.deal_one());
        }
        upcard = pack.deal_one();
    }

    void make_trump() {

        cout << upcard.get_rank() << " of " << upcard.get_suit() << " turned up" << "\n";
        bool make_trump;
        bool is_dealer;

        for (int round = 1; round < 3; round++) {
            for (int player_num = 1; player_num < 5; player_num++) {
                is_dealer = (((dealer_index + player_num) % 4) == dealer_index);
                make_trump = (*euchre_players[(dealer_index + player_num) % 4]).make_trump(upcard, is_dealer, round, trump);
                if (make_trump == true && round == 1) {
                    cout << (*euchre_players[(dealer_index + player_num) % 4]).get_name()
                        << " orders up " << trump << endl;
                    (*euchre_players[dealer_index]).add_and_discard(upcard);
                    cout << endl;
                    order_index = (dealer_index + player_num) % 4; //this is the player that orders up
                    return;
                }
                else if (make_trump == false) {
                    cout << (*euchre_players[(dealer_index + player_num) % 4]).get_name()
                        << " passes" << endl;
                }
                else if (make_trump == true && round == 2) {
                    cout << (*euchre_players[(dealer_index + player_num) % 4]).get_name()
                        << " orders up " << trump << endl;
                    cout << endl;
                    order_index = (dealer_index + player_num) % 4;
                    return;
                }
            }

        }
    }

    void play_hand() {
        int lead_index = (dealer_index + 1) % 4;
        int biggest_index = lead_index;
        int current_index;
        Card lead_card;
        Card biggest_card;
        Card follow_card;

        for (int trick = 1; trick < 6; trick++) {
            lead_card = (*euchre_players[lead_index]).lead_card(trump);
            biggest_card = lead_card;
            cout << lead_card << " led by " << (*euchre_players[lead_index]).get_name() << endl;
            for (int player = 1; player < 4; player++) {
                current_index = (lead_index + player) % 4;
                follow_card = (*euchre_players[current_index]).play_card(lead_card, trump);
                cout << follow_card << " played by " << (*euchre_players[current_index]).get_name() << endl;
                if (Card_less(biggest_card, follow_card, lead_card, trump)) {
                    biggest_card = follow_card;
                    biggest_index = current_index;
                }
            }
            
            cout << (*euchre_players[biggest_index]).get_name() << " takes the trick" << endl << endl;
            tricks_per_player[biggest_index] ++;
            lead_index = biggest_index;
        }
        //team 1 wins the trick
        if ((tricks_per_player[0] + tricks_per_player[2]) >= 3) {
            cout << (*euchre_players[0]).get_name() << " and " << (*euchre_players[2]).get_name()
                << " win the hand" << endl;
            //team 1 orders up
            if (order_index == 0 || order_index == 2) {
                //team 1 takes five tricks
                if (tricks_per_player[0] + tricks_per_player[2] == 5) {
                    cout << "march!" << endl;
                    points_t1 += 2;
                }
                //team 1 takes 3 or 4 tricks
                else {
                    points_t1 += 1;
                }
            }
            //team 2 orders up
            else {
                cout << "euchred!" << endl;
                points_t1 += 2;
            }
        }
        else {
            cout << (*euchre_players[1]).get_name() << " and " << (*euchre_players[3]).get_name()
                << " win the hand" << endl;
            //team 2 orders up
            if (order_index == 1 || order_index == 3) {
                //team 2 takes five tricks
                if (tricks_per_player[1] + tricks_per_player[3] == 5) {
                    cout << "march!" << endl;
                    points_t2 += 2;
                }
                //team 2 takes 3 or 4 tricks
                else {
                    points_t2 += 1;
                }
            }
            //team 1 orders up
            else {
                cout << "euchred!" << endl;
                points_t2 += 2;
            }
        }

        cout << (*euchre_players[0]).get_name() << " and " << (*euchre_players[2]).get_name()
            << " have " << points_t1 << " points" << endl;
        cout << (*euchre_players[1]).get_name() << " and " << (*euchre_players[3]).get_name()
            << " have " << points_t2 << " points" << endl << endl;

        for (int i = 0; i < 4; i++) {
            tricks_per_player[i] = 0;
        }
    }
};


    // EFFECTS: check the errors of the input arguments
    bool errors_check(int argc, char** argv) {
        // check if there are exactly 12 arguments
        if (argc != 12) {
            return false;
        }
        // check the shuffle argument
        string shuffl_Argu = argv[2];
        if ((shuffl_Argu != "shuffle") && (shuffl_Argu != "noshuffle")) {
            return false;
        }
        // check points to win the game is between 1 and 100, inclusive
        if ((atoi(argv[3]) > 100) || (atoi(argv[3]) < 1)) {
            return false;
        }
        // check the types of each of the players 
        // are either "Simple" or "Human"
        for (int i = 5; i < 12; i += 2) {
            string player_type = argv[i];
            if ((player_type != "Simple") && (player_type != "Human")) {
                return false;
            }
        }

        return true;
    }

    int main(int argc, char** argv) {

        // check for errors in the arguments
        if (errors_check(argc, argv) == false) {
            cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
                << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
                << "NAME4 TYPE4" << endl;
            return 1;
        }
        // check if the pack input file opens successfully
        ifstream fin;
        string pack_in = argv[1];
        fin.open(pack_in);
        if (!fin.is_open()) {
            cout << "Error opening " << pack_in << endl;
            return 1;
        }

        // print the arugments
        for (int i = 0; i < argc; i++) {
            cout << argv[i] << " ";
        }
        cout << endl;

        // get shuffle decision
        string shuffle = argv[2];
        // get points for winning
        int winning_point = atoi(argv[3]);
        // get card pack
        Pack pack(fin);
        // get game players
        vector<Player*> players;
        for (int i = 4; i < 12; i += 2) {
            players.push_back(Player_factory(string(argv[i]), string(argv[i + 1])));
        }

        Game newGame = Game(players, pack, shuffle);
        while ((newGame.get_points_team1() < winning_point) &&
            (newGame.get_points_team2() < winning_point)) {
            newGame.play();
        }

        if (newGame.get_points_team1() >= winning_point) {
            cout << newGame.get_player(0) << " and " << newGame.get_player(2)
                << " win!" << endl;
        }
        else {
            cout << newGame.get_player(1) << " and " << newGame.get_player(3)
                << " win!" << endl;
        }
        return 0;
    }