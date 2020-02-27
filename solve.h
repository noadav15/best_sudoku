/*
 * solve.h
 *
 *  Created on: Feb 25, 2020
 *      Author: noada
 */
/*
 * solver.c
 * This module implements the backtracking algorithm.
 */

#ifndef SOLVE_H_
#define SOLVE_H_
#include "game.h"

int countSolutions(Game *game);
/*algorithem Deterministic
 * get board and return 1 if there a solution to the board and update the board
 * if there no solution return 0
 */

void setCellForAlgo(int row, int column, int value,Game *game);
/*get the cell index and value and update the cell*/

void fillArrWithOption(int *arr_of_options,Game *game,int row,int column);
int stillHasOptionForCell(int *arr_of_options, Game *game);
#endif /* SOLVER_H_ */


