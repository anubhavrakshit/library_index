library_index:
	g++ -pipe -O2 -std=c++14 -o index main.cpp -lm
clean:
	rm *.o library_index

