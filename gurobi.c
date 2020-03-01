
#define ERRORMAL "ERROR: problem with memory allocation\n"
#define ERRORGUR "Error: Gurobi optimizer has failed\n"
#include <stdlib.h>
#include <stdio.h>
#include "gurobi_c.h"
#include "game.h"
#include "solve.h"

typedef struct{
	int row;
	int column;
	int value;
} Option;

GRBenv   *env   = NULL;
GRBmodel *model = NULL;
int       error = 0;
double    *sol;
int       *ind;
double    *val;
double    *obj;
double    *lb; /*determines lower bounds for the variables*/
double    *ub;  /*determines an upper bound for the variables.*/
Option    *obj_value;
char      *vtype;
int       optimstatus;
double    objval;

int number_of_varibals=0;
/*
 * creates a gurobi environment.
 */
int createEnv(){
	int error;
	error = GRBloadenv(&env, NULL);
	if (error) {
		return 0;
	}
	return 1;
}
int noMessages(){
	error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
	if (error) {
		printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		return 0;
	}
	return 1;
}


/* Create an empty model named "mip1" */
int createEmptyModel(){
	error = GRBnewmodel(env, &model, "mip1", number_of_varibals, obj, lb, ub, vtype, NULL);
	if (error) {
		printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
		return 0;
	}
	return 1;
}
/*create arr obj_value with the key between obj index and the real ijk indicator*/
void fillObjValue(Game *game){
	int i,j,value,invalid,count=0;
	for(i=1;i<=game->board_size;i++){
		for(j=1;j<=game->board_size;j++){
			if(game->board[i][j].value==0){
				for(value=1;value<=game->board_size;value++){
					game->board[i][j].value=value;
					invalid=invalidCell(game,i,j,0);
					if(invalid==0){
						obj_value[count].row=i;
						obj_value[count].column=j;
						obj_value[count].value=value;
						count++;
					}
					game->board[i][j].value=0;
				}
			}
		}
	}
}
int createObjAndSendToModel(Game *game, int ILP){
	int count=0,i,j,value,invalid=0;
	for(i=1;i<=game->board_size;i++){
		for(j=1;j<=game->board_size;j++){
			if(game->board[i][j].value==0){
				for(value=1;value<=game->board_size;value++){
					game->board[i][j].value=value;
					invalid=invalidCell(game,i,j,0);
					if(invalid==0){
						count++;
					}
					game->board[i][j].value=0;
				}
			}
		}
	}
	number_of_varibals=count;
	obj_value= (Option*)malloc(count*sizeof(Option));
	fillObjValue(game);
	obj=(double*)calloc(count,sizeof(double));
	if(obj==NULL){
		printf(ERRORMAL);
		return 0;
	}
	vtype=(char*)calloc(count,sizeof(char));
	if(vtype==NULL){
		printf(ERRORMAL);
		return 0;
	}
	/*put random coefficients for variables and  variable types to vtype arr*/
	for(i=0;i<count;i++){
		obj[i]= rand()%(2*game->board_size);
		if(ILP){
			vtype[i]=GRB_BINARY;
		}
		else{
			vtype[i]=GRB_CONTINUOUS;
		}
	}
	return 1;
}
int sendToModel(){

	/* add variables to model */
	error = GRBaddvars(model, number_of_varibals, 0, NULL, NULL, NULL, obj, lb, ub, vtype, NULL);
	if (error) {
		printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
		return 0;
	}
	/* Change objective sense to maximization */
	error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MINIMIZE);
	if (error) {
		printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		return 0;
	}
	/* update the model - to integrate new variables */
	error = GRBupdatemodel(model);
	if (error) {
		printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
		return 0;
	}
	return 1;
}

int findPlaceForOption(int i,int j,int value){
	int place;
	for(place=0;place<number_of_varibals;place++){
		if(obj_value[place].column==j && obj_value[place].row==i && obj_value[place].value==value){
			return place;
		}
	}
	return -1;
}
/*
 * sets cell constraints (every cell should be filled with exactly one value)
 */
