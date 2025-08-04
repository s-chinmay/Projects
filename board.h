#ifndef BOARD_H
#define BOARD_H

#include "Tile.h"
#include "player.h" 
#include <vector>
#include <string>
using namespace std; 

class Board {
private:
    static const int _BOARD_SIZE = 52;
    Tile _tiles[2][_BOARD_SIZE];
    static const int _MAX_PLAYERS = 3;
    int _player_count;
    int _player_position[_MAX_PLAYERS];
    int _previous_position[_MAX_PLAYERS];

    int playerChoice[_MAX_PLAYERS]; // stores the player's choice of tile to draw from the deck

    void displayTile(int player_index, int pos);
    void initializeTiles(int player_index);
    bool isPlayerOnTile(int player_index, int pos);
    void handleTileEvent(int player_index);


    vector<Player> players;  // Add a vector of Player objects
    bool extraTurn;  // Variable to track if a player gets an extra turn
public:
Board();
    Board(int player_count, int player1, int player2);
    void displayTrack(int player_index);
    void initializeBoard();
   int getTileType(int player_index) const;
    void displayBoard(vector<Player>);
    bool movePlayer(int player_index, int spaces);
    int getPlayerPosition(int player_index) const;
    bool askRiddle();

    vector<pair<string,string>> loadRiddles(string filename);



    void movePlayerBackward(int player_index, int steps);
    void returnToPreviousPosition(int player_index);

};

#endif