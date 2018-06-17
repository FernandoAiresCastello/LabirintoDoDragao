/*------------------------------------------------------------------------------
*
*   LABIRINTO DO DRAGAO
*   (C) Marco/2010 - Fernando Aires Castello
*
*   GAME.CPP
*
*   Classe Game representa o jogo com o labirinto e o jogador.
*   Controla as interacoes entre jogador, labirinto e objetos do labirinto.
*   Mostra as telas de titulo, game over, creditos, etc.
*   Conta cada passo dado pelo jogador e cada tesouro encontrado por ele.
*   Calcula a pontuacao do jogador e contem o loop principal do jogo.
*
*-----------------------------------------------------------------------------*/

#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include "game.h"
#include "defs.h"
#include "screen.h"

//------------------------------------------------------------------------------
//  METODOS PARA INICIALIZACAO, FINALIZACAO E DEBUG
//------------------------------------------------------------------------------

Game::Game() : cheating(false), treasures_found(0), score(0), moves(0)
{
    maze = new Maze;        // Instancia o labirinto
    player = new Player;    // Instancia o jogador
}

Game::~Game()
{
    quit();     // Finaliza o jogo
}

void Game::init()
{
    init_curses();          // Inicializa a biblioteca Curses
    init_color_pairs();     // Inicializa pares de cores
    curs_set(false);        // Cursor invisivel

    srand( time (NULL) );   // Inicializa o gerador de numeros aleatorios

    init_maze();            // Inicializa um labirinto
}

void Game::quit()
{
    clear();        // Limpa a tela
    refresh();      // Atualiza a tela (senao o clear() nao tem efeito...)
    curs_set(true); // Torna o cursor visivel

    delete maze;    // Adeus, dados do labirinto
    maze = NULL;    // Ponteiro aponta pra lugar-nenhum (ainda bem)

    delete player;  // Adeus, dados do jogador
    player = NULL;  // Ponteiro aponta pra lugar-nenhum (ainda bem)

    running = false;    // Sinaliza que o jogo nao esta mais rodando
}

void Game::restart()
{
    // Recomeca tudo do zero
    
    init_maze();    // Inicializa dados do labirinto
    player->setxy(PLAYER_START_X, PLAYER_START_Y);  // Coloca o jogador na posicao inicial
    player->set_rope(false);    // Recomeca sem a corda
    player->set_arrow(false);   // Recomeca sem a flecha

    score = 0;              // Zero pontos
    treasures_found = 0;    // Zero tesouros encontrados
    moves = 0;              // Zero movimentos

    run();  // Roda o jogo denovo desde o inicio
}

void Game::init_maze()
{
    maze->reset();      // Inicializa o labirinto
    maze->populate();   // Enche o labirinto aleatoriamente com os objetos
}

void Game::test()   // Metodo serve apenas para testes
{
    noecho();           // Teclas pressionadas nao tem o valor mostrado na tela
    curs_set(false);    // Cursor invisivel
    chtype keypressed = 0;  // Ultima tecla pressionada

    do  // Fica inicializando e mostrando um mapa aleatorio
    {
        maze->reset();      // Reseta o mapa
        maze->populate();   // Inicializa o mapa aleatoriamente com os objetos

        show_map();         // Mostra o mapa

        keypressed = getch();   // Aguarda pressionar uma tecla
    }
    while ( keypressed != ALT_Q );  // Repete tudo ate pressionar ALT+Q
}

void Game::show_map()
{
    if ( cheating )             // Se estiver no modo 'cheat'...
        maze->debug( true );    // Mostra o mapa indicando os objetos
    else                        // Senao...
        maze->debug( false );   // Mostra o mapa sem indicar os objetos
    
    // A seguir, mostra um 'X' no mapa para indicar a localizacao do jogador
    color( CGREEN, true );
    // Acrescenta-se 50 para deslocar a posicao do simbolo, ficando sobre o mapa
    mvaddch( player->gety(), player->getx() + 50, 'X' );
}

//------------------------------------------------------------------------------
//  METODOS PARA MOSTRAR TELAS DE TITULO, CONGRATULACOES E GAME OVER
//------------------------------------------------------------------------------

void Game::show_title_screen()
{
    clear();    // Apaga a tela
    flash();    // Pisca a tela (se possivel)

    color( CGREEN, true );
    mvprintw( 1, 28, "LABIRINTO DO DRAGAO v1.0" );
    color( CWHITE, true );
    mvprintw( 3, 20, "%c Marco/2010 - Fernando Aires Castello", 0x1b8 );

    color( CCYAN, true );
    mvprintw( 11, 27, "TERRIBILIS EST LOCUS IST" );
    color( CCYAN, true );
    mvprintw( 13, 11, "Voce conseguira encontrar a toca do dragao e derrota-lo?" );

    color( CWHITE, true );
    mvprintw( 23, 22, "*** PRESSIONE QUALQUER TECLA ***" );

    getch();    // Trava tudo, esperando pressionar uma tecla qualquer
}

