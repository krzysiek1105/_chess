CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -Wpedantic
INCLUDE = ./includes/
SOURCE = ./src

ISFML = D:/SFML-2.5.1/include
LDIR = D:/SFML-2.5.1/lib
LIBS = -lsfml-graphics -lsfml-system -lsfml-window

LOGIC_OBJS = chessboard.o piece_legal_moves.o chessboard_utils.o chessboard_checks.o
GUI_OBJS = chessboard_gui.o main.o

all: $(LOGIC_OBJS) $(GUI_OBJS)
	$(CC) $(LOGIC_OBJS) $(GUI_OBJS) -I$(ISFML) -I$(INCLUDE) -L$(LDIR) $(LIBS) $(CFLAGS)

%.o: $(SOURCE)/%.cpp
	$(CC) -I$(ISFML) -I$(INCLUDE) -L$(LDIR) $(LIBS) -c $< -o $@

logic: $(LOGIC_OBJS)

gui: chessboard_gui.o

clean:
	del *.o
	del *.exe