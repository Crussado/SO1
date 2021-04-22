PROGRAMA = main.c
EJECUTABLE = simulador

CARPETAGAME = Game/
CARPETABOARD = $(CARPETAGAME)Board/
CARPETASLIST = $(CARPETABOARD)SList/

GAME = Game.o
SLIST =	Slist.o
BOARD = Board.o

BANDERAS = -Wall -Wextra -Werror -g -std=gnu99 -pthread 

armar: programa 

programa: $(PROGRAMA) $(GAME)
	gcc -o $(EJECUTABLE) $(PROGRAMA) $(CARPETAGAME)$(GAME) $(CARPETABOARD)$(BOARD) $(CARPETASLIST)$(SLIST) $(BANDERAS)
$(GAME):
	cd $(CARPETAGAME) && make
clean: 
	rm *.o