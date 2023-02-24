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

// EFFECTS: Used for managing team score
class Team {
public:
    Team() {
        score = 0;
        tricks_won = 0;
    }

    Team(Player* player1_in, Player* player2_in) {
        player1 = player1_in;
        player2 = player2_in;
        tricks_won = 0;
        score = 0;
    }

    string getPlayer1() {
        return player1->get_name();
    }

    string getPlayer2() {
        return player2->get_name();
    }

    void addTricksWon() {
        tricks_won++;
    }

    int getTricksWon() {
        return tricks_won;
    }

    void addPoint(int point) {
        score += point;
    }

    int getScore() {
        return score;
    }

    void printScore() {
        cout << getPlayer1() << " and " << getPlayer2()
            << " have " << getScore() << " points" << endl;
    }

private:
    Player* player1;
    Player* player2;
    int tricks_won;
    int score;
};

class Game {
public:
    Game(const Pack pack_in, const string shuffleDecision,
        const int winning_point, const vector<Player*> player_pack) {
        pack = pack_in;
        shuffle_Dec = shuffleDecision;
        points_to_win = winning_point;
        players = player_pack;
        hand = 0;
        round = 1;
        make_trump_hand = 0;
        dealer = players[hand];
        team1 = Team(players[0], players[2]);
        team2 = Team(players[1], players[3]);
    }
    void play() {
        // shuffle the cards first
        shuffle();
        // deal the cards
        deal();
        // make trump card
        make_trump();
        // play the game
        play_hand();
        hand++;
    }

    bool gameOver() {
        bool is_game_over = false;
        // when the game is over, print the winnners of the game
        if (team1.getScore() >= points_to_win) {
            cout << team1.getPlayer1() << " and "
                << team1.getPlayer2() << " win!" << endl;
            is_game_over = true;
        }
        else if (team2.getScore() >= points_to_win) {
            cout << team2.getPlayer1() << " and "
                << team2.getPlayer2() << " win!" << endl;
            is_game_over = true;
        }

        // delete the players
        if (is_game_over) {
            for (size_t i = 0; i < 4; i++) {
                delete players[i];
            }
        }
        dealerNum++;
        dealer = players[(dealerNum) % 4];
        pack.reset();
        return is_game_over;
    }

private:
    Pack pack;
    string shuffle_Dec;
    int points_to_win;
    vector<Player*> players;
    int hand;
    int dealerNum;
    string dealer_name;
    Player* dealer;
    Card upcard;
    int round;
    Suit trump;
    int make_trump_hand;
    int ordered_player;
    Team team1;
    Team team2;

    // EFFECTS: if player choose to shuffle cards
    //          then do it
    void shuffle() {
        if (shuffle_Dec == "shuffle") {
            pack.shuffle();
        }
    }

    // EFFECTS: deal the cards to each player
    void deal() {
        // find the dealer
        dealerNum = hand % 4;
        dealer_name = players[dealerNum]->get_name();

        // deal 3-2-3-2 cards
        players[(dealerNum + 1) % 4]->add_card(pack.deal_one());
        players[(dealerNum + 1) % 4]->add_card(pack.deal_one());
        players[(dealerNum + 1) % 4]->add_card(pack.deal_one());
        players[(dealerNum + 2) % 4]->add_card(pack.deal_one());
        players[(dealerNum + 2) % 4]->add_card(pack.deal_one());
        players[(dealerNum + 3) % 4]->add_card(pack.deal_one());
        players[(dealerNum + 3) % 4]->add_card(pack.deal_one());
        players[(dealerNum + 3) % 4]->add_card(pack.deal_one());
        dealer->add_card(pack.deal_one());
        dealer->add_card(pack.deal_one());

        // deal 2-3-2-3 cards
        players[(dealerNum + 1) % 4]->add_card(pack.deal_one());
        players[(dealerNum + 1) % 4]->add_card(pack.deal_one());
        players[(dealerNum + 2) % 4]->add_card(pack.deal_one());
        players[(dealerNum + 2) % 4]->add_card(pack.deal_one());
        players[(dealerNum + 2) % 4]->add_card(pack.deal_one());
        players[(dealerNum + 3) % 4]->add_card(pack.deal_one());
        players[(dealerNum + 3) % 4]->add_card(pack.deal_one());
        dealer->add_card(pack.deal_one());
        dealer->add_card(pack.deal_one());
        dealer->add_card(pack.deal_one());

        // get the upcard
        upcard = pack.deal_one();
        // initialize the trump suit to be the suit of upcard
        trump = upcard.get_suit();
    }

