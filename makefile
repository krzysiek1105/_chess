CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -Wpedantic
INCLUDE = ./includes/
SOURCE = ./src

ISFML = D:/SFML-2.5.1/include
ITGUI = D:/TGUI-0.8/include
LSFML = D:/SFML-2.5.1/lib
LTGUI = D:/TGUI-0.8/lib
LIBS = -lsfml-graphics-d -lsfml-system-d -lsfml-window-d -ltgui-d

LOGIC_OBJS = chessboard.o piece_legal_moves.o chessboard_utils.o chessboard_checks.o chessboard_notations.o
GUI_OBJS = chessboard_gui.o main.o

all: $(LOGIC_OBJS) $(GUI_OBJS)
	$(CC) $(LOGIC_OBJS) $(GUI_OBJS) -I$(ISFML) -I$(ITGUI) -I$(INCLUDE) -L$(LSFML) -L$(LTGUI) $(LIBS) $(CFLAGS)

%.o: $(SOURCE)/%.cpp
	$(CC) -I$(ISFML) -I$(ITGUI) -I$(INCLUDE) -L$(LSFML) -L$(LTGUI) $(LIBS) -c $< -o $@

logic: $(LOGIC_OBJS)

gui: chessboard_gui.o

clean:
	del *.o
	del *.exe