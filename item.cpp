#ifndef ITEM_H 
#define ITEM_H

#include <string>
#include <map>
#include "user.cpp"
#include "admin.cpp"
using namespace std; 

// This class represents an item, which a user can auction.
// Item objects track their own auction details. 
// Constructor takes item name, ID, days left on its auction, current (highest) bid,
// and the current (highest) bidder. 
class Item{
public:

	string name;  
	int itemID; 

	int daysLeft; 	// Days left on the auction. 
	float currentBid;	// The current-highest bid on the item.
	User currentBidder;	// Name of user with the highest bid. 

	// Tracks bids made by users.
	map<User, float> bidLog; 
	
	// Constructors
	Item(){
		name = "null";
		itemID = -1; 
		daysLeft = -1; 
		currentBid = -1; 
		

	}

	Item(string n, int id, int dl, float cB, User cBr){
		name = n; 
		itemID = id;
		daysLeft = dl; 
		currentBid = cB; 
		currentBidder = cBr; 
	}
};


#endif