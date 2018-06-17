/*------------------------------------------------------------------------------
*
*   LABIRINTO DO DRAGAO
*   (C) Marco/2010 - Fernando Aires Castello
*
*   PLAYER.CPP
*
*   Classe Player representa o jogador com seus itens
*
*-----------------------------------------------------------------------------*/

#include <stdlib.h>
#include "player.h"
#include "maze.h"

Player::Player()
{
    setxy( PLAYER_START_X, PLAYER_START_Y );    // Inicializa coordenadas
    set_rope(false);    // Inicia sem a corda
    set_arrow(false);   // Inicia sem a flecha
}

void Player::setxy(char ix, char iy)
{
    setx( ix );
    sety( iy );
}

void Player::setx(char ix)
{
    // Se ultrapassar os limites, vai parar do lado extremo oposto
    if ( ix >= MAZE_WIDTH ) ix = 0;
    if ( ix < 0 ) ix = MAZE_WIDTH - 1;

    x = ix;
}

void Player::sety(char iy)
{
    // Se ultrapassar os limites, vai parar do lado extremo oposto
    if ( iy >= MAZE_HEIGHT ) iy = 0;
    if ( iy < 0 ) iy = MAZE_HEIGHT - 1;

    y = iy;
}

void Player::setxy_random()
{
    // Define duas coordenadas aleatoriamente
    char random_ix = (char) rand() % MAZE_WIDTH;
    char random_iy = (char) rand() % MAZE_HEIGHT;

    // Coloca o jogador nas coordenadas aleatorias
    setxy( random_ix, random_iy );
}

void Player::set_rope(bool rope_flag)
{
    rope = rope_flag;   // Da (true) ou tira (false) a corda do jogador
}

void Player::set_arrow(bool arrow_flag)
{
    arrow = arrow_flag; // Da (true) ou tira (false) a flecha do jogador
}
