//THIS IS THE BASE
#include <iostream> 
#include <string>
#include <vector>
#include <map>
#include "user.cpp"
#include "admin.cpp"
#include "item.cpp"
#include <fstream>

using namespace std; 
User* user = NULL; //creates user instance. NULL will implied the user is logged out
Admin* admin = NULL; //same as user, but for admin type user

vector <string> accountsFile_data;			//stores accounts
vector <string> itemsFile_data;				//stores items

string accountsFile;
string itemsFile;

//template function that will return the current active user.
template <class T=User*>
T activeUser() {
	if (user != NULL)
		return user;
	else if (admin != NULL)
		return admin;
	else
		return NULL;
}


//function to remove trailing spaces
string removeSpaces(string str) {
	int lastSpaceLoc;
	for (int i = str.length(); i > 0; i--) {

		if (str[i - 1] != ' ') {
			lastSpaceLoc = i;
			break;
		}
	}
	return str.substr(0, lastSpaceLoc);
}


//function to read the current accounts file | called upon every login request
void read_accountsFile() {
	accountsFile_data.clear();	//clear the current values in accountsFile
	ifstream read;					
	read.open( accountsFile );

	string line;
	while (getline(read, line)) {
		if (line == "END") break;
		accountsFile_data.push_back(line);
	}
	read.close();
}

//function to read the current available items file | called upon every succesful login
void read_itemsFile() {
	itemsFile_data.clear();
	ifstream read;
	read.open(itemsFile);

	string line;
	while (getline(read, line)) {
		if (line == "END") break;
		itemsFile_data.push_back(line);
	}
	read.close();
}

//TRANSACTIONS:

//Responsible for handling the login command. 
void execute_login(){
	
	read_accountsFile();

	//request and take in username input
	cout << "enter username: ";
	string username;
	cin >> username;

	for (auto& account : accountsFile_data) {
		string account_name = removeSpaces(account.substr(0, 15));		//gets username data from account
		string account_perms = removeSpaces(account.substr(16, 2));		//gets permission data from account
		float account_balance = stof(account.substr(19, 9));			//get balance data from account
		if (username == account_name) { //login username found in accounts file
			if (account_perms == "AA") //admin user login
				admin = new Admin(account_name, account_balance);
			else {
				user = new User(account_name,account_perms,account_balance);
			}
		}
	}
}

// Responsible for handling the logout command.
void execute_logout(){
	//write transaction file 
	activeUser()->logout();		//will create daily transaction files
	user = NULL;
	admin = NULL;
	cout << "successfully logged out" << endl;
}

// Responsible for handling the advertise command.
void execute_advertise(){
	string itemName; 
	float minBid;
	int daysRemaining;

	//prompt/store for itemname
	cout << "Enter the name of the item you wish to advertise: ";
	cin >> itemName; 

	if (!(itemName.length() <= 25 || itemName.length() == 0)) {
		cout << "ERROR: Item's name must be at least 1 character and at most 25." << endl;
		return; 
	} 

	//prompt/store for minimum bid
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

	//prompt/store for days
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

	activeUser()->advertise(itemName, minBid, daysRemaining);
}

// Responsible for handling the bid command.
void execute_bid(){
	string itemName;
	string sellerName;
	float userBid;

	cout << "Enter the seller's username: ";
	cin >> sellerName;

	string un;
	string in;
	string highestBidder;
	int daysLeft;
	float highestBid;

	bool sellerExists = false;
	bool itemExists = false;

	cout << "Enter the name of the item you wish to bid on: ";
	cin >> itemName;
	
	//TODO
	//itterate through users to find seller
	//itterate throguh items with matching seller to find item


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


	activeUser()-> bid(itemName, sellerName , userBid);
}

// Responsible for handling the addcredit command.
void execute_addcredit(){
	float credit;
	string username;
	User* target;

	if (activeUser() == admin) {
		cout << "Enter the name of the user you wish to add credit to: ";
		cin >> username;

		//read through users, find user if exists
		//store user in target
		//TO DO
	}

	//get credit
	cout << "Enter the amount of credit you wish to add: ";
	cin >> credit;

	if (credit > 1000) {
		cout << "ERROR: Maximum amount of credit that can be added is $1000" << endl;
		return;
	}

	if (credit <= 0) {
		cout << "ERROR: Cannot add 0 / negative credit" << endl;
		return;
	}

	if (activeUser() == admin) {
		if (target->creditBalance + credit > 999999.99) {
			cout << "ERROR: Adding this credit would make user exceed maximum credit limit!" << endl;
			return;
		}
		admin->addCredit(target, (target->creditBalance + credit) );
	}

	if (activeUser() == user) {
		if (user->creditBalance + credit > 999999.99) {
			cout << "ERROR: Adding this credit would make you exceed maximum credit limit!" << endl;
			return;
		}
		user->addCredit(user->creditBalance + credit);
	}
}

