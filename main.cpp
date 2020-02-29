#include <iostream> 
#include <string>
#include <vector>
#include <map>
#include "user.cpp"
#include "admin.cpp"
#include "item.cpp"
#include <fstream>
#include <math.h>
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
vector <string> accountsFile; 
vector <string> itemsFile;
ifstream readAccounts;
ifstream readItems; 
ofstream dailyTransactions; 
string dailyTransactionFile; 
string removeSpaces(string str){

	int lastSpaceLoc;
	for(int i = str.length(); i > 0; i--){

		if(str[i-1] != ' '){
			lastSpaceLoc = i;
			break; 
		}
	}

	return str.substr(0, lastSpaceLoc);

}

void newFindUser(string username){
	// Read through accounts vector, search for username

	for(auto &account : accountsFile){

		string name = removeSpaces(account.substr(0, 15));
		
		if(username == name){
			
			string userType = removeSpaces(account.substr(16, 2));
			cout << userType << endl; 
			if(userType == "AA"){
				activeUser = 1; 
				float credits = stof(account.substr(19, 9));			
				Admin admin1(name, true, credits);
				admin = admin1; 

			}else{	
				activeUser = 0; 
				float credits = stof(account.substr(19, 9));
				User user1(name, true, userType, credits);
				user = user1; 
			}
		}
	}
}

void writeDailyTransactionFile(vector<string> transactions){
	

	dailyTransactions.open(dailyTransactionFile, ios_base::app);

	for(auto &transaction : transactions){

		dailyTransactions << transaction << endl;

	}
	dailyTransactions << "\n" << endl; 
	dailyTransactions.close();
	return; 
}

// This function truncates any decimal points after the first 2 for a user's credit balance.
// It also converts this value to a string and appends the proper
// ammount of zeroes to the front of that string to it can be properly
// written to the Daily Transaction File. 
string parseCreditBalance(float credits){
	

	string creditBalanceString = to_string(credits); 
	int totalPreDecimalLength = 6;
	int postDecimalLength = 3;
	int decimalLoc; 

	for(int i = 0; i < creditBalanceString.length(); i++){
		if(creditBalanceString[i] == '.') decimalLoc = i; 
	}

	// Appends correct amount of 0s to start of string. 
	creditBalanceString = string(totalPreDecimalLength - decimalLoc, '0') + creditBalanceString;

	// Truncates extra decimal values. 
	creditBalanceString = creditBalanceString.substr(0, decimalLoc + (totalPreDecimalLength - decimalLoc + postDecimalLength));

	return creditBalanceString;
}


// This function truncates any decimal points after the first 2 for a user's bid.
// It also converts this value to a string and appends the proper
// ammount of zeroes to the front of that string to it can be properly
// written to the Daily Transaction File. 
string parseBid(float bid){

	string creditBalanceString = to_string(bid); 
	int totalPreDecimalLength = 3;
	int postDecimalLength = 3;
	int decimalLoc; 

	for(int i = 0; i < creditBalanceString.length(); i++){
		if(creditBalanceString[i] == '.') decimalLoc = i; 
	}

	// Appends correct amount of 0s to start of string. 
	creditBalanceString = string(totalPreDecimalLength - decimalLoc, '0') + creditBalanceString;

	// Truncates extra decimal values. 
	creditBalanceString = creditBalanceString.substr(0, decimalLoc + (totalPreDecimalLength - decimalLoc + postDecimalLength));

	return creditBalanceString;
}

// Responsible for handling the login command. 
void execute_login(){

	string username; 
	cout << "Enter your username: ";
	cin >> username; 

	//User user = findUser(username);
	newFindUser(username);
	//cout << admin.username << endl; 
	//cout << user.username << endl; 
	if(user.username == "null" && admin.username == "null"){
		cout << "ERROR: Username not found. " << endl; 
		return;
	}

	// Admin login
	if(activeUser == 1){
		//admin.username = user.username; 
		//admin.loggedIn = true; 
		//admin.creditBalance = user.creditBalance; 	

		cout << "Admin successfully logged in!" << endl; 
	}

	// Regular user login
	else{
		cout << "User successfully logged in!" << endl; 
	}
	
	
	// Login is not written to the Daily Transaction file. 
	return;
}

