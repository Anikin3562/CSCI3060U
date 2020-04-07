#include <iostream> 
#include <string>
#include <vector>
#include <map>
#include "user.cpp"
#include "item.cpp"
#include <fstream>

#define MAX_USERNAME_LEN 15
#define MAX_ITEMNAME_LEN 19
#define MAX_CREDIT_DOLLAR_LEN 6
#define MAX_CREDITS 99999900
#define MAX_CREDITS_ADDED 100000
#define MAX_ITEM_PRICE 99999
#define MAX_AUCTION_LEN 100

using namespace std;

string getUserInput();
bool isStringValidCredit(string);
string intToCreditString(int);
int creditStringToInt(string);
void loadUsers(map<string, User*>&, string);
void loadItems(map<pair<string,string>, Item*>&, string);
void writeTransactionA(ofstream& file, string transCode, string username, string userType, string credits);
void writeTransactionB(ofstream& file, string transCode, string buyerName, string sellerName, string credits);
void writeTransactionC(ofstream& file, string transCode, string itemName, string sellerName, string daysLeft, string price);
void writeTransactionD(ofstream& file, string transCode, string itemName, string sellerName, string bidderName, string bid);

int main(int argc, char** argv){
	//load data from accounts file
	map<string, User*> accounts;
	loadUsers(accounts, argv[1]);
	//load data form items file
	map<pair<string,string>, Item*> items;
	loadItems(items, argv[2]);
	//create transaction file ofstream
	ofstream transactionOFS(argv[3]);

	string input;
	User* currentUser = NULL;

	while(true){
		cout << (currentUser != NULL ? "("+currentUser->get_username()+") " : "")  << "enter transaction: " << endl;
		input = getUserInput();
		if(input == "exit"){
			break;
		} else if(input == "login"){
		//***************HANDLE LOGIN TRASACTION***************
			cout << "enter username:" << endl;
			input = getUserInput();
			//check if username exists, if not print error message
			if(accounts.find(input)!=accounts.end()){
				currentUser = accounts[input];
			} else{
				cout << "ERROR: account not found" << endl;
			}
		} else if(input == "logout"){
		//***************HANDLE LOGOUT TRANSACTION***************
			if(currentUser != NULL){
				writeTransactionA(transactionOFS, "00", currentUser->get_username(), currentUser->get_type(), intToCreditString(currentUser->get_credit()));
				currentUser = NULL;
				cout << "logout successful" << endl;
			} else{
				cout << "ERROR: must be logged in" << endl;
			}
		} else if(input == "create"){
		//***************HANDLE CREATE TRANSACTION***************
			//check if the current user is an admin, if not then continue, else print an error message
			if(currentUser->get_type() == "AA"){
				string username, type, credit_str;
				int credit;
				cout << "enter the username of the new user:" << endl;
				username = getUserInput();
				//check if the username is too long, if so print an error message, else check if username already exists, if it does then print an error message, else continue
				if(username.length() > MAX_USERNAME_LEN){
					cout << "failed: usernames must be 15 characters or less" << endl;
				} else if(accounts.find(username)!=accounts.end()){
					cout << "failed: username already exists" << endl;
				}
				else{
					cout << "enter the user's type:" << endl;
					type = getUserInput();
					//check if the type is valid, if not then print an error message, else continue
					if(!(type=="AA" || type=="FS" || type=="BS" || type=="SS")){
						cout << "failed: valid user types are AA (admin), FS (full standard), BS (buy standard), and SS (sell standard)" << endl;
					} else{
						cout << "enter the amount of credits the user will start with:" << endl;
						credit_str = getUserInput();
						//check if credit_str is a valid integer, if not print an error message, else continue
						if(!isStringValidCredit(credit_str)){
							cout << "failed: credit must be positive and entered in dollar (dot) decimal notation" << endl;
						} else{
							credit = creditStringToInt(credit_str);
							//check if credit is within bounds, if not then print error message, else continue to create new user account
							if(credit > MAX_CREDITS){
								cout << "failed: credit value too large, maximum is 999999.00" << endl;
							} else{
								accounts.insert(pair<string, User*>(username,new User(username, type, credit)));
								writeTransactionA(transactionOFS, "01", username, type, intToCreditString(credit));
							}
						}
					}
				}
			} else{
				cout << "ERROR: transaction not available" << endl;
			}
		} else if(input == "delete"){
		//***************HANDLE DELETE TRANSACTION***************
			//make sure user is an admin
			if(currentUser->get_type() == "AA"){
				cout << "enter username:" << endl;
				input = getUserInput();
				//if the user is trying to delete themselves, print an error message
				if(input == currentUser->get_username()){
					cout << "ERROR: cannot delete self" << endl;
				//if the username doesn't exist, print an error message
				}else if(accounts.find(input)==accounts.end()){
					cout << "failed: username does not exist" << endl;
				//if neither of the previous guard conditions failed, write out the delete transaction
				}else{
					writeTransactionA(transactionOFS, "02", accounts[input]->get_username(), accounts[input]->get_type(), intToCreditString(accounts[input]->get_credit()));
					delete accounts[input];
					accounts.erase(input);
				}
			} else{
				cout << "ERROR: transaction not available" << endl;
			}
		} else if(input == "addcredit"){
		//***************HANDLE ADDCREDIT TRANSACTION***************
			string credit_str, username;
			//check if user is an admin, this transaction works differently for admins and regular users
			if(currentUser->get_type() == "AA"){
			//get username for admins
				string username;
				cout << "enter the name of the user you wish to add credit to:" << endl;
				username = getUserInput();
				//if the username doesn't exist, print an error message
				if(accounts.find(username)==accounts.end()){
					cout << "failed: username does not exist" << endl;
					goto TRANSACTION_FAILED;
				}
			} else{
			//set username for non-admins (set it to equal their own username)
				username = currentUser->get_username();
			}
			//continue wiht rest of transaction regardless of admin or non-admin
			cout << "enter the amount of credit you wish to add:" << endl;
			credit_str = getUserInput();
			if(!isStringValidCredit(credit_str)){
				cout << "failed: credit must be positive and entered in dollar (dot) decimal notation" << endl;
				goto TRANSACTION_FAILED;
			}
			int amount = creditStringToInt(credit_str);
			if(accounts[username]->get_creditAdded()+amount > MAX_CREDITS_ADDED){
				cout << "failed: no more than 1000.00 credits can be added to any one user per day" << endl;
				goto TRANSACTION_FAILED;
			}
			if(accounts[username]->get_credit()+amount > MAX_CREDITS){
				cout << "failed: adding this amount would cause the user to exceed the credit limit" << endl;
				goto TRANSACTION_FAILED;
			}
			accounts[username]->add_credit(amount);
			writeTransactionA(transactionOFS, "06", username, accounts[username]->get_type(), amount);
		} else if(input == "refund"){
		//***************HANDLE REFUND TRANSACTOIN***************
			if(currentUser->get_type() != "AA"){
				cout << "ERROR: transaction not available" << endl;
				goto TRANSACTION_FAILED;
			}
			cout << "enter the buyer's username:" << endl;
			string buyerName = getUserInput();
			if(accounts.find(buyerName)==accounts.end()){
				cout << "failed: username does not exist" << endl;
				goto TRANSACTION_FAILED;
			}
			cout << "enter the seller's username:" << endl;
			string sellerName = getUserInput();
			if(accounts.find(sellerName)==accounts.end()){
				cout << "failed: username does not exist" << endl;
				goto TRANSACTION_FAILED;
			}
			cout << "enter the amount of credit you wish to refund:" << endl;
			string credit_str = getUserInput();
			if(!isStringValidCredit(credit_str)){
				cout << "failed: credit must be positive and entered in dollar (dot) decimal notation" << endl;
				goto TRANSACTION_FAILED;
			}
			int amount = creditStringToInt(credit_str);
			if(accounts[sellerName]->get_credit < amount){
				cout << "failed: seller has insufficient funds" << endl;
				goto TRANSACTION_FAILED;
			}
			if(accounts[buyerName]->get_credit()+amount > MAX_CREDITS){
				cout << "failed: adding this amount to buyer's account would cause the user to exceed the credit limit" << endl;
				goto TRANSACTION_FAILED;
			}
			accounts[sellerName]->remove_credit(amount);
			accounts[buyerName]->add_credit(amount);
			writeTransactionB(transactionOFS, "05", buyerName, sellerName, amount);
		} else if(input == "advertise"){
		//***************HANDLE ADVERTISE TRANSACTION***************
			if(currentUser->get_type() == "BS"){
				cout << "ERROR: transaction not available" << endl;
				goto TRANSACTION_FAILED;
			}
			cout << "enter the name of the item you wish to advertise:"
			string itemName = getUserInput();
			if(itemName.length() > MAX_ITEMNAME_LEN){
				cout << "failed: max item name length is 19 characters" << endl;
				goto TRANSACTION_FAILED;
			}
			string sellerName = currentUser->get_username;
			if(items[pair<string,string>(itemName, sellerName)] != items.end()){
				cout << "failed: you are already selling an item with this name" << endl;
				goto TRANSACTION_FAILED;
			}
			cout << "enter the minimum bid for the item:" << endl;
			string credit_str = getUserInput();
			if(!isStringValidCredit(credit_str)){
				cout << "failed: credit must be positive and entered in dollar (dot) decimal notation" << endl;
				goto TRANSACTION_FAILED;
			}
			int minBid = creditStringToInt(credit_str);
			if(minBid > MAX_ITEM_PRICE){
				cout << "failed: price cannot exceed 999.99" << endl;
				goto TRANSACTION_FAILED;
			}
			cout << "enter the number of days the auction will run for:" << endl;
			string numDays_str = getUserInput();
			for(int i=0; i < numDays_str.length(); i++){
				if(!isdigit(numDays_str[i])){
					cout << "failed: number of days must be positive and entered as a valid integer" << endl;
					goto TRANSACTION_FAILED;
				}
			}
			int numDays = stoi(numDays_str);
			if(numDays > MAX_AUCTION_LEN){
				cout << "failed: maximum length of an auction is 100 days" << endl;
				goto TRANSACTION_FAILED;
			}
			items.insert(pair<pair<string,string>, Item*>(pair<string,string>(itemName, sellerName), new Item(itemName, sellerName, NULL, numDays, minBid)));
			writeTransactionC(transactionOFS, "03", itemName, sellerName, numDays, intToCreditString(minBid));
		} else if(input == "bid"){
		//***************HANDLE BID TRANSACTION***************
			if(currentUser->get_type() == "SS"){
				cout << "ERROR: transaction not available" << endl;
				goto TRANSACTION_FAILED;
			}
			cout << "enter item name:" << endl;
			string itemName = getUserInput();
			cout << "enter seller name:" << endl;
			string sellerName = getUserInput();
			if(items[pair<string,string>(itemName, sellerName)] == items.end()){
				cout << "failed: item not found" << endl;
				goto TRANSACTION_FAILED;
			}
			if(sellerName == currentUser->get_username()){
				cout << "failed: cannot bid on your own item" << endl;
				goto TRANSACTION_FAILED;
			}
			Item item = items[pair<string,string>(itemName, sellerName)];
			cout << "highest bid: $" << intToCreditString(item->get_price()) << endl;
			cout << "enter your bid:" << endl;
			string bid_str = getUserInput();
			if(!isStringValidCredit(bid_str)){
				cout << "failed: bid must be positive and entered in dollar (dot) decimal notation" << endl;
				goto TRANSACTION_FAILED;
			}
			int bid = creditStringToInt(bid_str);
			if(bid > currentUser->get_credit()){
				cout << "failed: insufficient funds" << endl;
				goto TRANSACTION_FAILED;
			}
			if(currentUser->get_type() == "AA" && !(bid > item->get_price())){
				cout << "failed: your bid must be higher than highest bid" << endl;
				goto TRANSACTION_FAILED;
			}
			if(currentUser->get_type() != "AA" && !(bid >= 1.05*item->get_price())){
				cout << "failed: your bid must be at least 5% higher than highest bid" << endl;
				goto TRANSACTION_FAILED;
			}
			item.updateBid(currentUser->get_username(), bid);
			currentUser->remove_credit(bid);
			writeTransactionD(transactionOFS, "04", itemName, sellerName, currentUser->get_username(), bid);
		} else if(input != "exit"){
			cout << "invalid input" << endl;
		}
		TRANSACTION_FAILED:
	}
	transactionOFS.close();
	return 0;
}

