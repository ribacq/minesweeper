#ifndef DATA_STRUCT_H_INCLUDED
#define DATA_STRUCT_H_INCLUDED

/**
 * \file data_struct.h
 * Defines the data structures for the minesweeper
 */

#include <stdlib.h>

///\brief Status of Cell
typedef enum{HIDDEN, FLAG, KNOWN} Status;

///\brief Cell data structure
typedef struct{
	int proximity; ///< \brief Proximity with bombs. -1: bomb; 0-8: number of bombs in neighbors.
	Status status; ///< \brief Whether the cell is hidden, flagged or known.
} Cell;

///\brief Board data structure
typedef struct{
	int h; ///< \brief Height in lines.
	int w; ///< \brief Width in columns.
	int bombs; ///< \brief Number of bombs.
	Cell **cells; ///< \brief 2D array of Cell.
} Board;

///Board functions
Board *new_board(int, int, int);
void free_board(Board *);

#endif

