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
int autoFillBoard(Game *game);
void printBoard(Game *game);
int boardValueAreValid(Game *game);
void freeGame(Game *game);
void intilizeEmptyCell(Cell *c);
void setCell(int x, int y, int z, Game *game, int start);
void undoMove(Game *game);
void redoMove(Game *game);
void resetBoard(Game *game);
void markInvalidCells(Game *game);
int checkFixedCells(Game *game);
void fixCellsWithValues(Game *game);
Game* solve(char* fileName);
Game* edit(char* fileName);
void exitGame(Game *game);
void markErrors(int mark);
Game* initializeGame(int num_of_rows_in_block, int num_of_columns_in_block);
int invalidCell(Game *game, int row , int column, int ignore_not_fixed);
#endif /* GAME_H_ */
