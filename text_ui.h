#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include "game.h"
#include "data_struct.h"

/**
 * \file text_ui.h
 * \brief Text User Interface definition
 *
 * The functions and data here use ncurses.h in order to display information in
 * the GNU/Linux terminal.
 */

///\defgroup controls_text_ui_gp Key Bindings in the Text UI
///\{
static const int CTRLS_LEFT[]	= {KEY_LEFT,	't'};
static const int CTRLS_DOWN[]	= {KEY_DOWN,	's'};
static const int CTRLS_UP[]	= {KEY_UP,	'r'};
static const int CTRLS_RIGHT[]	= {KEY_RIGHT,	'n'};
static const int CTRLS_FLAG[]	= {'f',		'f'};
static const int CTRLS_OK[]	= {'\n',	' '};
static const int CTRLS_QUIT[]	= {'q',		'q'};
///\}

///\defgroup general_text_ui_gp General Text UI functions and data
///\{

///\brief User Interface data structure
typedef struct{
	WINDOW *main_win; ///< \brief Pointer to main curses window.
	enum { CONTINUE, QUIT } signal; ///< \brief message to the outside.
} UI;
UI *ui_init();
void ui_terminate(UI *);
void ui_clear(UI *);
void ui_pause(UI *);
void ui_refresh(UI *);
///\}

///\brief Color pair identifiers
typedef enum{
	CP_NULL,
	CP_HIDDEN,
	CP_FLAG,
	CP_BOMB,
	CP_ZERO,
	CP_ONE,
	CP_TWO,
	CP_THREE,
	CP_FOUR,
	CP_FIVE,
	CP_SIX,
	CP_SEVEN,
	CP_EIGHT
} ColorPairID;

//Input and Output
void print_board(UI *, Board *);
int select_cell(UI *, Board *, Player *);
void disp_msg(UI *, Board *, char []);
void msleep(float);

#endif

