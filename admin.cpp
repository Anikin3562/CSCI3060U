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
		permissionType = 0;
	}

	Admin(string un, float cB ){
		username = un; 
		creditBalance = cB;
		permissionType = 0; 
	}

	int create(string username, int permissions, float credits); 
	int refund(string sellerUsername, string buyerUsername, float amount); 
	int deleteUser(string username);
	int addCredit(float credit);

};

int Admin::create(string username, int permissions, float credits){
	return 0; 
}

int Admin::refund(string sellerUsername, string buyerUsername, float amount){
	return 0; 
}

int Admin::deleteUser(string username){
	return 0;
}

int Admin::addCredit(float credit){
	return 0;
}

#endif