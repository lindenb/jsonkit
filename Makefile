
all:  event.tab.o lex.jsevent.o
	${CC} $^ 

lex.jsevent.o: lex.jsevent.c  event.tab.h
	${CC} -c -o $@ $<

lex.jsevent.c : event.l 
	flex -d -P jsevent $<

event.tab.o: event.tab.c event.tab.h
	${CC} -c -o $@ $<

event.tab.h: event.tab.c
	touch -c $@

event.tab.c: event.y
	bison -x -d -p jsevent $<
