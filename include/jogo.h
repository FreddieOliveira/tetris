#ifndef JOGO_H
#define JOGO_H

typedef enum
{
    TELA_MENU = 0,
    TELA_DIFICULDADE,
    TELA_JOGO,
    SAIR
} estados_e;

int executaJogo(void);

#endif

