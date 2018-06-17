/*------------------------------------------------------------------------------
*
*   LABIRINTO DO DRAGAO
*   (C) Marco/2010 - Fernando Aires Castello
*
*   GAME.H
*
*   Classe Game representa o jogo com o labirinto e o jogador.
*   Controla as interacoes entre jogador, labirinto e objetos do labirinto.
*   Mostra as telas de titulo, game over, creditos, etc.
*   Conta cada passo dado pelo jogador e cada tesouro encontrado por ele.
*   Calcula a pontuacao do jogador e contem o loop principal do jogo.
*
*-----------------------------------------------------------------------------*/

#ifndef _GAME_H_
#define _GAME_H_

#include "maze.h"
#include "player.h"

class Game
{
    //--------------------------------------------------------------------------
    //  Interface Publica
    //--------------------------------------------------------------------------
    public:
    Game();         // Construtor
    ~Game();        // Destrutor

    void init();    // Inicializa o jogo
    void quit();    // Encerra o jogo
    void test();    // Serve para debug
    void run();     // Loop principal do jogo
    void restart(); // Reinicia o jogo

    bool is_running() { return running; }   // Indica se o jogo esta rodando

    void show_title_screen();       // Mostra a tela de titulo do jogo
    void show_game_over_screen();   // Mostra a tela de game over
    void show_try_again_screen();   // Mostra a tela que pergunta se quer jogar novamente
    void show_winner_screen();      // Mostra a tela quando o jogador vence

    void show_instructions();       // Mostra a tela de instrucoes
    void show_credits();            // Mostra os creditos do jogo

    void init_maze();               // Inicializa o labirinto

    void show_room();               // Desenha a sala do labirinto
    void show_adjacent_objects();   // Mostra objetos q estao nas salas adjac.
    void show_player();             // Mostra os itens do jogador e sua posicao

    void show_map();                // Mostra o mapa do labirinto (modo cheat)
    void show_gameover_map();       // Mostra o mapa do labirinto (game over)

    void show_player_move();        // Mostra a mensagem do jogador se movendo
    void show_player_shoot();       // Mostra a mensagem do jogador atirando
    void show_player_missed();      // Mostra a msg. da flecha caindo ao chao
    void show_player_fall();        // Mostra mensagem caindo na armadilha
    void show_player_escape();      // Mostra mensagem escapando da armadilha
    void show_player_dead();        // Mostra mensagem do jogador morrendo
    void show_wizard();             // Mostra feiticeiro encontrado
    void show_rope();               // Mostra corda encontrada
    void show_arrow();              // Mostra flecha encontrada
    void show_treasure();           // Mostra tesouro encontrado
    void show_blackhole();          // Mostra buraco-negro encontrado
    void show_dragon_dead();        // Mostra dragao morto
    void detect_object_in_room();   // Detecta o objeto na sala

    // Aqui poderia ser shoot_arrow(int direcao), mas nao eh apenas a direcao
    // que muda neste caso
    void shoot_arrow_north();       // Atira flecha para o norte
    void shoot_arrow_south();       // Atira flecha para o sul
    void shoot_arrow_east();        // Atira flecha para o leste
    void shoot_arrow_west();        // Atira flecha para o oeste

    void calculate_score();         // Calcula a pontuacao do jogador
    void load_hiscore();            // Carrega o highscore do arquivo
    void save_hiscore();            // Salva o highscore do arquivo

    //--------------------------------------------------------------------------
    //  Dados privados
    //--------------------------------------------------------------------------
    private:

    bool cheating;      // True se o jogador estiver no modo 'trapacear' (cheat)
    bool running;       // True se o jogo estiver rodando
    byte treasures_found;    // Numero de tesouros encontrados
    Maze* maze;         // Ponteiro para o labirinto
    Player* player;     // Ponteiro para o jogador
    uint score;         // Pontuacao final
    uint moves;         // Quantidade de movimentos do jogador

};

#endif
