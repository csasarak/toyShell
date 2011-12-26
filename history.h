/*
  Filename: hist.h
  Author: Christopher Sasarak <cms5347@rit.edu>
*/
#ifndef HISTORY_H

#define HISTORY_H

void add_cmd(char *cmd);
char* next_cmd(int reset);
void print_history();
#endif
