lab1_sched.out: lab1_sched.o lab1_sched_test.o
	g++ -g -o lab1_sched.out lab1_sched.o lab1_sched_test.o

lab1_sched.o: lab1_sched.cpp
	g++ -c -g -o lab1_sched.o lab1_sched.cpp

lab1_sched_test.o: lab1_sched_test.cpp
	g++ -c -g -o lab1_sched_test.o lab1_sched_test.cpp

clean:
	rm *.o lab1_sched.out