// Responsible for handling the logout command.
void execute_logout(){

	string name; 
	string userType; 
	float creditBalance;
	vector <string> transactions; 

	// You must be logged in to log out.
	if (user.loggedIn == false && admin.loggedIn == false) {
		cout << "Already Logged Out. " << endl; 
		return; 
	}

	else {
		
		// Check whether active user is an admin or a regular user. 
		if(activeUser == 0){
			name = user.username; 
			userType = user.permissionType; 
			creditBalance = user.creditBalance; 
		}

		else{ 
			name = admin.username; 
			userType = admin.permissionType; 
			creditBalance = admin.creditBalance; 
		}

		// Writing to the Daily Transaction File

		// Transaction Code
		string transaction = "00 ";

		// Name with proper amount of spacing. 
		transaction += (name + string(15 - name.length() + 1, ' '));

		// User's type. 
		transaction += userType + ' ';

		// Ensures only two decimals are written to file. 
		string newBalanceString = parseCreditBalance(creditBalance);

		transaction += newBalanceString;

		cout << transaction << endl;

		if(activeUser == 0){
			user.transactions.push_back(transaction);
			transactions = user.transactions; 
		}

		else{
			admin.transactions.push_back(transaction);
			transactions = admin.transactions;

			cout << transactions[0] << endl; 
		}



	}
	User tempUser; 
	user = tempUser;

	Admin tempAdmin;  
	admin = tempAdmin;

	cout << "Logging out... " << endl; 

	// Write daily transaction file
	writeDailyTransactionFile(transactions);
	return;
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
	string transaction = "03 ";

	transaction += itemName + string(25 - itemName.length() + 1, ' ');

	if(activeUser == 0)
		transaction += user.username + string(15 - user.username.length() + 1, ' ');
	else
		transaction += admin.username + string(15 - admin.username.length() + 1, ' ');

	string daysLeftString = to_string(daysRemaining);
	if(daysLeftString.length() == 2){
		daysLeftString = '0' + daysLeftString + ' ';
	}

	if(daysLeftString.length() == 1){
		daysLeftString = "00" + daysLeftString + ' ';
	}

	transaction += daysLeftString;

	// Put the bid into a transaction-writeable form
	string newBalanceString = parseBid(minBid);
	
	transaction += newBalanceString;

	cout << transaction << endl;

	if(activeUser == 0){
		user.transactions.push_back(transaction);
	}
	else{
		admin.transactions.push_back(transaction);
	}

}

// Responsible for handling the bid command.
void execute_bid(){
	string itemName; 
	string sellerName; 
	float userBid; 

	cout << "Enter the seller's username: ";
	cin >> sellerName; 

	// Check that seller exists.
	string un;
	string in; 
	string highestBidder;
	int daysLeft; 
	float highestBid; 

	bool sellerExists = false; 
	bool itemExists = false;

	cout << "Enter the name of the item you wish to bid on: ";
	cin >> itemName;
	
	// Check that item exists. 
	// Read item into item instantiation.

	for(auto &item : itemsFile){
		if(itemName == removeSpaces(item.substr(0, 25))){
			itemExists = true;
			if(sellerName == removeSpaces(item.substr(26, 15))){
				sellerExists = true;
				highestBidder = removeSpaces(item.substr(42, 15));
				daysLeft = stoi(item.substr(58, 3));
				highestBid = stof(item.substr(62, 6));	
			}
			
		}
	}


	if(!sellerExists || !itemExists){
		cout << "ERROR: This user does not exist/ Item not sold by this user." << endl; 
	}
	
	Item item(itemName, daysLeft, highestBid, highestBidder);

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
	// Write DTF
	string transaction = "04 ";
	string typeString; 

	transaction += itemName + string(25 - itemName.length() + 1, ' ');
	transaction += sellerName + string(15 - sellerName.length() + 1, ' ');

	if(activeUser == 0)
		transaction += user.username + string(15 - user.username.length() + 1, ' ');
	else
		transaction += admin.username + string(15 - admin.username.length() + 1, ' ');

	string newBalanceString = parseBid(userBid);

	transaction += newBalanceString;

	cout << transaction << endl;

	if(activeUser == 0){
		user.transactions.push_back(transaction);
	}
	else{
		admin.transactions.push_back(transaction);
	}


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

		// Write DTF
		string transaction = "06 ";
		string typeString = user.permissionType; 
		transaction += user.username + string(15 - user.username.length() + 1, ' ');

		transaction += typeString; 

		string newBalanceString = parseCreditBalance(credit);
		
		transaction += newBalanceString;

		cout << transaction << endl;

		if(activeUser == 0){
			user.transactions.push_back(transaction);
		}
		else{
			admin.transactions.push_back(transaction);
		}

		return; 


	}

	else{ 
 
		string username;
		float credit; 

		cout << "Enter the name of the user you wish to add credit to: ";
		cin >> username; 

	 
		string accountType; 
		float accountBalance; 
		bool userExists = false; 

		for(auto &account : accountsFile){

			if(username == removeSpaces(account.substr(0, 15))){
				userExists = true; 
				accountType = account.substr(16, 2);
				accountBalance = stof(account.substr(19, 9));
			}
		}

		if(!userExists){
			cout << "ERROR: User does not exist!" << endl; 
			return; 
		}

		User temp(username, false, accountType, accountBalance);

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
		string transaction = "06 ";
		transaction += username + string(15 - username.length() + 1, ' ');

		transaction += (accountType + " "); 

		string newBalanceString = parseCreditBalance(credit);

		transaction += newBalanceString;

		cout << transaction << endl;
		
		admin.transactions.push_back(transaction);


	}
}

