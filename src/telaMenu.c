#include <ctype.h>
#include <ncurses.h>
#include "tela.h"
#include "telaMenu.h"
#include "jogo.h"

typedef enum
{
    NOVO_JOGO = 0,
    DIFICULDADE,
    SAIR_OP,
    NUM_DE_OPCOES
} opcoes_e;

typedef struct
{
    int posicaoX;
    int posicaoY;
    char texto[30];
} opcoes_s;

static opcoes_e opcaoAtual;
static opcoes_s opcoes[] = {{0, 0, "Novo Jogo"},
                            {0, 0, "Dificuldade"},
                            {0, 0, "Sair"}};

static void removeSelecao(opcoes_e op);
static void desenhaSelecao(opcoes_e op);
static void desenhaOpcoes(void);
static void desenhaLogo(void);
static void desenhaTela(void);

estados_e
processaTelaMenu(void)
{
    static bool necessitaDesenhar = true;
    estados_e estado = TELA_MENU;
    int ch;

    if (necessitaDesenhar == true)
    {
        opcaoAtual = 0;
        desenhaTela();
        necessitaDesenhar = false;
    }

    // capitaliza o input para simplificar os cases
    ch = toupper(getch());

    switch (ch)
    {
        // redesenha a tela com ctrl-L
        case CTRL('l'):
            desenhaTela();
            break;

        // move o cursor pra cima qnd precionado a seta pra cima
        case KEY_UP:
            removeSelecao(opcaoAtual);
            opcaoAtual = (opcaoAtual + NUM_DE_OPCOES - 1) % NUM_DE_OPCOES;
            desenhaSelecao(opcaoAtual);
            break;

        // move o cursor pra baixo qnd precionado a seta pra baixo
        case KEY_DOWN:
            removeSelecao(opcaoAtual);
            opcaoAtual = (opcaoAtual + 1) % NUM_DE_OPCOES;
            desenhaSelecao(opcaoAtual);
            break;

        // move o cursor pra baixo qnd precionado a seta pra baixo
        case KEY_RIGHT:
        case KEY_ENTER:
        case 10:
            if (opcaoAtual == NOVO_JOGO)
            {
                estado = TELA_JOGO;
            }
            else if (opcaoAtual == SAIR_OP)
            {
                estado = SAIR;
            }
            necessitaDesenhar = true;
            break;

        default:
            break;
    }


    return estado;
}

static void
removeSelecao(opcoes_e op)
{
    // ativa cores se possivel
    if (has_colors())
    {
        attron(COLOR_PAIR(PAIR_SELECAO_OPCAO_MENU));
    }

    mvaddstr(opcoes[op].posicaoY, opcoes[op].posicaoX - 2, " ");

    if (has_colors())
    {
        attroff(COLOR_PAIR(PAIR_SELECAO_OPCAO_MENU));
    }

    refresh();
}

static void
desenhaSelecao(opcoes_e op)
{
    // ativa cores se possivel
    if (has_colors())
    {
        attron(COLOR_PAIR(PAIR_SELECAO_OPCAO_MENU));
    }

    mvaddstr(opcoes[op].posicaoY, opcoes[op].posicaoX - 2, ">");

    if (has_colors())
    {
        attroff(COLOR_PAIR(PAIR_SELECAO_OPCAO_MENU));
    }

    refresh();
}

static void
desenhaOpcoes(void)
{
    int maxY, maxX;

    // pega as dimensoes da tela
    getmaxyx(stdscr, maxY, maxX);

    // determina a posicao onde desenhar o menu
    opcoes[0].posicaoY = maxY / 2 + 3;
    opcoes[0].posicaoX = maxX / 2 - 11;

    for (int i = 1; i < NUM_DE_OPCOES; i++)
    {
        opcoes[i].posicaoY = opcoes[i - 1].posicaoY + 1;
        opcoes[i].posicaoX = opcoes[0].posicaoX;
    }

    // ativa cores se possivel
    if (has_colors())
    {
        attron(COLOR_PAIR(PAIR_OPCOES_MENU));
    }

    for (int i = 0; i < NUM_DE_OPCOES; i++)
    {
        mvaddstr(opcoes[i].posicaoY, opcoes[i].posicaoX, opcoes[i].texto);
    }

    // desativa cores se possivel
    if (has_colors())
    {
        attroff(COLOR_PAIR(PAIR_OPCOES_MENU));
    }

    refresh();
}

static void
desenhaLogo(void)
{
    int maxY, maxX;
    int logoPositionY, logoPositionX;

    // pega as dimensoes da tela
    getmaxyx(stdscr, maxY, maxX);

    // determina onde a logo sera desenhada
    logoPositionY = maxY / 2 - 9;
    logoPositionX = maxX / 2 - 33;

    // ativa cores se possivel
    if (has_colors())
    {
        attron(COLOR_PAIR(PAIR_LOGO_MENU_1));
    }

    // desenha a logo
    mvaddstr(logoPositionY,     logoPositionX, " ________           __                __           ");
    mvaddstr(logoPositionY + 1, logoPositionX, "|\\       \\         |\\ \\              |\\ \\          ");
    mvaddstr(logoPositionY + 2, logoPositionX, " \\@@@@@@@@______  _| @@_     ______   \\@@  _______ ");
    mvaddstr(logoPositionY + 3, logoPositionX, "   | @@  /\\     \\|\\ \\@@ \\   /\\     \\ |\\ \\ /\\      \\");
    mvaddstr(logoPositionY + 4, logoPositionX, "   | @@ |\\ @@@@@@\\\\@@@@@@  |\\ @@@@@@\\| @@|\\ @@@@@@@");
    mvaddstr(logoPositionY + 5, logoPositionX, "   | @@ | @@    @@ | @@ __ | @@   \\@@| @@ \\@@    \\ ");
    mvaddstr(logoPositionY + 6, logoPositionX, "   | @@ | @@@@@@@@ | @@|\\ \\| @@      | @@ _\\@@@@@@\\");
    mvaddstr(logoPositionY + 7, logoPositionX, "   | @@  \\@@     \\  \\@@ \\@@| @@      | @@|\\      @@");
    mvaddstr(logoPositionY + 8, logoPositionX, "    \\@@   \\@@@@@@@   \\@@@@  \\@@       \\@@ \\@@@@@@@");

    if (has_colors())
    {
        attroff(COLOR_PAIR(PAIR_LOGO_MENU_1));
        attron(COLOR_PAIR(PAIR_LOGO_MENU_2));

        for (int i = 0; i <= 8; i++)
        {
            for (int j = 0; logoPositionX + j < maxX; j++)
            {
                if ((mvinch(logoPositionY + i, logoPositionX + j) & A_CHARTEXT) == '@')
                {
                    mvaddstr(logoPositionY + i, logoPositionX + j, " ");
                }
            }
        }

        attroff(COLOR_PAIR(PAIR_LOGO_MENU_2));
        attron(COLOR_PAIR(PAIR_AUTOR));
    }

    mvaddstr(logoPositionY + 10, logoPositionX + 41, "by Freddie");

    // desativa cores se possivel
    if (has_colors())
    {
        attroff(COLOR_PAIR(PAIR_AUTOR));
    }

    refresh();
}

/*
 * Desenha toda a tela de menu
 */

static void
desenhaTela(void)
{
    // reseta ncurses
    endwin();
    refresh();

    // limpa a tela
    clear();

    // redesenha tudo
    desenhaLogo();
    desenhaOpcoes();
    desenhaSelecao(opcaoAtual);
}