// Responsible for handling the create command.
void execute_create(){
	string username;
	string newType;
	float newBalance;

	//take username
	cout << "Enter the username of the new user: ";
	cin >> username; 
	if (username.length() > 15 || username.length() == 0) {	
		cout << "ERROR: Username length not valid." << endl; 
		return;
	}

	//find user account in user accounts file
	//TODO

	cout << "Enter the user's type: ";
	cin >> newType;

	if (newType != "AA" && newType != "FS" && newType != "SS" && newType != "BS") {
		cout << "Invalid Usertype." << endl;
		return;
	}

	cout << "Enter the amount of credits the user will start with: ";
	cin >> newBalance;

	if (newBalance > 999999.99) {
		cout << "ERROR: Maximum credits a user can have is 999999.99" << endl;
		return;
	}

	if (newBalance < 0) {
		cout << "ERROR: Minimum credits a user can have is 0!" << endl;
		return;
	}
	User* target = new User(username, newType, newBalance);
	admin->create(target); //ADMIN PERMISSION ISSUES
}

// Responsible for handling the delete command.
void execute_delete(){
	string target_username;
	User* target;

	//take username
	cout << "Enter username of account to delete: ";
	cin >> target_username;
	//invalid username length
	if (target_username.length() > 15 || target_username.length() == 0) {
		cout << "ERROR: Username length not valid." << endl;
		return;
	}
	//can't delete self
	if (target_username == activeUser()->username) {
		cout << "ERROR: Cannot delete self" << endl;
		return;
	}

	//TODO
	//find username match
	//if account doesnt exist, error, return
	//if it does, set target = account
	
	admin->deleteUser(target); //ADMIN PERMISSION ISSUES
}

// Responsible for handling the refund command.
void execute_refund(){
	string buyerName;
	string sellerName;
	float credit;
	User* buyer;
	User* seller;

	cout << "Enter the buyer's username: ";
	cin >> buyerName;

	//TODO
	//verify buyer exists, return if false
	//store in buyer

	cout << "Enter the seller's username: ";
	cin >> sellerName;

	//TODO
	//verify seller exists, return if false
	//store in seller

	cout << "Enter the amount of credit you wish to refund: ";
	cin >> credit;

	if (seller->creditBalance < credit) {
		cout << "ERROR: Seller does not have enough credits to transfer!" << endl;
		return;
	}

	if ((buyer->creditBalance + credit) > 999999.99) {
		cout << "ERROR: Buyer's credit would exceed the maximum allowed credits!" << endl;
		return;
	}
	admin->refund(buyer, seller, credit);
}

// Read user input into userInput, detect and execute command accordingly. 
void read_input(){
	string userInput; 
	//if user is logged in, include username - mostly to show that user is logged in
	if (activeUser() != NULL)
		cout << "(" << activeUser()->username << ") ";
	//request user input, store in userInput
	cout << "enter transaction: ";
	cin >> userInput;

	//exitrequest
	if (userInput == "exit") 
		exit(0);
	
	//only login accepted if not logged in
	if (activeUser()==NULL) {
		if (userInput == "login") 
			execute_login();
		else 
			cout << "ERROR: transaction not available" << endl;
		return;
	}
	
	//other transactions accepted
	else if (activeUser() != NULL) {
		//transactions available to all users

		if (userInput == "login") 
			cout << "ERROR: already logged in" << endl;

		else if (userInput == "logout") 
			execute_logout();
		
		else if (userInput == "advertise") 
			execute_advertise();
		
		else if (userInput == "bid") 
			execute_bid();
		
		else if (userInput == "addcredit") 
			execute_addcredit();
		
		//transactions available to admin users
		if (activeUser() == admin) {	
			if (userInput == "create") 
				execute_create();
			
			else if (userInput == "delete") 
				execute_delete();
			
			else if (userInput == "refund") 
				execute_refund();
		}

		else if (activeUser() != admin){
			cout << "ERROR: You must be an admin to perform this transaction" << endl;
		}

		else {
			cout << "ERROR: transaction does not exist" << endl;
		}
		return;
	}



	else{
		cout << "Error: Invalid Input" << endl; 
	}

}

int main(int argc, char** argv){
	if (argc == 3) {
		accountsFile = argv[1];
		itemsFile = argv[2];
	}
	else {
		cout << "usage: ./a.out accounts.txt items.txt" << endl;
		exit(0);
	}

	cout << "/////////////////////////\n* Auction System Online *\n/////////////////////////\n" << endl; 

	while(1){
		read_input();
	}
}