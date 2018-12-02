#include <ctype.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include "telaJogo.h"
#include "telaMenu.h"
#include "tela.h"
#include "jogo.h"

#define ARENA_ALTURA  17
#define ARENA_LARGURA 10

typedef enum
{
    ESQUERDA = -1,
    DIREITA = 1,
    BAIXO = 2
} direcao_e;

typedef enum
{
    TIPO_T = 0,
    TIPO_Z1,
    TIPO_Z2,
    TIPO_O,
    TIPO_L1,
    TIPO_L2,
    TIPO_I,
    NUM_TIPO_PECAS,
    PECA_INVALIDA
} tipoPecas_e;
                               //  T
static int pecas[][2][4] = {{{0, 1, 0, 0},
                             {1, 2, 1, 0}},
                               // Z-1
                            {{1, 1, 0, 0},
                             {0, 2, 1, 0}},
                               // Z-2
                            {{0, 1, 1, 0},
                             {1, 2, 0, 0}},
                               //  O
                            {{2, 1, 0, 0},
                             {1, 1, 0, 0}},
                               // L-1
                            {{1, 2, 1, 0},
                             {0, 0, 1, 0}},
                               // L-2
                            {{1, 2, 1, 0},
                             {1, 0, 0, 0}},
                               //  I
                            {{1, 2, 1, 1},
                             {0, 0, 0, 0}}};

typedef struct
{
    tipoPecas_e tipoPeca;
    int rotacao;
    int numCoordenadas;
    struct
    {
        int posicaoX;
        int posicaoY;
    } coordenadas[4];
} peca_s;

static peca_s pecaAtual, projecao;
static tipoPecas_e proximoTipoPeca;
static int numPecasEscolhidas[NUM_TIPO_PECAS];
static int arena[ARENA_ALTURA][ARENA_LARGURA];
static int pontuacao;
static int numLinhasEliminadas;
static int dificuldade;
static bool gameOver;

static int getPieceLowerPosY(peca_s peca);
static int getPieceHigherPosY(peca_s peca);
static void desenhaPecaGenerica(tipoPecas_e tipoPeca, int posicaoY, int posicaoX);
static void atualizaRecorde(void);
static void desenhaBordas(void);
static void desenhaDashBoard(void);
static bool configuraTimer(void);
static bool iniciaTimer(int tempo);
static int  atualizaArena(void);
static void atualizaEstatisticas(int num);
static bool confirmacao(char *texto);
static int rotacionaPecaAtual(void);
static int rotacionaPeca(peca_s *peca);
static int rotacionaPecaAux(peca_s *peca);
static int  movimentaPeca(peca_s *peca, direcao_e direcao);
static int  movimentaPecaAtual(direcao_e direcao);
static void reiniciaJogo(void);
static void finalizaJogo(void);
static bool verificaGameOver(void);
static void escolhePecaAtual(void);
static void apagaPecaAtual(void);
static void apagaPeca(peca_s peca);
static void desenhaNumLinhasEliminadas(void);
static void desenhaArena(void);
static void desenhaPecaAtual(void);
static void desenhaPeca(peca_s peca);
static void desenhaTela(void);
static void desenhaLogoJogo(void);
static void desenhaGameOver(void);
static void abaixaPecaAtual(void);
static void sigalrmHandler(int _ __attribute__((unused)));
static void eliminaLinha(int linha);

