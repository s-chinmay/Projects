#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <algorithm>
#include "board.h"
#include "player.h"

using namespace std;

// function to read a file line by line and return the lines as a vector of strings
vector<string> readFile(string filename) {
    vector<string> lines; // to store the lines from the file
    ifstream file(filename); // opens the file
    string line;
    while (getline(file, line)) { // reads each file from the file
        lines.push_back(line); // add the lines to the vector 
    }
    return lines; // returns the vector of lines (strings)
}

// function to initialize players 
vector<Player> initializePlayers(int numPlayers) {
    vector<Player> players; // to store player objects
    vector<string> characters = readFile("character.txt"); // to load characters from file character.txt as a vector of strings
    vector<string> availableCharacters = characters; // this is a copy of chracters vector, avaliable characters used for selection, in the main menu 

//this loop that will run numPlayers times
    for (int i = 0; i < numPlayers; i++) { // loops through each player in the game
      // the loop ensures that each player is processed one by one, from player 1 to the last player, to store the player's name and the character they choose. 
        string name, character;
// holds the name of the player entered
// holds the character details selected by the player during the game. 

        // prompts for players name 
        cout << "Player " << i + 1 << ": Enter your name: ";
        cin >> name; // to store the player name input  

        // displays available characters
        cout << "Available characters:" << endl;
        //availableCharacters.size(): The total number of characters available in the chracters vector 
        for (size_t j = 0; j < availableCharacters.size(); j++) { // loops through each character in the availableCharacters list
           // stream that separates by extracted character line by |
            istringstream iss(availableCharacters[j]);
            string charName;
            getline(iss, charName, '|'); // extracts the first piece of data (character name) from the string and stores it in charName
            cout << j + 1 << ". " << charName << endl;
            // j + 1 because loop starts from 0. this displays  the index (j + 1) and the character name for the current entry in the list
        }

        int charChoice; //declares a variable charChoice to store the player's selection
        do { // ensures error handeling. that player enters a valid number corresponding to a character in the list
            cout << "Select a Lion Character (Enter number from the list): ";
            cin >> charChoice; //captures the player's choice
        } while (charChoice < 1 || charChoice > static_cast<int>(availableCharacters.size()));
        // static_cast<int> makes sures that the size of the availableCharacters vector is converted to an int
        
        // to access the selected character's data from availableCharacters
        // input string stream (iss)
        istringstream iss(availableCharacters[charChoice - 1]);
        string charName, age, strength, stamina, wisdom, pridePoints;
        //this is to extract individual attributes (separated by |) from the string
        getline(iss, charName, '|');
        getline(iss, age, '|');
        getline(iss, strength, '|');
        getline(iss, stamina, '|');
        getline(iss, wisdom, '|');
        getline(iss, pridePoints, '|');

        // emplace_back, adds a new player to the players vector
        // this is to store the player's name and character details, and converts to string to int. since this data holds numbers, which are being from the file as strings
        players.emplace_back(name, charName, stoi(age), stoi(stamina), stoi(strength), stoi(wisdom), stoi(pridePoints));
        // displays confirmation message showing the character chosen
        cout << "Character selected: " << charName << endl;

        // .erase () removes the selected character from the availableCharacters vector, so that the next player cannot select it again
        availableCharacters.erase(availableCharacters.begin() + charChoice - 1);

        int pathChoice; //declares a variable pathChoice, which holds the path type of the player
        do { // keeps prompting the user to enter the right values
            cout << "Path selection (1 for Cub Training, 2 for Straight to Pridelands): ";
            cin >> pathChoice;
        } while (pathChoice != 1 && pathChoice != 2);

        players[i].setPath(pathChoice); // records whether the player chose "Cub Training" or "Straight to Pridelands."

        if (pathChoice == 1) { // updates the points based on the players path selection
            players[i].updateStats(500, 500, 1000, -5000);
            // if path is cub training, then prompt to select an advisor
            // reads from the advisor.txt file and stores them in an vector of strings
            vector<string> advisors = readFile("advisor.txt"); 
            // prompts the user to select an advisor
            cout << "Select an Advisor:" << endl;
            for (int k = 0; k < advisors.size(); k++) {
                cout << k + 1 << ". " << advisors[k] << endl;
            }
            int advisorChoice;
            cin >> advisorChoice;
            players[i].setAdvisor(to_string(advisorChoice));
        } else {
            players[i].updateStats(200, 200, 200, 5000);
        }

        cout << endl;
    }

    return players;
}

int rollDice() {
    return rand() % 6 + 1;
}

void displayMainMenu() {
    cout << "Main Menu:" << endl;
    cout << "1. Check Player Progress" << endl;
    cout << "2. Review Character" << endl;
    cout << "3. Check Position" << endl;
    cout << "4. Review your Advisor" << endl;
    cout << "5. Move Forward" << endl;
}

