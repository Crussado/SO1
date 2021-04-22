CARPETABOARD = Board/
BOARD = Board.o
GAME = Game
BANDERAS = -Wall -Wextra -Werror -g -std=gnu99 -pthread

armar: $(GAME).o

$(GAME).o: $(GAME).c $(GAME).h $(BOARD) 
	gcc -c $(GAME).c $(BANDERAS)
$(BOARD):
	cd $(CARPETABOARD) && make
clean: 
	rm *.o