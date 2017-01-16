CXX=g++-5
SRC=./src
BIN=./bin
BUILD=./build
TEST=./test
CXX_FLAG=-g -Wl,--no-as-needed -lz -I$(SRC)/dex/ -I$(SRC)/apk/ -I$(SRC)/common/ -std=c++11 -Wall -Wextra 

all: apk_test xml_test

$(BIN)/apk_test: $(BUILD)/apk.o $(TEST)/apk_test.cpp $(TEST)/test_suite.h $(BUILD)/test_suite.o $(BUILD)/common.o 
	g++ $(CXX_FLAG) $(BUILD)/test_suite.o $(BUILD)/apk.o $(TEST)/apk_test.cpp $(BUILD)/common.o -o $(BIN)/apk_test
	ln -sf $(BIN)/apk_test ./apk_test-bin

$(BIN)/xml_test: $(BUILD)/xml.o $(TEST)/xml_test.cpp $(TEST)/test_suite.h $(BUILD)/test_suite.o $(BUILD)/common.o
	g++ $(CXX_FLAG) $(BUILD)/test_suite.o $(TEST)/xml_test.cpp $(BUILD)/common.o $(BUILD)/xml.o -o $(BIN)/xml_test
	ln -sf $(BIN)/xml_test ./xml_test-bin

apk_test: $(BIN)/apk_test
xml_test: $(BIN)/xml_test

$(BUILD)/common.o: $(SRC)/common/common.cpp $(SRC)/common/common.h
	g++ -c $(CXX_FLAG) $(SRC)/common/common.cpp -o $(BUILD)/common.o

$(BUILD)/test_suite.o: $(TEST)/test_suite.cpp $(TEST)/test_suite.h
	g++ -c $(CXX_FLAG) $(TEST)/test_suite.cpp -o $(BUILD)/test_suite.o

$(BUILD)/apk.o: $(SRC)/apk/apk.cpp $(SRC)/apk/apk.h
	g++ -c $(CXX_FLAG) $(SRC)/apk/apk.cpp -o $(BUILD)/apk.o

$(BUILD)/xml.o: $(SRC)/apk/xml.cpp $(SRC)/apk/xml.h
	g++ -c $(CXX_FLAG) $(SRC)/apk/xml.cpp -o $(BUILD)/xml.o

prepare:
	mkdir -p bin
	mkdir -p build

clean:
	rm -f ./build/*
	rm -f ./bin/*
