CC         = g++
EXE	   	   = tgrep
OBJ		   = tgrep.o configuration.o userinterface.o
LFLAGS 	   = `pkg-config --libs taglib`
CFLAGS     = -c `pkg-config --cflags taglib`

$(EXE): CFLAGS += -O
$(EXE): LFLAGS += -O
$(EXE): $(OBJ)
	$(CC) $(LFLAGS) $(OBJ) -o $(EXE)

debug: CFLAGS += -Wall -DDEBUG -g
debug: LFLAGS += -Wall -DDEBUG -g
debug: $(OBJ)
	$(CC) $(LFLAGS) $(OBJ) -o $(EXE)

%.o: %.cpp
	$(CC) $(CFLAGS) $(DEBUG) $<

.PHONY: clean
clean:
	rm *.o $(EXE)
