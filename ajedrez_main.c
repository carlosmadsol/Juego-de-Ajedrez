#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SIZE 8

// Definir las piezas
#define EMPTY ' '
#define WHITE_PAWN 'P'
#define BLACK_PAWN 'p'
#define WHITE_ROOK 'R'
#define BLACK_ROOK 'r'
#define WHITE_KNIGHT 'N'
#define BLACK_KNIGHT 'n'
#define WHITE_BISHOP 'B'
#define BLACK_BISHOP 'b'
#define WHITE_QUEEN 'Q'
#define BLACK_QUEEN 'q'
#define WHITE_KING 'K'
#define BLACK_KING 'k'

typedef struct {
    char board[BOARD_SIZE][BOARD_SIZE];
    int turn; // 0 para blanco, 1 para negro
} ChessGame;

// Función para inicializar el tablero
void initBoard(ChessGame *game) {
    // Inicializar las piezas blancas y negras
    const char *initial_rows[] = {
        "RNBQKBNR", // Primera fila: piezas negras
        "PPPPPPPP", // Segunda fila: peones negros
        "        ", // Vacío
        "        ", // Vacío
        "        ", // Vacío
        "        ", // Vacío
        "pppppppp", // Séptima fila: peones blancos
        "rnbqkbnr"  // Octava fila: piezas blancas
    };

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            game->board[i][j] = initial_rows[i][j];
        }
    }
    game->turn = 0; // Comienza el jugador blanco
}

// Función para imprimir el tablero
void printBoard(ChessGame *game) {
    printf("\n  a b c d e f g h\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%d ", BOARD_SIZE - i);
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%c ", game->board[i][j]);
        }
        printf("%d\n", BOARD_SIZE - i);
    }
    printf("  a b c d e f g h\n");
}

// Función para verificar si un movimiento es válido
int is MovVal(ChessGame *game, const char *move) {
    // Falta 
}
// Función para ejecutar un movimiento
void Mover(ChessGame *game, const char *move) {
    // Convertir la notación algebraica en índices de tablero
    int from_row = 8 - (move[1] - '0');
    int from_col = move[0] - 'a';
    int to_row = 8 - (move[3] - '0');
    int to_col = move[2] - 'a';

    // Mover la pieza en el tablero
    game->board[to_row][to_col] = game->board[from_row][from_col];
    game->board[from_row][from_col] = EMPTY;
}

// Función principal del juego
void playGame() {
    ChessGame game;
    initBoard(&game);
    char move[5]; // Para almacenar un movimiento en notación algebraica (ej. "e2 e4")

    while (1) {
        printBoard(&game);
        printf("%s's turn (e.g., e2 e4): ", game.turn == 0 ? "White" : "Black");
        fgets(move, sizeof(move), stdin);
        move[strcspn(move, "\n")] = 0; // Eliminar salto de línea

        // Validar y realizar el movimiento
        if (is MovVal (&game, move)) {
            Mover(&game, move);
            game.turn = 1 - game.turn; // Cambiar el turno
        } else {
            printf("Invalid move! Try again.\n");
        }
    }
}

int main() {
    playGame();
    return 0;
}