estados_e
processaTelaJogo(void)
{
    static bool necessitaDesenhar = true;
    estados_e estado = TELA_JOGO;
    int ch;

    if (necessitaDesenhar == true)
    {
        srandom(time(NULL));
        if (configuraTimer() == false)
        {
            finalizaNcurses();
            exit(1);
        }
        reiniciaJogo();
        desenhaTela();
        necessitaDesenhar = false;
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
            rotacionaPecaAtual();
            break;

        // move o cursor pra baixo qnd precionado a seta pra baixo
        case KEY_RIGHT:
            movimentaPecaAtual(DIREITA);
            break;

        // move o cursor pra baixo qnd precionado a seta pra baixo
        case KEY_LEFT:
            movimentaPecaAtual(ESQUERDA);
            break;

        // hard drop
        case KEY_DOWN:
            abaixaPecaAtual();
            break;

        // voltar para menu
        case 'M':
        case KEY_BACKSPACE:
        case KEY_DC:
        case 127:
            if (gameOver == false)
            {
                iniciaTimer(0);

                if (confirmacao("Deseja voltar para o menu?") == true)
                {
                    finalizaJogo();
                    estado = TELA_MENU;
                    necessitaDesenhar = true;
                }
                else
                {
                    desenhaTela();
                    iniciaTimer(1000000 - dificuldade * 20000);
                }
            }
            else
            {
                estado = TELA_MENU;
                necessitaDesenhar = true;
            }
            break;

        // pause
        case 'P':
            if (gameOver == false)
            {
                iniciaTimer(0);
                while (confirmacao("Jogo pausado. Deseja voltar?") == false);
                desenhaTela();
                iniciaTimer(1000000 - dificuldade * 20000);
            }
            break;

        // novo jogo
        case 'N':
            if (gameOver == false)
            {
                iniciaTimer(0);

                if (confirmacao("Deseja iniciar um novo jogo?") == true) 
                {
                    reiniciaJogo();
                }
                else
                {
                    iniciaTimer(1000000 - dificuldade * 20000);
                }
            }
            else
            {
                reiniciaJogo();
            }
            desenhaTela();
            break;

        default:
            break;
    }


    return estado;
}

static void
reiniciaJogo(void)
{
    finalizaJogo();
    gameOver = false;
    escolhePecaAtual();
    iniciaTimer(1000000 - dificuldade * 20000);
}

static bool
confirmacao(char *texto)
{
    int janelaAltura = 5;
    int janelaLargura = 30;
    int maxY, maxX;
    int posY, posX;
    int offset;
    int ch;
    bool status = false;

    // pega as dimensoes da tela
    getmaxyx(stdscr, maxY, maxX);

    // determina a posicao onde desenhar o menu
    posY = (maxY - janelaAltura) / 2;
    posX = (maxX - janelaLargura) / 2;

    // ativa cores se possivel
    if (has_colors())
    {
        attron(COLOR_PAIR(PAIR_CONFIRMACAO_JOGO));
    }

    clear();

    for (int i = 0; i < janelaAltura; i++)
    {
        for (int j = 0; j < janelaLargura; j++)
        {
            mvaddstr(i + posY, j + posX, " ");
        }
    }

    offset = (janelaLargura - strlen(texto)) / 2;

    mvaddstr(posY + 1, posX + offset, texto);
    mvaddstr(posY + janelaAltura - 2, posX + offset + 4, "[S]im");
    mvaddstr(posY + janelaAltura - 2, posX + janelaLargura - offset - 4 - 5, "[N]ao");

    // desativa cores se possivel
    if (has_colors())
    {
        attroff(COLOR_PAIR(PAIR_CONFIRMACAO_JOGO));
    }

    refresh();

    do
    {
        ch = toupper(getch());

        if (ch == 'S')
        {
            status = true;
        }
        else if (ch == 'N')
        {
            status = false;
        }
    } while (ch != 'S' && ch != 'N');


    return status;
}

// sentido anti-horario

static int
rotacionaPecaAux(peca_s *peca)
{
    if (peca->tipoPeca == PECA_INVALIDA)
    {
        return 0;
    }

    for (int i = 1; i < peca->numCoordenadas; i++)
    {
        int dy = peca->coordenadas[0].posicaoY - peca->coordenadas[i].posicaoY;
        int dx = peca->coordenadas[0].posicaoX - peca->coordenadas[i].posicaoX;
    
        if (peca->coordenadas[0].posicaoY + dx >= ARENA_ALTURA ||
            peca->coordenadas[0].posicaoX - dy < 0 ||
            peca->coordenadas[0].posicaoX - dy >= ARENA_LARGURA ||
            (peca->coordenadas[0].posicaoY + dx > 0 &&
             arena[peca->coordenadas[0].posicaoY + dx][peca->coordenadas[0].posicaoX - dy] != PECA_INVALIDA))
        {
            return 0;
        }
    }
    
    for (int i = 1; i < peca->numCoordenadas; i++)
    {
        int dy = peca->coordenadas[0].posicaoY - peca->coordenadas[i].posicaoY;
        int dx = peca->coordenadas[0].posicaoX - peca->coordenadas[i].posicaoX;
    
        peca->coordenadas[i].posicaoY = peca->coordenadas[0].posicaoY + dx;
        peca->coordenadas[i].posicaoX = peca->coordenadas[0].posicaoX - dy;
    }

    return 1;
}

