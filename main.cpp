
/* ---------------------------------------------
Program 6: March Madness Frenzy 2.0
--
   This program reads in data from NCAA Basketball Tournaments (aka March Madness).
   It uses the hierarchy of the brackets (implemented via linked lists) to allow you
   to see the path taken to the championship, and evaluate the resulting brackets if
   some games had a different outcome.

--

Course: CS 141, Spring 2022
System: Windows 10 Using Clion
Authors: George Fashho
---------------------------------------------
*/

#include <iostream>   // For Input and Output
#include <fstream>    // For file input and output
#include <cassert>    // For the assert statement
#include <vector>     // For vectors
#include <sstream>    // For reading in CSV file and splitting by commas
#include <algorithm>

using namespace std;

class Node; // this is the declaration, so that it can be used inside class Game below
// the definition of the Node class comes after the Game class

class Game {
public:
    // Default up to Fully-Qualified Constructor
    Game(string region = "N/A", int rankOne = -1, string teamOne = "N/A", int scoreOne = -1,
         int rankTwo = -1, string teamTwo = "N/A", int scoreTwo = -1,
         string winningTeam = "N/A", int roundNumber = -1, int gameNumber = -1) {
        this->region = region;
        this->team1Rank = rankOne;
        this->team1Name = teamOne;
        this->team1Score = scoreOne;
        this->team2Rank = rankTwo;
        this->team2Name = teamTwo;
        this->team2Score = scoreTwo;
        this->winningTeam = winningTeam;
        this->roundNum = roundNumber;
        this->gameNum = gameNumber;
        this->subBrackets = NULL;
    }

    // Copy Constructor
    Game(const Game &otherGame) {
        this->region = otherGame.region;
        this->team1Rank = otherGame.team1Rank;
        this->team1Name = otherGame.team1Name;
        this->team1Score = otherGame.team1Score;
        this->team2Rank = otherGame.team2Rank;
        this->team2Name = otherGame.team2Name;
        this->team2Score = otherGame.team2Score;
        this->winningTeam = otherGame.winningTeam;
        this->roundNum = otherGame.roundNum;
        this->gameNum = otherGame.gameNum;
        this->subBrackets = otherGame.subBrackets;
    }

    // Getters / Accessor methods
    string getRegion() { return region; }

    int getRoundNum() { return roundNum; }

    string getTeamOneName() { return team1Name; }

    int getTeamOneRank() { return team1Rank; }

    int getTeamOneScore() { return team1Score; }

    string getTeamTwoName() { return team2Name; }

    int getTeamTwoRank() { return team2Rank; }

    int getTeamTwoScore() { return team2Score; }

    string getWinner() { return winningTeam; }

    Node *getSubBrackets() { return subBrackets; }

    // Setters / Mutator methods
    void setTeamOneName(string newTeamOneName) { this->team1Name = newTeamOneName; }

    void setTeamTwoName(string newTeamTwoName) { this->team2Name = newTeamTwoName; }

    void setWinner(string newWinner) { this->winningTeam = newWinner; }

    void setSubBrackets(Node *newSubBrackets) { this->subBrackets = newSubBrackets; }

    // Utility function to display the game
    void displayGame() {
        cout << "Round " << roundNum << ", Game " << gameNum << ": "
             << team1Name << " vs " << team2Name << ". "
             << "Winner: " << winningTeam << "\n";
    }

private:
    string region;
    int roundNum;
    int gameNum;
    string team1Name;
    string team2Name;
    string winningTeam;
    int team1Rank;
    int team2Rank;
    int team1Score;
    int team2Score;
    Node *subBrackets;  // This is a pointer to a Node, in order to represent the subbracts as a linked list
};

// Node class in order to implement linked lists
class Node {

public:
    Node() {
        pNext = NULL;
    }

    Node(Game theSubBracket, Node *thePNext) {
        subBracket = theSubBracket;
        pNext = thePNext;
    }

    // ideally these should be private, but we are making them public to make the code a bit simpler
    Game subBracket;
    Node *pNext;
};

void reverseList(Node *pHead, Node *&newHead) {
    if (pHead->pNext == NULL) {
        newHead = pHead;
    } else {
        reverseList(pHead->pNext, newHead);
        pHead->pNext->pNext = pHead;
    }
}