void Game::show_winner_screen()
{
    clear();
    flash();

    color( CGREEN, true );
    mvprintw( 7, 1, "*** PARABENS, VOCE VENCEU O LABIRINTO DO DRAGAO! ***" );

    color( CWHITE, false );
    mvprintw( 9, 1, "Voce realizou %d movimentos.", moves );
    mvprintw( 10, 1, "Voce encontrou %d tesouros.", treasures_found );

    calculate_score();  // Calcula a pontuacao para exibi-la a seguir

    color( CWHITE, true );
    mvprintw( 12, 1, "                 Pontuacao final:" );
    color( CYELLOW, true );
    mvprintw( 13, 1, "                                   %d pontos", score );

    getch();
    show_gameover_map();    // Mostra o mapa completo com os objetos

    getch();
    show_try_again_screen();    // Pergunta se quer jogar denovo
}

void Game::show_game_over_screen()
{
    clear();
    flash();
    beep();

    color( CRED, true );
    mvprintw( 7, 1, "*** SINTO MUITO, MAS VOCE MORREU! MWAHWAHWA! ***" );
    color( CWHITE, false );
    mvprintw( 9, 1, "Voce realizou %d movimentos.", moves );
    mvprintw( 10, 1, "Voce encontrou %d tesouros.", treasures_found );
    color( CWHITE, true );
    mvprintw( 12, 1, "Como voce nao conseguiu derrotar o dragao do labirinto," );
    mvprintw( 13, 1, "a sua pontuacao nao sera mostrada! Tente novamente!" );

    getch();
    show_gameover_map();    // Mostra tela de game over

    getch();
    show_try_again_screen();    // Pergunta se quer jogar denovo
}

void Game::show_gameover_map()
{
    clear();
    flash();

    maze->debug( true );    // Mostra o mapa com os objetos

    color( CGREEN, true );
    mvprintw( 1, 1, "LABIRINTO DO DRAGAO" );

    color( CWHITE, true );
    mvprintw( 3, 1, "Confira o mapa do labirinto onde voce estava:" );

    color( CBLACK, true );
    mvprintw( 5, 1, ". = Vazio" );
    color( CGREEN, true );
    mvprintw( 6, 1, "E = Entrada" );
    color( CYELLOW, true );
    mvprintw( 7, 1, "T = Tesouro" );
    color( CCYAN, true );
    mvprintw( 8, 1, "R = Corda" );
    color( CBLUE, true );
    mvprintw( 9, 1, "A = Flecha" );
    color( CBLACK, true );
    mvprintw( 10, 1, "P = Armadilha" );
    color( CMAGENTA, true );
    mvprintw( 11, 1, "W = Feiticeiro" );
    color( CWHITE, true );
    mvprintw( 12, 1, "B = Buraco-Negro" );
    color( CRED, true );
    mvprintw( 13, 1, "D = Dragao" );
}

void Game::show_try_again_screen()
{
    clear();
    color( CGREEN, true );
    mvprintw( 10, 25, "*** Deseja jogar novamente? ***" );
    color( CWHITE, true );
    mvprintw( 12, 25, "[ALT+S] Sim!      [ALT+N] Nao...");

    chtype keypressed = 0;  // Ultima tecla pressionada

    while ( running )   // Enquanto o jogo estiver rodando...
    {
        keypressed = getch();   // Espera pressionar uma tecla

        if ( keypressed == ALT_S )  // Se pressionou ALT+S...
        {
            flash();    // Pisca a tela (se possivel)
            restart();  // Reinicia o jogo
        }
        else if ( keypressed == ALT_N ) // Se pressionou ALT+N...
        {
            show_credits(); // Mostra os creditos
            quit();         // Finaliza o jogo
        }
        else            // Senao...
            continue;   // Repete tudo ate pressionar ALT+S ou ALT+N
    }
}

//------------------------------------------------------------------------------
//  METODOS PARA MOSTRAR TELAS DE INTRODUCAO, INSTRUCOES E CREDITOS
//------------------------------------------------------------------------------