string intToCreditString(int credit){
	return to_string(credit/100) + '.' + to_string(credit%100);
}

int creditStringToInt(string s){
	int i;
	bool has_cents = false;
	for(i=0; i < s.length(); i++){
		if(s[i] == '.'){
			has_cents = true;
			break;
		}
	}
	int dollars = stoi(s.substr(0,i));
	int cents;
	if(has_cents)
		cents = stoi(s.substr(i+1,2));
	else
		cents = 0;

	return (dollars*100)+cents;
}

bool isStringValidCredit(string s){
	int i;
	for(i=0; i < s.length(); i++){
		if(s[i] == '.') break;
		if(!isdigit(s[i])) return false; //also returns false if it sees a '-' so negative numbers will not pass
	}
	int j = i+1;
	for(j; j < s.length(); j++){
		if(!isdigit(s[j])) return false;
	}
	if(j != i+3) return false; //this just checks if the last for loop iterated twice, if it didn't iterate exactly twice that would indicate that there were not exactly 2 digits after the period as there should have been, so return false
	return true;
}

string getUserInput(){
	string input;
	getline(cin,input);
	int i;
	for(i=input.length()-1; i>0; i--){
		if(input[i] != ' ' && input[i] != '\n') break;
	}
	return input.substr(0, i+1);
}

