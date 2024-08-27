# Regra padrão: compilar e executar
all:
	gcc main.c ./src/*.c -o app.out
	./app.out