    // EFFECTS: making trump
    void make_trump() {
        // At the beginning of each hand, 
        // announce the hand, starting at zero, 
        // followed by the dealer and the upcard.
        cout << "Hand " << hand << endl;
        cout << dealer->get_name() << " deals" << endl;
        cout << upcard << " turned up" << endl;

        bool order_up = false;
        Player* currPlayer;
        bool is_dealer = false;
        // record the hand when making trump in each round
        int make_trump_hand = 1;

        for (round = 1; round <= 2; round++) {
            while ((!order_up) && make_trump_hand <= 4) {
                // find the player who is currently 
                // deciding to make trump
                currPlayer = players[(dealerNum + make_trump_hand) % 4];
                if (dealer_name == currPlayer->get_name()) {
                    is_dealer = true;
                }
                order_up = currPlayer->make_trump(upcard, is_dealer,
                    round, trump);

                // Print the decision of each player during the making procedure.
                if (!order_up) {
                    cout << currPlayer->get_name() << " passes" << endl;
                }
                else {
                    // record the player who ordered up the trump
                    ordered_player = (dealerNum + make_trump_hand) % 4;
                    if (round == 1) {
                        dealer->add_and_discard(upcard);
                    }
                    cout << currPlayer->get_name()
                        << " orders up " << trump << endl;
                    cout << endl;
                }
                // increment the hand for making trump
                make_trump_hand++;
            }
            // reset the hand for making trump
            make_trump_hand = 1;
        }
    }

    // EFFECTS: keep updating the point for each team
    void pointKeeper() {
        Team winnerTeam, loseTeam;
        // if team 1 ordered the trump
        if (ordered_player == 0 || ordered_player == 2) {
            // Team 1 get 1 point for taking 3 or 4 tricks
            if (team1.getTricksWon() == 3 || team1.getTricksWon() == 4) {
                team1.addPoint(1);
                winnerTeam = team1, loseTeam = team2;
                cout << team1.getPlayer1() << " and " << team1.getPlayer2()
                    << " win the hand" << endl;
            }
            // Team 1 get 2 points for taking all 5
            // Taking all 5 tricks is called a march
            else if (team1.getTricksWon() == 5) {
                team1.addPoint(2);
                winnerTeam = team1, loseTeam = team2;
                cout << team1.getPlayer1() << " and " << team1.getPlayer2()
                    << " win the hand" << endl;
                cout << "march!" << endl;
            }
            // Team 2 get 2 points for taking 3-5 tricks
            // This is called euchred
            if (team2.getTricksWon() >= 3) {
                team2.addPoint(2);
                winnerTeam = team2, loseTeam = team1;
                cout << team2.getPlayer1() << " and " << team2.getPlayer2()
                    << " win the hand" << endl;
                cout << "euchred!" << endl;
            }
        }
        // if team 2 ordered the trump
        else {
            // Team 2 get 1 point for taking 3 or 4 tricks
            if (team2.getTricksWon() == 3 || team2.getTricksWon() == 4) {
                team2.addPoint(1);
                winnerTeam = team2, loseTeam = team1;
                cout << team2.getPlayer1() << " and " << team2.getPlayer2()
                    << " win the hand" << endl;
            }
            // Team 2 get 2 points for taking all 5
            // Taking all 5 tricks is called a march
            else if (team2.getTricksWon() == 5) {
                team2.addPoint(2);
                winnerTeam = team2, loseTeam = team1;
                cout << team2.getPlayer1() << " and " << team2.getPlayer2()
                    << " win the hand" << endl;
                cout << "march!" << endl;
            }
            // Team 1 get 2 points for taking 3-5 tricks
            // This is called euchred
            if (team1.getTricksWon() >= 3) {
                team1.addPoint(2);
                winnerTeam = team1, loseTeam = team2;
                cout << team1.getPlayer1() << " and " << team1.getPlayer2()
                    << " win the hand" << endl;
                cout << "euchred!" << endl;
            }
        }

        // print the score, followed by an extra newline
        winnerTeam.printScore();
        loseTeam.printScore();
        cout << endl;
    }

    // EFFECTS: play the cards
    void play_hand() {
        // eldest hand is the player who
        // lead the first trick
        int lead_player_num = (dealerNum + 1) % 4;
        Player* leadPlayer = players[lead_player_num];
        int trick = 1;
        int highest_player_index = 0;

        while (trick <= 5) {
            Card highestCard = leadPlayer->lead_card(trump);
            Card leadCard = highestCard;

            // print the lead card and lead player
            cout << leadCard << " led by " << leadPlayer->get_name() << endl;
            for (int player_index = lead_player_num + 1;
                player_index <= lead_player_num + 3; player_index++) {
                Card playCard = players[player_index % 4]->play_card
                (leadCard, trump);
                // print each trick
                cout << playCard << " played by "
                    << players[player_index % 4]->get_name()
                    << endl;

                if (Card_less(highestCard, playCard, leadCard, trump)) {
                    highestCard = playCard;
                    highest_player_index = player_index % 4;
                }
            }
            // print the winner for the current trick
            cout << players[highest_player_index]->get_name()
                << " takes the trick" << endl;
            // print extra newline at the end of each trick
            cout << endl;

            // record tricks won for each team
            if (players[highest_player_index]->get_name() ==
                players[0]->get_name() ||
                players[highest_player_index]->get_name() ==
                players[2]->get_name()) {
                team1.addTricksWon();
            }
            else {
                team2.addTricksWon();
            }
            // increment trick for use of next trick
            trick++;
            // update lead player for next trick
            lead_player_num = highest_player_index;
            leadPlayer = players[lead_player_num];
        }
        // update points
        pointKeeper();
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

    Game game(pack, shuffle, winning_point, players);
    while (!game.gameOver()) {
        game.play();//might have problems here
    }

    fin.close();
    return 0;
}