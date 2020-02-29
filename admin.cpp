#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include <string>
#include "user.cpp"

// Admin is a special instance of User with additional functions.
// Admins can create and delete other users, as well as refund credits
// from a buyer's account to a seller's. 
class Admin : public User{
public: 

	// Overloaded Constructors
	Admin(){
		permissionType = "AA";
	}

	Admin(string un, bool lI, float cB ){
		username = un; 
		loggedIn = lI;
		creditBalance = cB;
		permissionType = "AA"; 
	}

	// Functions
	int create(string username, int permissions, float credits); 
	int refund(string sellerUsername, string buyerUsername, float amount); 
	int deleteUser(string username);

	// Overloaded Function
	int addCredit(float credit);

};

// Only admins can create other Users.
int Admin::create(string username, int permissions, float credits){
	return 0; 
}

// Refunds credits from a Seller's balance to a buyer's. 
int Admin::refund(string sellerUsername, string buyerUsername, float amount){
	return 0; 
}

// Only admins can delete other Users.
int Admin::deleteUser(string username){
	return 0;
}

// This function works different for Admins 
// than it does for other users. 
int Admin::addCredit(float credit){
	return 0;
}

#endif