#include <stdbool.h>
#include <stdio.h>
#include "jogo.h"
#include "tela.h"
#include "telaMenu.h"

int
executaJogo(void)
{
    estados_e estado = 0;

    // iniciar ncurses
    if (iniciarNcurses() == false)
    {
        fprintf(stderr, "Erro iniciando ncurses!\n");
        return -1;
    }

    do
    {
        switch (estado)
        {
            case TELA_MENU:
                estado = processaTelaMenu();
                break;

            //case TELA_DIFICULDADE:
                // estado = processaTelaDificuldade();
                //break;

            //case TELA_JOGO:
                // estado = processaTelaJogo();
                //break;

            default:
                estado = TELA_MENU;
                break;
        }
    } while (estado != SAIR);

    // finaliza ncurses
    finalizarNcurses();

    return 0;
}