void checkPlayerProgress(Player player) {
    cout << "Pride Points: " << player.getPridePoints() << endl;
    cout << "Stamina: " << player.getStamina() << endl;
    cout << "Strength: " << player.getStrength() << endl;
    cout << "Wisdom: " << player.getWisdom() << endl;
}

void reviewCharacter(Player player) {
    cout << "Name: " << player.getName() << endl;
    cout << "Character: " << player.getCharacter() << endl;
    cout << "Age: " << player.getAge() << endl;
}

void reviewAdvisor(Player player) {
    cout << "Your current advisor: " << player.getAdvisor() << endl;
}

vector<Player> handleTileEffect(Board board, vector<Player> players, int currentPlayer) {
    char tileType = board.getTileType(currentPlayer);
    Player player = players[currentPlayer];  // Get the current player by value

    switch (tileType) {
        case 'G': // Grasslands
            cout << "You landed on Grasslands. Enjoy a moment of rest." << endl;
            break;

        case 'B': // Blue Tile
            cout << "You landed on a Blue Tile. Gaining 200 Stamina!" << endl;
            player.updateStats(0, 200, 0, 0);
            break;

        case 'P': // Pink Tile
            cout << "You landed on a Pink Tile. Facing a random event!" << endl;
            if (player.getAdvisor() == "None") {
                cout << "No advisor to help! Losing 200 Pride Points." << endl;
                player.updateStats(0, 0, 0, -200);
            } else {
                cout << "Your advisor saved you from the event!" << endl;
            }
            break;

        case 'R': // Red Tile
            cout << "You landed on a Red Tile. Losing 100 Strength!" << endl;
            player.updateStats(0, 0, -100, 0);
            break;

        case 'U': // Purple Tile
            cout << "You landed on a Purple Tile. Answer this riddle!" << endl;
            if (board.askRiddle()) {
                player.updateStats(0, 0, 0, 500); // Gain wisdom if correct
            }
            break;

        case 'N': // Brown Tile
            cout << "You landed on a Brown Tile. Losing 50 Stamina and Pride Points." << endl;
            player.updateStats(-50, 0, 0, -50);
            break;

        case 'O': // Orange Tile
            cout << "You have reached the end of the path! Congratulations!" << endl;
            break;

        default:
            cout << "Mystry tile!" << endl;
    }

    players[currentPlayer] = player;

    return players; 
}


int main() {
    srand(time(0));

    int numPlayers;
    do { // This gets a valid number of players, making sure not below 2 or above 3
        cout << "Enter number of players (2-3): "; // Keep prompting the user to enter a valid number 2 or 3
        cin >> numPlayers;
    } while (numPlayers < 2 || numPlayers > 3);

    vector<Player> players = initializePlayers(numPlayers);
    int player1Choice = players[0].getPath(); 
    int player2Choice = players[1].getPath();
    Board board(2, player1Choice, player2Choice); // Initialize the board with the number of players and their choices

    bool gameOver = false;
    int currentPlayer = 0;

    while (!gameOver) {
        cout << "\nCurrent player: " << players[currentPlayer].getName() << endl;
        board.displayBoard(players);

        int choice;
        do {
            displayMainMenu();
            cout << "Enter your choice: ";
            cin >> choice;

            int roll = rollDice();
            switch (choice) {
                case 1:
                    checkPlayerProgress(players[currentPlayer]);
                    break;
                case 2:
                    reviewCharacter(players[currentPlayer]);
                    break;
                case 3:
                    board.displayBoard(players);
                    break;
                case 4:
                    reviewAdvisor(players[currentPlayer]);
                    break;
                case 5:
                     roll = rollDice();
                    cout << "You rolled a " << roll << endl;
                    if (board.movePlayer(currentPlayer, roll)) {
                        cout << players[currentPlayer].getName() << " has reached Pride Rock!" << endl;
                        gameOver = true;
    } else {
        players = handleTileEffect(board, players, currentPlayer);
    //    players = handleTileEffect(board, players, currentPlayer);
        if (board.getTileType(currentPlayer) == 4 || board.getTileType(currentPlayer) == 6) {
                            cout << "You landed on a special tile!" << endl;
        }
    }
    currentPlayer = (currentPlayer + 1) % numPlayers; // this make sures the switch turns for all the players
    break;
    default:
        cout << "Invalid choice. Please try again." << endl;
            }
         } while (choice != 5 && !gameOver);
    }

    cout << "\nGame Over!" << endl;
    int maxPridePoints = 0;
    string winner;
    for (int i = 0; i < players.size(); i++) {
        int pridePoints = players[i].getPridePoints();
        if (pridePoints > maxPridePoints) {
            maxPridePoints = pridePoints;
            winner = players[i].getName();
        }
    }
    cout << "The winner is " << winner << " with " << maxPridePoints << " Pride Points!" << endl;

    return 0;
}