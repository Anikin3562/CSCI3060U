main: main.o admin.o item.o user.o
	g++ main.o admin.o item.o user.o -o auction_system

main.o: main.cpp
	g++ -c main.cpp

admin.o: admin.cpp
	g++ -c admin.cpp

item.o: item.cpp
	g++ -c item.cpp

user.o: user.cpp
	g++ -c user.cpp

clean:
	rm *.o auction_system






