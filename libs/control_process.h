#ifndef CONTROL_PROCESS
#define CONTROL_PROCESS

#include <stdio.h>
#include <stdlib.h>

#include "process_manager.h"

#define TAM_MAX_MNS 100

void initialize_control_process();

void show_main_menu();
void show_menu_escalonamento();

void read_arq(char *retorno);
void read_terminal_input(char *return_char);
void remove_char(char *str, char c);

#endif //CONTROL_PROCESS