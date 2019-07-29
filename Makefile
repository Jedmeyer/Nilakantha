
Nila: Nilakantha.cpp
	g++ -o Nila Nilakantha.cpp -lpthread -lgmpxx -lgmp

clean:
	rm Nila
	
install-dep:
	sudo apt-get install libgmp3-dev