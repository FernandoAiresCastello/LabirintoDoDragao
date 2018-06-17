/*------------------------------------------------------------------------------
*
*   LABIRINTO DO DRAGAO
*   (C) Marco/2010 - Fernando Aires Castello
*
*   MAZE.H
*
*   Classe Maze representa o labirinto do jogo com suas salas e objetos
*
*-----------------------------------------------------------------------------*/

#ifndef _MAZE_H_
#define _MAZE_H_

#include "defs.h"

// Dimensoes do labirinto, medido em "salas"
// ( total 10x10 = 100 salas :P )
#define MAZE_WIDTH      10          // 10 "salas" de largura
#define MAZE_HEIGHT     10          // 10 "salas" de altura

// Valores atribuidos aos indices do array bidimensional que representa o labirinto
// Cada valor desses representa um tipo de objeto que pode estar numa sala do labirinto

// Nao poderia ter usado um enum aqui???
// Estou mais acostumado com #defines :P

#define OBJ_NOTHING         0   // Representa "vazio" (sala nao tem nenhum objeto)
#define OBJ_GOLD            1   // Representa "tesouro"
#define OBJ_WIZARD          2   // Representa "feiticeiro"
#define OBJ_ROPE            3   // Representa "corda magica"
#define OBJ_PIT             4   // Representa "armadilha"
#define OBJ_ARROW           5   // Representa "flecha"
#define OBJ_DRAGON          6   // Representa "dragao"
#define OBJ_ENTRANCE        7   // Representa a entrada do labirinto
#define OBJ_BLACKHOLE       8   // Representa "buraco-negro"
#define MAX_OBJECTS         9   // Indica o numero maximo de tipos de objeto

#define MAX_PITS            10  // Indica o numero maximo de armadilhas no labirinto

// Indicam a sala onde o jogador inicia (indices [0][0] do array que representa o labirinto)
#define PLAYER_START_X      0
#define PLAYER_START_Y      0

class Maze
{
    //--------------------------------------------------------------------------
    //  Interface Publica
    //--------------------------------------------------------------------------
    public:

    Maze();     // Construtor
    ~Maze();    // Destrutor

    void put(byte, byte, byte);     // Coloca um objeto na sala especificada
    void put_rnd(byte);             // Coloca o objeto especificado numa sala aleatoria
    byte get(int, int);             // Obtem o objeto da sala especificada

    void populate();    // Distribui aleatoriamente objetos nas salas do labirinto
    void reset();       // Reinicializa o labirinto
    void debug(bool);   // Mostra um mapa do labirinto para testes
                        // (true = mostra objetos; false = nao mostra objetos)

    //--------------------------------------------------------------------------
    //  Dados privados
    //--------------------------------------------------------------------------
    private:
    /*
       Array bidimensional representa o "complexo de 100 salas" (10x10).
       Cada elemento do array pode ser um valor entre 0 e 255, onde cada valor
       eh interpretado como sendo um tipo de objeto (veja os #define logo no
       inicio deste arquivo. Cada sala pode ter apenas um objeto (pois cada
       indice deste array so pode ter um unico valor).
    */
    byte rooms[MAZE_HEIGHT][MAZE_WIDTH];

    //--------------------------------------------------------------------------
    //  Metodos utilitarios
    //--------------------------------------------------------------------------
    void init_maze();   // Inicializa o labirinto com valores consistentes
};

#endif
