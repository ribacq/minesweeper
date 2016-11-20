#include "game.h"

///\brief Constructor for Player
Player *init_player(Board *b){
	Player *plr = (Player *) malloc(sizeof(Player));
	plr->y = b->h/2;
	plr->x = b->w/2;
	plr->nb_flags = 0;
	plr->status = RUNNING;
	return plr;
}

/**
 * \brief Places bombs on the board and sets correct numbers in every cell.
 * \param *b the board to set
 * \param beg_y Y-coordinate of a cell that must stay free
 * \param beg_x X-coordinate of a cell that must stay free
 */
void gen_board(Board *b, int beg_y, int beg_x){
	//Placing bombs
	int b_y, b_x;
	int i = 0;
	while(i < b->bombs){
		b_y = rand()%b->h;
		b_x = rand()%b->w;
		//Random cell is suitable for a bomb if there’s no bomb already there and if this is not (beg_y, beg_x) or a neighbor of it.
		if((b->cells[b_y][b_x].proximity != -1) && ((b_y < beg_y-1) || (b_y > beg_y+1) || (b_x < beg_x-1) || (b_x > beg_x+1))){
			//Set bomb
			b->cells[b_y][b_x].proximity = -1;
			//Increment neighbors
			if(b_y > 0){
				//top-left
				if((b_x > 0) && (b->cells[b_y-1][b_x-1].proximity != -1)) b->cells[b_y-1][b_x-1].proximity++;
				//top-mid
				if(b->cells[b_y-1][b_x].proximity != -1) b->cells[b_y-1][b_x].proximity++;
				//top-right
				if((b_x < b->w-1) && (b->cells[b_y-1][b_x+1].proximity != -1)) b->cells[b_y-1][b_x+1].proximity++;
			}
			//mid-left
			if((b_x > 0) && (b->cells[b_y][b_x-1].proximity != -1)) b->cells[b_y][b_x-1].proximity++;
			//mid-right
			if((b_x < b->w-1) && (b->cells[b_y][b_x+1].proximity != -1)) b->cells[b_y][b_x+1].proximity++;
			if(b_y < b->h-1){
				//bot-left
				if((b_x > 0) && (b->cells[b_y+1][b_x-1].proximity != -1)) b->cells[b_y+1][b_x-1].proximity++;
				//bot-mid
				if(b->cells[b_y+1][b_x].proximity != -1) b->cells[b_y+1][b_x].proximity++;
				//bot-right
				if((b_x < b->w-1) && (b->cells[b_y+1][b_x+1].proximity != -1)) b->cells[b_y+1][b_x+1].proximity++;
			}
			//One less bomb to set
			i++;
		}
	}
	return;
}

/**
 * \brief Function that presses a cell and its non-bomb neighbors, recursively
 * \return number from -1 to 8: Cell::proximity; -2: wrong input
 */
void press_cell(Board *b, int y, int x){
	//Wrong input: invalid coordinates
	if((y < 0) || (y >= b->h) || (x < 0) || (x >= b->w)) return;

	//Wrong input: cell is already KNOWN
	if(b->cells[y][x].status == KNOWN) return;

	//Correct input
	b->cells[y][x].status = KNOWN;
	if(b->cells[y][x].proximity == 0){
		if(y > 0){
			//top-left
			if((x > 0) && (b->cells[y-1][x-1].proximity >= 0)) press_cell(b, y-1, x-1);
			//top-mid
			if(b->cells[y-1][x].proximity >= 0) press_cell(b, y-1, x);
			//top-right
			if((x < b->w-1) && (b->cells[y-1][x+1].proximity >= 0)) press_cell(b, y-1, x+1);
		}
		//mid-left
		if((x > 0) && (b->cells[y][x-1].proximity >= 0)) press_cell(b, y, x-1);
		//mid-right
		if((x < b->w-1) && (b->cells[y][x+1].proximity >= 0)) press_cell(b, y, x+1);
		if(y < b->h-1){
			//bot-left
			if((x > 0) && (b->cells[y+1][x-1].proximity >= 0)) press_cell(b, y+1, x-1);
			//bot-mid
			if(b->cells[y+1][x].proximity >= 0) press_cell(b, y+1, x);
			//bot-right
			if((x < b->w-1) && (b->cells[y+1][x+1].proximity >= 0)) press_cell(b, y+1, x+1);
		}
	}
	return;
}

/**
 * \brief Testes end of game and updates Player::status
 *
 * Goes through the whole board.
 * If there is a KNOWN bomb, Player::status is set to LOST.
 * If number of HIDDEN or FLAG cells == Board::bombs, Player::status is set to WON.
 */
void test_end_of_game(Board *b, Player *plr){
	int nb_hidden = 0;
	int i, j;
	for(i=0; i<b->h; i++){
		for(j=0; j<b->w; j++){
			if(b->cells[i][j].status != KNOWN){
				nb_hidden++;
			}else if(b->cells[i][j].proximity == -1){
				plr->status = LOST;
				return;
			}
		}
	}
	if(nb_hidden == b->bombs){
		plr->status = WON;
	}
	return;
}