static int
rotacionaPeca(peca_s *peca)
{
    int status = -1;

    if (peca->tipoPeca == PECA_INVALIDA)
    {
        return -1;
    }

    switch (peca->tipoPeca)
    {
        case TIPO_T:
            status = rotacionaPecaAux(peca);
            break;

        case TIPO_Z1:
            status = rotacionaPecaAux(peca);
            break;

        case TIPO_Z2:
            status = rotacionaPecaAux(peca);
            break;

        case TIPO_O:
            status = 1;
            break;

        case TIPO_L1:
            status = rotacionaPecaAux(peca);
            break;

        case TIPO_L2:
            status = rotacionaPecaAux(peca);
            break;

        case TIPO_I:
            status = rotacionaPecaAux(peca);
            break;

        default:
            break;
    }

    return status;
}

static int
rotacionaPecaAtual(void)
{
    int status;

    if (pecaAtual.tipoPeca == PECA_INVALIDA)
    {
        return -1;
    }

    apagaPecaAtual();
    status = rotacionaPeca(&pecaAtual);
    memcpy(&projecao.coordenadas, &pecaAtual.coordenadas, sizeof(projecao.coordenadas));
    while (movimentaPeca(&projecao, BAIXO) == true);
    desenhaPecaAtual();

    return status;
}

static void
apagaPecaAtual(void)
{
    apagaPeca(pecaAtual);
    apagaPeca(projecao);
}

static void
desenhaPecaAtual(void)
{
    desenhaPeca(pecaAtual);

    if (getPieceHigherPosY(projecao) - getPieceLowerPosY(pecaAtual) > 3)
    {
        desenhaPeca(projecao);
    }
}

static int
getPieceLowerPosY(peca_s peca)
{
    int lowerPos;

    if (peca.tipoPeca == PECA_INVALIDA)
    {
        return -1;
    }

    lowerPos = peca.coordenadas[0].posicaoY;

    for (int i = 1; i < peca.numCoordenadas; i++)
    {
        if (peca.coordenadas[i].posicaoY > lowerPos)
        {
            lowerPos = peca.coordenadas[i].posicaoY;
        }
    }

    return lowerPos;
}

static int
getPieceHigherPosY(peca_s peca)
{
    int higherPos;

    if (peca.tipoPeca == PECA_INVALIDA)
    {
        return -1;
    }

    higherPos = peca.coordenadas[0].posicaoY;

    for (int i = 1; i < peca.numCoordenadas; i++)
    {
        if (peca.coordenadas[i].posicaoY < higherPos)
        {
            higherPos = peca.coordenadas[i].posicaoY;
        }
    }

    return higherPos;
}

static int
movimentaPecaAtual(direcao_e direcao)
{
    int status;

    if (pecaAtual.tipoPeca == PECA_INVALIDA)
    {
        return -1;
    }

    apagaPecaAtual();
    status = movimentaPeca(&pecaAtual, direcao);

    if (direcao != BAIXO)
    {
        memcpy(&projecao.coordenadas, &pecaAtual.coordenadas, sizeof(projecao.coordenadas));
        while (movimentaPeca(&projecao, BAIXO) == true);
    }

    desenhaPecaAtual();

    return status;
}
static int
movimentaPeca(peca_s *peca, direcao_e direcao)
{
    if (peca->tipoPeca == PECA_INVALIDA)
    {
        return -1;
    }

    for (int i = 0; i < peca->numCoordenadas; i++)
    {
        if (direcao == DIREITA || direcao == ESQUERDA)
        {
            if (peca->coordenadas[i].posicaoX + direcao < 0  ||
                peca->coordenadas[i].posicaoX + direcao >= ARENA_LARGURA ||
                (peca->coordenadas[i].posicaoY > 0 &&
                 arena[peca->coordenadas[i].posicaoY][peca->coordenadas[i].posicaoX + direcao] != PECA_INVALIDA))
            {
                return 0;
            }
        }
        else
        {
            if (peca->coordenadas[i].posicaoY + 1 >= ARENA_ALTURA ||
                (peca->coordenadas[i].posicaoY + 1 > 0 &&
                 arena[peca->coordenadas[i].posicaoY + 1][peca->coordenadas[i].posicaoX] != PECA_INVALIDA))
            {
                return 0;
            }
        }
    }

    for (int i = 0; i < peca->numCoordenadas; i++)
    {
        if (direcao == DIREITA || direcao == ESQUERDA)
        {
            peca->coordenadas[i].posicaoX += direcao;
        }
        else
        {
            peca->coordenadas[i].posicaoY++;
        }
    }

    return 1;
}

