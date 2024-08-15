# Regra padrão: compilar e executar
all:
	gcc ./src/*.c -o app.out
	./app.out

test:
	gcc ./tests/*.c ./src/*.c -o test.out
	./test.out
