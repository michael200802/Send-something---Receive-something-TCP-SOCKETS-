compiler = gcc

ReceiveSomething: ReceiveSomething.c Message.o Mystring.o
	$(compiler) -pthread $^ -o $@

SendSomething: SendSomething.c Message.o Mystring.o
	$(compiler) $? -o $@

Mystring.o: Mystring.h Mystring.c
	$(compiler) -c Mystring.c -o $@ 

Message_test: Message_test.c Message.o
	$(compiler) $^ -o $@

Message.o: Message.c Message.h
	$(compiler) -c Message.c -o $@

clean: *.o
	rm $^