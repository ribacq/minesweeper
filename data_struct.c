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

bool exists(Board *b, int y, int x){
	return (y >= 0) && (y < b->h) && (x >= 0) && (x < b->w);
}

void get_neighbors(int y, int x, int yarr[8], int xarr[8]){
	yarr[0] = y-1;	xarr[0] = x-1;
	yarr[1] = y-1;	xarr[1] = x;
	yarr[2] = y-1;	xarr[2] = x+1;
	yarr[3] = y;	xarr[3] = x-1;
	yarr[4] = y;	xarr[4] = x+1;
	yarr[5] = y+1;	xarr[5] = x-1;
	yarr[6] = y+1;	xarr[6] = x;
	yarr[7] = y+1;	xarr[7] = x+1;
	return;
}

int nb_neighbors_status(Board *b, int y, int x, Status status){
	int ret = 0;
	int neighs_y[8], neighs_x[8];
	get_neighbors(y, x, neighs_y, neighs_x);
	int i;
	for(i=0; i<8; i++){
		if(exists(b, neighs_y[i], neighs_x[i]) && (b->cells[neighs_y[i]][neighs_x[i]].status == status))
			ret++;
	}
	return ret;
}

