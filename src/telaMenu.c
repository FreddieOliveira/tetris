#include <ctype.h>
#include <ncurses.h>
#include <string.h>
#include "tela.h"
#include "telaMenu.h"
#include "jogo.h"

typedef enum
{
    OP_NOVO_JOGO = 0,
    OP_DIFICULDADE,
    OP_RECORDES,
    OP_SAIR,
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
                            {0, 0, "Recordes"},
                            {0, 0, "Sair"}};

static void apagaSelecao(opcoes_e op);
static void desenhaLogo(void);
static void desenhaOpcoes(void);
static void desenhaRecordes(void);
static void desenhaSelecao(opcoes_e op);
static void desenhaTela(void);
static void escolheDificuldade(void);

estados_e
processaTelaMenu(void)
{
    static bool primeiraExecucao = true;
    estados_e estado = TELA_MENU;
    int ch;

    if (primeiraExecucao == true)
    {
        opcaoAtual = 0;
        desenhaTela();
        primeiraExecucao = false;
    }

    // capitaliza o input para simplificar os cases
    ch = toupper(getch());

    switch (ch)
    {
        // redesenha a tela com ctrl-L
        case CTRL('l'):
            // reseta ncurses
            endwin();
            refresh();
            desenhaTela();
            break;

        // move o cursor pra cima qnd precionado a seta pra cima
        case KEY_UP:
            apagaSelecao(opcaoAtual);
            opcaoAtual = (opcaoAtual + NUM_DE_OPCOES - 1) % NUM_DE_OPCOES;
            desenhaSelecao(opcaoAtual);
            break;

        // move o cursor pra baixo qnd precionado a seta pra baixo
        case KEY_DOWN:
            apagaSelecao(opcaoAtual);
            opcaoAtual = (opcaoAtual + 1) % NUM_DE_OPCOES;
            desenhaSelecao(opcaoAtual);
            break;

        // seleciona a opcao atual
        case KEY_RIGHT:
        case KEY_ENTER:
        case 10:
            if (opcaoAtual == OP_NOVO_JOGO)
            {
                estado = TELA_JOGO;
                primeiraExecucao = true;
            }
            else if (opcaoAtual == OP_DIFICULDADE)
            {
                escolheDificuldade();
            }
            else if (opcaoAtual == OP_RECORDES)
            {
                desenhaRecordes();
            }
            else if (opcaoAtual == OP_SAIR)
            {
                estado = SAIR;
            }
            break;

        default:
            break;
    }


    return estado;
}

