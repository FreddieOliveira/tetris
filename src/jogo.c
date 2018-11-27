#include <stdbool.h>
#include <stdio.h>
#include "jogo.h"
#include "tela.h"
#include "telaMenu.h"
#include "telaJogo.h"

static bool carregaDados(void);
static bool armazenaDados(void);

int
executaJogo(void)
{
    estados_e estado = 0;

    // iniciar ncurses
    if (iniciaNcurses() == false)
    {
        fprintf(stderr, "Erro iniciando ncurses!\n");
        return -1;
    }

    if (carregaDados() == false)
    {
        fprintf(stderr, "Erro ao carregar dados!\n");
    }

    do
    {
        switch (estado)
        {
            case TELA_MENU:
                estado = processaTelaMenu();
                break;

            case TELA_JOGO:
                estado = processaTelaJogo();
                break;

            default:
                estado = TELA_MENU;
                break;
        }
    } while (estado != SAIR);

    // finaliza ncurses
    finalizaNcurses();

    if (armazenaDados() == false)
    {
        fprintf(stderr, "Erro ao armazenar dados!\n");
    }

    return 0;
}

static bool
carregaDados(void)
{
    FILE *fp;

    if ((fp = fopen("arquivos/recordes.txt", "r")) == NULL)
    {
        return false;
    }

    for (int i = 0; i < sizeof(jogo.recordes) / sizeof(jogo.recordes[0]) || feof(fp); i++)
    {
        int num;

        fscanf(fp, "%d", &num);

        // insertion sort
        for (int i = 0; i < sizeof(jogo.recordes) / sizeof(jogo.recordes[0]); i++)
        {
            if (num > jogo.recordes[i])
            {
                for (int j = sizeof(jogo.recordes) / sizeof(jogo.recordes[0]) - 1; j > i; j--)
                {
                    jogo.recordes[j] = jogo.recordes[j - 1];
                }

                jogo.recordes[i] = num;
                break;
            }
        }
    }

    fclose(fp);

    return true;
}

static bool
armazenaDados(void)
{
    FILE *fp;

    if ((fp = fopen("arquivos/recordes.txt", "w")) == NULL)
    {
        return false;
    }
    
    for (int i = 0; i < sizeof(jogo.recordes) / sizeof(jogo.recordes[0]); i++)
    {
        fprintf(fp, "%d\n", jogo.recordes[i]);
    }

    fclose(fp);

    return true;
}

