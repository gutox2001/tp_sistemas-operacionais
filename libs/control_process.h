#ifndef CONTROL_PROCESS
#define CONTROL_PROCESS

#include <stdio.h>
#include <stdlib.h>

#include "process_manager.h"

#define TAM_MAX_MNS 100

void initialize_control_process(int type_input, int type_escalonamento, char *send_string);

void show_main_menu();
void show_menu_escalonamento();

void read_arq(char *retorno);
void read_terminal_input(char *return_char);
void remove_char(char *str, char c);

void show_control_process(double execution_time);

#endif //CONTROL_PROCESS