void Game::show_instructions()
{
    flash();

    clear();
    color( CGREEN, true );
    mvprintw( 1, 30, "LABIRINTO DO DRAGAO" );
    color( CWHITE, true );
    mvprintw( 3, 26, "Como jogar (pagina 1 de 3):" );

    color( CWHITE, false );
    mvprintw( 5, 1, "O objetivo do jogo e localizar o covil do dragao e atirar a flecha magica na" );
    mvprintw( 6, 1, "direcao dele. Voce nao deve entrar na sala onde o dragao esta! Voce deve"  );
    mvprintw( 7, 1, "atirar a flecha de longe, isto e, de alguma sala adjacente. Voce sabera" );
    mvprintw( 8, 1, "quando o dragao esta proximo, assim que voce sentir a presenca dele. Se voce" );
    mvprintw( 9, 1, "acerta-lo, voce vence o jogo. Senao, a flecha vai parar em outro lugar" );
    mvprintw( 10, 1, "qualquer e voce devera encontra-la novamente e repetir o processo ate vencer." );

    mvprintw( 12, 1, "Voce e capaz de sentir a presenca de todos os objetos do labirinto que" );
    mvprintw( 13, 1, "estiverem em salas adjacentes (imediatamente ao norte, sul, leste ou oeste.)" );

    mvprintw( 15, 1, "TESOURO: Acrescenta um bonus a sua pontuacao final." );
    mvprintw( 16, 1, "CORDA MAGICA: Voce a usa quando cai numa armadilha, dessa forma voce nao" );
    mvprintw( 17, 1, "              morrera, mas ganhara mais uma chance. Ao usar a corda, ela vai" );
    mvprintw( 18, 1, "              parar em algum outro local." );
    mvprintw( 19, 1, "FLECHA MAGICA: Voce precisa dela para derrotar o dragao. Se voce errar o" );
    mvprintw( 20, 1, "               alvo, ela vai parar em algum outro lugar." );
    mvprintw( 21, 1, "FEITICEIRO: Caso voce o encontre, ele lancara um feitico para teletransportar" );
    mvprintw( 22, 1, "            voce a um local qualquer do labirinto. Depois disso ele se" );
    mvprintw( 23, 1, "            teletransporta, isto e, ele muda de lugar tambem." );

    getch();

    clear();
    color( CGREEN, true );
    mvprintw( 1, 30, "LABIRINTO DO DRAGAO" );
    color( CWHITE, true );
    mvprintw( 3, 26, "Como jogar (pagina 2 de 3):" );

    color( CWHITE, false );
    mvprintw( 5, 1, "BURACO-NEGRO: Buracos-negros sao objetos astronomicos hipoteticos... Mas no" );
    mvprintw( 6, 1, "              labirinto do dragao eles comprovadamente existem! E mais, se" );
    mvprintw( 7, 1, "              voce nao tomar cuidado, pode acabar sendo sugado por um! Se isso" );
    mvprintw( 8, 1, "              acontecer, voce sera transportado para outro labirinto" );
    mvprintw( 9, 1, "              completamente diferente, e se voce tiver a flecha e/ou a corda," );
    mvprintw(10, 1, "              voce as perdera. Os tesouros encontrados nao serao perdidos." );

    mvprintw(12, 1, "ARMADILHA: Se voce cair numa armadilha, voce morrera a nao ser que voce possua" );
    mvprintw(13, 1, "           a corda magica. Se voce tiver a corda magica, voce conseguira sair," );
    mvprintw(14, 1, "           e a corda ira parar em algum outro local. Existem diversas" );
    mvprintw(15, 1, "           armadilhas pelo labirinto. Tenha muito cuidado por onde pisa!" );

    mvprintw(17, 1, "DRAGAO: O motivo de voce estar aqui. Ele esta escondido em algum lugar pelo" );
    mvprintw(18, 1, "        labirinto, que pertence a ele. A sala onde ele esta adormecido eh o" );
    mvprintw(19, 1, "        covil dele. Voce deve atirar uma flecha de longe, isto e, de alguma" );
    mvprintw(20, 1, "        sala adjacente (imediatamente ao norte, sul, leste ou oeste), e se" );
    mvprintw(21, 1, "        voce acerta-lo, voce o derrota, salva o dia e vence o jogo!" );

    getch();

    clear();
    color( CGREEN, true );
    mvprintw( 1, 30, "LABIRINTO DO DRAGAO" );
    color( CWHITE, true );
    mvprintw( 3, 26, "Como jogar (pagina 3 de 3):" );

    color( CWHITE, false );
    mvprintw( 5, 1, "O LABIRINTO: Eh um conjunto de 100 salas interligadas. Voce pode ver a sua" );
    mvprintw( 6, 1, "             localizacao no labirinto, isto e, a sala onde voce se encontra" );
    mvprintw( 7, 1, "             na parte superior da tela principal. Sua posicao e indicada no" );
    mvprintw( 8, 1, "             formato LETRA-NUMERO, onde letra representa a coordenada X e o" );
    mvprintw( 9, 1, "             numero representa a coordenada Y. O que importa eh que o tamanho" );
    mvprintw(10, 1, "             do labirinto eh 10x10 (=100 salas), e que como se trata de um" );
    mvprintw(11, 1, "             labirinto magico, ao ultrapassar os limites dele voce emergira" );
    mvprintw(12, 1, "             automaticamente do lado extremo oposto. Alem disso, voce e capaz" );
    mvprintw(13, 1, "             de sentir a presenca de objetos nos lados extremos opostos tambem." );

    mvprintw(15, 1, "A PONTUACAO: O calculo da sua pontuacao final se baseia na quantidade de" );
    mvprintw(16, 1, "             movimentos que voce fez ate matar o dragao. Os tesouros que" );
    mvprintw(17, 1, "             voce encontra sao adicionados como bonus a pontuacao final." );
    mvprintw(18, 1, "             Quanto MENOS movimentos voce fizer, mais pontos ganha. E quanto" );
    mvprintw(19, 1, "             MAIS tesouros encontrar, maior sera o bonus adicionado." );

    color( CGREEN, true );
    mvprintw(21, 1, "Veja todos os comandos disponiveis na parte inferior da tela principal." );
    mvprintw(22, 1, "Se voce gostou deste jogo, por favor me envie um e-mail: romeno19@gmail.com" );
    mvprintw(23, 1, "Se quer outros jogos como esse, visite: http://fernando-aires.blogspot.com/" );

    getch();
}

