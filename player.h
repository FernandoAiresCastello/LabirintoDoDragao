/*------------------------------------------------------------------------------
*
*   LABIRINTO DO DRAGAO
*   (C) Marco/2010 - Fernando Aires Castello
*
*   PLAYER.H
*
*   Classe Player representa o jogador com seus itens
*
*-----------------------------------------------------------------------------*/

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "defs.h"

class Player
{
    //--------------------------------------------------------------------------
    //  Interface Publica
    //--------------------------------------------------------------------------
    public:
    Player();   // Construtor
    
    void setxy(char, char);     // Configura coordenadas X e Y de uma vez so
    void setx(char);            // Configura coordenada X
    void sety(char);            // Configura coordenada Y
    void setxy_random();        // Configura aleatoriamente as coordenadas X e Y

    void set_rope(bool);        // Define se possui a corda (true) ou nao (false)
    void set_arrow(bool);       // Define se possui a flecha (true) ou nao (false)

    int getx() { return x; }    // Retorna X
    int gety() { return y; }    // Retorna Y
    bool has_rope() { return rope; }    // Retorna se possui corda (true) ou nao (false)
    bool has_arrow() { return arrow; }  // Retorna se possui flecha (true) ou nao (false)

    //--------------------------------------------------------------------------
    //  Dados privados
    //--------------------------------------------------------------------------
    private:

    byte x;     // Coordenada X
    byte y;     // Coordenada Y

    bool rope;  // Possui a corda? (true = sim, false = nao)
    bool arrow; // Possui a flecha? (true = sim, false = nao)
};

#endif