static void
apagaPeca(peca_s peca)
{
    int maxY, maxX;
    int posY, posX;

    if (peca.tipoPeca == PECA_INVALIDA)
    {
        return;
    }

    // pega as dimensoes da tela
    getmaxyx(stdscr, maxY, maxX);

    // determina a posicao onde desenhar o menu
    posY = (maxY - ARENA_ALTURA) / 2;
    posX = (maxX - ARENA_LARGURA) / 2;

    // ativa cores se possivel
    if (has_colors())
    {
        attron(COLOR_PAIR(PAIR_ARENA_JOGO));
    }

    for (int i = 0; i < peca.numCoordenadas; i++)
    {
        if (peca.coordenadas[i].posicaoY > 0)
        {
            mvaddstr(peca.coordenadas[i].posicaoY + posY,
                     peca.coordenadas[i].posicaoX * 2 + posX,
                     "  ");
        }
    }

    // desativa cores se possivel
    if (has_colors())
    {
        attroff(COLOR_PAIR(PAIR_ARENA_JOGO));
    }

    refresh();
}

static int
atualizaArena(void)
{
    int j;
    int num = 0;

    for (int i = 0; i < pecaAtual.numCoordenadas; i++)
    {
        arena[pecaAtual.coordenadas[i].posicaoY][pecaAtual.coordenadas[i].posicaoX] = pecaAtual.tipoPeca;
    }

    for (int i = 1; i < ARENA_ALTURA; i++)
    {
        for (j = 0; j < ARENA_LARGURA; j++)
        {
            if (arena[i][j] == PECA_INVALIDA)
            {
                break;
            }
        }

        if (j == ARENA_LARGURA)
        {
            eliminaLinha(i);
            num++;
        }
    }

    pecaAtual.numCoordenadas = 0;
    pecaAtual.tipoPeca = PECA_INVALIDA;

    return num;
}

static void
atualizaEstatisticas(int num)
{
    int valorBase[] = {40, 100, 300, 1200};

    if (num != 0)
    {
        int aux1, aux2;

        aux1 = numLinhasEliminadas / 10;
        numLinhasEliminadas += num;
        aux2 = numLinhasEliminadas / 10;

        if (aux1 != aux2)
        {
            dificuldade = (dificuldade + aux2 - aux1 > 30) ? 30 : dificuldade + aux2 - aux1;
            iniciaTimer(1000000 - dificuldade * 20000);
        }

        pontuacao += valorBase[num - 1] * (dificuldade + 1);
    }
}

static void
eliminaLinha(int linha)
{
    for (int i = linha; i > 0; i--)
    {
        for (int j = 0; j < ARENA_LARGURA; j++)
        {
            arena[i][j] = arena[i - 1][j];
        }
    }
}

static void
finalizaJogo(void)
{
    atualizaRecorde();
    for (int i = 0; i < ARENA_ALTURA; i++)
    {
        for (int j = 0; j < ARENA_LARGURA; j++)
        {
            arena[i][j] = PECA_INVALIDA;
        }
    }
    memset(numPecasEscolhidas, 0, sizeof(numPecasEscolhidas));
    pecaAtual.numCoordenadas = 0;
    pecaAtual.tipoPeca = PECA_INVALIDA;
    proximoTipoPeca = PECA_INVALIDA;
    pontuacao = 0;
    dificuldade = jogo.dificuldadeInicial;
    numLinhasEliminadas = 0;
    gameOver = true;
    iniciaTimer(0);
}