void Game::show_credits()
{
    clear();
    flash();

    color( CGREEN, true );
    mvprintw( 1, 30, "LABIRINTO DO DRAGAO" );
    color( CWHITE, true );
    mvprintw( 3, 20, "%c Marco/2010 - Fernando Aires Castello", 0x1b8 );

    color( CYELLOW, true );
    mvprintw( 5,  1, "Este jogo eh FREEWARE. Se voce pagou por este jogo, por favor envie nome e en-" );
    mvprintw( 6,  1, "dereco completo do estelionatario, para que eu possa enviar um dragao ate ele." );

    color( CWHITE, true );
    mvprintw(  9, 33, "*** Equipe ***" );
    color( CCYAN, true );
    mvprintw( 11, 18, "Projetado por:    Fernando Aires Castello" );
    mvprintw( 12, 18, "Programado por:   Fernando Aires Castello" );
    mvprintw( 13, 18, "Desenvolvido por: Fernando Aires Castello" );
    mvprintw( 14, 18, "Testado por:      Fernando Aires Castello" );
    mvprintw( 15, 18, "Distribuido por:  Fernando Aires Castello" );
    mvprintw( 16, 18, "Mantido por:      Fernando Aires Castello" );

    color( CWHITE, false );
    mvprintw( 19, 23, "E-mails para: romeno19@gmail.com" );
    mvprintw( 20, 13, "Mais jogos em: http://fernando-aires.blogspot.com/" );

    color( CRED, true );
    mvprintw( 23, 6, "*** NENHUM DRAGAO FOI MALTRATADO DURANTE A PRODUCAO DESTE JOGO ***" );

    getch();
}

//------------------------------------------------------------------------------
//  METODOS PARA APRESENTACAO DA TELA PRINCIPAL DO JOGO
//------------------------------------------------------------------------------

void Game::show_room()
{
    clear();

    color( CGREEN, false );
    mvprintw( 1, 1, "LABIRINTO DO DRAGAO" );

    // Obtem as coordenadas do jogador
    byte room_latitude = player->gety();
    byte room_longitude = player->getx();
    // Transforma o numero numa letra maiuscula que representa esse numero
    char room_longitude_ch = room_longitude + 65;

    color( CWHITE, false );
    mvprintw( 3, 1, "Sua localizacao no labirinto:" );
    color( CGREEN, true );
    mvprintw( 3, 31, "%c-%d", room_longitude_ch, room_latitude );

    // Mostra comandos...
    color ( CWHITE, true );
    mvprintw( 18, 1, "Comandos:");
    color ( CWHITE, false );
    mvprintw( 20, 1, "[F1]           Instrucoes" );
    mvprintw( 21, 1, "[SETA ACIMA]   Ir para o norte   [SETA DIREITA]   Ir para o leste" );
    mvprintw( 22, 1, "[SETA ABAIXO]  Ir para o sul     [SETA ESQUERDA]  Ir para o oeste" );
    mvprintw( 23, 1, "[ALT+A]        Atirar flecha     [ALT+Q]          Desistir do jogo" );
}

void Game::show_player()
{
    if ( player->has_rope() )   // Se o jogador tem a corda...
    {
        color( CWHITE, true );
        mvprintw( 5, 1, "- Possui a corda!" );
    }
    else
    {
        color( CBLACK, true );
        mvprintw( 5, 1, "- Nao possui a corda." );
    }

    if ( player->has_arrow() )  // Se o jogador tem a flecha...
    {
        color( CWHITE, true );
        mvprintw( 6, 1, "- Possui a flecha!" );
    }
    else
    {
        color( CBLACK, true );
        mvprintw( 6, 1, "- Nao possui a flecha." );
    }

    color( CWHITE, false );
    mvprintw( 8, 1, "Encontrou" );
    color( CYELLOW, true );
    mvprintw( 8, 12, "%3d", treasures_found );  // Mostra tesouros encontrados
    color( CWHITE, false );
    mvprintw( 8, 16, "tesouros." );
}

