All : task1 task2

task1 : task1.cpp input.txt
			g++ -std=c++14 task1.cpp -o task1

task2 : task2.cpp input.txt
			g++ -std=c++14 task2.cpp -o task2

clean :
		rm -f *o task1 task2

run1:	task1
		./task1

run2:task2
		./task2
