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

}

// Responsible for handling the addcredit command.
void execute_addcredit(){
	
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
	cout << "Enter the username of the new user: ";
	cin >> newUser; 
	if (newUser.length() > 15 || newUser.length() == 0) {		//
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
	cout << "Enter username of account to delete";

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
	if (targetUser == admin.username) {
		return;
	}

}

// Responsible for handling the refund command.
void execute_refund(){

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