CXX=g++-5
SRC=./src
BIN=./bin
BUILD=./build
TEST=./test
CXX_FLAG=-g -I$(SRC)/dex/ -I$(SRC)/apk/

$(BIN)/apk_test: $(BUILD)/apk.o $(TEST)/apk_test.cpp $(TEST)/test_suite.h $(BUILD)/test_suite.o
	g++ $(CXX_FLAG) $(BUILD)/apk.o $(TEST)/apk_test.cpp -o $(BIN)/apk_test

$(BUILD)/test_suite.o: $(TEST)/test_suite.cpp $(TEST)/test_suite.h
	g++ $(CXX_FLAG) $(TEST)/test_suite.cpp -o $(BUILD)/test_suite.o

$(BUILD)/apk.o: $(SRC)/apk/apk.cpp $(SRC)/apk/apk.h
	g++ $(CXX_FLAG) $(SRC)/apk/apk.cpp -o $(BUILD)/apk.o

prepare:
	mkdir -p bin
	mkdir -p build