void loadUsers(map<string, User*> &accounts, string filename){
	ifstream file(filename);
	string input;
	string username;
	string type;
	int credit_dollars, credit_cents;
	
	while(getline(file, input)){
		if(input == "END") break;
		int i;
		//GET USERNAME
		for(i=MAX_USERNAME_LEN-1; i>0; i--){
			if(input[i] != ' ') break;
		}
		username = input.substr(0, i+1);
		//GET USER TYPE
		type = input.substr(MAX_USERNAME_LEN+1, 2);
		//GET USER CREDIT
		credit_dollars = stoi(input.substr(MAX_USERNAME_LEN+4, MAX_CREDIT_DOLLAR_LEN));
		credit_cents = stoi(input.substr(MAX_USERNAME_LEN+4+MAX_CREDIT_DOLLAR_LEN+1, 2));
		//ADD USER TO ACCOUNTS MAP
		int credit = credit_dollars*100 + credit_cents;
		accounts.insert(pair<string, User*>(username,new User(username, type, credit)));
	}
	file.close();
}

void loadItems(map<pair<string,string>, Item*> &items, string filename){
	ifstream file(filename);
	string input;
	string itemName, sellerName, highestBidder;
	int daysLeft, price_dollars, price_cents;

	while(getline(file, input)){
		int i;
		int i0;
		//GET ITEMNAME
		i0 = 0;
		for(i=i0+MAX_ITEMNAME_LEN-1; i>i0; i--){
			if(input[i] != ' ') break;
		}
		itemName = input.substr(i0, i+1-i0);
		//GET SELLERNAME
		i0 += MAX_ITEMNAME_LEN+1;
		for(i=i0+MAX_USERNAME_LEN; i>i0; i--){
			if(input[i] != ' ') break;
		}
		sellerName = input.substr(i0, i+1-i0);
		//GET HIGHESTBIDDER
		i0 += MAX_USERNAME_LEN+1;
		for(i=i0+MAX_USERNAME_LEN; i>i0; i--){
			if(input[i] != ' ') break;
		}
		highestBidder = input.substr(i0, i+1-i0);
		//GET DAYSLEFT
		i0 += MAX_USERNAME_LEN+1;
		daysLeft = stoi(input.substr(i0, 3));
		//GET PRICE
		i0 += 4;
		price_dollars = stoi(input.substr(i0,3));
		i0 += 4;
		price_cents = stoi(input.substr(i0, 2));

		//ADD ITEM TO ITEMS MAP
		int price = price_dollars*100 + price_cents;
		items.insert(pair<pair<string,string>, Item*>(pair<string,string>(itemName, sellerName), new Item(itemName, sellerName, highestBidder, daysLeft, price)));
	}
	file.close();
}

