#include <stdlib.h>
#include <stdbool.h>
#include "data_struct.h"
#include "game.h"
#include "text_ui.h"

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

	char *conv_test;
	int val_test;
	if(argc > 1){
		val_test = strtol(argv[1], &conv_test, 10);
		if((*conv_test == '\0') && (val_test >= 10)){
			height = val_test;
		}
	}
	if(argc > 2){
		val_test = strtol(argv[2], &conv_test, 10);
		if((*conv_test == '\0') && (val_test >= 10)){
			width = val_test;
		}
	}
	if(argc > 3){
		val_test = strtol(argv[3], &conv_test, 10);
		if((*conv_test == '\0') && (val_test >= 10)){
			bombs = val_test;
		}
	}

	//UI init
	UI *ui = ui_init();

	//Data instanciation
	Board *b = new_board(height, width, bombs);
	bool board_empty = true;
	Player *plr = init_player(b);

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

	//End of game
	if(board_empty){
		gen_board(b, plr->y, plr->x);
		board_empty = false;
	}
	int i, j;
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

