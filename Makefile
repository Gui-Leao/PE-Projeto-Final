all: client

client: client.o bignumber.o auxiliar.o
	gcc client.o bignumber.o auxiliar.o -lm -o client.exe

# Compilação de client.o
client.o: client.c auxiliar.h
	gcc -std=c99 -Wall -Wextra -Wvla -g -c client.c

# Compilação de bignumber.o
bignumber.o: bignumber.c bignumber.h auxiliar.h
	gcc -std=c99 -Wall -Wextra -Wvla -g -c bignumber.c

# Compilação de auxiliar.o
auxiliar.o: auxiliar.c auxiliar.h bignumber.h
	gcc -std=c99 -Wall -Wextra -Wvla -g -c auxiliar.c


