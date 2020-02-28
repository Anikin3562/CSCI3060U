#include <iostream> 
#include <string>
#include <vector>
#include <map>
#include "user.cpp"
#include "admin.cpp"
#include "item.cpp"
#include <fstream>

using namespace std; 
User* user = NULL; 
Admin* admin = NULL;
// This is the main program. 
// This where user interaction with the system occurs. 
// The main method continuously reads input via the read_input()
// function. This function will detect command keywords (such as login, logout, auction, etc.) and
// then (given that the user has proper permissions) these functions will be executed by the
// execute functions. 


template <class T=User*>
T activeUser() {
	if (user != NULL)
		return user;
	else if (admin != NULL)
		return admin;
	else
		return NULL;
}

// These execute functions will utilize the previously declared functions in User, Admin, and Item.


// Responsible for handling the login command. 
void execute_login(){
	cout << "enter username: ";
	string username;
	cin >> username;

	ifstream infile;
	infile.open("accounts.txt");
	string bufUsername;
	string bufUserType;
	float bufUserCredit;

	while (infile >> bufUsername) {
		if (bufUsername == "END") {		//reached end of file, username didnt match.
			cout << "Specified username not found" << endl;
			return;
		}
		if (bufUsername == username) {	//found username match in account file
			infile >> bufUserType;
			infile >> bufUserCredit;

			int userType;
			if (bufUserType == "AA") userType = 0;
			if (bufUserType == "FS") userType = 1;
			if (bufUserType == "BS") userType = 2;
			if (bufUserType == "SS") userType = 3;

			if (userType != 0) {
				user = new User(bufUsername, userType, bufUserCredit);
				cout << "user logged in" <<endl;
			}
			else {
				admin = new Admin(bufUsername, bufUserCredit);
				cout << "admin logged in" <<endl;
			}
			return;
		}

	}
	infile.close();
}

// Responsible for handling the logout command.
void execute_logout(){
	//write transaction file 
	activeUser()->logout();		//will log transaction files
	free( activeUser() );			//free memory
	//activeUser() = NULL;		//simulates 'not being logged on
	cout << "successfully logged out" << endl;
}

// Responsible for handling the advertise command.
void execute_advertise(){
	string itemName; 
	float minBid;
	int daysRemaining;

	cout << "Enter the name of the item you wish to advertise: ";
	cin >> itemName; 

	if(!(itemName.length() <= 25)){
		cout << "ERROR: Item's name must not exceed 25 characters." << endl;
		return; 
	} 

	cout << "Enter the minimum bid for the item: ";
	cin >> minBid; 

	if(minBid < 0){
		cout << "ERROR: starting bid must surpass $0.00" << endl;
		return;  
	}

	if(minBid > 999.99){
		cout << "ERROR: starting bid cannot exceed $999.99." << endl; 
		return; 
	}

	cout << "Enter the number of days the auction will run for: ";
	cin >> daysRemaining;

	if(daysRemaining < 1){
		cout << "ERROR: Advertisement must run for at least a day!" << endl; 
		return; 
	}

	if(daysRemaining > 100){
		cout << "ERROR: Advertisement can run for at most 100 days!" << endl; 
		return; 
	}

	activeUser()->advertise(itemName, activeUser()->username, minBid, daysRemaining);
}