static bool
verificaGameOver(void)
{
    for (int j = 0; j < ARENA_LARGURA; j++)
    {
        if (arena[0][j] != PECA_INVALIDA)
        {
            return true;
        }
    }

    return false;
}

static void
escolhePecaAtual(void)
{
    tipoPecas_e tipoPeca;

    if (proximoTipoPeca == PECA_INVALIDA)
    {
        proximoTipoPeca = random() % NUM_TIPO_PECAS;
    }

    numPecasEscolhidas[proximoTipoPeca]++;
    pecaAtual.tipoPeca = proximoTipoPeca;
    pecaAtual.numCoordenadas = 1;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (pecas[proximoTipoPeca][i][j] == 1)
            {
                pecaAtual.coordenadas[pecaAtual.numCoordenadas].posicaoX = j;
                pecaAtual.coordenadas[pecaAtual.numCoordenadas].posicaoY = i;
                pecaAtual.numCoordenadas++;
            }
            // ponto de rotacao eh sempre a posicao 0
            else if (pecas[proximoTipoPeca][i][j] == 2)
            {
                pecaAtual.coordenadas[0].posicaoX = j;
                pecaAtual.coordenadas[0].posicaoY = i;
            }

        }
    }

    for (int i = 0, j = random() % ARENA_LARGURA; i < j; i++)
    {
        movimentaPeca(&pecaAtual, DIREITA);
    }

    for (int i = 0, j = random() % 4; i < j; i++)
    {
        rotacionaPeca(&pecaAtual);
    }

    memcpy(&projecao, &pecaAtual, sizeof(projecao));
    while (movimentaPeca(&projecao, BAIXO) == true);
    proximoTipoPeca = random() % NUM_TIPO_PECAS;
}

static void
desenhaDashBoard(void)
{
    int maxY, maxX;
    int posY, posX;
    int baseY;
    int total = 0;

    // pega as dimensoes da tela
    getmaxyx(stdscr, maxY, maxX);

    // determina a posicao onde desenhar o menu
    posY = (maxY - ARENA_ALTURA) / 2 + 1;
    posX = (maxX - ARENA_LARGURA) / 2 - 22;
    baseY = posY - ((NUM_TIPO_PECAS * 2 + NUM_TIPO_PECAS - 1) - ARENA_ALTURA) / 2 - 2;

    for (int k = 0; k < NUM_TIPO_PECAS; k++)
    {
        total += numPecasEscolhidas[k];
        desenhaPecaGenerica(k, baseY + k * 3, posX + ARENA_LARGURA * 2 + 27);

        if (has_colors())
        {
            attron(COLOR_PAIR(PAIR_DASHBOARD_JOGO));
        }

        mvprintw(baseY + k * 3, posX + ARENA_LARGURA * 2 + 36, "%d", numPecasEscolhidas[k]);

        if (has_colors())
        {
            attroff(COLOR_PAIR(PAIR_DASHBOARD_JOGO));
        }

        if (k == proximoTipoPeca)
        {
            desenhaPecaGenerica(k, posY, posX + 12);
        }
    }

    if (has_colors())
    {
        attron(COLOR_PAIR(PAIR_DASHBOARD_JOGO));
    }

    mvaddstr(posY, posX, "Prox. peca: ");
    mvprintw(baseY + NUM_TIPO_PECAS * 2 + NUM_TIPO_PECAS - 1, posX + ARENA_LARGURA * 2 + 29, "Total: %d", total);
    mvprintw(posY + ARENA_ALTURA - 5, posX, "Recorde: %d", jogo.recordes[0]);
    mvprintw(posY + ARENA_ALTURA - 4, posX, "Pontos: %d", pontuacao);
    mvprintw(posY + ARENA_ALTURA - 3, posX, "Linhas: %d", numLinhasEliminadas);
    mvprintw(posY + ARENA_ALTURA - 2, posX, "Dificuldade: %d", dificuldade);

    // desativa cores se possivel
    if (has_colors())
    {
        attroff(COLOR_PAIR(PAIR_DASHBOARD_JOGO));
    }

    refresh();
}

