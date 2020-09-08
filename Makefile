

537ps: 537ps.o dirParser.o procParser.o optionParser.o
	gcc -o 537ps 537ps.o dirParser.o procParser.o optionParser.o


537ps.o: 537ps.c dirParser.h procParser.h optionParser.h
	gcc -c 537ps.c


dirParser.o: dirParser.c dirParser.h procParser.h optionParser.h
	gcc -c dirParser.c


procParser.o: procParser.c procParser.h optionParser.h
	gcc -c procParser.c


optionParser.o: optionParser.c optionParser.h
	gcc -c optionParser.c


clean:
	-rm 537ps 537ps.o dirParser.o procParser.o optionParser.o