// Responsible for handling the bid command.
void execute_bid(){
	string itemName;
	string sellerName;
	float userBid;

	ifstream items;
	items.open("items.txt");

	cout << "Enter the seller's username: ";
	cin >> sellerName;


	// Check that seller exists.
	string un;
	string in;
	string highestBidder;
	int daysLeft;
	float highestBid;

	bool sellerExists;
	bool itemExists;

	cout << "Enter the name of the item you wish to bid on: ";
	cin >> itemName;

	// Check that item exists. 
	// Read item into item instantiation.

	while (items >> in) {

		if (in == "END") break;

		items >> un;
		items >> highestBidder;
		items >> daysLeft;
		items >> highestBid;

		if ((in == itemName) && (un == sellerName)) {
			itemExists = true;
			sellerExists = true;
			break;
		}

		// Ensures that item comes from specified user. 
		itemExists = false;
		sellerExists = false;

	}

	if (!sellerExists) {
		cout << "ERROR: Seller does not exist!" << endl;
		items.close();
		return;
	}

	if (!itemExists) {
		cout << "ERROR: Item does not exist!" << endl;
		items.close();
		return;
	}

	Item item(itemName, daysLeft, highestBid, highestBidder);
	items.close();

	cout << item.currentBid << endl;

	cout << "Enter your bid: ";
	cin >> userBid;

	if (userBid <= highestBid) {
		cout << "ERROR: Your bid must be higher than the previous bid!" << endl;
		return;
	}

	if (userBid > 999.99) {
		cout << "ERROR: Maximum bid is $999.99." << endl;
		return;
	}

	item.currentBid = userBid;
	item.currentBidder = user->username;

	//cout << item.currentBidder << ": " << item.currentBid << endl; 

	activeUser()-> bid(itemName, sellerName, user->username , userBid);
}

// Responsible for handling the addcredit command.
void execute_addcredit(){

	if (activeUser() != admin) {
		float credit;
		cout << "Enter the amount of credit you wish to add: ";
		cin >> credit;

		if (credit > 1000) {
			cout << "ERROR: Maximum amount of credit that can be added is $1000" << endl;
			return;
		}


		if (credit <= 0) {
			cout << "ERROR: Invalid credit amount." << endl;
			return;
		}

		user->creditBalance += credit;
		return;
	}

	else {
		float credit;
		string username;

		cout << "Enter the name of the user you wish to add credit to: ";
		cin >> username;

		ifstream accounts;
		accounts.open("accounts.txt");

		string un;
		string accountType;
		float accountBalance;

		while (accounts >> un) {
			if (un == "END") {
				cout << "ERROR: User does not exist!" << endl;
				return;
			}
			accounts >> accountType;
			accounts >> accountBalance;
			if (un == username) {
				break;
			}
		}

		int accType;
		if (accountType == "AA") accType = 0;
		if (accountType == "FS") accType = 1;
		if (accountType == "BS") accType = 2;
		if (accountType == "SS") accType = 3;

		cout << "Enter the amount of credit you wish to add: ";
		cin >> credit;

		if (credit > 1000) {
			cout << "ERROR: Maximum amount of credit that can be added is $1000" << endl;
			return;
		}

		if (credit <= 0) {
			cout << "ERROR: Invalid credit amount." << endl;
			return;
		}

		if (accountBalance+credit > 999999.99) {
			cout << "ERROR: Adding this credit would make user exceed maximum credit limit!" << endl;
			return;
		}

		accountBalance += credit;
		//cout << temp.creditBalance << endl;
	}
}

// Responsible for handling the create command.
void execute_create(){
	string newUser;
	int newType;
	float newBalance;

	//take username
	cout << "Enter the username of the new user: ";
	cin >> newUser; 
	if (newUser.length() > 15 || newUser.length() == 0) {	
		cout << "ERROR: Username length not valid." << endl; 
		return;
	}

	//find user account in user accounts file
	ifstream infile;
	infile.open("accounts.txt");

	string un; 
	while(infile >> un){
		if(un == "END") break; 

		if(newUser == un){
			cout << "ERROR: That user already exists." << endl;
			infile.close();
			return;  
		}
	}
	infile.close();

	//take usertype
	cout << "Enter the new user's type \n 0 -> admin, 1-> full standard, 2-> buy standard, 3-> sell standard): ";

	cin >> newType;
	if (newType < 0 || newType>3) {
		cout << "ERROR: Invalid Usertype." << endl;
		return;
	}

	//takes initial creditbalance
	cout << "Enter the new user's initial credit balance: ";
	cin >> newBalance;
	if (newBalance < 0 || newBalance>999999) {
		cout << "ERROR: balance out of accepted range"<<endl;
		return;
	}
	if (activeUser()==admin)
		admin->create(newUser,newType,newBalance); //ADMIN PERMISSION ISSUES
}

