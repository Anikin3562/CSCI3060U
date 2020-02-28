#include <iostream> 
#include <string>
#include <vector>
#include <map>
#include "user.cpp"
#include "admin.cpp"
#include "item.cpp"
#include <fstream>

using namespace std; 

User user; 
Admin admin;
int activeUser; // 0 if user, 1 if admin. 
// This is the main program. 
// This where user interaction with the system occurs. 
// The main method continuously reads input via the read_input()
// function. This function will detect command keywords (such as login, logout, auction, etc.) and
// then (given that the user has proper permissions) these functions will be executed by the
// execute functions. 

// These execute functions will utilize the previously declared functions in User, Admin, and Item.

void findUser(string username){

	ifstream infile;
	infile.open("accounts.txt");
	string bufUsername; 
	string bufUserType;
	float bufUserCredit;


	while(infile >> bufUsername){
		
		if(bufUsername == "END")
			break; 

		if(bufUsername == username){
			infile >> bufUserType;
			infile >> bufUserCredit;	
			
			{
				int userType;

				if(bufUserType == "AA"){ 
					userType = 0;
					activeUser = 1; 
				} 
				else{
					activeUser = 0; 
					
					if(bufUserType == "FS") userType = 1;
					if(bufUserType == "BS") userType = 2; 
					if(bufUserType == "SS") userType = 3; 
				}

				User user1(bufUsername, true, userType, bufUserCredit);
				user = user1;
				
			}
		}
		
	}

	infile.close();
	
}

// Responsible for handling the login command. 
void execute_login(){
	string username; 
	cout << "Enter your username: ";
	cin >> username; 

	//User user = findUser(username);
	findUser(username);



	if(user.username == "null"){
		cout << "ERROR: Username not found. " << endl; 
		return;
	}

	// Admin login
	if(activeUser == 1){
		admin.username = user.username; 
		admin.loggedIn = true; 
		admin.creditBalance = user.creditBalance; 	

		cout << "Admin successfully logged in!" << endl; 
	}

	// Regular user login
	else{
		cout << "User successfully logged in!" << endl; 
	}
	
	
	// Write daily transaction
}

// Responsible for handling the logout command.
void execute_logout(){
	//if the user isnt logged in
	if (user.loggedIn == false) {
		cout << "Already Logged Out. " << endl; 
		return; 
	}

	//user is logged in
	else {
		//write transaction file

		User tempUser; 
		user = tempUser;

		Admin tempAdmin;  
		admin = tempAdmin;

		cout << "Logging out... " << endl; 

		//write daily transaction file
	}
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
		cout << "ERROR: Minimum bid at least be $0." << endl;
		return;  
	}

	if(minBid > 999.99){
		cout << "ERROR: Item price cannot exceed $999.99." << endl; 
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

	// Write daily transction


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

	while(items >> in){

		if(in == "END") break; 

		items >> un; 
		items >> highestBidder; 
		items >> daysLeft;
		items >> highestBid;

		if((in == itemName) && (un == sellerName)){
			itemExists = true;
			sellerExists = true; 
			break;
		}

		// Ensures that item comes from specified user. 
		itemExists = false; 
		sellerExists = false; 

	} 

	if(!sellerExists){
		cout << "ERROR: Seller does not exist!" << endl; 
		items.close();
		return; 
	}

	if(!itemExists){
		cout << "ERROR: Item does not exist!" << endl; 
		items.close();
		return; 
	}

	Item item(itemName, daysLeft, highestBid, highestBidder);
	items.close();

	cout << item.currentBid << endl; 

	cout << "Enter your bid: ";
	cin >> userBid; 

	if(userBid <= highestBid){
		cout << "ERROR: Your bid must be higher than the previous bid!" << endl;
		return; 
	}

	if(userBid > 999.99){
		cout << "ERROR: Maximum bid is $999.99." << endl;
		return; 
	}

	item.currentBid = userBid; 

	if(activeUser == 0){
		item.currentBidder = user.username;
	}

	else{
		item.currentBidder = admin.username; 
	}

	//cout << item.currentBidder << ": " << item.currentBid << endl; 

	// Write to daily transaction file. 


}

