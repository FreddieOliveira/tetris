#ifndef TELA_H
#define TELA_H

// macro do control
#define CTRL(x) ((x) & ~0140)

// cor da logo
#define FG_LOGO_MENU_1 COLOR_YELLOW
#define BG_LOGO_MENU_1 COLOR_BLACK

// cor da logo
#define FG_LOGO_MENU_2 COLOR_WHITE
#define BG_LOGO_MENU_2 COLOR_YELLOW

// cor do autor
#define FG_AUTOR COLOR_CYAN
#define BG_AUTOR COLOR_BLACK

// cor das opcoes do menu
#define FG_OPCOES_MENU COLOR_WHITE
#define BG_OPCOES_MENU COLOR_BLACK

// cor da selacao da opcao do menu
#define FG_SELECAO_OPCAO_MENU COLOR_RED
#define BG_SELECAO_OPCAO_MENU COLOR_BLACK

// cor da selacao da opcao do menu
#define FG_ARENA_JOGO COLOR_WHITE
#define BG_ARENA_JOGO 243

// cor da selacao da opcao do menu
#define FG_PECA_TIPO_T_JOGO COLOR_WHITE
#define BG_PECA_TIPO_T_JOGO COLOR_RED

// cor da selacao da opcao do menu
#define FG_PECA_TIPO_Z1_JOGO COLOR_WHITE
#define BG_PECA_TIPO_Z1_JOGO 238

// cor da selacao da opcao do menu
#define FG_PECA_TIPO_Z2_JOGO COLOR_WHITE
#define BG_PECA_TIPO_Z2_JOGO COLOR_GREEN

// cor da selacao da opcao do menu
#define FG_PECA_TIPO_O_JOGO COLOR_WHITE
#define BG_PECA_TIPO_O_JOGO COLOR_YELLOW

// cor da selacao da opcao do menu
#define FG_PECA_TIPO_L1_JOGO COLOR_WHITE
#define BG_PECA_TIPO_L1_JOGO COLOR_BLUE

// cor da selacao da opcao do menu
#define FG_PECA_TIPO_L2_JOGO COLOR_WHITE
#define BG_PECA_TIPO_L2_JOGO COLOR_MAGENTA

// cor da selacao da opcao do menu
#define FG_PECA_TIPO_I_JOGO COLOR_WHITE
#define BG_PECA_TIPO_I_JOGO COLOR_CYAN

// cor da selacao da opcao do menu
#define FG_CONFIRMACAO_JOGO COLOR_YELLOW
#define BG_CONFIRMACAO_JOGO 236

// cor da selacao da opcao do menu
#define FG_GAMEOVER_JOGO COLOR_RED
#define BG_GAMEOVER_JOGO COLOR_BLACK

// cor da selacao da opcao do menu
#define FG_DASHBOARD_JOGO 15
#define BG_DASHBOARD_JOGO COLOR_BLACK

// cor da selacao da opcao do menu
#define FG_BORDAS_JOGO COLOR_YELLOW
#define BG_BORDAS_JOGO 237

// apelido para os pares de cores
enum
{
    PAIR_LOGO_MENU_1 = 1,
    PAIR_LOGO_MENU_2,
    PAIR_AUTOR,
    PAIR_OPCOES_MENU,
    PAIR_SELECAO_OPCAO_MENU,
    PAIR_ARENA_JOGO,
    PAIR_PECA_TIPO_T_JOGO,
    PAIR_PECA_TIPO_Z1_JOGO,
    PAIR_PECA_TIPO_Z2_JOGO,
    PAIR_PECA_TIPO_O_JOGO,
    PAIR_PECA_TIPO_L1_JOGO,
    PAIR_PECA_TIPO_L2_JOGO,
    PAIR_PECA_TIPO_I_JOGO,
    PAIR_CONFIRMACAO_JOGO,
    PAIR_GAMEOVER_JOGO,
    PAIR_DASHBOARD_JOGO,
    PAIR_BORDAS_JOGO
};

bool iniciaNcurses(void);
void finalizaNcurses(void);

#endif /* TELA_H */

