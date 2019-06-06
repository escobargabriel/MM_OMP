# GNU Makefile

CC = gcc -fopenmp
CCFLAGS = -Wall -O3 #-llikwid #-funroll-all-loops
LDFLAGS = -fopenmp #-pg
TARGET = main_matriz gera_matriz2 help
EXE = ./gera_matriz2

all: $(TARGET)

%.o: %.c
		$(CC) $(CCFLAGS) -c $<

%: %.o
		$(CC) $(LDFLAGS) $^ -o $@

main_matriz: main_matriz.c matriz.o toolsv2.o matriz-operacoes.o matriz-operacoes-omp.o
		$(CC) $(CCFLAGS) matriz-operacoes-omp.o matriz-operacoes.o matriz.o toolsv2.o main_matriz.c -o $@ $(LDFLAGS)

gera_matriz2: matriz.o toolsv2.o gera_matriz2.c
		$(CC) $(CCFLAGS) matriz.o toolsv2.o gera_matriz2.c -o $@ $(LDFLAGS)

help:
		@echo "A Multiplicação em blocos pode ter seu tempo melhorado conforme a dimensão do bloco escolhido! \n Teste com diferentes dimensões de blocos"
		@echo "Caso seja escolhido um valor inadequado a aplicação ajusta para um valor válido, \n porém este valor pode não ser o melhor"
		@echo "####### Exemplo de Execução #######"
		@echo "./main_matriz 128x128-mat.map 128x128-mat1.map 2"

clean:
		rm -f *.o *~ $(TARGET) *.map *.map-result