static void
desenhaPecaGenerica(tipoPecas_e tipoPeca, int posicaoY, int posicaoX)
{
    int pecaCor[] = {PAIR_PECA_TIPO_T_JOGO,
                     PAIR_PECA_TIPO_Z1_JOGO,
                     PAIR_PECA_TIPO_Z2_JOGO,
                     PAIR_PECA_TIPO_O_JOGO,
                     PAIR_PECA_TIPO_L1_JOGO,
                     PAIR_PECA_TIPO_L2_JOGO,
                     PAIR_PECA_TIPO_I_JOGO};

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (pecas[tipoPeca][i][j] != 0)
            {
                if (has_colors())
                {
                    attron(COLOR_PAIR(pecaCor[tipoPeca]));
                }

                mvaddstr(posicaoY + i, posicaoX + j * 2, "..");

                if (has_colors())
                {
                    attroff(COLOR_PAIR(pecaCor[tipoPeca]));
                }
            }
            else 
            {
                if (has_colors())
                {
                    attron(COLOR_PAIR(PAIR_OPCOES_MENU));
                }

                mvaddstr(posicaoY + i, posicaoX + j * 2, "  ");

                if (has_colors())
                {
                    attroff(COLOR_PAIR(PAIR_OPCOES_MENU));
                }
            }
        }
    }
}

static void
desenhaGameOver(void)
{
    int maxY, maxX;
    int posY, posX;
    char *texto = "Fim de jogo!";

    // pega as dimensoes da tela
    getmaxyx(stdscr, maxY, maxX);

    // determina a posicao onde desenhar o menu
    posY = (maxY + ARENA_ALTURA) / 2 + 2;
    posX = (maxX - strlen(texto)) / 2;

    // ativa cores se possivel
    if (has_colors())
    {
        attron(COLOR_PAIR(PAIR_GAMEOVER_JOGO));
    }

    mvaddstr(posY, posX, texto);

    // desativa cores se possivel
    if (has_colors())
    {
        attroff(COLOR_PAIR(PAIR_GAMEOVER_JOGO));
    }

    refresh();
}

static void
desenhaArena(void)
{
    int maxY, maxX;
    int posY, posX;

    // pega as dimensoes da tela
    getmaxyx(stdscr, maxY, maxX);

    // determina a posicao onde desenhar a arena
    posY = (maxY - ARENA_ALTURA) / 2;
    posX = (maxX - ARENA_LARGURA) / 2;


    for (int i = 1; i < ARENA_ALTURA; i++)
    {
        for (int j = 0; j < ARENA_LARGURA; j++)
        {
            if (arena[i][j] == PECA_INVALIDA)
            {
                // ativa cores se possivel
                if (has_colors())
                {
                    attron(COLOR_PAIR(PAIR_ARENA_JOGO));
                }

                mvaddstr(posY + i, posX + j * 2, "  ");

                // desativa cores se possivel
                if (has_colors())
                {
                    attroff(COLOR_PAIR(PAIR_ARENA_JOGO));
                }
            }
            else
            {
                int pecaCor[] = {PAIR_PECA_TIPO_T_JOGO,
                                 PAIR_PECA_TIPO_Z1_JOGO,
                                 PAIR_PECA_TIPO_Z2_JOGO,
                                 PAIR_PECA_TIPO_O_JOGO,
                                 PAIR_PECA_TIPO_L1_JOGO,
                                 PAIR_PECA_TIPO_L2_JOGO,
                                 PAIR_PECA_TIPO_I_JOGO};

                // ativa cores se possivel
                if (has_colors())
                {
                    attron(COLOR_PAIR(pecaCor[arena[i][j]]));
                }

                mvaddstr(posY + i, posX + j * 2, "..");

                // desativa cores se possivel
                if (has_colors())
                {
                    attroff(COLOR_PAIR(pecaCor[arena[i][j]]));
                }
            }
        }
    }

    refresh();
}

static void
desenhaLogoJogo(void)
{

}

