CXX=g++-5
SRC=./src
BIN=./bin
BUILD=./build
TEST=./test
CXX_FLAG=-g -Wl,--no-as-needed -lz -I$(SRC)/dex/ -I$(SRC)/apk/ -I$(SRC)/common/ -I$(SRC)/res/ -std=c++11 -Wall -Wextra 

all: apk_test xml_test

$(BIN)/apk_test: $(BUILD)/apk.o $(TEST)/apk_test.cpp $(TEST)/test_suite.h $(BUILD)/test_suite.o $(BUILD)/common.o 
	g++ $(CXX_FLAG) $(BUILD)/test_suite.o $(BUILD)/apk.o $(TEST)/apk_test.cpp $(BUILD)/common.o $(BUILD)/res_base.o -o $(BIN)/apk_test
	ln -sf $(BIN)/apk_test ./apk_test-bin

$(BIN)/xml_test: $(BUILD)/xml.o $(TEST)/xml_test.cpp $(BUILD)/test_suite.o $(BUILD)/common.o $(BUILD)/res_base.o
	g++ $(CXX_FLAG) $(BUILD)/test_suite.o $(TEST)/xml_test.cpp $(BUILD)/common.o $(BUILD)/xml.o -o $(BIN)/xml_test
	ln -sf $(BIN)/xml_test ./xml_test-bin

$(BIN)/buffer_test: $(TEST)/buffer_test.cpp $(TEST)/test_suite.h $(BUILD)/test_suite.o $(BUILD)/common.o
	g++ $(CXX_FLAG) $(BUILD)/test_suite.o $(TEST)/buffer_test.cpp $(BUILD)/common.o -o $(BIN)/buffer_test
	ln -sf $(BIN)/buffer_test ./buffer_test-bin

$(BIN)/res_table_test: $(TEST)/res_table_test.cpp $(BUILD)/test_suite.o $(BUILD)/common.o $(BUILD)/res_table.o $(BUILD)/res_base.o
	g++ $(CXX_FLAG) $(BUILD)/test_suite.o $(TEST)/res_table_test.cpp $(BUILD)/common.o $(BUILD)/res_base.o -o $(BIN)/res_table_test
	ln -sf $(BIN)/res_table_test ./res_table_test-bin

apk_test: $(BIN)/apk_test
xml_test: $(BIN)/xml_test
buffer_test: $(BIN)/buffer_test
res_table_test: $(BIN)/res_table_test

$(BUILD)/common.o: $(SRC)/common/common.cpp $(SRC)/common/common.h $(SRC)/common/utf.h $(SRC)/common/buffer.h
	g++ -c $(CXX_FLAG) $(SRC)/common/common.cpp -o $(BUILD)/common.o

$(BUILD)/test_suite.o: $(TEST)/test_suite.cpp $(TEST)/test_suite.h
	g++ -c $(CXX_FLAG) $(TEST)/test_suite.cpp -o $(BUILD)/test_suite.o

$(BUILD)/apk.o: $(SRC)/apk/apk.cpp $(SRC)/apk/apk.h
	g++ -c $(CXX_FLAG) $(SRC)/apk/apk.cpp -o $(BUILD)/apk.o

$(BUILD)/xml.o: $(SRC)/res/xml.cpp $(SRC)/res/xml.h $(SRC)/res/res_base.h
	g++ -c $(CXX_FLAG) $(SRC)/res/xml.cpp -o $(BUILD)/xml.o

$(BUILD)/res_table.o: $(SRC)/res/res_table.cpp $(SRC)/res/res_table.h $(SRC)/res/res_base.h
	g++ -c $(CXX_FLAG) $(SRC)/res/res_table.cpp -o $(BUILD)/res_table.o

$(BUILD)/res_base.o: $(SRC)/res/res_base.cpp $(SRC)/res/res_base.h
	g++ -c $(CXX_FLAG) $(SRC)/res/res_base.cpp -o $(BUILD)/res_base.o

prepare:
	mkdir -p bin
	mkdir -p build
	mkdir -o run_test

clean:
	rm -f ./build/*
	rm -f ./bin/*
