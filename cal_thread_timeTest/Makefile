.SUFFIXES : .cpp .o 

CXX = g++
LINK = -L/usr/lib/
CFLAGS = -lpthread
OBJS = pevents.o cal.o

TARGET = cal


CAL_OBJ = cal.cpp
PEVENT_OBJ = 3rdParty/pevents.cpp

all:
	$(CXX) -c -D_DEBUG -Wall -ggdb -std=c++0x $(PEVENT_OBJ)
	$(CXX) -c -D_DEBUG -Wall -ggdb -std=c++0x $(CAL_OBJ)
	$(CXX) main.cpp -ggdb -std=c++0x -o $(TARGET) -D_DEBUG -Wall -std=c++0x $(INC) $(OBJS) $(CFLAGS)

clean : 
	rm -rf *.gch 3rdParty/*.gch $(OBJS) $(TARGET)

