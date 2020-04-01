#ifndef USER_H
#define USER_H

#include <string>
#include <vector> 

#define MAX_CREDIT 999999
//define user types

using namespace std;

class User{

private:
	string username;
	string type;
	int credit;

public:
	//Constructor
	User(string _username, string _type, int _credit){
		username = _username;
		type = _type;
		credit = 0;
		add_credit(_credit); //do it this way to make sure credit is not greater than MAX_CREDIT without having code repetition
	}

	//getters
	string get_username(){return username;}
	string get_type(){return type;}
	int get_credit(){return credit;}

	//alter credit
	void add_credit(int amount){
		credit += amount;
	}
	void remove_credit(int amount){
		credit -= amount;
	}

};

#endif

/*

// Represents users of this system.
// Constructor takes a (unique) username, login status, account type, and initial credit balance. 
// Contains methods to allow users to login, logout, advertise an item, bid on an item, 
// and add credit to their accounts.

class User_acct{
private:
	string username;
	int type;
	int credit;

public: 
	//Constructor
	User_acct(string _username, int _type, int _credit){
		username = _username;
		type = _type;
		credit = 0;
		add_credit(_credit); //do it this way to make sure credit is not greater than MAX_CREDIT without having code repetition
	}

	//getters
	string 	get_username(){return username;}
	int 	get_type(){return type;}
	int  	get_credit(){return credit;}

	//change credit value
	void add_credit(int amount){
		credit += amount;
		if(credit > MAX_CREDIT) credit = MAX_CREDIT;
	}
	int remove_credit(int amount){
		if(credit >= amount){
			credit -= amount;
			return 0;
		} else {
			return -1;
		}
	}

};

*/