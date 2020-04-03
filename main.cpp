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

using namespace std;

string getUserInput();
void loadUsers(map<string, User*>&, string);
void loadItems(map<pair<string,string>, Item*>&, string);

int main(int argc, char** argv){
	//load data from accounts file
	map<string, User*> accounts;
	loadUsers(accounts, argv[1]);
	//load data form items file
	map<pair<string,string>, Item*> items;
	loadItems(items, argv[2]);

	string input;
	User* currentUser = NULL;

	while(true){
		cout << (currentUser != NULL ? "("+currentUser->get_username()+") " : "")  << "enter transaction: " << endl;
		input = getUserInput();
		if(input == "exit"){
			break;
		} else if(input == "login"){
			cout << "enter username:" << endl;
			input = getUserInput();
			if(accounts.find(input)!=accounts.end()){
				currentUser = accounts[input];
			} else{
				cout << "ERROR: account not found" << endl;
			}
		} else if(input == "logout"){
			if(currentUser != NULL){
				currentUser = NULL;
				cout << "logout successful" << endl;
			} else{
				cout << "ERROR: must be logged in" << endl;
			}
		} else if(input == "create"){
			
		} else if(input == "delete"){
			
		} else if(input == "addcredit"){
			
		} else if(input == "refund"){
			
		} else if(input == "advertise"){
			
		} else if(input == "bid"){
			
		} else if(input != "exit"){
			cout << "invalid input" << endl;
		}
	}

	return 0;
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
		cout << input << endl;
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
	/*