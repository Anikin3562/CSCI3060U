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
class Item {
public:

	string name;
	string sellerName;
	int daysLeft; 	// Days left on the auction. 
	float currentBid;	// The current-highest bid on the item.
	string currentBidder;	// Name of user with the highest bid. 
	// Constructors
	Item() {
		name = "null";
		sellerName = "null";
		daysLeft = -1;
		currentBid = -1;
		currentBidder = "null";
	}

	Item(string n, string sN, int dl, float cB, string cBr) {
		name = n;
		sellerName = sN;
		daysLeft = dl;
		currentBid = cB;
		currentBidder = cBr;
	}
};


#endif