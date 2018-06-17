/*------------------------------------------------------------------------------
*
*   LABIRINTO DO DRAGAO
*   (C) Marco/2010 - Fernando Aires Castello
*
*   SCREEN.H
*
*   Funcoes globais para inicializar a biblioteca Curses e
*   para manipular as cores da tela
*
*-----------------------------------------------------------------------------*/

#ifndef _SCREEN_H_
#define _SCREEN_H_

#include "defs.h"

// Estes valores representam as cores para escrever na tela

#define CBLUE        1  // Azul
#define CGREEN       2  // Verde
#define CCYAN        3  // Ciano (azul celeste)
#define CRED         4  // Vermelho
#define CMAGENTA     5  // Magenta (roxo?)
#define CYELLOW      6  // Amarelo
#define CWHITE       7  // Branco
#define CBLACK       8  // Preto (ou cinza)

void init_curses();         // Inicializa a biblioteca Curses
void init_color_pairs();    // Inicializa pares de cores
void color(byte, bool);     // Muda a cor das proximas palavras escritas
                            // O primeiro arg. representa o codigo de cor;
                            // O segundo arg. indica se a cor eh intensa;

#endif
