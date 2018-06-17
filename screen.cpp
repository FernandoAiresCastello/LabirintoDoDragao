/*------------------------------------------------------------------------------
*
*   LABIRINTO DO DRAGAO
*   (C) Marco/2010 - Fernando Aires Castello
*
*   SCREEN.CPP
*
*   Funcoes globais para inicializar a biblioteca Curses e
*   para manipular as cores da tela
*
*-----------------------------------------------------------------------------*/

// Para saber detalhes do que essas funcoes fazem, veja a documentacao da
// biblioteca PDCurses.

#include <curses.h>
#include "screen.h"

void init_curses()
{
    initscr();
    start_color();
    cbreak();
    keypad(stdscr, true);
    nodelay(stdscr, false);
    noecho();
}

void init_color_pairs()
{
    init_pair( CBLUE, COLOR_BLUE, COLOR_BLACK );
    init_pair( CGREEN, COLOR_GREEN, COLOR_BLACK );
    init_pair( CCYAN, COLOR_CYAN, COLOR_BLACK );
    init_pair( CRED, COLOR_RED, COLOR_BLACK );
    init_pair( CMAGENTA, COLOR_MAGENTA, COLOR_BLACK );
    init_pair( CYELLOW, COLOR_YELLOW, COLOR_BLACK );
    init_pair( CWHITE, COLOR_WHITE, COLOR_BLACK );
    init_pair( CBLACK, COLOR_BLACK, COLOR_BLACK );
}

void color(byte color_code, bool intense)
{
    if ( intense )
        attrset( COLOR_PAIR( color_code ) | A_BOLD );
    else
        attrset( COLOR_PAIR( color_code ) | A_NORMAL );
}