// Responsible for handling the addcredit command.
void execute_addcredit(){
	
	if(activeUser == 0){
		float credit; 
		cout << "Enter the amount of credit you wish to add: ";
		cin >> credit; 

		if(credit > 1000){
			cout << "ERROR: Maximum amount of credit that can be added is $1000" << endl; 
			return;
		}

		if(credit <= 0){
			cout << "ERROR: Invalid credit amount." << endl; 
			return; 
		}

		user.creditBalance += credit; 
		return; 
	}

	else{ 

		float credit; 
		string username;

		cout << "Enter the name of the user you wish to add credit to: ";
		cin >> username; 

		ifstream accounts; 
		accounts.open("accounts.txt");

		string un; 
		string accountType; 
		float accountBalance; 

		bool userExists; 

		while(accounts >> un){

			if(un == "END") break; 

			accounts >> accountType; 
			accounts >> accountBalance; 

			
			if(un == username){
				userExists = true; 
				break; 
			}

		}

		if(!userExists){
			cout << "ERROR: User does not exist!" << endl; 
			return; 
		}

		int accType;

		if(accountType == "AA") accType = 0;
		if(accountType == "FS") accType = 1; 
		if(accountType == "BS") accType = 2; 
		if(accountType == "SS") accType = 3; 

		User temp(un, false, accType, accountBalance);

		cout << "Enter the amount of credit you wish to add: ";
		cin >> credit; 

		if(credit > 1000){
			cout << "ERROR: Maximum amount of credit that can be added is $1000" << endl; 
			return;
		}

		if(credit <= 0){
			cout << "ERROR: Invalid credit amount." << endl; 
			return; 
		}

		if(temp.creditBalance > 999999.99){
			cout << "ERROR: Adding this credit would make user exceed maximum credit limit!" << endl; 
			return; 
		}

		temp.creditBalance += credit; 
		//cout << temp.creditBalance << endl;
		
		// Write to daily transcation
	}
}

// Responsible for handling the create command.
void execute_create(){
	if (user.loggedIn == false) {
		//termop -> error: not logged in
		cout << "ERROR: User not logged in." << endl;
		return;
	}
	if (user.permissionType != 0) {
		//termop -> error: non admin user
		cout << "ERROR: You must be an admin to perform this command. " << endl; 
		return;
	}

	string newUser;
	int newType;
	float newBalance;

	//take username

		//input -> newUser;
		//username length must be 15 chars or less
	cout << "Enter the username of the new user: ";
	cin >> newUser; 
		//username selection

	if (newUser.length() > 15 || newUser.length() == 0) {
			//out invalid username
		cout << "ERROR: Username length not valid." << endl; 
		return;
	}


	//find user account in user accounts file
	//if not found, continue to start of loop
	ifstream infile;
	infile.open("accounts.txt");

	string un; 

	while(infile >> un){
		
		if(un == "END") break; 

		if(newUser == un){
			cout << "ERROR: That user already exists." << endl;
			return;  
		}
	}

	infile.close();



	//take usertype
	
	//input -> userType
	cout << "Enter the user's type: ";
	cin >> newType;
	if (newType < 0 || newType > 3) {
		//out invalid type
		cout << "Invalid Usertype." << endl;
		return; 
	}


	// Write to daily transaction
}

// Responsible for handling the delete command.
void execute_delete(){

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

	while(infile >> un){

		if(un == "END") break; 

		if(un == sellerName){
			sellerExists = true;
			infile >> tempBuffer; 
			infile >> sellerCredit;
		}  

		if(un == buyerName){
			buyerExists = true;
			infile >> tempBuffer; 
			infile >> buyerCredit; 
			
		}  

	}

	if(!sellerExists){
		cout << "ERROR: Seller does not exist!" << endl; 
		infile.close();
		return; 
	}

	if(!buyerExists){
		cout << "ERROR: Buyer does not exist!" << endl; 
		infile.close();
		return; 
	}

	cout << buyerCredit << " " << sellerCredit << endl; 
	// Check existence

	cout << "Enter the amount of credit you wish to refund: ";
	cin >> credit;

	if(sellerCredit < credit){
		cout << "ERROR: Seller does not have enough credits to transfer!" << endl; 
		return; 
	}

	if((buyerCredit + credit) > 999999.99){
		cout << "ERROR: Buyer's credit would exceed the maximum allowed credits!" << endl;
		return; 
	}

	buyerCredit += credit; 
	sellerCredit -= credit; 

	// Write to daily transaction file.



	// Check seller has that much credit to refund.
	// Check buyer has space for the credit. 
	infile.close();


}

// Read user input into userInput, detect and execute command accordingly. 
void read_input(){

	string userInput; 

	cin >> userInput;

	if(userInput == "login"){
		execute_login();
	}

	else if(userInput == "logout"){
		execute_logout();
	}

	else if(userInput == "advertise"){
		execute_advertise();
	}

	else if(userInput == "bid"){
		execute_bid();
	}

	else if(userInput == "addcredit"){
		execute_addcredit(); 
	}

	else if(userInput == "create"){
		execute_create(); 
	}

	else if(userInput == "delete"){
		execute_delete();
	}

	else if(userInput == "refund"){
		execute_refund();
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