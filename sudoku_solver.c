/* sudoku_solver.c
 * 
 * This is a simple sudoku solver using backtracking.
 * A valid file for input consist of nine lines of nine digits,
 * without space or delimiters between digits.
 * Empty boxes are represented by the digit zero.
 *
 * Notation: i for column, j for row, both zero indexed 
 * and starting from the upper left corner of the grid.
 *
 * */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

typedef uint_fast8_t digit;
typedef uint_fast8_t pos;	

void load_grid(FILE *, digit board[9][9]);
void solve(digit board[9][9]);
void set_first_pos(pos * i, pos * j, const bool grid[9][9]);
void next_cell(pos * i, pos * j, const bool grid[9][9]);
void previous_cell(pos * i, pos * j, const bool grid[9][9]);
bool is_legit(pos i, pos j, const digit board[9][9]);
void print_board(const digit board[9][9]);

int main(int argc, char * argv[])
{
	FILE * fp;
	digit board[9][9];

	if (argc != 2) {
		fprintf(stderr, "Usage: %s [FILE]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if ((fp = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "Can't open %s for read: %s\n",
			argv[1], strerror(errno));
		exit(EXIT_FAILURE);
	}
	load_grid(fp, board);
	fclose(fp);

	solve(board);
	print_board(board);

	return EXIT_SUCCESS;
}

void load_grid(FILE * fp, digit board[9][9])
/* set up the initial grid */
{
	for (pos i = 0; i < 9; ++i) {
		for (pos j = 0; j < 9; ++j)
			board[i][j] = fgetc(fp) - '0';
		fgetc(fp);
	}
}

void solve(digit board[9][9])
/* solve the grid using backtracking */
{
	bool grid[9][9];
	for (pos i = 0; i < 9; ++i) {
		for (pos j = 0; j < 9; ++j)
			grid[i][j] = board[i][j] != 0;
	}
	pos i;
	pos j;
	set_first_pos(&i, &j, grid);
	board[i][j] = 1;
	while (i != 8 || j != 8 || !is_legit(8, 8, board)) {
		if (is_legit(i, j, board)) {        //forward to the next box
			next_cell(&i, &j, grid);
			board[i][j] = 1;
		} else {               //try next digit
			++board[i][j];
			while (board[i][j] > 9) {
				previous_cell(&i, &j, grid);
				++board[i][j];
			}
		}

	}
}

void set_first_pos(pos * i, pos * j, const bool grid[9][9])
/* Set the position to the first valid position */
{
	*i = 0;
	*j = 0;
	while (grid[*i][*j]) {
		if (*j == 8) {
			(*j) = 0;
			++(*i);
		} else
			++(*j);
	}
}	

void next_cell(pos * i, pos * j, const bool grid[9][9])
/* set the position to the next empty cell 
 * or last cell if everything is filled */
{
	do {
		if (*j == 8) {
			(*j) = 0;
			++(*i);
		} else
			++(*j);
	} while (grid[*i][*j] && (*i != 8 && *j != 8));
}

void previous_cell(pos * i, pos * j, const bool grid[9][9])
/* set the position to the previous valid cell */
{
	do {
		if (*j == 0) {
			(*j) = 8;
			--(*i);
		} else
			--(*j);
	} while (grid[*i][*j]);
}

bool is_legit(pos i, pos j, const digit board[9][9])
/* check whether the digit at the position is legit */
{
	digit d = board[i][j];
	pos n, m;
	// check for duplicate in the row j
	for (n = 0; n < 9; ++n) {
		if (board[i][n] == d && n != j)
			return false;
	}
	// check for duplicate in the column i
	for (n = 0; n < 9; ++n) {
		if (board[n][j] == d && n != i)
			return false;
	}
	// check for duplicate in the 3x3 block
	pos u = i - i % 3 + 3;
	pos v = j - j % 3;
	pos w = j - j % 3 + 3;
	for (n = i - i % 3; n < u; ++n) {
		for (m = v; m < w; ++m) {
			if (board[n][m] == d && n != i && m != j)
				return false;
		}
	}
	return true;
}

void print_board(const digit board[9][9])
{
	for (pos i = 0; i < 9; ++i) {
		for (pos j = 0; j < 9; ++j)
			printf("%c ", '0' + board[i][j]);
		printf("\b\n");
	}
}