void Game::show_adjacent_objects()
{
    // Estas flags indicam se o objeto em questao esta em uma sala adjacente
    // Por enquanto, tudo falso
    
    bool wizard_is_near = false;    // Feiticeiro esta proximo? Nao sei ainda
    bool treasure_is_near = false;  // Tesouro esta proximo? Nao sei ainda
    bool rope_is_near = false;      // Corda esta proxima? Nao sei ainda
    bool arrow_is_near = false;     // Flecha esta proxima? Nao sei ainda
    bool dragon_is_near = false;    // Dragao esta proximo? Nao sei ainda
    bool blackhole_is_near = false; // Buraco-Negro esta proximo? Nao sei ainda

    // Obtem as coordenadas do jogador
    byte px = player->getx();
    byte py = player->gety();

    // Detecta a flecha nas salas adjacentes
    if ( maze->get( px + 1 , py ) == OBJ_ARROW || maze->get( px , py + 1 ) == OBJ_ARROW ||
         maze->get( px - 1 , py ) == OBJ_ARROW || maze->get( px , py - 1 ) == OBJ_ARROW )
    {
        arrow_is_near = true;   // Flecha esta proxima!
    }

    // Detecta o feiticeiro nas salas adjacentes
    if ( maze->get( px + 1 , py ) == OBJ_WIZARD || maze->get( px , py + 1 ) == OBJ_WIZARD ||
         maze->get( px - 1 , py ) == OBJ_WIZARD || maze->get( px , py - 1 ) == OBJ_WIZARD )
    {
        wizard_is_near = true;  // Feiticeiro esta proximo!
    }

    // Detecta a corda nas salas adjacentes
    if ( maze->get( px + 1 , py ) == OBJ_ROPE || maze->get( px , py + 1 ) == OBJ_ROPE ||
         maze->get( px - 1 , py ) == OBJ_ROPE || maze->get( px , py - 1 ) == OBJ_ROPE )
    {
        rope_is_near = true;    // Corda esta proxima!
    }

    // Detecta o dragao nas salas adjacentes
    if ( maze->get( px + 1 , py ) == OBJ_DRAGON || maze->get( px , py + 1 ) == OBJ_DRAGON ||
         maze->get( px - 1 , py ) == OBJ_DRAGON || maze->get( px , py - 1 ) == OBJ_DRAGON )
    {
        dragon_is_near = true;  // Dragao esta proximo!
    }

    // Detecta um tesouro nas salas adjacentes
    if ( maze->get( px + 1 , py ) == OBJ_GOLD || maze->get( px , py + 1 ) == OBJ_GOLD ||
         maze->get( px - 1 , py ) == OBJ_GOLD || maze->get( px , py - 1 ) == OBJ_GOLD )
    {
        treasure_is_near = true;    // Tesouro esta proximo!
    }

    // Detecta o buraco-negro nas salas adjacentes
    if ( maze->get( px + 1 , py ) == OBJ_BLACKHOLE || maze->get( px , py + 1 ) == OBJ_BLACKHOLE ||
         maze->get( px - 1 , py ) == OBJ_BLACKHOLE || maze->get( px , py - 1 ) == OBJ_BLACKHOLE )
    {
        blackhole_is_near = true;   // Buraco-Negro esta proximo!
    }

    // Contador do numero de armadilhas adjacentes
    int number_of_pits = 0;

    // Detecta armadilhas nas salas adjacentes e incrementa o contador se precisp
    if ( maze->get( px + 1, py ) == OBJ_PIT )   // Detecta armadilha a leste
        number_of_pits++;
    if ( maze->get( px - 1, py ) == OBJ_PIT )   // Detecta armadilha a oeste
        number_of_pits++;
    if ( maze->get( px, py + 1 ) == OBJ_PIT )   // Detecta armadilha ao sul
        number_of_pits++;
    if ( maze->get( px, py - 1 ) == OBJ_PIT )   // Detecta armadilha ao norte
        number_of_pits++;

    // Indica a linha da tela onde imprimir as mensagens
    // Cada mensagem imprimida incrementa esta variavel para que apareca
    // uma mensagem embaixo da outra
    int y = 10;

    color( CWHITE, true );

    // Verifica o contador para indicar a quantidade de armadilhas adjacentes
    // E imprime a mensagem indicando quantas armadilhas foram detectadas
    
    if ( number_of_pits == 1 )
    {
        mvprintw( y, 1, "- Voce sente a presenca de uma armadilha!" );
        y++;
    }
    else if ( number_of_pits == 2 )
    {
        mvprintw( y, 1, "- Voce sente a presenca de duas armadilhas!" );
        y++;
    }
    else if ( number_of_pits == 3 )
    {
        mvprintw( y, 1, "- Voce sente a presenca de tres armadilhas!" );
        y++;
    }
    else if ( number_of_pits == 4 )
    {
        mvprintw( y, 1, "- Voce sente a presenca de quatro armadilhas!" );
        y++;
    }

    // Se o feiticeiro estiver numa sala adjacente...
    if ( wizard_is_near )
    {
        mvprintw( y, 1, "- Voce sente a presenca do feiticeiro!" );
        y++;
    }
    // Se a flecha estiver numa sala adjacente...
    if ( arrow_is_near )
    {
        mvprintw( y, 1, "- Voce sente a presenca da flecha magica!" );
        y++;
    }
    // Se algum tesouro estiver numa sala adjacente...
    if ( treasure_is_near )
    {
        mvprintw( y, 1, "- Voce sente a presenca de um tesouro valioso!" );
        y++;
    }
    // Se o buraco-negro estiver numa sala adjacente...
    if ( blackhole_is_near )
    {
        mvprintw( y, 1, "- Voce sente a presenca do buraco-negro!" );
        y++;
    }
    // Se a corda estiver numa sala adjacente...
    if ( rope_is_near )
    {
        mvprintw( y, 1, "- Voce sente a presenca da corda magica!" );
        y++;
    }
    // Se o dragao estiver numa sala adjacente...
    if ( dragon_is_near )
    {
        mvprintw( y, 1, "- Voce sente a presenca do dragao!" );
        y++;
    }
}

void Game::detect_object_in_room()
{
    // Obtem as coordenadas do jogador
    int px = player->getx();
    int py = player->gety();

    // Obtem o "objeto" que esta nas mesmas coordenadas que o jogador
    // e age de acordo com o que estiver la

    if ( maze->get( px, py ) == OBJ_ARROW ) // Se tiver a flecha...
    {
        show_arrow();   // Mostra e obtem a flecha!
    }
    else if ( maze->get( px, py ) == OBJ_WIZARD )  // Se tiver o feiticeiro...
    {
        show_wizard();  // Mostra o feiticeiro!
    }
    else if ( maze->get( px, py ) == OBJ_ROPE ) // Se tiver a corda...
    {
        show_rope();    // Mostra e obtem a corda!
    }
    else if ( maze->get( px, py ) == OBJ_GOLD ) // Se tiver um tesouro...
    {
        show_treasure();    // Mostra e obtem o tesouro!
    }
    else if ( maze->get( px, py ) == OBJ_PIT )  // Se tiver uma armadilha...
    {
        show_player_fall(); // Mostra o jogador caindo na armadilha!
    }
    else if ( maze->get( px, py ) == OBJ_BLACKHOLE ) // Se tiver o buraco-negro...
    {
        show_blackhole();   // Mostra o buraco-negro!!!
    }
    else if ( maze->get( px, py ) == OBJ_DRAGON )   // Se tiver o dragao...
    {
        show_player_dead(); // Adeus.
    }
}

