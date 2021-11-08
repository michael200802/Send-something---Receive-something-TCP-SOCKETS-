compiler = gcc

GetMessage_test: GetMessage_test.c GetMessage.o
	$(compiler) $^ -o $@

GetMessage.o: GetMessage.c GetMessage.h
	$(compiler) -c GetMessage.c -o $@

clean: *.o
	rm $^