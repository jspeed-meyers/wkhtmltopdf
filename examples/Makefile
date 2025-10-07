CFLAGS= -I ../include
LDFLAGS=-L ../bin -lwkhtmltox -Wall -ansi -pedantic -ggdb

EXES=pdf_c_api image_c_api
TEST_EXES=pdf_c_api_test image_c_api_test

all: $(EXES)

test: $(TEST_EXES)

.PHONY: clean run runimage all test runtest runtestimage

run: pdf_c_api
	LD_LIBRARY_PATH=../bin ./pdf_c_api

runimage: image_c_api
	LD_LIBRARY_PATH=../bin ./image_c_api

runtest: pdf_c_api_test
	LD_LIBRARY_PATH=../bin ./pdf_c_api_test

runtestimage: image_c_api_test
	LD_LIBRARY_PATH=../bin ./image_c_api_test

clean:
	$(RM) $(EXES) $(TEST_EXES) test_output.pdf test_output.jpg

