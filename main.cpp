/*------------------------------------------------------------------------------
*
*   LABIRINTO DO DRAGAO
*   (C) Marco/2010 - Fernando Aires Castello
*
*   MAIN.CPP
*
*   Contem apenas a funcao main() que trata de instanciar o jogo (um objeto da
*   classe Game) e invocar metodos essenciais para preparar e iniciar o jogo.
*
*-----------------------------------------------------------------------------*/

#include "game.h"

// Este codigo indica o sucesso da execucao de main()
#define OK      0

// Aqui apenas a funcao main()!
int main(int argc, char* argv)
{
    // Instancia o jogo
    Game game;
    // Incializa o jogo
    game.init();
    // Mostra a tela de titulo do jogo
    game.show_title_screen();
    // Mostra as instrucoes de como jogar
    game.show_instructions();
    // Roda o jogo (executa o loop principal que eh infinito enquanto durar)
    game.run();

    // Tudo OK...
    return OK;
}
