/*------------------------------------------------------------------------------
*
*   LABIRINTO DO DRAGAO
*   (C) Marco/2010 - Fernando Aires Castello
*
*   MAZE.CPP
*
*   Classe Maze representa o labirinto do jogo com suas salas e objetos
*
*-----------------------------------------------------------------------------*/

#include <stdlib.h>
#include <curses.h>
#include "maze.h"
#include "screen.h"

Maze::Maze()
{
    init_maze();    // Inicializa o labirinto
}

Maze::~Maze()
{
    delete[] rooms;     // Libera a memoria
}

void Maze::init_maze()
{
    // Inicializa o labirinto.
    // Inicialmente todas as salas estao vazias (contem um OBJ_NOTHING)
    
    for ( int y = 0; y < MAZE_HEIGHT; y++ )
        for ( int x = 0; x < MAZE_WIDTH; x++ )
            put( x, y, OBJ_NOTHING );

    // Coloca um objeto que representa a entrada do labirinto, na mesma posicao
    // reservada onde o jogador vai iniciar. Isso evita que o labirinto coloque
    // qualquer tipo de objeto nessa posicao (apenas o jogador pode estar nessa posicao)
    
    put( PLAYER_START_X, PLAYER_START_Y, OBJ_ENTRANCE );
}

void Maze::reset()
{
    // Faz a mesma coisa que init_maze(), mas init_maze() eh privado.
    for ( int y = 0; y < MAZE_HEIGHT; y++ )
        for ( int x = 0; x < MAZE_WIDTH; x++ )
            put( x, y, OBJ_NOTHING );

    put( PLAYER_START_X, PLAYER_START_Y, OBJ_ENTRANCE );
}

byte Maze::get(int ix, int iy)
{
    // Os if's a seguir evitam que o metodo tente acessar memoria externa aos
    // limites do array que representa o labirinto
    
    if ( ix > MAZE_WIDTH-1 ) ix = 0;
    if ( iy > MAZE_HEIGHT-1 ) iy = 0;
    if ( ix < 0 ) ix = MAZE_WIDTH - 1;
    if ( iy < 0 ) iy = MAZE_HEIGHT - 1;

    // Acessa o indice especificado do array e retorna o valor que esta la
    return rooms[iy][ix];
}

void Maze::put(byte ix, byte iy, byte object)
{
    // Os 2 if's a seguir evitam que o metodo tente acessar memoria externa aos
    // limites do array que representa o labirinto
    
    if ( iy >= MAZE_HEIGHT ) iy = MAZE_HEIGHT - 1;
    if ( ix >= MAZE_WIDTH  ) ix = MAZE_WIDTH  - 1;
    
    // Este 'if' evita que o metodo coloque no array um valor que represente um
    // tipo invalido de objeto, ou seja, evita colocar no labirinto um tipo de
    // objeto desconhecido pelo sistema (aqui coloca um OBJ_NOTHING no lugar)
    
    if ( object >= MAX_OBJECTS ) object = OBJ_NOTHING;
    
    // Acessa o indice especificado do array e coloca la o valor especificado
    rooms[iy][ix] = object;
}

void Maze::populate()
{
    // Representam coordenadas aleatorias
    byte random_ix = 0;
    byte random_iy = 0;

    // Distribui armadilhas aleatoriamente pelo labirinto
    for ( int i = 0; i < MAX_PITS; i++ )
    {
        // Atribui valores aleatorios
        random_ix = (byte) rand() % MAZE_WIDTH;
        random_iy = (byte) rand() % MAZE_HEIGHT;

        // Verifica se aquela posicao esta vazia...
        if ( get( random_ix, random_iy ) == OBJ_NOTHING )
        {
            // Se estiver vazia, coloca uma armadilha la
            put( random_ix, random_iy, OBJ_PIT );
        }
        else // Senao, repete o laco ate encontrar uma posicao vazia
        {
            i--;
            continue;
        }
    }

    // Coloca o tesouro numa sala aleatoria do labirinto
    put_rnd(OBJ_GOLD);
    // Coloca a corda numa sala aleatoria do labirinto
    put_rnd(OBJ_ROPE);
    // Coloca o feiticeiro numa sala aleatoria do labirinto
    put_rnd(OBJ_WIZARD);
    // Coloca o buraco-negro numa sala aleatoria do labirinto
    put_rnd(OBJ_BLACKHOLE);
    // Coloca o dragao numa sala aleatoria do labirinto
    put_rnd(OBJ_DRAGON);
    // Coloca a flecha numa sala aleatoria do labirinto
    put_rnd(OBJ_ARROW);
}

void Maze::put_rnd(byte object)
{
    // Representam coordenadas aleatorias
    byte random_ix = 0;
    byte random_iy = 0;

    while ( true )  // Que seja infinito enquanto dure
    {
        // Atribui valores aleatorios
        random_ix = (byte) rand() % MAZE_WIDTH;
        random_iy = (byte) rand() % MAZE_HEIGHT;

        // Se aquela posicao estiver vazia...
        if ( get( random_ix, random_iy ) == OBJ_NOTHING )
        {
            // Coloca o objeto especificado la e...
            put( random_ix, random_iy, object );
            // Sai fora do laco infinito!
            break;
        }
        else
            // ...Senao, continua infinitamente ate encontrar uma posicao vazia!
            // Problema: se nao tiver nenhuma posicao vazia, trava tudo!
            // Tenho que arrumar isso...
            continue;
    }
}

void Maze::debug(bool show_objects)
{
    chtype object_rep = '?';    // Caractere que representa um tipo de objeto

    for ( int y = 0; y < MAZE_HEIGHT; y++ )
    {
        for ( int x = 0; x < MAZE_WIDTH; x++ )
        {
            switch ( rooms[y][x] )  // Olha numa posicao do array...
            {
                // Dependendo do valor que estiver naquela posicao, define
                // o caractere/cor que representara aquele valor no mapa:

                case OBJ_NOTHING: object_rep = '.';
                color( CBLACK, true );
                break;
                case OBJ_ENTRANCE: object_rep = 'E';
                color( CGREEN, true );
                break;
                case OBJ_GOLD: object_rep = 'T';
                color( CYELLOW, true );
                break;
                case OBJ_ARROW: object_rep = 'A';
                color( CBLUE, true );
                break;
                case OBJ_ROPE: object_rep = 'R';
                color( CCYAN, true );
                break;
                case OBJ_PIT: object_rep = 'P';
                color( CBLACK, true );
                break;
                case OBJ_WIZARD: object_rep = 'W';
                color( CMAGENTA, true );
                break;
                case OBJ_BLACKHOLE: object_rep = 'B';
                color( CWHITE, true );
                break;
                case OBJ_DRAGON: object_rep = 'D';
                color( CRED, true );
                break;
                
                // Mostra um '?' caso encontre um tipo de objeto desconhecido
                default:object_rep = '?';
                color( CWHITE, false );
                break;
            }

            if ( show_objects ) // Se precisa mostrar os objetos no mapa...
            {
                mvaddch( y, x+50, object_rep ); // Mostra os objetos no mapa
            }
            else
            {
                // Senao, mostra apenas um '.' em todas as posicoes
                color( CBLACK, true );
                mvaddch( y, x+50, '.' );
            }
        }
    }
}
