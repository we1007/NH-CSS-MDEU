MDR : main.o Uart0.o Myudp.o finger.o MyTCP.o
	$(CC) $(LDFLAGS) main.o Uart0.o Myudp.o finger.o MyTCP.o -o MDR

finger.o : finger.c Allinclude.h 
	$(CC) $(CFLAGS) -c finger.c -o finger.o	
Uart0.o : Uart0.c Allinclude.h
	$(CC) $(CFLAGS) -c Uart0.c -o Uart0.o
main.o : main.c Allinclude.h 
	$(CC) $(CFLAGS) -c main.c -o main.o
Myudp.o : Myudp.c Allinclude.h 
	$(CC) $(CFLAGS) -c Myudp.c -o Myudp.o
MyTCP.o : MyTCP.c Allinclude.h 
	$(CC) $(CFLAGS) -c MyTCP.c -o MyTCP.o
		

clean :
	rm *.o MDR