void writeTransactionA(ofstream& file, string transCode, string username, string userType, string credits){
	file << transCode << ' ' << username;
	for(int i=MAX_USERNAME_LEN; i > username.length(); i--){
		file << ' ';
	}
	file << ' ' << userType << ' ';
	for(int i=MAX_CREDIT_DOLLAR_LEN+3; i > credits.length(); i--){
		file << '0';
	}
	file << credits << endl;
}

void writeTransactionB(ofstream& file, string transCode, string buyerName, string sellerName, string credits){
	file << transCode << ' ' << buyerName;
	for(int i=MAX_USERNAME_LEN; i > buyerName.length(); i--){
		file << ' ';
	}
	file << ' ' << sellerName;
	for(int i=MAX_USERNAME_LEN; i > sellerName.length(); i--){
		file << ' ';
	}
	file << ' ';
	for(int i=MAX_CREDIT_DOLLAR_LEN+3; i > credits.length(); i--){
		file << '0';
	}
	file << credits << endl;
}

void writeTransactionC(ofstream& file, string transCode, string itemName, string sellerName, string daysLeft, string price){
	file << transCode << ' ' << itemName;
	for(int i=MAX_ITEMNAME_LEN; i > itemName.length(); i--){
		file << ' ';
	}
	file << ' ' << sellerName;
	for(int i=MAX_USERNAME_LEN; i > sellerName.length(); i--){
		file << ' ';
	}
	file << ' ';
	for(int i=3; i > daysLeft.length(); i--){
		file << '0';
	}
	file << daysLeft << ' ';
	for(int i=6; i > price.length(); i--){
		file << '0';
	}
	file << price << endl;
}