int setCellConstraints(Game *game){
	int i,j,place,value,current_k=0;
	for(i=1;i<=game->board_size;i++){
		for(j=1;j<=game->board_size;j++){
			current_k=0;
			for(value=1;value<=game->board_size;value++){
				place=findPlaceForOption(i,j,value);
				if(place!=-1){
					current_k++;
				}
			}
			if(current_k>0){
				ind = (int*)calloc(current_k,sizeof(int));
				if(ind==NULL){
					printf(ERRORMAL);
					return 0;
				}
				val = (double*)calloc(current_k,sizeof(double));
				if(val==NULL){
					printf(ERRORMAL);
					return 0;
				}
				current_k=0;
				for(value=1;value<=game->board_size;value++){
					place=findPlaceForOption(i,j,value);
					if(place!=-1){
						ind[current_k]=place;
						val[current_k]=1.0;
						current_k++;
					}
				}
				/*add constrain to mode*/
				error = GRBaddconstr(model, current_k, ind, val, GRB_EQUAL, 1, NULL);
				if (error) {
					printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
					free(ind);
					free(val);
					return 0;
				}
				free(ind);
				free(val);
			}
		}
	}

	return 1;
}

int setRowConstraints(Game *game){
	int i,j,place,value,current_k=0;
	for(i=1;i<=game->board_size;i++){
		for(value=1;value<=game->board_size;value++){
			current_k=0;
			for(j=1;j<=game->board_size;j++){
				place=findPlaceForOption(i,j,value);
				if(place!=-1){
					current_k++;
				}
			}
			if(current_k>0){
				ind = (int*)calloc(current_k,sizeof(int));
				if(ind==NULL){
					printf(ERRORMAL);
					return 0;
				}
				val = (double*)calloc(current_k,sizeof(double));

				if(val==NULL){
					printf(ERRORMAL);
					return 0;
				}
				current_k=0;
				for(j=1;j<=game->board_size;j++){
					place=findPlaceForOption(i,j,value);
					if(place!=-1){
						ind[current_k]=place;
						val[current_k]=1.0;
						current_k++;
					}
				}
				/*add constrain to mode*/
				error = GRBaddconstr(model, current_k, ind, val, GRB_EQUAL, 1, NULL);
				if (error) {
					printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
					free(ind);
					free(val);

					return 0;
				}
				free(ind);
				free(val);
			}
		}
	}
	return 1;
}
int setColumnConstraints(Game *game){
	int i,j,place,value,current_k=0;
	for(i=1;i<=game->board_size;i++){
		for(value=1;value<=game->board_size;value++){
			current_k=0;
			for(j=1;j<=game->board_size;j++){
				place=findPlaceForOption(j,i,value);
				if(place!=-1){
					current_k++;
				}
			}
			if(current_k>0){
				ind = (int*)calloc(current_k,sizeof(int));
				if(ind==NULL){
					printf(ERRORMAL);
					return 0;
				}
				val = (double*)calloc(current_k,sizeof(double));
				if(val==NULL){
					printf(ERRORMAL);
					return 0;
				}
				current_k=0;
				for(j=1;j<=game->board_size;j++){
					place=findPlaceForOption(j,i,value);
					if(place!=-1){
						ind[current_k]=place;
						val[current_k]=1.0;
						current_k++;
					}
				}
				/*add constrain to mode*/
				error = GRBaddconstr(model, current_k, ind, val, GRB_EQUAL, 1, NULL);
				if (error) {
					printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
					free(ind);
					free(val);
					return 0;
				}
				free(ind);
				free(val);
			}
		}
	}
	return 1;
}
int setBlockConstraints(Game *game){
	int i,j,place,value,current_k=0,row,column;
	/*for each block*/
	for(i=1;i<=game->board_size;i=i+game->num_of_columns_in_block){
		for(j=1;j<=game->board_size;j=j+game->num_of_rows_in_block){
			/*for each value in each block*/
			for(value=1;value<=game->board_size;value++){
				current_k=0;
				for(row=j;row<j+game->num_of_rows_in_block;row++){
					for(column=i;column<i+game->num_of_columns_in_block;column++){
						place=findPlaceForOption(row,column,value);
						if(place!=-1){
							current_k++;
						}
					}
				}
				if(current_k>0){

					ind = (int*)calloc(current_k,sizeof(int));

					if(ind==NULL){
						printf(ERRORMAL);
						return 0;
					}
					val = (double*)calloc(current_k,sizeof(double));

					if(val==NULL){
						printf(ERRORMAL);
						return 0;
					}
					current_k=0;
					for(row=j;row<j+game->num_of_rows_in_block;row++){
						for(column=i;column<i+game->num_of_columns_in_block;column++){
							place=findPlaceForOption(row,column,value);
							if(place!=-1){
								ind[current_k]=place;
								val[current_k]=1.0;
								current_k++;
							}
						}
					}
					/*add constrain to mode*/
					error = GRBaddconstr(model, current_k, ind, val, GRB_EQUAL, 1, NULL);
					if (error) {
						printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
						free(ind);
						free(val);
						return 0;
					}
					free(ind);
					free(val);
				}
			}
		}
	}
	return 1;
}
int createBounds(){
	int i;
	lb=(double*)malloc(number_of_varibals*sizeof(double));
	ub=(double*)malloc(number_of_varibals*sizeof(double));
	if(ub==NULL){
		printf(ERRORMAL);
		return 0;
	}
	if(lb==NULL){
		printf(ERRORMAL);
		return 0;
	}
	for(i=0;i<number_of_varibals;i++){
		lb[i]=0.0;
		ub[i]=1.0;
	}
	return 1;
}
int creatRangeConstrainInLp(){
	int i;
	for(i=0;i<number_of_varibals;i++){
		ind = (int*)calloc(1,sizeof(int));

		if(ind==NULL){
			printf(ERRORMAL);
			return 0;
		}
		val = (double*)calloc(1,sizeof(double));

		if(val==NULL){
			printf(ERRORMAL);
			return 0;
		}
		ind[0]=i;
		val[0]=1.0;


		error = GRBaddconstr(model, 1, ind, val, GRB_LESS_EQUAL, 1.0, NULL);
		if (error) {
			printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
			free(ind);
			free(val);
			return 0;
		}
		error = GRBaddconstr(model, 1, ind, val, GRB_GREATER_EQUAL, 0, NULL);
		if (error) {
			printf("ERROR %d 2st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
			free(ind);
			free(val);
			return 0;
		}
		free(ind);
		free(val);
	}
	return 1;
}

/* Optimize model - need to call this before calculation */
int optimizeTheModel(){
	sol = (double*)calloc(number_of_varibals,sizeof(double));
	if(sol==NULL){
		printf(ERRORMAL);
		return 0;
	}
	error = GRBoptimize(model);
	if (error) {
		printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
		return 0;
	}
	/* Write model to 'mip1.lp' - this is not necessary but very helpful */
	error = GRBwrite(model, "mip1.lp");
	if (error) {
		printf("ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
		return 0;
	}
	return 1;
}

int getSol(){
	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error) {
		printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
		return 0;
	}

	/* get the objective -- the optimal result of the function */
	error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
	if (error) {
		printf("ERROR %d GRBgettdblattr(): %s\n", error, GRBgeterrormsg(env));
		return 0;
	}

	/* get the solution - the assignment to each variable */
	error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, number_of_varibals, sol);
	if (error) {
		printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
		return 0;
	}
	return 1;
}

