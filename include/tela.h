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

// apelido para os pares de cores
enum
{
    PAIR_LOGO_MENU_1 = 1,
    PAIR_LOGO_MENU_2,
    PAIR_AUTOR,
    PAIR_OPCOES_MENU,
    PAIR_SELECAO_OPCAO_MENU
};

bool iniciarNcurses(void);
void finalizarNcurses(void);

#endif

