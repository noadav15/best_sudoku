/*This module is responsible for the functions related to solving a game board.*/


#ifndef SOLVE_H_
#define SOLVE_H_
#include "game.h"


int countSolutions(Game *game);

void fillArrWithOption(int *arr_of_options,Game *game,int row,int column);

int stillHasOptionForCell(int *arr_of_options, Game *game);
#endif /* SOLVER_H_ */


