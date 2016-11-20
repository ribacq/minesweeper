#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <stdlib.h>
#include "data_struct.h"

/**
 * \file game.h
 * \brief Data structures and functions for the game itself
 */

///\brief Player data
typedef struct{
	int y; ///< \brief Y-coordinate
	int x; ///< \brief X-coordinate
	int nb_flags; ///< \brief number of flags currently placed on board
	enum{RUNNING, WON, LOST} status; ///< \brief Game status
} Player;
Player *init_player(Board *);

//Before game
void gen_board(Board *, int, int);

//Game
void press_cell(Board *, int, int);
void test_end_of_game(Board *, Player *);

#endif