static void
desenhaRecordes(void)
{
    int maxY, maxX;
    int ch;
    chtype texto[200];

    // pega as dimensoes da tela
    getmaxyx(stdscr, maxY, maxX);

    // desloca as linhas das opcoes a partir de recorde cinco linhas pra baixo
    // para das espaco para os cinco melhores tempos que serao exibidos ali
    for (int i = NUM_DE_OPCOES - 1; i > OP_RECORDES; i--)
    {
        // copia a linha a partir da coluna 0 junto com todos seus atributos
        mvinchstr(opcoes[i].posicaoY, 0, texto);
        // cola-a cinco linhas abaixo
        mvaddchstr(opcoes[i].posicaoY + (sizeof(jogo.recordes) / sizeof(jogo.recordes[0])), 0, texto);
    }

    // ativa cores se possivel
    if (has_colors())
    {
        attron(COLOR_PAIR(PAIR_OPCOES_MENU));
    }

    // imprime os melhores tempos nas rescem deslocadas
    for (int i = 0; i < sizeof(jogo.recordes) / sizeof(jogo.recordes[0]); i++)
    {
        // apaga o texto ali escrito
        for (int j = 0; j < maxX; j++)
        {
            mvaddstr(opcoes[OP_RECORDES].posicaoY + i + 1, j, " ");
        }
        // imprime o novo texto
        mvprintw(opcoes[OP_RECORDES].posicaoY + i + 1,
                 opcoes[OP_RECORDES].posicaoX + 3,
                 "%d. %d", i + 1, jogo.recordes[i]);
    }

    // aguarda a tecla enter ser pressionada
    do
    {
        ch = toupper(getch());
    } while (ch != KEY_ENTER && ch != 10);

    // apaga as 8 linhas abaixo da opcao recorde (5 linhas dos
    // melhores tempos mais as 2 linhas restantes do menu)
    for (int i = 1; i < NUM_DE_OPCOES - OP_RECORDES + sizeof(jogo.recordes) / sizeof(jogo.recordes[0]); i++)
    {
        for (int j = 0; j < maxX; j++)
        {
            mvaddstr(opcoes[OP_RECORDES].posicaoY + i, j, " ");
        }
    }

    // desativa as cores
    if (has_colors())
    {
        attroff(COLOR_PAIR(PAIR_OPCOES_MENU));
    }

    desenhaOpcoes();

    refresh();
}
static void
escolheDificuldade(void)
{
    int maxY, maxX;
    int ch;
    chtype texto[200];

    // pega as dimensoes da tela
    getmaxyx(stdscr, maxY, maxX);

    // desloca as linhas das opcoes a partir de dificuldade uma linhas pra baixo
    // para dar espaco para a selecao de dificuldade que sera exibido ali
    for (int i = NUM_DE_OPCOES - 1; i > OP_DIFICULDADE; i--)
    {
        mvinchstr(opcoes[i].posicaoY, 0, texto);
        mvaddchstr(opcoes[i].posicaoY + 1, 0, texto);
    }

    if (has_colors())
    {
        attron(COLOR_PAIR(PAIR_OPCOES_MENU));
    }

    // apaga o antigo conteudo da linhas recem deslocada
    for (int i = 0; i < maxX; i++)
    {
        mvaddstr(opcoes[OP_DIFICULDADE + 1].posicaoY, i, " ");
    }

    // continua o codigo apenas qnd enter for pressionado
    do
    {
        // exibe a selecao de dificuldade na linha embaixo da opcao
        // de dificuldade alinhado com o centro de seu texto
        mvprintw(opcoes[OP_DIFICULDADE + 1].posicaoY,
                 opcoes[OP_DIFICULDADE].posicaoX + strlen(opcoes[OP_DIFICULDADE].texto) / 2 - 3,
                 "- %02d +", jogo.dificuldadeInicial);

        ch = toupper(getch());

        switch (ch)
        {
            case KEY_UP:
            case KEY_RIGHT:
                if (jogo.dificuldadeInicial < 30)
                {
                    jogo.dificuldadeInicial++;
                }
                break;

            case KEY_DOWN:
            case KEY_LEFT:
                if (jogo.dificuldadeInicial > 0)
                {
                    jogo.dificuldadeInicial--;
                }
                break;

            default:
                break;
        }
    } while (ch != KEY_ENTER && ch != 10);

    // antes de sair apaga as 3 linhas abaixo da opcao de dificuldade
    // (a linha da selecao mais as outras duas linhas do menu)
    for (int i = OP_DIFICULDADE; i < NUM_DE_OPCOES; i++)
    {
        for (int j = 0; j < maxX; j++)
        {
            mvaddstr(opcoes[i].posicaoY + 1, j, " ");
        }
    }

    if (has_colors())
    {
        attroff(COLOR_PAIR(PAIR_OPCOES_MENU));
    }

    desenhaOpcoes();

    refresh();
}

static void
apagaSelecao(opcoes_e op)
{
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

    // determina a posicao onde desenhar a primeira opcao do menu
    opcoes[0].posicaoY = maxY / 2 + 3;
    opcoes[0].posicaoX = maxX / 2 - strlen(opcoes[0].texto) / 2;

    // alinha todas as outras opcoes com a primeira
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
    logoPositionX = (maxX - 51) / 2;

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

        // substitui os @ por uma cor de fundo
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

    mvprintw(logoPositionY + 10, logoPositionX + 48 - strlen(AUTOR), "by %s", AUTOR);

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
    // limpa a tela
    clear();

    // redesenha tudo
    desenhaLogo();
    desenhaOpcoes();
    desenhaSelecao(opcaoAtual);
}

