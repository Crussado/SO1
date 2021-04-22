CARPETASLIST = SList/
SLIST = Slist.o
BOARD = Board
BANDERAS = -Wall -Wextra -Werror -g -std=gnu99 -pthread

armar: $(BOARD).o

$(BOARD).o: $(BOARD).c $(BOARD).h $(SLIST)
	gcc -c $(BOARD).c $(BANDERAS)
$(SLIST):
	cd $(CARPETASLIST) && make
clean: 
	rm *.o