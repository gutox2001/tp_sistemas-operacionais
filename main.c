
#include "./libs/control_process.h"

int main() {
    char string_input[TAM_MAX_MNS] = "";
    int type_input = 0;
    int type_escalonamento = 0;

    do {
        show_main_menu();

        switch (type_input) {
            case 1:
                read_terminal_input(string_input);
                break;

            case 2:
                read_arq(string_input);
                break;

            default:
                printf("Entrada invalida! Escolha 1 ou 2.\n\n");
                continue;
        }
    } while (type_input != 1 && type_input != 2);

    read_terminal_input(&string_input);

    return 1;
}