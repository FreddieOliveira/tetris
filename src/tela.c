#include <ncurses.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include "tela.h"

void tratarSinais(int signum);

bool
iniciarNcurses(void)
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
            init_pair(PAIR_SELECAO_OPCAO_MENU, FG_SELECAO_OPCAO_MENU, BG_SELECAO_OPCAO_MENU) == ERR)
        {
            endwin();
            return false;
        }
    }

    // habilita o uso das setas do teclado e desativa echo do input
    if (keypad(stdscr, true) == ERR || noecho() == ERR || raw() == ERR)
    {
        endwin();
        return false;
    }

    // handle = processa/tratar
    // Atualiza a tela caso a janela seja redimencionada.
	// signal(SIGWINCH | SIGINT, tratarSinais);
	signal(SIGWINCH, tratarSinais);
	signal(SIGINT, tratarSinais);

    // torna o cursor invisivel
    curs_set(0);

    timeout(1000);

    return true;
}
/*
 * Encerra ncurses.
 */

void
finalizarNcurses(void)
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
tratarSinais(int signum)
{
    if (signum == SIGWINCH)
    {
        // como saber qual tela redesenha???????
        // redraw_all();
    }
    else if (signum == SIGINT)
    {
        finalizarNcurses();
        exit(1);
    }
}

