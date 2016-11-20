#include "data_struct.h"

/**
 * \brief Board initialization
 * \param height number of lines
 * \param width number of columns
 * \param bombs number of bombs; must be < height*width.
 */
Board *new_board(int height, int width, int bombs){
	//General memory allocation
	Board *b = (Board *) malloc(sizeof(Board));

	//Height and Width
	b->h = height;
	b->w = width;

	//Bomb number reduction if argument is too big
	if(bombs >= height*width)
		bombs = height*width-1;
	b->bombs = bombs;
	
	//Cells allocation
	int i, j;
	b->cells = (Cell **) calloc(b->h, sizeof(Cell *));
	for(i=0; i < b->h; i++){
		b->cells[i] = (Cell *) calloc(b->w, sizeof(Cell));
		for(j=0; j<b->w; j++){
			b->cells[i][j].status = HIDDEN;
			b->cells[i][j].proximity = 0;
		}
	}
	
	
	return b;
}

///\brief Memory deallocation for board
void free_board(Board *b){
	int i;
	for(i=0; i<b->h; i++){
		free(b->cells[i]);
	}
	free(b->cells);
	free(b);
	return;
}

