#include "Board.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>
#include <sstream>
// Tile colors
#define RED "\033[48;2;230;10;10m"
#define GREEN "\033[48;2;34;139;34m"
#define BLUE "\033[48;2;10;10;230m"
#define PINK "\033[48;2;255;105;180m"
#define BROWN "\033[48;2;139;69;19m"
#define PURPLE "\033[48;2;128;0;128m"
#define ORANGE "\033[48;2;230;115;0m"
#define GREY "\033[48;2;128;128;128m"
#define RESET "\033[0m"

using namespace std;

Board::Board(int player_count, int player1, int player2) {
    if (player_count > _MAX_PLAYERS){

        _player_count = _MAX_PLAYERS;
    } else{
        _player_count = player_count;
    }

   // _player_count = (player_count > _MAX_PLAYERS) ? _MAX_PLAYERS : player_count;
    for (int i = 0; i < _player_count; i++) {
        _player_position[i] = 0; // current player position
       // _previous_position[i] = 0; // Initialize previous position to 
    }
    if(player1 == 1){
        playerChoice[0] = 0;
    } else {
        playerChoice[0] = 1;
    }
    if (player2 == 2){
        playerChoice[1] = 1;
    }else {
        playerChoice[1] = 0;
    }

    initializeBoard();
}

void Board::initializeBoard() {
    for (int i = 0; i < 2; i++) {
        initializeTiles(i);
    }
}

vector<pair<string, string>> Board::loadRiddles(string filename) {  
    vector<pair<string, string>> riddles;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string question, answer;
        getline(iss, question, '|');
        getline(iss, answer);
        riddles.push_back(make_pair(question, answer));
    }
    return riddles;
}

bool Board::askRiddle() { // for the purple tile, To ask riddles, randomly generated
    // Load the riddles from the file
    vector<pair<string, string>> riddles = loadRiddles("riddles.txt");  // 
    
    // Select a random riddle
    srand(time(0)); // Make sure randomness
    int riddleIndex = rand() % riddles.size();
    string riddle = riddles[riddleIndex].first;
    string correctAnswer = riddles[riddleIndex].second;

    // ask the riddle
    cout << "Riddle: " << riddle << endl;
    string playerAnswer;
    cout << "Your answer: ";
    cin >> playerAnswer;

    // Checks the answer
    if (playerAnswer == correctAnswer) {
        cout << "Correct! You earn 500 Wisdom Points!" << endl;
        return true;
    } else {
        cout << "Incorrect answer. Better luck next time!" << endl;
        return false;
    }
}

void Board::initializeTiles(int player_index) {
    int green_count = 0;
    for (int i = 0; i < _BOARD_SIZE; i++) {
        if (i == _BOARD_SIZE - 1) {
            _tiles[player_index][i].color = 'O'; // end tile orange
        } else if (i == 0) {
            _tiles[player_index][i].color = 'Y'; // start tile grey
        } else if (green_count < 30 && (rand() % (_BOARD_SIZE - i) < 30 - green_count)) {
            _tiles[player_index][i].color = 'G'; // grasslands 
            green_count++;
        } else {
            char colors[] = {'B', 'P', 'N', 'R', 'U'};
            _tiles[player_index][i].color = colors[rand() % 5];
        }
    }
}

bool Board::isPlayerOnTile(int player_index, int pos) {
    return _player_position[player_index] == pos;
}

void Board::displayTile(int player_index, int pos) {
    string color;

    int player1 = isPlayerOnTile(0,pos); 
    int player2 = isPlayerOnTile(1,pos); 

    switch (_tiles[player_index][pos].color) {
        case 'R': color = RED; break;
        case 'G': color = GREEN; break;
        case 'B': color = BLUE; break;
        case 'U': color = PURPLE; break;
        case 'N': color = BROWN; break;
        case 'P': color = PINK; break;
        case 'O': color = ORANGE; break;
        case 'Y': color = GREY; break;
        default: color = RESET;
    }
    if (player1 && playerChoice[0] == player_index && player2 && playerChoice[1] == player_index) {
    cout << color << "|1&2|" << RESET << endl;
    } else if (player1 && playerChoice[0] == player_index) {
        cout << color << "|" << 1 << "|" << RESET;
    // cout << color << "|" << (isPlayerOnTile(player_index, pos, 0) ? to_string(player_index + 1) : " ") << "|" << RESET;
} else if (player2 && playerChoice[1] == player_index) {
    cout << color << "|" << 2 << "|" << RESET;
} else{
    cout << color << "| |" << RESET;
}
} 

void Board::displayTrack(int player_index) {
    for (int i = 0; i < _BOARD_SIZE; i++) {
        displayTile(player_index, i);
    }
    cout << endl;
}

void Board::displayBoard(vector<Player> players) {
    for (int i = 0; i < 2; i++) {
        displayTrack(i);
        if (i == 0) cout << endl;
    }
}

bool Board::movePlayer(int player_index, int spaces) {
    _previous_position[player_index] = _player_position[player_index]; // Save the current position
    _player_position[player_index] += spaces;
    if (_player_position[player_index] >= _BOARD_SIZE - 1) {
        _player_position[player_index] = _BOARD_SIZE - 1;
        cout << "Player " << player_index + 1 << " has reached the PRIDE ROCK!" << endl;
        return true; 
    }
    return false;
}


int Board::getPlayerPosition(int player_index) const {
    return (player_index >= 0 && player_index < _player_count) ? _player_position[player_index] : -1;
}

int Board::getTileType(int player_index) const {
    // Validate player index
    if (player_index < 0 || player_index >= _player_count) {
        std::cerr << "Invalid player index!" << std::endl;
        return -1;
    }

    int col = _player_position[player_index]; // current position
    int row = player_index;
    cout << "Player index:" << endl;
    cout << player_index << endl;

    // Validate bounds
    if (col < 0 || col >= _BOARD_SIZE) {
        std::cerr << "Invalid tile position!" << std::endl;
        return -1;
    }

    return _tiles[row][col].color; // Return the tile's color type (e.g., 'R', 'G', etc.)
}

void Board::movePlayerBackward(int player_index, int steps) {
    _previous_position[player_index] = _player_position[player_index]; // Save current position
    _player_position[player_index] = max(0, _player_position[player_index] - steps); // move backward
}


void Board::returnToPreviousPosition(int player_index) {
    _player_position[player_index] = _previous_position[player_index]; // Return to the saved position
}
