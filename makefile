compiler = gcc

Message_test: Message_test.c Message.o
	$(compiler) $^ -o $@

Message.o: Message.c Message.h
	$(compiler) -c Message.c -o $@

clean: *.o
	rm $^