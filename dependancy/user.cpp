#ifndef USER_H
#define USER_H

#include <string>
#include <vector> 
#include <fstream>

using namespace std;

//returns credit value as a string (for transaction)
string creditBalance_toString(float credits) {
	string creditBalanceString = to_string(credits);
	int totalPreDecimalLength = 6;
	int postDecimalLength = 3;
	int decimalLoc;
	for (int i = 0; i < creditBalanceString.length(); i++) {
		if (creditBalanceString[i] == '.') decimalLoc = i;
	}
	// Appends correct amount of 0s to start of string. 
	creditBalanceString = string(totalPreDecimalLength - decimalLoc, '0') + creditBalanceString;
	// Truncates extra decimal values. 
	creditBalanceString = creditBalanceString.substr(0, decimalLoc + (totalPreDecimalLength - decimalLoc + postDecimalLength));
	return creditBalanceString;
}

//returns bid value as a string (for transaction)
string bid_toString(float bid) {

	string creditBalanceString = to_string(bid);
	int totalPreDecimalLength = 3;
	int postDecimalLength = 3;
	int decimalLoc;

	for (int i = 0; i < creditBalanceString.length(); i++) {
		if (creditBalanceString[i] == '.') decimalLoc = i;
	}

	// Appends correct amount of 0s to start of string. 
	creditBalanceString = string(totalPreDecimalLength - decimalLoc, '0') + creditBalanceString;

	// Truncates extra decimal values. 
	creditBalanceString = creditBalanceString.substr(0, decimalLoc + (totalPreDecimalLength - decimalLoc + postDecimalLength));

	return creditBalanceString;
}




// Represents users of this system.
// Constructor takes a (unique) username, login status, account type, and initial credit balance. 
// Contains methods to allow users to login, logout, advertise an item, bid on an item, 
// and add credit to their accounts.

class User {
public:
	string username;
	string permissionType;
	float creditBalance;

	// Keeps track of user's transactions, will be writted to Daily Transaction File 
	vector<string> transactions;

	// Constructors
	User() {
		username = "null";
		permissionType = -1;
		creditBalance = 0;
	}

	User(string un, string pT, float cB) {
		username = un;
		permissionType = pT;
		creditBalance = cB;
	}

	// Functions

	int logout(string fileName);
	int advertise(string item, float startingBid, int days);
	int bid(string item, string seller, float bid);
	int addCredit(float credit);
};

	int User::logout(string filenName){//WRITE THE DAILY TRANSACTION FILE
		//add logout transaction
		string transaction = "00 ";
		transaction += (this->username + string(this->username.length() + 1, ' '));
		transaction += this->permissionType + ' ';
		transaction += creditBalance_toString(this->creditBalance);
		this->transactions.push_back(transaction);
		//logout transaction added

		ofstream write;
		write.open(filenName);
		for (auto& transaction : this->transactions) {
			write << transaction << endl;
		}
		write << "\n" << endl;
		write.close();
		return 0;
	}

	//add values
	int User::advertise(string item, float startingBid, int days){
		string transaction = "03 ";

		//for seller
		transaction += item + string(25 - item.length() + 1, ' ');
		transaction += this->username + string(15 - this->username.length() + 1, ' ');

		//for daysleft
		string daysLeftString = to_string(days);
		if (daysLeftString.length() == 2) 
			daysLeftString = '0' + daysLeftString + ' ';
		if (daysLeftString.length() == 1) {
			daysLeftString = "00" + daysLeftString + ' ';
		}
		transaction += daysLeftString;

		// Put the bid into a transaction-writeable form
		string startingBid_str = bid_toString(startingBid);
		transaction += startingBid_str;

		//push transaction string onto vector
		this->transactions.push_back(transaction);
		return 0;
	}

	int User::bid(string item, string seller, float bid){
		string transaction = "04 ";

		//item name
		transaction += item + string(25 - item.length() + 1, ' ');
		//seller name
		transaction += seller + string(15 - seller.length() + 1, ' ');
		//buyer name
		transaction += this->username + string(15 - this->username.length() + 1, ' ');
		//bid
		transaction += bid_toString(bid);
		//add transaction
		this->transactions.push_back(transaction);
		return 0;
	}

	int User::addCredit(float credit){
		string transaction = "06 ";
		string typeString = this->permissionType;

		//username 
		transaction += this->username + string(15 - this->username.length() + 1, ' ');
		//account type
		transaction += typeString+" ";

		transaction += creditBalance_toString(credit);
		this->transactions.push_back(transaction);
		return 0;
	}


#endif