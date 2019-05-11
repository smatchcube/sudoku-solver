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

typedef uint_fast8_t digit;
typedef uint_fast8_t pos;	

void load_grid(FILE *, digit board[9][9]);
void solve(digit board[9][9]);
void set_first_pos(pos * i, pos * j, bool was_originally_here[9][9]);
void next_box(pos * i, pos * j, bool was_originally_here[9][9]);
void previous_box(pos * i, pos * j, bool was_originally_here[9][9]);
bool is_correct(pos i, pos j, digit board[9][9]);
void print_board(digit board[9][9]);

int main(int argc, char * argv[])
{
	FILE * fp;
	digit board[9][9];

	if (argc != 2) {
		printf("Usage: %s [FILE]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if ((fp = fopen(argv[1], "r")) == NULL) {
		printf("Can't open %s for read.\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	load_grid(fp, board);
	fclose(fp);

	solve(board);
	print_board(board);

	return 0;
}

void load_grid(FILE * fp, digit board[9][9])
/* set up the grid of the game */
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
	bool was_originally_here[9][9];
	for (pos i = 0; i < 9; ++i) {
		for (pos j = 0; j < 9; ++j)
			was_originally_here[i][j] = board[i][j] != 0;
	}
	pos i;
	pos j;
	set_first_pos(&i, &j, was_originally_here);
	board[i][j] = 1;
	while (i != 8 || j != 8 || !is_correct(8, 8, board)) {
		if (is_correct(i, j, board)) {        //forward to the next box
			next_box(&i, &j, was_originally_here);
			board[i][j] = 1;
		} else {               //try next digit
			++board[i][j];
			while (board[i][j] > 9) {
				previous_box(&i, &j, was_originally_here);
				++board[i][j];
			}
		}

	}
}

void set_first_pos(pos * i, pos * j, bool was_originally_here[9][9])
/* Set the position to the first valid position */
{
	*i = 0;
	*j = 0;
	while (was_originally_here[*i][*j]) {
		if (*j == 8) {
			(*j) = 0;
			++(*i);
		} else
			++(*j);
	}
}	

void next_box(pos * i, pos * j, bool was_originally_here[9][9])
/* set the position to the next empty box 
 * or last box if everything is filled */
{
	do {
		if (*j == 8) {
			(*j) = 0;
			++(*i);
		} else
			++(*j);
	} while (was_originally_here[*i][*j] && (*i != 8 && *j != 8));
}

void previous_box(pos * i, pos * j, bool was_originally_here[9][9])
/* set the position to the previous valid box */
{
	do {
		if (*j == 0) {
			(*j) = 8;
			--(*i);
		} else
			--(*j);
	} while (was_originally_here[*i][*j]);
}

bool is_correct(pos i, pos j, digit board[9][9])
/* check wether the digit at the position is legit */
{
	digit d = board[i][j];

	// check for duplicate in the row j
	for (pos n = 0; n < 9; ++n) {
		if (board[i][n] == d && n != j)
			return false;
	}
	// check for duplicate in the column i
	for (pos n = 0; n < 9; ++n) {
		if (board[n][j] == d && n != i)
			return false;
	}
	// check for duplicate in the square
	pos u = i - i % 3 + 3;
	pos v = j - j % 3;
	pos w = j - j % 3 + 3;
	for (pos n = i - i % 3; n < u; ++n) {
		for (pos m = v; m < w; ++m) {
			if (board[n][m] == d && n != i && m != j)
				return false;
		}
	}
	return true;
}

void print_board(digit board[9][9])
{
	for (pos i = 0; i < 9; ++i) {
		for (pos j = 0; j < 9; ++j)
			printf("%c ", '0' + board[i][j]);
		printf("\b\n");
	}
}
