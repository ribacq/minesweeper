#include "solver.h"

///\brief Minesweeper solver
void solver(UI *ui, Board *b, Player *plr){
	//First press to generate the board
	plr->y = rand()%b->h;
	plr->x = rand()%b->w;
	gen_board(b, plr->y, plr->x);
	press_cell(b, plr->y, plr->x);
	
	//Variables
	int neighs_y[8], neighs_x[8];
	int i;
	curs_set(TRUE);
	int y0 = (LINES-b->h)/2;
	int x0 = (COLS-b->w)/2;
	int nb_hidden, nb_flag, proximity;
	bool progress;

	//First display
	print_board(ui, b);
	ui_refresh(ui);
	
	//Main loop
	while((plr->status == RUNNING) && (ui->signal == CONTINUE)){
		nb_hidden = nb_neighbors_status(b, plr->y, plr->x, HIDDEN);
		nb_flag = nb_neighbors_status(b, plr->y, plr->x, FLAG);
		proximity = b->cells[plr->y][plr->x].proximity;
		progress = true;

		if((b->cells[plr->y][plr->x].status == KNOWN) && (proximity > 0)){
			if(nb_hidden+nb_flag == proximity){
				get_neighbors(plr->y, plr->x, neighs_y, neighs_x);
				i = 0;
				while((i < 8) && (!exists(b, neighs_y[i], neighs_x[i]) || (b->cells[neighs_y[i]][neighs_x[i]].status != HIDDEN))) i++;
				if(i < 8){
					disp_msg(ui, b, "flag");
					b->cells[neighs_y[i]][neighs_x[i]].status = FLAG;
					progress = false;
					print_board(ui, b);
					wmove(ui->main_win, y0+plr->y, x0+plr->x);
					ui_refresh(ui);
					msleep(100);
				}	
			}else if((nb_flag == proximity) && (nb_hidden > 0)){
				get_neighbors(plr->y, plr->x, neighs_y, neighs_x);
				i = 0;
				while((i < 8) && (!exists(b, neighs_y[i], neighs_x[i]) || (b->cells[neighs_y[i]][neighs_x[i]].status != HIDDEN))) i++;
				if(i < 8){
					disp_msg(ui, b, "press");
					press_cell(b, neighs_y[i], neighs_x[i]);
					test_end_of_game(b, plr);
					progress = false;
					print_board(ui, b);
					wmove(ui->main_win, y0+plr->y, x0+plr->x);
					ui_refresh(ui);
					msleep(100);
				}
			}
		}

		if(progress){
			disp_msg(ui, b, "next");
			wmove(ui->main_win, y0+plr->y, x0+plr->x);
			plr->x++;
			if(plr->x >= b->w){
				plr->x = 0;
				plr->y++;
				if(plr->y >= b->h){
					plr->y = 0;
				}
			}
		}
	}

	curs_set(FALSE);
	return;
}