// Responsible for handling the delete command.
void execute_delete(){
	cout << "Enter username of account to delete: ";

	//take username
	string targetUser;
	cin >> targetUser;
	if (targetUser.length() > 15 || targetUser.length() == 0) {		//
		cout << "ERROR: Username length not valid." << endl;
		return;
	}

	//find user account in user accounts file
	ifstream infile;
	infile.open("accounts.txt");

	string un;
	while (infile >> un) {
		if (un == "END") {
			cout << "ERROR: User doesn't exist" << endl;
			infile.close();
			return;
		}
		if (targetUser == un) {
			break;
		}
	}
	infile.close();
	if (targetUser == user->username) {
		cout << "ERROR: Cannot delete self" << endl;
		return;
	}
	if (activeUser() == admin)
		admin->deleteUser(targetUser); //ADMIN PERMISSION ISSUES
}

// Responsible for handling the refund command.
void execute_refund(){
	string buyerName;
	string sellerName;
	float credit;

	cout << "Enter the buyer's username: ";
	cin >> buyerName;

	cout << "Enter the seller's username: ";
	cin >> sellerName;

	// Check existence
	ifstream infile;

	string un;
	bool sellerExists;
	bool buyerExists;

	float sellerCredit;
	float buyerCredit;
	string tempBuffer;

	infile.open("accounts.txt");

	while (infile >> un) {

		if (un == "END") break;

		if (un == sellerName) {
			sellerExists = true;
			infile >> tempBuffer;
			infile >> sellerCredit;
		}

		if (un == buyerName) {
			buyerExists = true;
			infile >> tempBuffer;
			infile >> buyerCredit;

		}
	}

	if (!sellerExists) {
		cout << "ERROR: Seller does not exist!" << endl;
		infile.close();
		return;
	}

	if (!buyerExists) {
		cout << "ERROR: Buyer does not exist!" << endl;
		infile.close();
		return;
	}

	cout << buyerCredit << " " << sellerCredit << endl;
	// Check existence

	cout << "Enter the amount of credit you wish to refund: ";
	cin >> credit;

	if (sellerCredit < credit) {
		cout << "ERROR: Seller does not have enough credits to transfer!" << endl;
		return;
	}

	if ((buyerCredit + credit) > 999999.99) {
		cout << "ERROR: Buyer's credit would exceed the maximum allowed credits!" << endl;
		return;
	}

	buyerCredit += credit;
	sellerCredit -= credit;

	// Write to daily transaction file.
	infile.close();
	if (activeUser() == admin)
		admin->refund(buyerName, sellerName, credit); //ADMIN PERMISSION ISSUES
	//user->refund(buyerName, sellerName, credit);
}

// Read user input into userInput, detect and execute command accordingly. 
void read_input(){

	string userInput; 
	if (activeUser() != NULL)
		cout << "(" << activeUser()->username << ") ";
	cout << "enter transaction: ";
	cin >> userInput;

	//transactions allowed while not logged in
	if (activeUser()==NULL) {
		if (userInput == "login") 
			execute_login();
		else 
			cout << "ERROR: transaction not available" << endl;
		return;
	}
	
	//transactions allowed on login
	else if (activeUser() != NULL) {
		if (userInput == "login") {
			cout << "ERROR: already logged in" << endl;
			return;
		}

		else if (userInput == "logout") {
			execute_logout();
			return;
		}

		else if (userInput == "advertise") {
			execute_advertise();
			return;
		}

		else if (userInput == "bid") {
			execute_bid();
			return;
		}

		else if (userInput == "addcredit") {
			execute_addcredit();
			return;
		}

		if (activeUser() == admin) {	//admin
			if (userInput == "create") {
				execute_create();
				return;
			}
			if (userInput == "delete") {
				execute_delete();
				return;
			}
			if (userInput == "refund") {
				execute_refund();
				return;
			}
		}
		else if (activeUser() != admin){
			cout << "ERROR: You must be an admin to perform this transaction" << endl;
			return;
		}
		else {
			cout << "ERROR: transaction does not exist" << endl;
			return;
		}
	}

	else if(userInput == "exit"){
		exit(0);
	}

	else{
		cout << "Error: Invalid Input" << endl; 
	}

}

int main(int argc, char** argv){

	cout << "/////////////////////////\n* Auction System Online *\n/////////////////////////\n" << endl; 
	while(1){
		read_input();
	}
}