//--------------------------------------------------------------------------------
// Display welcome message, introducing the user to the program
void displayWelcomeMessage() {
    cout << "Program 6: March Madness Frenzy 2.0\n"
         << "CS 141, Spring 2022, UIC\n"
         << "\n"
         << "This program reads in data from NCAA Basketball Tournaments (aka March Madness). "
         << "It uses the hierarchy of the brackets (implemented via linked lists) to allow you "
         << "to see the path taken to the championship, and evaluate the resulting brackets if "
         << "some games had a different outcome.\n"
         << endl;
}//end displayWelcomeMessage()

//--------------------------------------------------------------------------------
// Read in the game data and store in a vector
void readInGameData(string filename, vector<Game> &games) {

    string region, teamOne, teamTwo, winningTeam;
    int rankOne, rankTwo, roundNumber, gameNumber, scoreOne, scoreTwo;

    ifstream inStream;
    inStream.open(filename);
    assert(inStream.fail() == false);

    string row;
    getline(inStream, row); // read in header from file, and ignore

    // read in each line of the file
    while (getline(inStream, row)) {
        stringstream s_stream(row); //create stringstream from the row

        // separate the line by commas into each column
        int colNum = 0;
        while (s_stream.good()) {
            string columnVal;
            getline(s_stream, columnVal, ','); //get first string delimited by comma

            // use the substring to fill in the appropriate part of the BillboardRecord
            switch (colNum) {
                case 0:
                    region = columnVal;
                    break;
                case 1:
                    rankOne = stoi(columnVal);
                    break;
                case 2:
                    teamOne = columnVal;
                    break;
                case 3:
                    if (columnVal != "") { scoreOne = stoi(columnVal); }
                    else { scoreOne = -1; }
                    break;
                case 4:
                    rankTwo = stoi(columnVal);
                    break;
                case 5:
                    teamTwo = columnVal;
                    break;
                case 6:
                    if (columnVal != "") { scoreTwo = stoi(columnVal); }
                    else { scoreTwo = -1; }
                    break;
                case 7:
                    winningTeam = columnVal;
                    break;
                case 8:
                    roundNumber = stoi(columnVal);
                    break;
                case 9:
                    gameNumber = stoi(columnVal);
                    break;
                default: // should not get here, because there are only 10 columns
                    cout << "More columns in the file than expected... Exiting program" << endl;
                    exit(0);
                    break;
            }
            colNum++;
        }
        // Create instance of the class using the column values
        Game oneGame;
        oneGame = Game(region, rankOne, teamOne, scoreOne, rankTwo, teamTwo, scoreTwo, winningTeam, roundNumber,
                       gameNumber);
        // and add it to the vector
        games.push_back(oneGame);

    } // end while(getline(instream, row))

} // end readInGameData() function

void getUserInput(string &userInput) {
    getline(cin, userInput);
}//end getUserInput()

