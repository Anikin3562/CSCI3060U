#include <iostream>
#include <fstream> 
#include <vector> 

using namespace std; 

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
int main(){

	ifstream infile;
	infile.open("accounts.txt");

	string line; 
	vector<string> accounts; 

	while(getline(infile, line)){
		if(line == "END") break; 

		accounts.push_back(line);
	}

	string name;
	string accountType; 
	float credits; 

	for(auto &account : accounts){
		cout << account << endl; 
		cout << account.length() << endl; 

		name = account.substr(0, 15);
		accountType = account.substr(16, 2);
		credits = stof(account.substr(19, 9));
		
		cout << removeSpaces(name) << endl;
		cout << removeSpaces(accountType) << endl;
		cout << credits << endl; 
	}


	infile.close();
}