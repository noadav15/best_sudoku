/*
 * game.h
 *
 *  Created on: Feb 25, 2020
 *      Author: noada
 */

#ifndef GAME_H_
#define GAME_H_
#include "structs.h"
void intilizeEmptyBoard(Game *game);
void printBoard(Game *game);
int boardValueAreValid(Game *game);
void freeGame(Game *game);
void intilizeEmptyCell(Cell *c);
void setCell(int x, int y, int z, Game *game, int start);
void undoMove(Game *game);
void redoMove(Game *game);
void resetBoard(Game *game);
#endif /* GAME_H_ */
