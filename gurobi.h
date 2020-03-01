/*
 * gurobi.h
 *
 *  Created on: Feb 27, 2020
 *      Author: noada
 */

#ifndef GUROBI_H_
#define GUROBI_H_

int createEnv();
int noMessages();
int createEmptyModel();
int getArrWithOnlyValidOption(int *arr_of_all_values, int *final_arr, Game *game);
int **createObjAndSendToModel(Game *game, int ILP);
int setCellConstraints(Game *game);
int setRowConstraints(Game *game);
int setColumnConstraints(Game *game);
int setBlockConstraints(Game *game);
int optimizeTheModel();
int getSol();
int end();
void freeGR();
int getHint(int row, int column, Game *game);
int creatRangeConstrainInLp();
void fillAllGuesses(Game *game,float X);
int createBounds();
int sendToModel();
void printGuessHint(Game *game, int row, int column);
Game *findHintBoard(Game *game);

#endif /* GUROBI_H_ */
