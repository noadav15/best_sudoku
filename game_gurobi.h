/*
 * game_gurobi.h
 *
 *  Created on: Feb 28, 2020
 *      Author: noada
 */

#ifndef GAME_GUROBI_H_
#define GAME_GUROBI_H_

int validate(Game *game);
void hint(Game *game, int row,int column);
int guess(Game *game, float X);
void guessHint(Game *game, int row, int column);
int generate(Game *game, int X,int Y);

#endif /* GAME_GUROBI_H_ */
