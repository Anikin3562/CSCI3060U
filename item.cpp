#ifndef ITEM_H 
#define ITEM_H

#include <string>
#include <map>
#include "user.cpp"
using namespace std; 

class Item{

private:
	string itemName;
	string sellerName;
	string highestBidder;
	int daysLeft;
	int price;

public:
	//Constructor
	Item(string _itemName, string _sellerName, string _highestBidder, int _daysLeft, int _price){
		itemName = _itemName;
		sellerName = _sellerName;
		highestBidder = _highestBidder;
		daysLeft = _daysLeft;
		price = _price;
	}

	//getters
	string get_itemName(){return itemName;}
	string get_sellerName(){return sellerName;}
	string get_highestBidder(){return highestBidder;}
	int get_daysLeft(){return daysLeft;}
	int get_price(){return price;}

	void updateBid(string bidder, int amount){
		highestBidder = bidder;
		price = amount;
	}
};

#endif