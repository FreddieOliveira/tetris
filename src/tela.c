#include <ncurses.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include "tela.h"

void trataSinais(int signum);

bool
iniciaNcurses(void)
{
    // inicializa ncurses
    if (initscr() == NULL)
    {
        return false;
    }

    // prepara as cores se possivel
    if (has_colors())
    {
        // ativa cores
        if (start_color() == ERR || attron(A_PROTECT) == ERR)
        {
            endwin();
            return false;
        }

        // inicializa os pares de cores
        if (init_pair(PAIR_LOGO_MENU_1, FG_LOGO_MENU_1, BG_LOGO_MENU_1) == ERR ||
            init_pair(PAIR_LOGO_MENU_2, FG_LOGO_MENU_2, BG_LOGO_MENU_2) == ERR ||
            init_pair(PAIR_AUTOR, FG_AUTOR, BG_AUTOR) == ERR ||
            init_pair(PAIR_OPCOES_MENU, FG_OPCOES_MENU, BG_OPCOES_MENU) == ERR ||
            init_pair(PAIR_SELECAO_OPCAO_MENU, FG_SELECAO_OPCAO_MENU, BG_SELECAO_OPCAO_MENU) == ERR ||
            init_pair(PAIR_ARENA_JOGO, FG_ARENA_JOGO, BG_ARENA_JOGO) == ERR ||
            init_pair(PAIR_PECA_TIPO_T_JOGO, FG_PECA_TIPO_T_JOGO, BG_PECA_TIPO_T_JOGO) == ERR ||
            init_pair(PAIR_PECA_TIPO_Z1_JOGO, FG_PECA_TIPO_Z1_JOGO, BG_PECA_TIPO_Z1_JOGO) == ERR ||
            init_pair(PAIR_PECA_TIPO_Z2_JOGO, FG_PECA_TIPO_Z2_JOGO, BG_PECA_TIPO_Z2_JOGO) == ERR ||
            init_pair(PAIR_PECA_TIPO_O_JOGO, FG_PECA_TIPO_O_JOGO, BG_PECA_TIPO_O_JOGO) == ERR ||
            init_pair(PAIR_PECA_TIPO_L1_JOGO, FG_PECA_TIPO_L1_JOGO, BG_PECA_TIPO_L1_JOGO) == ERR ||
            init_pair(PAIR_PECA_TIPO_L2_JOGO, FG_PECA_TIPO_L2_JOGO, BG_PECA_TIPO_L2_JOGO) == ERR ||
            init_pair(PAIR_PECA_TIPO_I_JOGO, FG_PECA_TIPO_I_JOGO, BG_PECA_TIPO_I_JOGO) == ERR ||
            init_pair(PAIR_CONFIRMACAO_JOGO, FG_CONFIRMACAO_JOGO, BG_CONFIRMACAO_JOGO) == ERR ||
            init_pair(PAIR_GAMEOVER_JOGO, FG_GAMEOVER_JOGO, BG_GAMEOVER_JOGO) == ERR ||
            init_pair(PAIR_DASHBOARD_JOGO, FG_DASHBOARD_JOGO, BG_DASHBOARD_JOGO) == ERR ||
            init_pair(PAIR_BORDAS_JOGO, FG_BORDAS_JOGO, BG_BORDAS_JOGO) == ERR)
        {
            endwin();
            return false;
        }
    }

    // habilita o uso das setas do teclado torna a funcao getch bloqueante
    // desabilita o echo do input
    if (keypad(stdscr, true) == ERR ||
        nodelay(stdscr, false) == ERR ||
        noecho() == ERR ||
        raw() == ERR)
    {
        endwin();
        return false;
    }

    // handle = processa/tratar
    // Atualiza a tela caso a janela seja redimencionada.
	// signal(SIGWINCH | SIGINT, tratarSinais);
	signal(SIGWINCH, trataSinais);
	signal(SIGINT, trataSinais);

    // torna o cursor invisivel
    curs_set(0);

    timeout(1000);

    return true;
}
/*
 * Encerra ncurses.
 */

void
finalizaNcurses(void)
{
    // finaliza ncurses
    endwin();

    // reset as configuracoes de cores e do cursor
    // usando ANSI escape sequences
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);

    // FIM!
    printf("\nFIM!\n\n");
}


/*
 * Inicia ncurses. Retorna true se sucesso.
 */


void
trataSinais(int signum)
{
    if (signum == SIGWINCH)
    {
        // como saber qual tela redesenha???????
        // redraw_all();
    }
    else if (signum == SIGINT)
    {
        finalizaNcurses();
        exit(1);
    }
}

