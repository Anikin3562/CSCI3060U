#include <iostream> 
#include <string>
#include <vector>
#include <map>
#include "user.cpp"
#include "admin.cpp"
#include "item.cpp"


using namespace std; 

// This is the main program. 
// This where user interaction with the system occurs. 
// The main method continuously reads input via the read_input()
// function. This function will detect command keywords (such as login, logout, auction, etc.) and
// then (given that the user has proper permissions) these functions will be executed by the
// execute functions. 

// These execute functions will utilize the previously declared functions in User, Admin, and Item.

// Responsible for handling the login command. 
User user = new Admin();



void execute_login(){

}

// Responsible for handling the logout command.
void execute_logout(){
	//if the user isnt logged in
	if (user.loggedIn == false) {
		//out-> already logged out
	}

	//user is logged in
	else {
		//write transaction file
		user.loggedIn == false;
		//termop -> logged out successfully

		//write daily transaction file
	}
}

// Responsible for handling the advertise command.
void execute_advertise(){

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
		return;
	}
	if (user.permissionType != 0) {
		//termop -> error: non admin user
		return;
	}

	string newUser;
	int newType;
	float newBalance;

	//take username
	while (true) {
		//input -> newUser;
		//username length must be 15 chars or less

		//username selection
		if (inputUser.length > 15 && inputUser.length == 0) {
			//out invalid username
			continue;
		}
		else if (inputUser.length <= 15) {
			//find user account in user accounts file
			//if not found, continue to start of loop
		}
		break;
	}
	//take usertype
	while (true) {
		//input -> userYype
		if (newType < 0 && newType > 3) {
			//out invalid type
			continue;
		}
		break;
	}


	while (true) {
		//input - > newBalance
		if (newBalance < 0 && newBalance > 999999) {
			//out invalid balance
			continue;
		}
		break;
		//add account to UAC
		//record transaction
	}


}

// Responsible for handling the delete command.
void execute_delete(){

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