//--------------------------------------------------------------------------------
// Identify the sub-brackets for each game and modify the class instances in the vector accordingly
void determineSubBrackets(vector<Game> &games) {

    // Nested loop to find the sub-brackets for each game
    // We go in reverse, starting at round 1 and going up to round 6
    //      Note - this is so that when we add subbrackets for round 6, the subbrackets for the games
    //             in round 5 have been determined, and so on and so forth
    for (int i = games.size() - 1; i >= 0; i--) {
        for (int j = games.size() - 1; j >= 0; j--) {
            // Check that we are not looking at the same game
            if (i == j) {
                continue;
            }
            // games[j] is a sub-bracket of games[i] if it is from the previous round,
            // and if the winning team from games[j] is one of the teams in games[i]
            if ((games.at(j).getRoundNum() == (games.at(i).getRoundNum() - 1)) &&
                (games.at(j).getWinner() == ga`mes.at(i).getTeamOneName() ||
                 games.at(j).getWinner() == games.at(i).getTeamTwoName())) {
                // TO DO: Add games[j] to the linked list of sub-brackets for games[i]
                // ...

                Node *newNode = games.at(i).getSubBrackets();
                Node *pHead = new Node(games.at(j), newNode);
                games.at(i).setSubBrackets(pHead);

            }
        }
    }

} // end of determineSubBrackets() function

void recursive3(Game &game, int userInput) {

    string winner = game.getWinner();
//    game.displayGame();
    if (userInput == 1) {
        if (game.getTeamOneName() == game.getWinner()) {
            game.setWinner(game.getTeamTwoName());

        } else {
            game.setWinner(game.getTeamOneName());
        }
        return;
    }
    Node *subBracket = game.getSubBrackets();
    while (subBracket != nullptr) {
        if (subBracket->subBracket.getWinner() == winner) {

            recursive3(subBracket->subBracket, userInput - 1);

            if (game.getTeamOneName() == winner) {
                game.setWinner(subBracket->subBracket.getWinner());
                game.setTeamOneName(subBracket->subBracket.getWinner());
            } else {
                game.setWinner(subBracket->subBracket.getWinner());
                game.setTeamTwoName(subBracket->subBracket.getWinner());

            }
        }
        subBracket = subBracket->pNext;
    }
}

void userInput(int &menuOption) {
    cout << "Select a menu option: \n"
            "   1. See the sub-brackets of one game\n"
            "   2. Display the path of the winning team to the championship\n"
            "   3. Undo the championship game for a given number of rounds\n"
            "   4. Exit\n"
            "Your choice --> ";
    cin >> menuOption;
}

void menuOption3(int &userInput) {
    cout << "Enter the number of rounds that you would like to undo (max of 6 allowed): \n";
    cin >> userInput;
}

// ----------------------------------------------------------------------
// Main() function of the program
int main() {
    //Vector to hold tournament games
    vector<Game> games;
    int menuOption;

    displayWelcomeMessage();

    // Prompt user for the name of the file with the data
    string fileName;
    cout << "Enter the name of the file with the data for the NCAA tournament: ";
    cin >> fileName;

    // Read in the data from the file into a vector
    readInGameData(fileName, games);
    // Also identify sub-brackets for each game
    determineSubBrackets(games);

    while (true) {

        userInput(menuOption);
        if (menuOption > 4 || menuOption < 1) {
            cout << "Invalid value.  Please re-enter a value from the menu options below. \n";
            userInput(menuOption);
        }


        if (menuOption == 1) {
            int userRoundNum;
            string userWinningTeam;

            cout << "Enter the round number of the game: \n";
            cin >> userRoundNum;
            cout << "Enter the winning team of the game: \n";
            getUserInput(userWinningTeam);
            getUserInput(userWinningTeam);

            bool game = false;
            for (int i = 0; i < games.size(); i++) {

                if (games.at(i).getRoundNum() == userRoundNum) {
                    if (games.at(i).getWinner() == userWinningTeam) {
                        cout << "The game is: \n";
                        games.at(i).displayGame();


                        cout << "Sub-brackets of this game are: \n";
                        Node *head = games.at(i).getSubBrackets();
                        Node *temp = head;


                        while (temp) {
                            temp->subBracket.displayGame();
                            temp = temp->pNext;
                        }
                        game = true;
                    }
                }
            }
            if (!game) {
                cout << "Sorry, no games matching that round number and winning team were found.";

            }
        }


        else if (menuOption == 2) {
            cout << "Path to the championship: \n";
            string winner = games.at(0).getWinner();

            Node *pHead = games.at(0).getSubBrackets();
            Node *temp = pHead;

//            reverseList(pHead,temp);
            vector<Game> subBrackets;
            while (temp != NULL) {

                if (temp->subBracket.getWinner() == winner) {

                    subBrackets.push_back(temp->subBracket);


                    temp = temp->subBracket.getSubBrackets();
                } else {
                    temp = temp->pNext;
                }
            }
            reverse(subBrackets.begin(), subBrackets.end());
            for (int i = 0; i < subBrackets.size(); i++) {

                subBrackets.at(i).displayGame();
            }
            games.at(0).displayGame();



        } else if (menuOption == 3) {
            int userInput2;

            menuOption3(userInput2);
            if (userInput2 > 6 || userInput2 < 1) {
                cout << "Invalid entry. Try again.\n";
                menuOption3(userInput2);
            }

            recursive3(games.at(0), userInput2);

            string winner = games.at(0).getWinner();

            Node *pHead2 = games.at(0).getSubBrackets();
            Node *temp2 = pHead2;

            vector<Game> subBrackets2;
            while (temp2 != NULL) {

                if (temp2->subBracket.getWinner() == winner) {

                    subBrackets2.push_back(temp2->subBracket);

                    temp2 = temp2->subBracket.getSubBrackets();
                } else {
                    temp2 = temp2->pNext;
                }
            }
            reverse(subBrackets2.begin(), subBrackets2.end());
            for (int i = 0; i < subBrackets2.size(); i++) {

                subBrackets2.at(i).displayGame();
            }
            games.at(0).displayGame();

        } else if (menuOption == 4) {
            break;
        }
    }

    cout << "Exiting program...";

    return 0;
} // end main()