//------------------------------------------------------------------------------
//  METODOS PARA APRESENTACAO DAS MENSAGENS DO JOGO
//------------------------------------------------------------------------------

void Game::show_player_move()
{
    // Incrementa o numero de movimentos
    moves++;
    
    // Aqui deveria mostrar uma mensagem da direcao que o jogador foi,
    // mas mudei de ideia e nao quiz mudar o nome do metodo (tirar o "show") :P
}

void Game::show_player_fall()
{
    clear();
    flash();

    color( CRED, true );
    mvprintw( 11, 1, "- CAIU NA ARMADILHA!" );
    mvprintw( 13, 1, "  Voce caiu num buraco profundo! E agora?!" );

    getch();

    // Se o jogador tiver a corda...
    if ( player->has_rope() )
        // Escapa da armadilha
        show_player_escape();
    else
    {
        // Senao, vai morrer dentro do buraco!
        clear();
        flash();
        color ( CRED, true );
        mvprintw( 14, 1, "  Voce nao possui a corda para poder escapar!" );
        mvprintw( 15, 1, "  Sinto muito, mas voce vai definhar ate a morte dentro do buraco..." );
        getch();

        // Morreu. Adeus.
        show_game_over_screen();
    }
}

void Game::show_player_escape()
{
    clear();
    flash();

    color( CYELLOW, true );
    mvprintw( 15, 1, "  Mas que sorte, voce tem uma corda!" );
    mvprintw( 16, 1, "  Voce usou a corda para escapar do buraco e continua a busca pelo dragao." );
    mvprintw( 18, 1, "  Como era uma corda magica, ela foi parar em algum canto do labirinto..." );

    // Tira a corda do jogador (pois ele acabou de usa-la para escapar)
    player->set_rope(false);
    // Coloca uma nova corda em outro local aleatorio
    maze->put_rnd( OBJ_ROPE );

    getch();
}

void Game::show_player_shoot()
{
    clear();

    if ( player->has_arrow() )  // Se o jogador tiver a flecha...
    {
        color( CRED, true );
        mvprintw( 4, 1, "- ATIRAR FLECHA:" );
        color( CWHITE, false );
        mvprintw( 6, 1, "  Em qual direcao voce deseja atirar?" );
        mvprintw( 8, 1, "  Pressione: " );
        color( CWHITE, true );
        mvprintw( 10, 1, "    [SETA ACIMA]            Atirar para o norte" );
        mvprintw( 11, 1, "    [SETA ABAIXO]           Atirar para o sul" );
        mvprintw( 12, 1, "    [SETA DIREITA]          Atirar para o leste" );
        mvprintw( 13, 1, "    [SETA ESQUERDA]         Atirar para o oeste" );
        mvprintw( 14, 1, "    [QUALQUER OUTRA TECLA]  Cancelar" );
        color( CWHITE, false );
        mvprintw( 16, 1, "  Sera que voce vai acertar algo???" );
        mvprintw( 17, 1, "  Se voce errar o alvo, a flecha vai parar em algum canto do labirinto..." );

        chtype keypressed = getch(); // Espera pressionar uma tecla

        switch ( keypressed )   // Verifica tecla pressionada...
        {
            case KEY_UP: shoot_arrow_north(); break;    // Pressionou seta acima
            case KEY_DOWN: shoot_arrow_south(); break;  // Pressionou seta abaixo
            case KEY_LEFT: shoot_arrow_west(); break;   // Pressionou seta p/ esquerda
            case KEY_RIGHT: shoot_arrow_east(); break;  // Pressionou seta p/ direita

            // Se pressionou qualquer outra tecla, cancela o ataque
            default:
                color( CWHITE, true );
                mvprintw( 19, 1, " Voce cancelou o ataque..." );
                getch();
                return;     // Retorna sem atirar a flecha
        }
    }
    else    // Senao... (se o jogador nao tem a flecha)
    {
        color( CWHITE, false );

        mvprintw( 10, 1, "- ATIRAR FLECHA? QUE FLECHA???" );
        mvprintw( 12, 1, "  Voce se armou com o arco, se preparou e atirou uma rajada de ar!" );
        mvprintw( 13, 1, "  Voce ficou se sentindo um inutil.");
        mvprintw( 14, 1, "  Sinto muito mas voce infelizmente ainda nao possui a flecha!" );
        mvprintw( 15, 1, "  Va encontra-la primeiro!" );

        getch();
        return;     // Retorna sem atirar nada!
    }
}

void Game::shoot_arrow_east()
{
    clear();
    flash();

    color( CWHITE, false );
    mvprintw( 10, 1, "- ATIROU FLECHA PARA O LESTE!" );
    mvprintw( 12, 1, "  Voce acaba de atirar a flecha para o leste..." );

    getch();

    // Obtem as coordenadas do jogador
    int px = player->getx();
    int py = player->gety();

    // Verifica se o dragao esta ao leste...
    if ( maze->get( px + 1, py ) == OBJ_DRAGON )
    {
        show_dragon_dead();     // Mostra o dragao morto!
    }
    else    // Se o dragao nao esta ao leste...
    {
        show_player_missed();   // Mostra que o jogador errou o alvo!
        return;
    }
}

