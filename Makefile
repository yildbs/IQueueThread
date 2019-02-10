run_example: example/main.cpp example/Modules.cpp
	g++ -o run_example $^ -lpthread -std=c++11 -fpermissive

clean:
	rm run_example
