#include "text_ui.h"

//General UI functions

///\brief UI initialization
UI *ui_init(){
	initscr();			//Starts ncurses mode
	raw();				//Disables line buffering
	noecho();			//Disables input echo
	curs_set(FALSE);		//Hides cursor
	
	//UI struct init.
	UI *ui = (UI *) malloc(sizeof(UI));
	ui->main_win = newwin(LINES, COLS, 0, 0);
	keypad(ui->main_win, TRUE);	//Enable special keys

	ui->signal = CONTINUE;
	
	//Colors
	if(has_colors() == FALSE){
		endwin();
		printf("Your terminal does not support color.\n");
		exit(1);
	}
	start_color();
	init_pair(CP_HIDDEN,	COLOR_WHITE,	COLOR_WHITE);
	init_pair(CP_FLAG,	COLOR_RED,	COLOR_WHITE);
	init_pair(CP_BOMB,	COLOR_RED,	COLOR_RED);
	init_pair(CP_ZERO,	COLOR_BLACK,	COLOR_BLACK);
	init_pair(CP_ONE,	COLOR_GREEN,	COLOR_BLACK);
	init_pair(CP_TWO,	COLOR_BLUE,	COLOR_BLACK);
	init_pair(CP_THREE,	COLOR_YELLOW,	COLOR_BLACK);
	init_pair(CP_FOUR,	COLOR_RED,	COLOR_BLACK);
	init_pair(CP_FIVE,	COLOR_CYAN,	COLOR_BLACK);
	init_pair(CP_SIX,	COLOR_MAGENTA,	COLOR_BLACK);
	init_pair(CP_SEVEN,	COLOR_WHITE,	COLOR_BLACK);
	init_pair(CP_EIGHT,	COLOR_RED,	COLOR_BLACK);

	//Display
	ui_clear(ui);
	return ui;
}

///\brief UI termination
void ui_terminate(UI *ui){
	wrefresh(ui->main_win);
	delwin(ui->main_win);
	endwin();
	return;
}

///\brief Correctly clear screen
void ui_clear(UI *ui){
	wstandend(ui->main_win);
	wclear(ui->main_win);
	wrefresh(ui->main_win);
	return;
}

///\brief Simply waits for key press
void ui_pause(UI *ui){
	wgetch(ui->main_win);
	return;
}

///\brief Board display
void print_board(UI *ui, Board *b){
	int y0 = (LINES-b->h)/2;
	int x0 = (COLS-b->w)/2;
	int i, j;
	wstandend(ui->main_win);
	mvwaddch(ui->main_win, y0-1, x0-1, ACS_ULCORNER);
	mvwaddch(ui->main_win, y0-1, x0+b->w, ACS_URCORNER);
	mvwaddch(ui->main_win, y0+b->h, x0-1, ACS_LLCORNER);
	mvwaddch(ui->main_win, y0+b->h, x0+b->w, ACS_LRCORNER);
	mvwhline(ui->main_win, y0-1, x0, 0, b->w);
	mvwhline(ui->main_win, y0+b->h, x0, 0, b->w);
	mvwvline(ui->main_win, y0, x0-1, 0, b->h);
	mvwvline(ui->main_win, y0, x0+b->w, 0, b->h);
	for(i=0; i<b->h; i++){
		for(j=0; j<b->w; j++){
			switch(b->cells[i][j].status){
			case KNOWN:
				wattrset(ui->main_win, COLOR_PAIR(CP_ZERO+b->cells[i][j].proximity)|A_BOLD);
				if(b->cells[i][j].proximity == -1){
					mvwprintw(ui->main_win, y0+i, x0+j, "*");
				}else if(b->cells[i][j].proximity == 0){
					mvwprintw(ui->main_win, y0+i, x0+j, " ");
				}else{
					mvwprintw(ui->main_win, y0+i, x0+j, "%d", b->cells[i][j].proximity);
				}
				break;
			case HIDDEN:
				wattrset(ui->main_win, COLOR_PAIR(CP_HIDDEN)|A_BOLD);
				mvwprintw(ui->main_win, y0+i, x0+j, "-");
				break;
			case FLAG:
				wattrset(ui->main_win, COLOR_PAIR(CP_FLAG) | A_BOLD);
				mvwprintw(ui->main_win, y0+i, x0+j, "?");
				break;
			default:
				break;
			}
		}
	}
	return;
}

/**
 * \brief Cell selection
 * \return -1 to do nothing, 0 to flag, 1 to press cell
 */
int select_cell(UI *ui, Board *b, Player *plr){
	int y0 = (LINES-b->h)/2;
	int x0 = (COLS-b->w)/2;
	curs_set(TRUE);
	wmove(ui->main_win, y0+plr->y, x0+plr->x);
	int action;
	int ch;
	bool cont = true;
	while(cont){
		ch = wgetch(ui->main_win);
		if(((ch == CTRLS_LEFT[0]) || (ch == CTRLS_LEFT[1])) && (plr->x > 0)){
			plr->x--;
		}else if(((ch == CTRLS_DOWN[0]) || (ch == CTRLS_DOWN[1])) && (plr->y < b->h-1)){
			plr->y++;
		}else if(((ch == CTRLS_UP[0]) || (ch == CTRLS_UP[1])) && (plr->y > 0)){
			plr->y--;
		}else if(((ch == CTRLS_RIGHT[0]) || (ch == CTRLS_RIGHT[1])) && (plr->x < b->w-1)){
			plr->x++;
		}else if((ch == CTRLS_QUIT[0]) || (ch == CTRLS_QUIT[1])){
			action = -1;
			cont = false;
		}else if((ch == CTRLS_OK[0]) || (ch == CTRLS_OK[1])){
			action = 1;
			cont = false;
		}else if((ch == CTRLS_FLAG[0]) || (ch == CTRLS_FLAG[1])){
			action = 0;
			cont = false;
		}
		wmove(ui->main_win, y0+plr->y, x0+plr->x);
	}
	curs_set(FALSE);
	return action;
}

///\brief A simple message display function
void disp_msg(UI *ui, Board *b, char msg[]){
	int y = (LINES-b->h)/2-2;
	int x = (COLS-strlen(msg))/2;
	wstandend(ui->main_win);
	mvwhline(ui->main_win, y, 0, ' ', COLS);
	wattrset(ui->main_win, COLOR_PAIR(CP_FLAG) | A_BOLD);
	mvwaddstr(ui->main_win, y, x, msg);
	return;
}