void writeTransactionD(ofstream& file, string transCode, string itemName, string sellerName, string bidderName, string bid){
	file << transCode << ' ' << itemName;
	for(int i=MAX_ITEMNAME_LEN; i > itemName.length(); i--){
		file << ' ';
	}
	file << ' ' << sellerName;
	for(int i=MAX_USERNAME_LEN; i > sellerName.length(); i--){
		file << ' ';
	}
	file << ' ' << bidderName;
	for(int i=MAX_USERNAME_LEN; i > bidderName.length(); i--){
		file << ' ';
	}
	file << ' ';
	for(int i=6; i > bid.length(); i--){
		file << '0';
	}
	file << bid << endl;
}


	/*
	//DEBUGGING ACCOUNTS MAP
	cout << accounts.size() << endl;
	map<string, User*>::iterator it = accounts.begin();
	while(it != accounts.end()){
		cout << "element's key: " << it->first << endl;
		it++;
	}
	cout << accounts["adminuser1"]->get_username() << ' ' << accounts["adminuser1"]->get_type() << ' ' << accounts["adminuser1"]->get_credit() << endl;

	//DEBUGGING ITEMS MAP
	cout << items.size() << endl;
	map<pair<string,string>,Item*>::iterator it = items.begin();
	while(it != items.end()){
		cout << "element's key: " << it->first.first << it->first.second << endl;
		it++;
	}
	cout << items[pair<string,string>("item","adminuser1")]->get_itemName() << ' ' << items[pair<string,string>("item","adminuser1")]->get_sellerName() << ' ' << items[pair<string,string>("item","adminuser1")]->get_highestBidder() << ' ' << items[pair<string,string>("item","adminuser1")]->get_daysLeft() << ' ' << items[pair<string,string>("item","adminuser1")]->get_price() << endl;
	*/