/*return 1-solution, 0 no solution, -1 problem in gurobi*/
int end(){
	/* solution found */
	if (optimstatus == GRB_OPTIMAL) {
		/*	printf("solution found- board solvable\n");*/
		return 1;
	}
	/* no solution found */
	else if (optimstatus == GRB_INF_OR_UNBD) {
		/*	printf("no solution found- board unsolvable\n");*/
		return 0;
	}
	/* error or calculation stopped */
	else {
		/*	printf("Optimization was stopped early\n");*/
		return -1;
	}
	return 0;
}
/* Get solution information */
Game *findHintBoard(Game *game){
	int i,j,row,column,value;
	Game *game_sol= initializeGame(game->num_of_rows_in_block,game->num_of_columns_in_block);
	printf("im here\n");
	printBoard(game_sol);
	for(i =1; i<=game->board_size;i++){
		for(j=1;j<=game->board_size;j++){
			game_sol->board[i][j].value=game->board[i][j].value;
			game_sol->board[i][j].invalid=game->board[i][j].invalid;
			game_sol->board[i][j].fixed=game->board[i][j].fixed;
		}
	}
	for(i=0;i<number_of_varibals;i++){
		if(sol[i]==1){
			row = obj_value[i].row;
			column= obj_value[i].column;
			value=obj_value[i].value;
			game_sol->board[row][column].value=value;
			if(!boardValueAreValid(game_sol)){
				game_sol->board[row][column].value=0;
			}
		}
	}
	return game_sol;
}
int getHint(int row, int column, Game *game){
	Game *game_sol;
	int value;
	game_sol=findHintBoard(game);
	printBoard(game_sol);
	value= game_sol->board[row][column].value;
	freeGame(game_sol);
	return value;
}
int getValueForCellByPrecent(Game *game_sol,int i, int j,float X){
	int value,place, num_possible=0,k=0,current_k=0,stop,random;
	float precent;
	float *arr_of_option_without_precent= (float*)calloc(game_sol->board_size+1,sizeof(float));
	int *arr_of_option_precent;
	for(value=1;value<=game_sol->board_size;value++){
		place=findPlaceForOption(i,j,value);
		precent=sol[place];
		if(precent>=X){
			arr_of_option_without_precent[value]=sol[place];
		}
		else{
			arr_of_option_without_precent[value]=0;
		}
	}
	for(value=1;value<=game_sol->board_size;value++){
		game_sol->board[i][j].value=value;
		if(boardValueAreValid(game_sol) && arr_of_option_without_precent[value]>0){
			num_possible+=(1000*arr_of_option_without_precent[value]);
		}
		else{
			arr_of_option_without_precent[value]=0;
		}
		game_sol->board[i][j].value=0;
	}
	if(num_possible>0){
		arr_of_option_precent=(int*)calloc(num_possible,sizeof(int));
		for(value=1;value<=game_sol->board_size;value++){
			if(arr_of_option_without_precent[value]>0){
				place=findPlaceForOption(i,j,value);
				stop=1000*sol[place]+k;
				for(current_k=k; current_k<stop;current_k++){
					arr_of_option_precent[current_k]=value;
					k++;
				}
			}
		}
		random=rand()%num_possible;
		value= arr_of_option_precent[random];
		free(arr_of_option_precent);
	}
	else{
		value=0;
	}
	free(arr_of_option_without_precent);
	return value;
}
Game *findGuseeBoard(Game *game,float X){
	int i,j;
	int value;
	Game *game_sol= initializeGame(game->num_of_rows_in_block,game->num_of_columns_in_block);
	for(i =1; i<=game->board_size;i++){
		for(j=1;j<=game->board_size;j++){
			game_sol->board[i][j].value=game->board[i][j].value;
			game_sol->board[i][j].invalid=game->board[i][j].invalid;
			game_sol->board[i][j].fixed=game->board[i][j].fixed;
		}
	}
	printBoard(game_sol);
	for(i=1;i<=game_sol->board_size;i++){
		for(j=1;j<=game_sol->board_size;j++){
			if(game_sol->board[i][j].value==0){
				value=getValueForCellByPrecent(game_sol, i, j,X);
				game_sol->board[i][j].value=value;
			}
		}
	}
	return game_sol;
}
void fillAllGuesses(Game *game,float X){
	Game *game_sol;
	int i,j;
	/*for(i=0;i<100;i++){
		printf("row=%d, column=%d, value=%d, index=%d, sol=%f\n", obj_value[i].row,obj_value[i].column,obj_value[i].value,i,sol[i]);
	}*/
	game_sol=findGuseeBoard(game,X);
	for(i=1;i<=game->board_size;i++){
		for(j=1;j<=game->board_size;j++){
			if(game->board[i][j].value==0 && game_sol->board[i][j].value!=0){
				setCell(j,i,game_sol->board[i][j].value,game,0);
			}
		}
	}
	freeGame(game_sol);
}
/* IMPORTANT !!! - Free model and environment */
void freeGR(){
	free(sol);
	free(obj);
	free(vtype);
	free(obj_value);
	GRBfreemodel(model);
	GRBfreeenv(env);

}