// Responsible for handling the create command.
void execute_create(){
	if (user.loggedIn == false && admin.loggedIn == false) {
		//termop -> error: not logged in
		cout << "ERROR: User not logged in." << endl;
		return;
	}
	if (activeUser == 0) {
		//termop -> error: non admin user
		cout << "ERROR: You must be an admin to perform this command. " << endl; 
		return;
	}

	string newUser;
	string newType;
	float newBalance;

	//take username

	//input -> newUser;
	cout << "Enter the username of the new user: ";
	cin >> newUser; 
	
	// Username length must be 15 chars or less
	if (newUser.length() > 15 || newUser.length() == 0) {
			//out invalid username
		cout << "ERROR: Username length not valid." << endl; 
		return;
	}

	// Check if name taken. 
	for(auto &account : accountsFile){
		if(removeSpaces(account.substr(0, 15)) == newUser){
			cout << "ERROR : That user already exists." << endl; 
			return; 
		}
	}


	
	cout << "Enter the user's type: ";
	cin >> newType;

	if (newType != "AA" && newType != "FS" && newType != "SS" && newType != "BS") {
		cout << "Invalid Usertype." << endl;
		return; 
	}

	cout << "Enter the amount of credits the user will start with: ";
	cin >> newBalance; 

	if(newBalance > 999999.99){
		cout << "ERROR: Maximum credits a user can have is 999999.99" << endl;
		return;
	}

	if(newBalance < 0){
		cout << "ERROR: Minimum credits a user can be created with is 0!" << endl; 
		return;
	}

	// Write to daily transaction
	// XX_UUUUUUUUUUUUUUU_TT_CCCCCCCCC
	// 01 username        AA 999999.00
	string transaction = "01 ";
	string typeString = newType; 
	transaction += newUser + string(15 - newUser.length() + 1, ' ');

	transaction += typeString + ' '; 

	//newBalance = floor(newBalance * 100.) / 100.;
	

	string newBalanceString = parseCreditBalance(newBalance);

	transaction += newBalanceString;

	cout << transaction << endl;

	if(activeUser == 0){
		user.transactions.push_back(transaction);
	}

	else{
		admin.transactions.push_back(transaction);
	}


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

	//find user account in accounts vector

	bool foundTarget = false;
	string userType; 
	float credits; 
	for(auto &account : accountsFile){
		if(removeSpaces(account.substr(0, 15)) == targetUser){
			foundTarget = true; 
			userType = removeSpaces(account.substr(16, 2)); 
			credits = stof(account.substr(19, 9));
		}
	}

	if(foundTarget == false){
		cout << "ERROR: User does not exist!" << endl; 
		return; 
	}

	if (targetUser == admin.username) {
		cout << "ERROR: Cannot delete self" << endl;
		return;
	}
	//user->deleteUser(user->username, targetUser, user->permissionType, user->creditBalance);//doc storage ???

	// Write to daily transaction file. 
	string transaction = "02 ";

	transaction += targetUser + string(15 - targetUser.length() + 1, ' ');

	transaction += (userType + " ");

	string newBalanceString = parseCreditBalance(credits);
	
	transaction += newBalanceString;

	cout << transaction << endl; 

	admin.transactions.push_back(transaction);
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
	string un; 
	bool sellerExists; 
	bool buyerExists; 

	float sellerCredit;
	float buyerCredit; 
	

	for(auto &account : accountsFile){

		if(removeSpaces(account.substr(0, 15)) == sellerName){
			sellerExists = true; 
			sellerCredit = stof(account.substr(19, 9));
		}

		else if(removeSpaces(account.substr(0, 15)) == buyerName){
			buyerExists = true; 
			buyerCredit = stof(account.substr(19, 9));
		}
	}

	if(!sellerExists){
		cout << "ERROR: Seller does not exist!" << endl; 
		return; 
	}

	if(!buyerExists){
		cout << "ERROR: Buyer does not exist!" << endl;
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
	// XX_UUUUUUUUUUUUUUU_SSSSSSSSSSSSSSS_CCCCCCCCC
	string transaction = "05 ";
	string typeString; 
	transaction += buyerName + string(15 - buyerName.length() + 1, ' ');
	transaction += sellerName + string(15 - sellerName.length() + 1, ' ');

	string newBalanceString = parseCreditBalance(credit);
	
	transaction += newBalanceString;

	cout << transaction << endl;

	if(activeUser == 0){
		user.transactions.push_back(transaction);
	}
	else{
		admin.transactions.push_back(transaction);
	}



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
	
	// Read in Accounts File, Items File. 
	if(argc == 4){

		readAccounts.open(argv[1]);
		readItems.open(argv[2]);
		//dailyTransactions.open(argv[3]);
		dailyTransactionFile = argv[3];

		string line; 
		while(getline(readAccounts, line)){
			if(line == "END") break; 

			accountsFile.push_back(line);
		}

		while(getline(readItems, line)){
			if(line == "END") break; 

			itemsFile.push_back(line);
		}




	}

	else{
		cout << "usage: ./a.out accounts.txt items.txt DailyTransactionFile.txt" << endl; 
		exit(0);
	}

	while(1){
		read_input();
	}
}