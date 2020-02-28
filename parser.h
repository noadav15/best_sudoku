
#ifndef PARSER_H_
#define PARSER_H_

Game* getCommand(Game *game, char input[1024], char command[4][1024]);
int checkIfStringIsInt(char *str);
int converStringToInt(char *str);
#endif /* PARSER_H_ */
