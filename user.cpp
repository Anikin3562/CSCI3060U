#ifndef USER_H
#define USER_H

#include <string>
#include <vector> 

using namespace std;

// Represents users of this system.
// Constructor takes a (unique) username, login status, account type, and initial credit balance. 
// Contains methods to allow users to login, logout, advertise an item, bid on an item, 
// and add credit to their accounts.

class User{
public: 

	string username; 
	bool loggedIn;
	string permissionType; // 0 -> admin, 1 -> Full-Standard, 2 -> Buy-Standard, -> 3 -> Sell-Standard
	float creditBalance;

	// Keeps track of user's transactions, will be writted to Daily Transaction File 
	// at end of user session. 
	// 01 -> create, 02 -> delete, 03 -> advertise, 04 -> bid, 05 -> refund, 06 ->addcredit  
	vector<string> transactions;
	
	// Constructors
	User(){
		username = "null";
		loggedIn = false; 
		permissionType = "";  
		creditBalance = 0; 
	}

	User(string un, bool lI, string pT, float cB){
		username = un; 
		loggedIn = lI;
		permissionType = pT;
		creditBalance = cB; 
	}

	// Functions
	int login(string username); 
	int logout(string username); 
	int advertise(string item, float startingBid, int days); 
	int bid(float bid); 
	int addCredit(float credit); 



	};

	// Must be logged in to preform transactions.
	int User::login(string username){
		return 0; 
	}

	// Ends user's session, responsible for writing transactions to Daily Transaction File.
	int User::logout(string username){
		return 0;
	}

	// Allows a user with proper permissions to advertise an item.
	// This function writes to the Available Items file so we can keep track of
	// the auction's progress. 
	int User::advertise(string item, float startingBid, int days){
		return 0;
	}

	// Allows a user with proper permissions to bid on an advertised item.
	// Item is located in the Available Items file.
	int User::bid(float bid){
		return 0;
	}

	// Add credit to a user's account.
	// Works differently for users other
	// than admins.  
	int User::addCredit(float credit){
		return 0;
	}

#endif