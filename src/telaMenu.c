#include <ctype.h>
#include <ncurses.h>
#include <math.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include "telaMenu.h"

char
despedida(void)
{
    char *c;

    if (g.cheats[0] != 0 || g.cheats[1] != 0 || g.dica != NULL)
        c = "Fim de jogo!";
    else
        c = "Parabens, fim de jogo!";

    curs_set(0);
    // show_timer();
    show_banner(c);
    int ch;
    repete:
    ch = getch();
    ch = toupper(ch);
    if (ch == 'E' || ch == KEY_ENTER)
        ch = 'E';
    else if (ch == 'R' || ch == 'N') {
        //g.ini = time(0);
    }
    else
        goto repete;

    curs_set(1);
    return ch;
}

/*
 * Desenha a board do jogo
 */

void
draw_borders(void)
{
    // pega as dimensoes da tela
    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);

    // ativa cores se possivel (else b&w highlighting)
    if (has_colors())
    {
        attron(A_PROTECT);
        attron(COLOR_PAIR(PAIR_BORDER));
    }
    else
        attron(A_REVERSE);

    // desenha as bordas
    for (int i = 0; i < maxx; i++)
    {
        mvaddch(0, i, ' ');
        mvaddch(maxy-1, i, ' ');
    }

    // desenha o cabecalho
    char header[maxx+1];
    sprintf(header, "%s by %s", TITULO, AUTOR);
    mvaddstr(0, (maxx - strlen(header)) / 2, header);

    // desenha o rodape
    mvaddstr(maxy-1, 1, "[N]ovo Jogo    [R]einicia Jogo");
    mvaddstr(maxy-1, maxx-15, "[E]ncerra Jogo");

    // desativa cores se possivel (else b&w highlighting)
    if (has_colors())
        attroff(COLOR_PAIR(PAIR_BORDER));
    else
        attroff(A_REVERSE);
}


void
desenhaOpcoesMenu(void)
{
    // pega as dimensoes da tela
    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);

    // determina onde o topo esquerdo da tela esta
    g.top  = maxy / 2 + 3;
    g.left = maxx / 2 - 11;

    // ativa cores se possivel
    if (has_colors())
        attron(COLOR_PAIR(PAIR_GRID));

    mvaddstr(g.top,     g.left, "Novo jogo");
    mvaddstr(g.top + 1, g.left, "Dificuldade");
    mvaddstr(g.top + 2, g.left, "Sair");

    g.x = g.top;
    g.y = g.left - 2;

    if (has_colors())
        attron(COLOR_PAIR(PAIR_MENU));

    mvaddstr(g.x, g.y, ">");

    // desativa cores se possivel
    if (has_colors())
        attroff(COLOR_PAIR(PAIR_GRID));
}

