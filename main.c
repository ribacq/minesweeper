#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "data_struct.h"
#include "game.h"
#include "text_ui.h"
#include "solver.h"

/**
 * \mainpage CLI Minesweeper game
 */

/**
 * \file main.c
 * \brief Main file for Minesweeper
 */

/**
 * \brief Main function
 */
int main(int argc, char *argv[]){
	//Console-line arguments
	int height = 20;
	int width = 20;
	int bombs = 40;
	int solver_on = false;

	char *conv_test;
	int val_test;
	int i = 1;
	while(i < argc){
		if(strcmp(argv[i], "-d") == 0){
			if(i+1 < argc){
				val_test = strtol(argv[i+1], &conv_test, 10);
				if((*conv_test == '\0') && (val_test >= 10)){
					height = val_test;
				}
			}
			if(i+2 < argc){
				val_test = strtol(argv[i+2], &conv_test, 10);
				if((*conv_test == '\0') && (val_test >= 10)){
					width = val_test;
				}
			}
			if(i+3 < argc){
				val_test = strtol(argv[i+3], &conv_test, 10);
				if((*conv_test == '\0') && (val_test >= 10)){
					bombs = val_test;
				}
			}
			i += 3;
		}else if(strcmp(argv[i], "-a") == 0){
			solver_on = true;
		}
		i++;
	}

	//UI init
	UI *ui = ui_init();
	srand(time(NULL));

	//Data instanciation
	Board *b = new_board(height, width, bombs);
	Player *plr = init_player(b);
	bool board_empty = true;
	
	if(solver_on){
		board_empty = false;
		solver(ui, b, plr);
	}else{
		//Main loop
		char msg[100];
		print_board(ui, b);
		int action;
		while(ui->signal == CONTINUE){
			sprintf(msg, "Size: %dx%d. Flags/Bombs: %d/%d", b->h, b->w, plr->nb_flags, b->bombs);
			disp_msg(ui, b, msg);
			action = select_cell(ui, b, plr);
			if(action == -1){
				//quit
				ui->signal = QUIT;
			}else if(action == 0){
				//flag
				if(b->cells[plr->y][plr->x].status == HIDDEN){
					b->cells[plr->y][plr->x].status = FLAG;
					plr->nb_flags++;
				}else if(b->cells[plr->y][plr->x].status == FLAG){
					b->cells[plr->y][plr->x].status = HIDDEN;
					plr->nb_flags--;
				}
			}else if(action == 1){
				//press
				if(board_empty){
					gen_board(b, plr->y, plr->x);
					board_empty = false;
				}
				press_cell(b, plr->y, plr->x);
				test_end_of_game(b, plr);
				if(plr->status != RUNNING){
					ui->signal = QUIT;
				}
			}
			print_board(ui, b);
		}
	}

	//End of game
	if(board_empty){
		gen_board(b, plr->y, plr->x);
		board_empty = false;
	}
	int j;
	for(i=0; i<b->h; i++){
		for(j=0; j<b->w; j++){
			if((b->cells[i][j].status != FLAG) || (b->cells[i][j].proximity != -1)){
				b->cells[i][j].status = KNOWN;
			}
		}
	}
	print_board(ui, b);
	switch(plr->status){
	case LOST:
		disp_msg(ui, b, "You lost the game. Try again!");
		break;
	case WON:
		disp_msg(ui, b, "You won the game. Congratulations!");
		break;
	default:
		disp_msg(ui, b, "Game interrupted. That's unfortunate.");
		break;
	}
	ui_pause(ui);

	//End of program
	ui_terminate(ui);
	free_board(b);
	free(plr);
	return EXIT_SUCCESS;
}

