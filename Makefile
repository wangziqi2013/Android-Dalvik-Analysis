CXX=g++-5
SRC=./src
BIN=./bin
BUILD=./build
TEST=./test
CXX_FLAG=-g -Wl,--no-as-needed -lz -I$(SRC)/dex/ -I$(SRC)/apk/ -I$(SRC)/common/ -I$(SRC)/res/ -std=c++11 -Wall -Wextra
OPT_FLAG= 
CXX_FLAG+=$(OPT_FLAG)

all: apk_test xml_test buffer_test res_table_test encoding_test dex_test

$(BIN)/apk_test: $(BUILD)/apk.o $(TEST)/apk_test.cpp $(TEST)/test_suite.h $(BUILD)/test_suite.o $(BUILD)/common.o $(BUILD)/res_base.o 
	g++ $(CXX_FLAG) $(BUILD)/test_suite.o $(BUILD)/apk.o $(TEST)/apk_test.cpp $(BUILD)/common.o $(BUILD)/res_base.o -o $(BIN)/apk_test
	ln -sf $(BIN)/apk_test ./apk_test-bin

$(BIN)/xml_test: $(BUILD)/xml.o $(TEST)/xml_test.cpp $(BUILD)/test_suite.o $(BUILD)/common.o $(BUILD)/res_base.o $(BUILD)/res_table.o $(BUILD)/package_group.o
	g++ $(CXX_FLAG) $(BUILD)/test_suite.o $(TEST)/xml_test.cpp $(BUILD)/common.o $(BUILD)/res_base.o $(BUILD)/res_table.o $(BUILD)/package_group.o $(BUILD)/xml.o -o $(BIN)/xml_test
	ln -sf $(BIN)/xml_test ./xml_test-bin

$(BIN)/buffer_test: $(TEST)/buffer_test.cpp $(TEST)/test_suite.h $(BUILD)/test_suite.o $(BUILD)/common.o
	g++ $(CXX_FLAG) $(BUILD)/test_suite.o $(TEST)/buffer_test.cpp $(BUILD)/common.o -o $(BIN)/buffer_test
	ln -sf $(BIN)/buffer_test ./buffer_test-bin

$(BIN)/encoding_test: $(TEST)/encoding_test.cpp $(TEST)/test_suite.h $(BUILD)/test_suite.o $(BUILD)/common.o
	g++ $(CXX_FLAG) $(BUILD)/test_suite.o $(TEST)/encoding_test.cpp $(BUILD)/common.o -o $(BIN)/encoding_test
	ln -sf $(BIN)/encoding_test ./encoding_test-bin

$(BIN)/res_table_test: $(TEST)/res_table_test.cpp $(BUILD)/test_suite.o $(BUILD)/common.o $(BUILD)/res_table.o $(BUILD)/res_base.o $(BUILD)/package_group.o $(BUILD)/xml.o
	g++ $(CXX_FLAG) $(BUILD)/test_suite.o $(TEST)/res_table_test.cpp $(BUILD)/common.o $(BUILD)/res_base.o $(BUILD)/res_table.o $(BUILD)/package_group.o $(BUILD)/xml.o -o $(BIN)/res_table_test
	ln -sf $(BIN)/res_table_test ./res_table_test-bin

$(BIN)/dex_test: $(TEST)/dex_test.cpp $(TEST)/test_suite.h $(BUILD)/test_suite.o $(BUILD)/common.o $(BUILD)/dex.o $(BUILD)/inst_format.o $(BUILD)/inst.o
	g++ $(CXX_FLAG) $(BUILD)/test_suite.o $(TEST)/dex_test.cpp $(BUILD)/common.o $(BUILD)/dex.o $(BUILD)/inst_format.o $(BUILD)/inst.o -o $(BIN)/dex_test
	ln -sf $(BIN)/dex_test ./dex_test-bin

apk_test: $(BIN)/apk_test
xml_test: $(BIN)/xml_test
buffer_test: $(BIN)/buffer_test
encoding_test: $(BIN)/encoding_test
res_table_test: $(BIN)/res_table_test
dex_test: $(BIN)/dex_test

$(BUILD)/common.o: $(SRC)/common/common.cpp $(SRC)/common/common.h $(SRC)/common/utf.h $(SRC)/common/buffer.h
	g++ -c $(CXX_FLAG) $(SRC)/common/common.cpp -o $(BUILD)/common.o

$(BUILD)/test_suite.o: $(TEST)/test_suite.cpp $(TEST)/test_suite.h
	g++ -c $(CXX_FLAG) $(TEST)/test_suite.cpp -o $(BUILD)/test_suite.o

$(BUILD)/apk.o: $(SRC)/apk/apk.cpp $(SRC)/apk/apk.h
	g++ -c $(CXX_FLAG) $(SRC)/apk/apk.cpp -o $(BUILD)/apk.o

$(BUILD)/xml.o: $(SRC)/res/xml.cpp $(SRC)/res/xml.h $(SRC)/res/res_base.h
	g++ -c $(CXX_FLAG) $(SRC)/res/xml.cpp -o $(BUILD)/xml.o

$(BUILD)/res_table.o: $(SRC)/res/res_table.cpp $(SRC)/res/res_table.h $(SRC)/res/res_base.h $(SRC)/res/res_value.h $(SRC)/res/type_config.h $(SRC)/common/utf.h
	g++ -c $(CXX_FLAG) $(SRC)/res/res_table.cpp -o $(BUILD)/res_table.o

$(BUILD)/package_group.o: $(SRC)/res/package_group.cpp $(SRC)/res/package_group.h
	g++ -c $(CXX_FLAG) $(SRC)/res/package_group.cpp -o $(BUILD)/package_group.o

$(BUILD)/res_base.o: $(SRC)/res/res_base.cpp $(SRC)/res/res_base.h $(SRC)/res/res_value.h $(SRC)/res/type_config.h $(SRC)/common/utf.h
	g++ -c $(CXX_FLAG) $(SRC)/res/res_base.cpp -o $(BUILD)/res_base.o

$(BUILD)/dex.o: $(SRC)/dex/dex.h $(SRC)/dex/dex.cpp $(SRC)/dex/dex_common.h $(BUILD)/common.o 
	g++ -c $(CXX_FLAG) $(SRC)/dex/dex.cpp -o $(BUILD)/dex.o

$(BUILD)/inst_format.o: $(SRC)/dex/inst_format.h $(SRC)/dex/inst_format.cpp $(BUILD)/common.o $(SRC)/dex/inst_format.py
	cd ./src/dex && python inst_format.py && cd ../../
	g++ -c $(CXX_FLAG) $(SRC)/dex/inst_format.cpp -o $(BUILD)/inst_format.o

$(BUILD)/inst.o: $(SRC)/dex/inst.h $(SRC)/dex/inst.cpp $(BUILD)/common.o $(BUILD)/inst_format.o
	g++ -c $(CXX_FLAG) $(SRC)/dex/inst.cpp -o $(BUILD)/inst.o

prepare:
	mkdir -p bin
	mkdir -p build
	mkdir -o run_test

clean:
	rm -f ./build/*
	rm -f ./bin/*
