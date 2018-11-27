#ifndef JOGO_H
#define JOGO_H

// titulo do jogo
#define TITULO "Tetris"
#define AUTOR  "Nicole"

typedef enum
{
    TELA_MENU = 0,
    TELA_JOGO,
    SAIR
} estados_e;

typedef struct
{
    int dificuldadeInicial;
    int recordes[5];
} jogo_s;

jogo_s jogo;
int executaJogo(void);

#endif /* JOGO_H */

