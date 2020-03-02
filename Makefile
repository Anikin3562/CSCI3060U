main: main.cpp dependancy/admin.cpp dependancy/item.cpp dependancy/user.cpp
	g++ main.cpp -o auction_system

clean:
	rm *.o auction_system
