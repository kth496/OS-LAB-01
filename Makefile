#
#	DKU Operating System Lab
#	    Lab1 (Scheduler Algorithm Simulator)
#	    Student id : 32131417, 32173656
#	    Student name : 김태홍, 이환주
#
#   Makfeile :
#       - Makefile for lab1 compilation.
#

lab1_sched.out: lab1_sched.o lab1_sched_test.o
	g++ -g -o lab1_sched.out lab1_sched.o lab1_sched_test.o

lab1_sched.o: lab1_sched.cpp
	g++ -c -g -o lab1_sched.o lab1_sched.cpp

lab1_sched_test.o: lab1_sched_test.cpp
	g++ -c -g -o lab1_sched_test.o lab1_sched_test.cpp

clean:
	rm *.o lab1_sched.out