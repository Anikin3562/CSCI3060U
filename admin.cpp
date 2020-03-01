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

	Admin(string un, float cB ){
		username = un; 
		creditBalance = cB;
		permissionType = "AA"; 
	}

	int create(User* target); 
	int refund(User* buyer, User* seller, float amount); 
	int deleteUser(User* target);
	int addCredit(User* target, float amount);

};

int Admin::create(User* target){
	string transaction = "01 ";
	//username
	transaction += target->username + string(15 - target->username.length() + 1, ' ');
	//permission
	transaction += target->permissionType + ' ';
	//balance
	transaction += creditBalance_toString(target->creditBalance);

	this->transactions.push_back(transaction);
	return 0; 
}

int Admin::refund(User* seller, User* buyer, float amount){
	string transaction = "05 ";
	//buyer
	transaction += buyer->username + string(15 - buyer->username.length() + 1, ' ');
	//seller
	transaction += seller->username + string(15 - seller->username.length() + 1, ' ');
	//refund value
	transaction += creditBalance_toString(amount);

	this->transactions.push_back(transaction);
	return 0; 
}

int Admin::deleteUser(User* target){
	string transaction = "02 ";
	//username
	transaction += target->username + string(15 - target->username.length() + 1, ' ');
	//permission
	transaction += (target->permissionType + " ");
	//balance
	transaction += creditBalance_toString(target->creditBalance);

	this->transactions.push_back(transaction);
	return 0;
}

int Admin::addCredit(User* target, float amount){
	string transaction = "06 ";

	//username 
	transaction += target->username + string(15 - target->username.length() + 1, ' ');
	//account type
	transaction += target->permissionType + ' ';

	transaction += creditBalance_toString(amount);
	this->transactions.push_back(transaction);

	return 0;
}

#endif