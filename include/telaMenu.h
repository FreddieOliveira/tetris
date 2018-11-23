// prototipos
void desenhaMenu(void);
void desenhaOpcoesMenu(void);
void apaga(void);
void alerta(void);
char corrige(void);
char despedida(void);
void show_dica(int op, int aux_i, int aux_j);
void draw_borders(void);
void draw_grid(void);
void draw_logo(void);
void draw_numbers(void);
void handle_signal(int signum);
void hide_banner(void);
void hide_timer(void);
bool load_board(void);
void load_dica(void);
void redraw_all(void);
bool restart_game(void);
void show_banner(char *b);
void show_cursor(void);
void show_sol(void);
void show_timer(void);
void shutdown(void);
bool startup(void);
bool valida(void);


// tamanho de cada int (bytes) em *.bin files
#define INTSIZE 4

// autor do jogo
#define AUTOR "Freddie"

// titulo do jogo
#define TITULO "Tetris"

// cor do banner
#define FG_BANNER COLOR_CYAN
#define BG_BANNER COLOR_BLACK

// cor do grid
#define FG_GRID COLOR_WHITE
#define BG_GRID COLOR_BLACK

// cor das boards
#define FG_BORDER COLOR_WHITE
#define BG_BORDER COLOR_YELLOW

// cor do logo
#define FG_LOGO COLOR_YELLOW
#define BG_LOGO COLOR_BLACK

// cor do cronometro
#define FG_TIMER COLOR_GREEN
#define BG_TIMER COLOR_BLACK

// cor do menu
#define FG_MENU COLOR_RED
#define BG_MENU COLOR_BLACK

// apelido para os pares de cores
enum
{
    PAIR_BANNER = 1,
    PAIR_GRID,
    PAIR_BORDER,
    PAIR_LOGO,
    PAIR_TIMER,
    PAIR_MENU
};

struct {
    // o level atual
    char *level;

    // a board do jogo
    int board[9][9];
    
    // a board solucionada
    int sol[9][9];

    //  array dos numeros ainda nao preenchidos
    int *dica;

    // o numero da board
    int number;

    // as coordenadas do topo esquerdo da board
    int top, left;

    // o local atual do cursor entre (0,0) e (8,8)
    int y, x;
    
    int yy, xx;
    
    int ex_number;
    
    int cont;

    int cheats[2];
    
} g;