static void
desenhaPeca(peca_s peca)
{
    int maxY, maxX;
    int posY, posX;
    int pecaCor[] = {PAIR_PECA_TIPO_T_JOGO,
                     PAIR_PECA_TIPO_Z1_JOGO,
                     PAIR_PECA_TIPO_Z2_JOGO,
                     PAIR_PECA_TIPO_O_JOGO,
                     PAIR_PECA_TIPO_L1_JOGO,
                     PAIR_PECA_TIPO_L2_JOGO,
                     PAIR_PECA_TIPO_I_JOGO};

    if (peca.tipoPeca == PECA_INVALIDA)
    {
        return;
    }

    // pega as dimensoes da tela
    getmaxyx(stdscr, maxY, maxX);

    // determina a posicao onde desenhar a arena
    posY = (maxY - ARENA_ALTURA) / 2;
    posX = (maxX - ARENA_LARGURA) / 2;

    // ativa cores se possivel
    if (has_colors())
    {
        attron(COLOR_PAIR(pecaCor[peca.tipoPeca]));
    }

    for (int i = 0; i < peca.numCoordenadas; i++)
    {
        if (peca.coordenadas[i].posicaoY > 0)
        {
            mvaddstr(peca.coordenadas[i].posicaoY + posY,
                     peca.coordenadas[i].posicaoX * 2 + posX,
                     "..");
        }
    }

    // desativa cores se possivel
    if (has_colors())
    {
        attroff(COLOR_PAIR(pecaCor[peca.tipoPeca]));
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
    desenhaLogoJogo();
    desenhaDashBoard();
    desenhaArena();
    desenhaPecaAtual();
    desenhaBordas();
}

static void
atualizaRecorde(void)
{
    for (int i = 0; i < sizeof(jogo.recordes) / sizeof(jogo.recordes[0]); i++)
    {
        if (pontuacao > jogo.recordes[i])
        {
            for (int j = sizeof(jogo.recordes) / sizeof(jogo.recordes[0]) - 1; j > i; j--)
            {
                jogo.recordes[j] = jogo.recordes[j - 1];
            }

            jogo.recordes[i] = pontuacao;
            break;
        }
    }
}

static void
abaixaPecaAtual(void)
{
    if (movimentaPecaAtual(BAIXO) == 0)
    {
        atualizaEstatisticas(atualizaArena());
        desenhaArena();

        if ((gameOver = verificaGameOver()) == true)
        {
            desenhaGameOver();
            finalizaJogo();
        }
        else
        {
            escolhePecaAtual();
            desenhaDashBoard();
        }
    }
}

static void
sigalrmHandler(int _ __attribute__((unused)))
{
    abaixaPecaAtual();
}

static bool
configuraTimer(void)
{ 
    static struct sigaction sa;

    // estrutura ja esta configurada
    if (sa.sa_handler != NULL)
    {
        return true;
    }

    // configura estrutura
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigalrmHandler;

    // zera a estrutura em caso de erro
    if (sigaction(SIGALRM, &sa, NULL) == -1)
    {
        memset(&sa, 0, sizeof(sa));
        return false;
    }

    return true;
}

static bool
iniciaTimer(int tempo)
{
    static struct itimerval itv;
    int sec = tempo / 1000000;
    int usec = tempo - sec * 1000000;

    itv.it_value.tv_sec = sec;
    itv.it_value.tv_usec = usec;
    itv.it_interval.tv_sec = sec;
    itv.it_interval.tv_usec = usec;

    if (setitimer(ITIMER_REAL, &itv, NULL) == -1)
    {
        return false;
    }

    return true;
}

static void
desenhaBordas(void)
{
    // pega as dimensoes da tela
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    // ativa cores se possivel (else b&w highlighting)
    if (has_colors())
    {
        attron(COLOR_PAIR(PAIR_BORDAS_JOGO));
    }

    // desenha as bordas
    for (int i = 0; i < maxX; i++)
    {
        mvaddch(0, i, ' ');
        mvaddch(maxY - 1, i, ' ');
    }

    // desenha o cabecalho
    mvprintw(0, (maxX - 17) / 2, "%s by %s", TITULO, AUTOR);

    // desenha o rodape
    mvaddstr(maxY - 1, (maxX - 40) / 2, "[N]ovo Jogo    [M]enu inicial    [P]ause");

    // desativa cores se possivel (else b&w highlighting)
    if (has_colors())
    {
        attroff(COLOR_PAIR(PAIR_BORDAS_JOGO));
    }
}

