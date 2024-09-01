# Regra padrão: compilar e executar
all:
	gcc main.c ./src/*.c -o app.out
	./app.out
#	rm -f ./src/disco.txt
test:
	gcc ./tests/*.c ./src/*.c -o test.out
	./test.out
