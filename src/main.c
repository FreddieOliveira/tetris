#include <ctype.h>
#include <ncurses.h>
#include <stdio.h>
#include "telaMenu.h"

// macro do control
#define CTRL(x) ((x) & ~0140)


int
main(int argc, char *argv[])
{
    int ch;

    // iniciar ncurses
    if (!startup())
    {
        fprintf(stderr, "Erro iniciando ncurses!\n");
        return -1;
    }

    desenhaMenu();
    
    do
    {
        // atualiza a tela
        refresh();

        // capitaliza o input para simplificar os cases
        ch = toupper(getch());

        //if (tela == MENU)
        //{
            switch (ch)
            {
                // saida de emergencia
                case CTRL('c'):
                    ch = 'E';
                    break;

                // redesenha a tela com ctrl-L
                case CTRL('l'):
                    desenhaMenu();
                    break;

                // move o cursor pra cima qnd precionado a seta pra cima
                case KEY_UP:
                    mvaddstr(g.x, g.y, " ");
                    g.x = g.top + (g.x - g.top + 2) % 3;
    if (has_colors())
        attron(COLOR_PAIR(PAIR_MENU));
                    mvaddstr(g.x, g.y, ">");
                    break;

                // move o cursor pra baixo qnd precionado a seta pra baixo
                case KEY_DOWN:
                    mvaddstr(g.x, g.y, " ");
    if (has_colors())
        attron(COLOR_PAIR(PAIR_MENU));
                    g.x = g.top + (g.x - g.top + 1) % 3;
                    mvaddstr(g.x, g.y, ">");
                    break;

                // move o cursor pra baixo qnd precionado a seta pra baixo
                case KEY_RIGHT:
                case KEY_ENTER:
                    if (g.x == g.top)
                    {
                        //tela = NOVO_JOGO;
                    }
                    else if (g.x == g.top + 2)
                    {
                        ch = 'E';
                    }
                    break;

                default:
                    break;
            }
        //}
    }
    while (ch != 'E');

    // shuts down ncurses
    shutdown();

    // configura a tela (usando ANSI escape sequences)
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);

    // FIM!
    printf("\nFIM!\n\n");
    printf("%d", ch);
    return 0;
}
