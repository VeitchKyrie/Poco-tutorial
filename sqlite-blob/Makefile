#
# Makefile for a execute
#
CC=g++
APP=sqlite-blob

INC=-I/usr/local/Cellar/poco/1.9.0_1/include
LIBS=-L/usr/local/opt/poco/lib/
INC_LIB = -lPocoFoundation
INC_LIB += -lPocoNet  -lPocoJSON -lPocoUtil -lPocoData
INC_LIB += -lPocoDataSQLite

all:
	$(CC) -g *.cpp -o $(APP) $(INC) $(LIBS) $(INC_LIB)

clean:
	rm -rf $(APP) $(APP).dSYM

exec:
	export DYLD_LIBRARY_PATH=/usr/local/Cellar/poco/1.9.0_1/lib/ && ./$(APP) 