void Game::shoot_arrow_west()
{
    clear();
    flash();

    color( CWHITE, false );
    mvprintw( 10, 1, "- ATIROU FLECHA PARA O OESTE!" );
    mvprintw( 12, 1, "  Voce acaba de atirar a flecha para o oeste..." );

    getch();

    // Obtem as coordenadas do jogador
    int px = player->getx();
    int py = player->gety();

    // Verifica se o dragao esta ao oeste
    if ( maze->get( px - 1, py ) == OBJ_DRAGON )
    {
        show_dragon_dead();
    }
    else
    {
        show_player_missed();
        return;
    }
}

void Game::shoot_arrow_south()
{
    clear();
    flash();

    color( CWHITE, false );
    mvprintw( 10, 1, "- ATIROU FLECHA PARA O SUL!" );
    mvprintw( 12, 1, "  Voce acaba de atirar a flecha para o sul..." );

    getch();

    // Obtem as coordenadas do jogador
    int px = player->getx();
    int py = player->gety();

    // Verifica se o dragao esta ao sul
    if ( maze->get( px, py + 1 ) == OBJ_DRAGON )
    {
        show_dragon_dead();
    }
    else
    {
        show_player_missed();
        return;
    }
}

void Game::shoot_arrow_north()
{
    clear();
    flash();

    color( CWHITE, false );
    mvprintw( 10, 1, "- ATIROU FLECHA PARA O NORTE!" );
    mvprintw( 12, 1, "  Voce acaba de atirar a flecha para o norte..." );

    getch();

    // Obtem as coordenadas do jogador
    int px = player->getx();
    int py = player->gety();

    // Verifica se o dragao esta ao norte
    if ( maze->get( px, py - 1 ) == OBJ_DRAGON )
    {
        show_dragon_dead();
    }
    else
    {
        show_player_missed();
        return;
    }
}

void Game::show_player_missed()
{
    clear();

    color( CWHITE, true );
    mvprintw( 10, 1, "- A FLECHA ATINGIU O CHAO." );
    mvprintw( 12, 1, "  Sinto muito, voce nao acertou nada e sua flecha caiu direto no chao." );
    mvprintw( 13, 1, "  Como trata-se de uma flecha magica, ela foi parar em algum outro canto" );
    mvprintw( 14, 1, "  qualquer do labirinto... Va encontra-la novamente!" );

    // Tira a flecha do jogador (pois ele acabou de usa-la)
    player->set_arrow(false);
    // Coloca a flecha em algum outro lugar aleatorio
    maze->put_rnd(OBJ_ARROW);

   getch();
}

void Game::show_dragon_dead()
{
    clear();
    flash();
    beep();

    color( CRED, true );
    mvprintw( 11, 1, "- VOCE ACERTOU EM CHEIO NO DRAGAO!!!" );
    color( CYELLOW, true );
    mvprintw( 13, 1, "  Parabens, voce acaba de derrotar o dragao deste labirinto!" );

    // Tira a flecha do jogador (pois ele acabou de usa-la)
    player->set_arrow(false);
    // Coloca a flecha em algum outro lugar aleatorio
    maze->put_rnd(OBJ_ARROW);

    getch();

    show_winner_screen();   // Mostra a tela indicando que o jogador venceu!
}

void Game::show_player_dead()
{
    clear();

    color( CRED, true );
    mvprintw( 10, 1, "- O DRAGAO ACABOU COM VOCE!" );
    mvprintw( 12, 1, "  Voce teve o azar de entrar bem na toca no dragao." );
    mvprintw( 13, 1, "  Com uma longa e rapida baforada ele transformou voce em cinzas" );
    mvprintw( 14, 1, "  instantaneamente... Adeus. Mwahwahwahwa!" );

    getch();
    show_game_over_screen();    // Virou churrasco.
}

void Game::show_treasure()
{
    clear();
    flash();

    color( CYELLOW, true );
    mvprintw( 11, 1, "- ENCONTROU UM TESOURO!" );
    mvprintw( 13, 1, "  Parabens!" );
    mvprintw( 14, 1, "  Voce acaba de desenterrar um dos tesouros do dragao!" );
    mvprintw( 15, 1, "  Sera que existem mais? Va procurar!" );

    // Incrementa o numero de tesouros encontrados
    treasures_found++;
    // Exclui o tesouro daquele local (pois o jogador ja o pegou)
    maze->put( player->getx(), player->gety(), OBJ_NOTHING );
    // Coloca um novo tesouro em outro local aleatorio
    maze->put_rnd( OBJ_GOLD );

    getch();
}

void Game::show_arrow()
{
    clear();
    flash();

    color( CWHITE, true );
    mvprintw( 11, 1, "- ENCONTROU A FLECHA!" );
    mvprintw( 13, 1, "  Agora voce pode usa-la para atirar no dragao quando sentir a" );
    mvprintw( 14, 1, "  presenca dele! Lembre-se: assim que sentir a presenca do dragao," );
    mvprintw( 15, 1, "  pressione [ALT+A] para atirar a flecha!" );

    // Da a flecha para o jogador
    player->set_arrow(true);
    // Exclui a flecha daquele local (pois o jogador ja a pegou)
    maze->put( player->getx(), player->gety(), OBJ_NOTHING );

    getch();
}