void
desenhaLogoMenu(void)
{
    // determina as coordenadas do topo esquerdo do logo
    // pega as dimensoes da tela
    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);

    // determina onde o topo esquerdo da tela esta
    g.top  = maxy / 2 - 9;
    g.left = maxx / 2 - 33;

    // ativa cores se possivel
    if (has_colors())
        attron(COLOR_PAIR(PAIR_LOGO));

    // desenha o logo
    mvaddstr(g.top + 0, g.left, " ________           __                __           ");
    mvaddstr(g.top + 1, g.left, "|\\       \\         |\\ \\              |\\ \\          ");
    mvaddstr(g.top + 2, g.left, " \\@@@@@@@@______  _| @@_     ______   \\@@  _______ ");
    mvaddstr(g.top + 3, g.left, "   | @@  /\\     \\|\\ \\@@ \\   /\\     \\ |\\ \\ /\\      \\");
    mvaddstr(g.top + 4, g.left, "   | @@ |\\ @@@@@@\\\\@@@@@@  |\\ @@@@@@\\| @@|\\ @@@@@@@");
    mvaddstr(g.top + 5, g.left, "   | @@ | @@    @@ | @@ __ | @@   \\@@| @@ \\@@    \\ ");
    mvaddstr(g.top + 6, g.left, "   | @@ | @@@@@@@@ | @@|\\ \\| @@      | @@ _\\@@@@@@\\");
    mvaddstr(g.top + 7, g.left, "   | @@  \\@@     \\  \\@@ \\@@| @@      | @@|\\      @@");
    mvaddstr(g.top + 8, g.left, "    \\@@   \\@@@@@@@   \\@@@@  \\@@       \\@@ \\@@@@@@@");

        attron(COLOR_PAIR(PAIR_BORDER));
    mvaddstr(g.top + 2, g.left + 2, "        ");
    mvaddstr(g.top + 2, g.left + 21, "  ");
    mvaddstr(g.top + 2, g.left + 39, "  ");
    mvaddstr(g.top + 3, g.left + 5, "  ");
    mvaddstr(g.top + 3, g.left + 21, "  ");
    mvaddstr(g.top + 4, g.left + 5, "  ");
    mvaddstr(g.top + 4, g.left + 11, "      ");
    mvaddstr(g.top + 4, g.left + 19, "      ");
    mvaddstr(g.top + 4, g.left + 30, "      ");
    mvaddstr(g.top + 4, g.left + 39, "  ");
    mvaddstr(g.top + 4, g.left + 44, "       ");
    mvaddstr(g.top + 5, g.left + 5, "  ");
    mvaddstr(g.top + 5, g.left + 10, "  ");
    mvaddstr(g.top + 5, g.left + 16, "  ");
    mvaddstr(g.top + 5, g.left + 21, "  ");
    mvaddstr(g.top + 5, g.left + 29, "  ");
    mvaddstr(g.top + 5, g.left + 35, "  ");
    mvaddstr(g.top + 5, g.left + 39, "  ");
    mvaddstr(g.top + 5, g.left + 43, "  ");
    mvaddstr(g.top + 6, g.left + 5, "  ");
    mvaddstr(g.top + 6, g.left + 10, "        ");
    mvaddstr(g.top + 6, g.left + 21, "  ");
    mvaddstr(g.top + 6, g.left + 29, "  ");
    mvaddstr(g.top + 6, g.left + 39, "  ");
    mvaddstr(g.top + 6, g.left + 44, "      ");
    mvaddstr(g.top + 7, g.left + 5, "  ");
    mvaddstr(g.top + 7, g.left + 10, "  ");
    mvaddstr(g.top + 7, g.left + 21, "  ");
    mvaddstr(g.top + 7, g.left + 25, "  ");
    mvaddstr(g.top + 7, g.left + 29, "  ");
    mvaddstr(g.top + 7, g.left + 39, "  ");
    mvaddstr(g.top + 7, g.left + 49, "  ");
    mvaddstr(g.top + 8, g.left + 5, "  ");
    mvaddstr(g.top + 8, g.left + 11, "       ");
    mvaddstr(g.top + 8, g.left + 22, "    ");
    mvaddstr(g.top + 8, g.left + 24, "  ");
    mvaddstr(g.top + 8, g.left + 29, "  ");
    mvaddstr(g.top + 8, g.left + 39, "  ");
    mvaddstr(g.top + 8, g.left + 43, "       ");

    // assina o logo
    char signature[3+strlen(AUTOR)+1];
    sprintf(signature, "by %s", AUTOR);

    if (has_colors())
        attron(COLOR_PAIR(PAIR_BANNER));

    mvaddstr(g.top + 10, g.left + 52 - strlen(signature) - 1, signature);

    // desativa cores se possivel
    if (has_colors())
        attroff(COLOR_PAIR(PAIR_LOGO));
}

/*
 * Desenha os numeros do jogo.
 */

void
handle_signal(int signum)
{
    if (signum == SIGWINCH)
        redraw_all();

    signal(signum, (void (*)(int)) handle_signal);
}


void
desenhaMenu(void)
{
    // reseta ncurses
    endwin();
    refresh();

    // limpa a tela
    clear();

    // redesenha tudo
    desenhaLogoMenu();
    desenhaOpcoesMenu();

    move(g.x, g.y);
}


/*
 * Encerra ncurses.
 */

void
shutdown(void) {
    endwin();
}


/*
 * Inicia ncurses. Retorna true se sucesso.
 */

bool
startup(void)
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
        if (init_pair(PAIR_BANNER, FG_BANNER, BG_BANNER) == ERR ||
            init_pair(PAIR_BORDER, FG_BORDER, BG_BORDER) == ERR ||
            init_pair(PAIR_TIMER, FG_TIMER, BG_TIMER)    == ERR ||
            init_pair(PAIR_GRID, FG_GRID, BG_GRID)       == ERR ||
            init_pair(PAIR_LOGO, FG_LOGO, BG_LOGO)       == ERR ||
            init_pair(PAIR_MENU, FG_MENU, BG_MENU)       == ERR)
        {
            endwin();
            return false;
        }
    }

    // habilita o uso das setas do teclado e desativa echo no input do usuario
    if (keypad(stdscr, true) == ERR || noecho() == ERR || raw() == ERR)
    {
        endwin();
        return false;
    }

    // Atualiza a tela caso a janela seja redimencionada.
	signal(SIGWINCH, (void (*)(int)) handle_signal);

    // torna o cursor invisivel
    curs_set(0);

    timeout(1000);

    return true;
}

