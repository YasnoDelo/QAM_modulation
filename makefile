all:
	g++ src/main.cpp src/demodulation.cpp src/modulation.cpp src/channel.cpp src/py_file_ctor.cpp src/help.cpp -o main;
	./main;