void Game::show_blackhole()
{
    clear();
    flash();

    color( CGREEN, true );
    mvprintw( 11, 1, "- ENCONTROU O BURACO-NEGRO!" );
    mvprintw( 13, 1, "  Voce foi sugado pelo buraco-negro, que distorce tempo e espaco." );
    mvprintw( 14, 1, "  Incrivel, voce nao morreu. Mas foi parar em outro labirinto diferente" );
    mvprintw( 15, 1, "  em algum canto qualquer do multiverso..." );

    // Recomeca tudo, mas nao zera o score nem a quantidade de movimentos
    init_maze();
    player->setxy(PLAYER_START_X, PLAYER_START_Y);
    player->set_rope(false);
    player->set_arrow(false);

    getch();
}

void Game::show_wizard()
{
    clear();
    flash();

    color( CMAGENTA, true );
    mvprintw( 11, 1, "- ENCONTROU O FEITICEIRO!" );
    mvprintw( 13, 1, "  O feiticeiro jamais permitira que voce destrua o dragao!" );
    mvprintw( 14, 1, "  Por isso, ele lancou um feitico, e voce foi teleportado para" );
    mvprintw( 15, 1, "  uma sala qualquer do labirinto..." );
    mvprintw( 17, 1, "  Onde sera que voce foi parar???" );

    // Define duas coordenadas aleatoriamente
    int random_x = rand() % MAZE_WIDTH;
    int random_y = rand() % MAZE_HEIGHT;

    // Exclui o feiticeiro daquele local
    maze->put( player->getx(), player->gety(), OBJ_NOTHING );
    // Coloca o feiticeiro em outro local aleatorio
    maze->put_rnd( OBJ_WIZARD );
    // Coloca o jogador nas coordenadas aleatorias
    player->setxy( random_x, random_y );

    getch();
}

void Game::show_rope()
{
    clear();
    flash();

    color( CCYAN, true );
    mvprintw( 11, 1, "- ENCONTROU A CORDA!" );
    mvprintw( 13, 1, "  Agora voce podera usa-la para sair de uma armadilha se por ventura" );
    mvprintw( 14, 1, "  voce cair acidentalmente em alguma!" );

    player->set_rope(true); // Da a flecha pro jogador
    
    // Exclui a flecha daquele local (pois o jogador acabou de pega-la)
    maze->put( player->getx(), player->gety(), OBJ_NOTHING );

    getch();
}

//------------------------------------------------------------------------------
//  METODOS PARA MANIPULAR A PONTUACAO DO JOGADOR
//------------------------------------------------------------------------------

void Game::calculate_score()
{
    // A pontuacao se baseia na quantidade de movimentos e...
    // (Quanto mais movimentos, maior a penalidade)
    
    score = 1000 - ( moves * 10 );  // 100 ou mais movimentos zera a pontuacao
    if ( score < 0 ) score = 0;     // Evita pontuacao negativa

    // ...Na quantidade de tesouros encontrados
    // (Quanto mais tesouros, maior o bonus)
    score += ( treasures_found * 10 );
}

void Game::load_hiscore()
{
    // No futuro, quem sabe...
}

void Game::save_hiscore()
{
    // No futuro, quem sabe...
}

//------------------------------------------------------------------------------
//  LOOP PRINCIPAL DO JOGO ONDE TUDO ACONTECE...
//------------------------------------------------------------------------------

void Game::run()
{
    running = true;         // Indica que o jogo esta rodando...

    chtype keypressed = 0;  // Representa a ultima tecla pressionada

    while ( running )       // Enquanto o jogo estiver rodando...
    {
        show_room();                // Mostra as informacoes da sala
        show_player();              // Mostra as informacoes do jogador
        show_adjacent_objects();    // Mostra os objetos de salas adjacentes
        show_map();                 // Mostra o mapa do labirinto

        // int line = 15;               // Linha onde sera mostrada a mensagem
        // color( CCYAN, true );        // Cor da mensagem

        keypressed = getch();   // Aguarda um pressionamento de tecla do jogador

        switch ( keypressed )   // Verifica a tecla pressionada e age de acordo
        {
            case KEY_UP:
                // mvprintw( line, 1, "Voce foi para o NORTE..." );
                // getch();
                show_player_move();
                player->sety( player->gety() - 1 );
                detect_object_in_room();
                break;
            case KEY_DOWN:
                // mvprintw( line, 1, "Voce foi para o SUL..." );
                // getch();
                show_player_move();
                player->sety( player->gety() + 1 );
                detect_object_in_room();
                break;
            case KEY_LEFT:
                // mvprintw( line, 1, "Voce foi para o OESTE..." );
                // getch();
                show_player_move();
                player->setx( player->getx() - 1 );
                detect_object_in_room();
                break;
            case KEY_RIGHT:
                // mvprintw( line, 1, "Voce foi para o LESTE..." );
                // getch();
                show_player_move();
                player->setx( player->getx() + 1 );
                detect_object_in_room();
                break;
            case ALT_A:
                show_player_shoot();
                break;
            case ALT_C:
                cheating = !cheating;
                break;
            case KEY_F(1):
                show_instructions();
                break;
            case ALT_Q:
                show_credits();
                quit();
                break;
            default:
                continue;
        }
    }

    return;     // ---> Codigo morto. So pra evitar warnings...
}
