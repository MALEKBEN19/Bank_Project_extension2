#include <iostream>
#include<string>
#include<fstream>
#include<vector>
#include<iomanip>
using namespace std;
void ShowMainMenu(short);
void ShowTransactionsMenu(short);
void ShowManageUsersMenu(short);
void Login();
const string ClientFile = "ClientsList.txt";
const string UsersFile = "Users.txt";

enum enMainMenuOptions { ShowClient = 1, AddClient = 2, DeleteClient = 3, UpdateClient = 4, FindClient = 5, Transactions=6,Manage_Users=7,Logout = 8 };
enum enTransactionsMenuOptions { Deposit = 1, Withdraw = 2, TotalBalances = 3, ReturnToMainMenu = 4};
enum enManageUsersMenu{ListUsers=1,AddNewUser=2,DeleteUser=3,UpdateUser=4,FindUser=5,GoBack=6};


struct stClients {
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance = 0;
	bool MarkClient = false;
};

struct stUsers {
	string UserName;
	string Password;
	short Parmissions;
	short MarkUser = false;
};



//show client screen :
vector<string>SplitString(string Txt, string Delim) {
	vector<string>vWords;
	short Pos = 0;
	string word = "";
	while ((Pos = Txt.find(Delim)) != std::string::npos) {
		word = Txt.substr(0, Pos);
		if (word != "") {
			vWords.push_back(word);
		}
		Txt.erase(0, Pos + Delim.length());
	}
	if (Txt != "") {
		vWords.push_back(Txt);
	}
	return vWords;
}
stClients ConvertLineToRecord(string Line, string sperator = "#//#") {
	vector<string>vLine = SplitString(Line, sperator);
	stClients client;
	client.AccountNumber = vLine.at(0);
	client.PinCode = vLine.at(1);
	client.Name = vLine.at(2);
	client.Phone = vLine.at(3);
	client.AccountBalance = stod(vLine.at(4));

	return client;
}
vector<stClients> LoadClientsDataFromFile(string file) {
	vector<stClients>vClients;
	fstream Myfile;
	stClients client;
	Myfile.open(file, ios::in);
	if (Myfile.is_open()) {
		string Line;
		while (getline(Myfile, Line)) {
			client = ConvertLineToRecord(Line);
			vClients.push_back(client);
		}
		Myfile.close();
	}
	return vClients;
}

void PrintClientForShowClientScreen(stClients& clt) {
	cout << "| " << left << setw(15) << clt.AccountNumber;
	cout << "| " << left << setw(12) << clt.PinCode;
	cout << "| " << left << setw(30) << clt.Name;
	cout << "| " << left << setw(15) << clt.Phone;
	cout << "| " << left << setw(13) << to_string(clt.AccountBalance);
}
void ShowClientListScreen() {
	vector<stClients>vClient = LoadClientsDataFromFile(ClientFile);
	cout << "\n\t\t\t\t Client List " << vClient.size() << " Client(s)\n";
	cout << "\n______________________________________________________________________________________________________\n\n";
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(12) << "Pin Code";
	cout << "| " << left << setw(30) << "           Name";
	cout << "| " << left << setw(15) << "Phone";
	cout << "| " << left << setw(13) << "Account Balance";
	cout << "\n______________________________________________________________________________________________________\n\n";

	if (vClient.size() == 0) {
		cout << "\n\t\t\t\t No Client Available In The System!";
	}
	else

		for (stClients& clt : vClient) {
			PrintClientForShowClientScreen(clt);
			cout << endl;
		}
	cout << "\n\n______________________________________________________________________________________________________\n";

}


//Add client screen :
bool IsAccountNumberExist(string AccountNumber, string file) {
	vector<stClients>vClient;
	fstream Myfile;
	Myfile.open(file, ios::in);
	if (Myfile.is_open()) {
		string line;
		stClients client;
		while (getline(Myfile, line)) {
			client = ConvertLineToRecord(line);
			if (client.AccountNumber == AccountNumber) {
				Myfile.close();
				return true;
			}
			vClient.push_back(client);
		}
		Myfile.close();
	}
	return false;
}
stClients GetClientInfo() {
	stClients  client;
	cout << "Enter Account Number? ";
	getline(cin >> ws, client.AccountNumber);
	while (IsAccountNumberExist(client.AccountNumber, ClientFile)) {
		cout << "\nClient with [" << client.AccountNumber << "] already exists, Enter another Account Number ";
		getline(cin >> ws, client.AccountNumber);
	}
	cout << "Enter Pin Code : ";
	getline(cin >> ws, client.PinCode);

	cout << "Enter Name : ";
	getline(cin >> ws, client.Name);

	cout << "Enter Phone : ";
	getline(cin >> ws, client.Phone);

	cout << "Enter Account Balance : ";
	cin >> client.AccountBalance;

	return client;

}
string ConvertRecordToLine(stClients& clt, string sperator = "#//#") {
	string Line;
	Line += clt.AccountNumber + sperator;
	Line += clt.PinCode + sperator;
	Line += clt.Name + sperator;
	Line += clt.Phone + sperator;
	Line += to_string(clt.AccountBalance);
	return Line;
}
void AddDataLineToFile(string file, string line,bool NewLineBeforOrAfter=false) {
	fstream Myfile;
	Myfile.open(file, ios::out | ios::app);
	if (Myfile.is_open()) {
		if (NewLineBeforOrAfter) {
			Myfile << line << endl;
		}
		else {
			Myfile << endl << line ;
		}
		Myfile.close();
	}
}
void AddOneClient() {
	stClients client;
	client = GetClientInfo();
	AddDataLineToFile(ClientFile, ConvertRecordToLine(client),true);
}
void AddClientsScreen() {
	char AddMore = 'Y';

	do {
		system("cls");
		cout << "\n________________________________________________\n\n";
		cout << "\t\t Add Client Screen.\n";
		cout << "________________________________________________\n\n";
		cout << "Adding Client :\n\n";
		AddOneClient();
		cout << "\nClient Added Successfully, do you want to Add more? (Y/N) ";
		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');
}

//Delete client screen :
string AccountNumber() {
	cout << "Please enter Account Number? ";
	string AccountNum = "";
	cin >> AccountNum;
	return AccountNum;
}
void PrintClientCardForEdit(stClients clt) {
	cout << "\nThe Following are Client Details : \n";
	cout << "--------------------------------------\n";
	cout << "\nAccount Number : " << clt.AccountNumber << endl;
	cout << "Pin Code : " << clt.PinCode << endl;
	cout << "Name : " << clt.Name << endl;
	cout << "Phone : " << clt.Phone << endl;
	cout << "Account Balance : " << clt.AccountBalance << endl;
	cout << "\n--------------------------------------\n";
}
bool IsAccountNumberInVector(string AccountNum, vector<stClients>& vClient, stClients& client) {
	for (stClients& clt : vClient) {
		if (AccountNum == clt.AccountNumber) {
			client = clt;
			return true;
		}
	}
	return false;
}
bool MarkAccountForDeleteByAccountNumber(string AccountNum, vector<stClients>& vClient) {
	for (stClients& clt : vClient) {
		if (AccountNum == clt.AccountNumber) {
			clt.MarkClient = true;
			return true;
		}
	}
	return false;
}
void SaveClientDataInFileAgain(vector<stClients>& vClient, string file) {
	fstream Myfile;
	Myfile.open(file, ios::out);
	if (Myfile.is_open()) {
		string Line;
		for (stClients& clt : vClient) {
			if (clt.MarkClient == false) {
				Line = ConvertRecordToLine(clt);
				Myfile << Line << endl;
			}

		}
		Myfile.close();
	}


}
void DeleteAccountByAccountNumber(string ReadAccount, vector<stClients>& vClient) {
	stClients client;

	if (IsAccountNumberInVector(ReadAccount, vClient, client)) {
		PrintClientCardForEdit(client);
		char Delete = 'Y';
		cout << "\nAre you sure you want to delete this Account? (Y/N) ";
		cin >> Delete;
		if (toupper(Delete) == 'Y') {
			MarkAccountForDeleteByAccountNumber(ReadAccount, vClient);
			SaveClientDataInFileAgain(vClient, ClientFile);
			vClient = LoadClientsDataFromFile(ClientFile);
			cout << "\nClient Deleted Successfully.";

		}

	}
	else {
		cout << "\nThe Account with N: " << ReadAccount << " Doesn't Exist! ";
	}
}
void DeleteClientScreen() {
	cout << "\n________________________________________________\n\n";
	cout << "\t\t Delete Client Screen.\n";
	cout << "_________________________________________________\n\n";
	vector<stClients>vClient = LoadClientsDataFromFile(ClientFile);
	string ReadAccount = AccountNumber();
	DeleteAccountByAccountNumber(ReadAccount, vClient);
}


//Update client screen :

stClients MakeAnUpdateOnAccount(string AccountNum) {
	stClients client;
	client.AccountNumber = AccountNum;
	cout << "\nEnter Pin Code : ";
	getline(cin >> ws, client.PinCode);
	cout << "Enter Name : ";
	getline(cin >> ws, client.Name);
	cout << "Enter Phone : ";
	getline(cin >> ws, client.Phone);
	cout << "Enter Account Balance : ";
	cin >> client.AccountBalance;
	return client;
}

bool UpdateClientByAccountNumber(string AcctNum, vector < stClients >& vClient) {
	stClients client;
	if (IsAccountNumberInVector(AcctNum, vClient, client)) {
		PrintClientCardForEdit(client);
		char Update = 'Y';
		cout << "\nAre you sure you want to Update this account? (Y/N) ";
		cin >> Update;
		if (toupper(Update) == 'Y') {
			for (stClients& clt : vClient) {
				if (clt.AccountNumber == AcctNum) {
					clt = MakeAnUpdateOnAccount(AcctNum);
				}
			}

			SaveClientDataInFileAgain(vClient, ClientFile);
			vClient = LoadClientsDataFromFile(ClientFile);
			cout << "\nClient Updated Successfully.\n";
			return true;
		}
	}
	else {
		cout << "\nThe Account with N: " << AcctNum << " Not Found!\n";
	}
	return false;
}
void UpdateClientScreen() {
	cout << "\n________________________________________________\n\n";
	cout << "\t\t Update Client Screen.\n";
	cout << "_________________________________________________\n\n";
	vector<stClients>vClient = LoadClientsDataFromFile(ClientFile);
	string ReadAccount = AccountNumber();
	UpdateClientByAccountNumber(ReadAccount, vClient);
}

//Find client screen :

bool FindClientByAccountNumber(string AcctNum, vector < stClients >& vClient) {

	stClients client;
	if (IsAccountNumberInVector(AcctNum, vClient, client)) {
		PrintClientCardForEdit(client);
		return true;
	}
	else {
		cout << "\nThe Account with N: " << AcctNum << " Not Found!\n";
	}
	return false;
}
void FindClientScreen() {
	cout << "\n________________________________________________\n\n";
	cout << "\t\t Find Client Screen.\n";
	cout << "_________________________________________________\n\n";
	vector<stClients>vClient = LoadClientsDataFromFile(ClientFile);
	string ReadAccount = AccountNumber();
	FindClientByAccountNumber(ReadAccount, vClient);
}


// Transactions part : 

double AskUserToEnterAmount() {
	double Amount=0;
	cout << "\n\nPlease enter amount? ";
	cin >> Amount;
	return Amount;
}

// Deposit : 

bool DepositBalanceToClientByAccountNumber(string AccountNumber,double Amount,vector<stClients>&vClient) {
	char answer = 'Y';
	cout << "\nAre you sure you want to performe this transactions? (Y/N) ";
	cin >> answer;
	if (toupper(answer) == 'Y') {
		for (stClients& clt : vClient) {
			if (clt.AccountNumber == AccountNumber) {
				clt.AccountBalance += Amount;
				SaveClientDataInFileAgain(vClient, ClientFile);
				cout << "\nDone Successfully, New Balance is " << clt.AccountBalance << endl;
				return true;
			}
	
		}
	}
	return false;
}
void DepositAmountScreen() {
	cout << "\n________________________________________________\n\n";
	cout << "\t\t Deposit Screen.\n";
	cout << "_________________________________________________\n\n";
	vector<stClients>vClient = LoadClientsDataFromFile(ClientFile);
	string ReadAccount = AccountNumber();
	stClients client;
	while (!IsAccountNumberInVector(ReadAccount, vClient, client)) {
		cout << "Client with Number : " << ReadAccount << " Does Not Exist.\n ";
		ReadAccount = AccountNumber();
	}
	PrintClientCardForEdit(client);
	double Amount = 0;
	cout << "\nPlease enter deposit amount? ";
	cin >> Amount;

	DepositBalanceToClientByAccountNumber(ReadAccount,Amount,vClient);
}


// withdraw : 

void WithdrawAmountScreen() {
	cout << "\n________________________________________________\n\n";
	cout << "\t\t Withdraw Screen.\n";
	cout << "_________________________________________________\n\n";
	vector<stClients>vClient = LoadClientsDataFromFile(ClientFile);
	string ReadAccount = AccountNumber();
	stClients client;
	while (!IsAccountNumberInVector(ReadAccount, vClient, client)) {
		cout << "Client with Number : " << ReadAccount << " Does Not Exist.\n ";
		ReadAccount = AccountNumber();
	}
	PrintClientCardForEdit(client);
	double Amount = 0;
	cout << "\nPlease enter withdraw amount? ";
	cin >> Amount;
	while (Amount > client.AccountBalance) {
		cout << "Amount Exceeds The Balance, You Can Withdraw Up To : " << client.AccountBalance << endl;
		cout << "\nPlease enter another amount? ";
		cin >> Amount;
	}
	DepositBalanceToClientByAccountNumber(ReadAccount, Amount * -1, vClient);
}



// show Balances screen : 

void PrintClientForBalanceScreen(stClients& clt) {
	cout << "| " << left << setw(22) << clt.AccountNumber;
	cout << "| " << left << setw(35) << clt.Name;
	cout << "| " << left << setw(22) << to_string(clt.AccountBalance);
}
void ShowBalancesListScreen() {
	vector<stClients>vClient = LoadClientsDataFromFile(ClientFile);
	double TotalBalances = 0;
	cout << "\n\t\t\t\t Balances List " << vClient.size() << " Client(s)\n";
	cout << "\n______________________________________________________________________________________________________\n\n";
	cout << "| " << left << setw(22) << "Account Number";
	cout << "| " << left << setw(35) << "Name";
	cout << "| " << left << setw(22) << "Account Balance";
	cout << "\n______________________________________________________________________________________________________\n\n";

	if (vClient.size() == 0) {
		cout << "\n\t\t\t\t No Client Available In The System!";
	}
	else

		for (stClients& clt : vClient) {
			PrintClientForBalanceScreen(clt);
			TotalBalances += clt.AccountBalance;
			cout << endl;
		}
	cout << "\n\n______________________________________________________________________________________________________\n";
	cout << "\n\t\t\t\t\t Total Balances = " << TotalBalances << endl;
}


//Transaction Operations : 

void GoBackToTransactionsMenu(short Permissions) {
	cout << "\n\nPress any Key to go back to Tansactions Menu...";
	system("pause>0");
	ShowTransactionsMenu(Permissions);
}
short ReadTransactionsMenuOptions() {
	short Choice = 0;
	cout << "Choose what do you want to do? [1-4] ";
	cin >> Choice;
	return Choice;
}
void TransactionsMenu() {
	cout << "================================================\n";
	cout << "\t Transactions Menu Screen\n";
	cout << "================================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] Total Balances.\n";
	cout << "\t[4] Main Menu.\n";
	cout << "================================================\n";
}
void PerformTransactionsMenuOptions(enTransactionsMenuOptions TransactionsMenuOptions,short Permission) {
	switch (TransactionsMenuOptions) {
	case enTransactionsMenuOptions::Deposit: {
		system("cls");
		DepositAmountScreen();
		GoBackToTransactionsMenu(Permission);
		break;
	}
	case enTransactionsMenuOptions::Withdraw: {
		system("cls");
		WithdrawAmountScreen();
		GoBackToTransactionsMenu(Permission);
		break;
	}
	case enTransactionsMenuOptions::TotalBalances: {
		system("cls");
		ShowBalancesListScreen();
		GoBackToTransactionsMenu(Permission);
		break;
	}
	case enTransactionsMenuOptions::ReturnToMainMenu: {
		ShowMainMenu(Permission);
		break;
	}

	}

}
void ShowTransactionsMenu(short Permission) {
	system("cls");
	TransactionsMenu();
	PerformTransactionsMenuOptions((enTransactionsMenuOptions)ReadTransactionsMenuOptions(), Permission);
}







//Manage Users : 
stUsers ConvertLineToRecordUser(string Line, string sperator = "#//#") {
	vector<string>vLine = SplitString(Line, sperator);
	stUsers User;
	User.UserName = vLine.at(0);
	User.Password = vLine.at(1);
	User.Parmissions = stoi(vLine.at(2));
	return User;
}

//Users List
vector<stUsers> LoadUsersDataFromFile(string file) {
	vector<stUsers>vUsers;
	fstream Myfile;
	stUsers User;
	Myfile.open(file, ios::in);
	if (Myfile.is_open()) {
		string Line;
		while (getline(Myfile, Line)) {
			User = ConvertLineToRecordUser(Line);
			vUsers.push_back(User);
		}
		Myfile.close();
	}
	return vUsers;
}
void PrintUsersForUsersList(stUsers &User) {
	cout << "| " << left << setw(35) << User.UserName;
	cout << "| " << left << setw(35) << User.Password;
	cout << "| " << left << setw(15) << User.Parmissions;
	cout << right << setw(12) << "| ";
	cout << endl;
}
void ShowUsersList() {
	vector<stUsers> vUsers = LoadUsersDataFromFile(UsersFile);
	cout << "\n\t\t\t\t\t Users List " << vUsers.size() << " User(s)\n";
	cout << "______________________________________________________________________________________________________\n\n";
	cout << "| " << left << setw(35) << "User Names";
	cout << "| " << left << setw(35) << "Passwords";
	cout << "| " << left << setw(15) << "Permissions";
	cout <<  right << setw(12) << "| ";
	cout << "\n______________________________________________________________________________________________________\n\n";
	for (stUsers& Usr : vUsers) {
		PrintUsersForUsersList(Usr);
	}
	cout << "______________________________________________________________________________________________________";
}


// Add User

bool IsUserExistInFile(string UserName,string file) {
	fstream Myfile;
	Myfile.open(file, ios::in);
	if (Myfile.is_open()) {
		string line;
		stUsers User;
		while (getline(Myfile, line)) {
			User = ConvertLineToRecordUser(line);
			if (User.UserName == UserName) {
				Myfile.close();
				return true;
			}

		}
		Myfile.close();
	}
	return false;
}

short GivePermissionsToUser() {
	char AskUser = 'n';
	short Parmission = 0;
	short Admin = -1;
	cout << "\nDo You want To give it full access? Y/N ";
	cin >> AskUser;
	if (toupper(AskUser) == 'Y') {
		return Admin;
	}
	cout << "\nDo You want to give access to : \n";
	cout << "Show Client List Y/N : ";

	cin >> AskUser;
	if (toupper(AskUser) == 'Y') {
		Parmission |= 1;
	}
	cout << "Add Client Y/N : ";
	cin >> AskUser;
	if (toupper(AskUser) == 'Y') {
		Parmission |=  2;
	}
	cout << "Delete Client Y/N : ";
	cin >> AskUser;
	if (toupper(AskUser) == 'Y') {
		Parmission |= 4;
	}
	cout << "Update Client Y/N : ";
	cin >> AskUser;
	if (toupper(AskUser) == 'Y') {
		Parmission |= 8;
	}
	cout << "Find Client Y/N : ";
	cin >> AskUser;
	if (toupper(AskUser) == 'Y') {
		Parmission |= 16;
	}
	cout << "Transactions Y/N : ";
	cin >> AskUser;
	if (toupper(AskUser) == 'Y') {
		Parmission |= 32;
	}
	cout << "Manage Users Y/N : ";
	cin >> AskUser;
	if (toupper(AskUser) == 'Y') {
		Parmission |= 64;
	}
	return Parmission;

}

stUsers GetUserInfo() {
	stUsers User;
	cout << "Enter User Name : ";
	getline(cin >> ws, User.UserName);
	while (IsUserExistInFile(User.UserName, UsersFile)) {
		cout << "User [" << User.UserName << "] Exist Before Try another? \n";
		cout << "Enter User Name : ";
		getline(cin >> ws, User.UserName);
	}
	cout << "Enter Password : ";
	getline(cin >> ws, User.Password);
	User.Parmissions = GivePermissionsToUser();
	return User;
}

string ConvertRecordUserToLine(stUsers& User, string sperator = "#//#") {
	string Line;
	Line += User.UserName + sperator;
	Line += User.Password + sperator;
	Line += to_string(User.Parmissions);
	return Line;
}

void AddUser() {
	stUsers user;
	user = GetUserInfo();
	AddDataLineToFile(UsersFile, ConvertRecordUserToLine(user),true);
}

void AddUsersScreen() {
	char AddMore = 'Y';
	do {
		system("cls");
		cout << "\n________________________________________________\n\n";
		cout << "\t\t Add Users Screen.\n";
		cout << "________________________________________________\n\n";
		cout << "Adding New User :\n\n";
		AddUser();
		cout << "\nUser Added Successfully, do you want to Add more? (Y/N) ";
		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');
}

// Delete user :

string ReadUserName() {
	cout << "Please enter User Name? ";
	string UserName = "";
	cin >> UserName;
	return UserName;
}

void PrintUserCardForEdit(stUsers Usr) {
	cout << "\nThe Following are User Details : \n";
	cout << "--------------------------------------\n";
	cout << "\nUser Name : " << Usr.UserName<< endl;
	cout << "Password: " << Usr.Password << endl;
	cout << "Parmission : " << Usr.Parmissions << endl;
	cout << "\n--------------------------------------\n";
}

bool IsUserNameInVector(string UserName, vector<stUsers>& vUsers, stUsers& user) {
	for (stUsers& Usr : vUsers) {
		if (UserName == Usr.UserName) {
			user = Usr;
			return true;
		}
	}
	return false;
}

bool MarkAccountForDeleteByUserName(string UserName, vector<stUsers>& vUsers) {
	for (stUsers& Usr : vUsers) {
		if (UserName == Usr.UserName) {
			Usr.MarkUser = true;
			return true;
		}
	}
	return false;
}

void SaveUserDataInFileAgain(vector<stUsers>& vUser, string file) {
	fstream Myfile;
	Myfile.open(file, ios::out);
	if (Myfile.is_open()) {
		string Line;
		for (stUsers& Usr : vUser) {
			if (Usr.MarkUser == false) {
				Line = ConvertRecordUserToLine(Usr);
				Myfile << Line << endl;
			}
		}
		Myfile.close();
	}
}

void DeleteUsertByUserName(string ReadUserName, vector<stUsers>& vUsers) {
	stUsers Usr;
	if (ReadUserName=="Admin") {
		cout << "\nYou can not delete an admin!\n";
	}
	else if (IsUserNameInVector(ReadUserName, vUsers, Usr)) {
		PrintUserCardForEdit(Usr);
		char Delete = 'Y';
		cout << "\nAre you sure you want to delete this User? (Y/N) ";
		cin >> Delete;
		if (toupper(Delete) == 'Y') {
			MarkAccountForDeleteByUserName(ReadUserName, vUsers);
			SaveUserDataInFileAgain(vUsers, UsersFile);
			vUsers = LoadUsersDataFromFile(UsersFile);
			cout << "\nUser Deleted Successfully.";

		}

	}
	else {
		cout << "\nThe Account with N: " << ReadUserName << " Doesn't Exist! ";
	}
}

void DeleteUsersScreen() {
	cout << "\n________________________________________________\n\n";
	cout << "\t\t Delete User Screen.\n";
	cout << "_________________________________________________\n\n";
	vector<stUsers>vUsers = LoadUsersDataFromFile(UsersFile);
	string ReadAccount = ReadUserName();
	DeleteUsertByUserName(ReadAccount, vUsers);
}

//Update User : 
stUsers MakeAnUpdateOnUser(string UserName) {
	stUsers Usr;
	Usr.UserName = UserName;
	cout << "Enter Password : ";
	getline(cin >> ws, Usr.Password);
	Usr.Parmissions = GivePermissionsToUser();
	return Usr;
}
bool UpdateUserByUserName(string UserName, vector < stUsers >& vUsers) {
	stUsers user;
	if (UserName == "Admin") {
		cout << "\nYou can not Update an admin!\n";
	}
	else if (IsUserNameInVector(UserName, vUsers, user)) {
		PrintUserCardForEdit(user);
		char Update = 'Y';
		cout << "\nAre you sure you want to Update this User? (Y/N) ";
		cin >> Update;
		if (toupper(Update) == 'Y') {
			for (stUsers& Usr : vUsers) {
				if ( Usr.UserName== UserName) {
					Usr = MakeAnUpdateOnUser(UserName);
				}
			}
			SaveUserDataInFileAgain(vUsers, UsersFile);
			vUsers = LoadUsersDataFromFile(UsersFile);
			cout << "\nUser Updated Successfully.\n";
			return true;
		}
	}
	else {
		cout << "\nThe User : " << UserName << " Not Found!\n";
	}
	return false;
}
void UpdateUserScreen() {
	cout << "\n________________________________________________\n\n";
	cout << "\t\t Update User Screen.\n";
	cout << "_________________________________________________\n\n";
	vector<stUsers>vUsers = LoadUsersDataFromFile(UsersFile);
	string ReadUser = ReadUserName();
	UpdateUserByUserName(ReadUser, vUsers);
}

//Find User : 

bool FindUserByUserName(string UserName, vector < stUsers >& vUsers) {

	stUsers User;
	if (IsUserNameInVector(UserName, vUsers, User)) {
		PrintUserCardForEdit(User);
		return true;
	}
	else {
		cout << "\nThe User  " << UserName << " Not Found!\n";
	}
	return false;
}
void FindUserScreen() {
	cout << "\n________________________________________________\n\n";
	cout << "\t\t Find User Screen.\n";
	cout << "_________________________________________________\n\n";
	vector<stUsers>vUsers = LoadUsersDataFromFile(UsersFile);
	string UserName = ReadUserName();
	FindUserByUserName(UserName, vUsers);
}

//Perform ManageUsr MenuOptions : 
void GoBackToManageUsersMenu(short Permissions) {
	cout << "\n\nPress any Key to go back to Main Menu...";
	system("pause>0");
	ShowManageUsersMenu(Permissions);
}

void PerformManageUsersMenuOptions(enManageUsersMenu ManageUsersMenu,short Permissions) {
	switch (ManageUsersMenu) {
	case enManageUsersMenu::ListUsers:
		system("cls");
		ShowUsersList();
		GoBackToManageUsersMenu(Permissions);
		break;
	case enManageUsersMenu::AddNewUser:
		system("cls");
		AddUsersScreen();
		GoBackToManageUsersMenu(Permissions);
		break;
	case enManageUsersMenu::DeleteUser:
		system("cls");
		DeleteUsersScreen();
		GoBackToManageUsersMenu(Permissions);
		break;
	case enManageUsersMenu::UpdateUser:
		system("cls");
		UpdateUserScreen();
		GoBackToManageUsersMenu(Permissions);
		break;
	case enManageUsersMenu::FindUser:

		system("cls");
		FindUserScreen();
		GoBackToManageUsersMenu(Permissions);
		break;
	case enManageUsersMenu::GoBack:
		system("cls");
		ShowMainMenu(Permissions);
		break;
	}

}

short ReadManageUsersMenuOptions() {
	short Choice = 0;
	cout << "Choose what do you want to do? [1-6] ";
	cin >> Choice;
	return Choice;
}

void ManageUsersMenu() {
	cout << "================================================\n";
	cout << "\t\t Manage Users Menu Screen\n";
	cout << "================================================\n";
	cout << "\t[1] List Users.\n";
	cout << "\t[2] Add New User.\n";
	cout << "\t[3] Delete User.\n";
	cout << "\t[4] Update User.\n";
	cout << "\t[5] Find User.\n";
	cout << "\t[6] Main Menu.\n";
	cout << "================================================\n";
}

void ShowManageUsersMenu(short Permissions) {
	system("cls");
	ManageUsersMenu();
	PerformManageUsersMenuOptions((enManageUsersMenu)ReadManageUsersMenuOptions(),Permissions);
}



//Login screen :

short LoadParmissionFromFile(string User,string File) {
	fstream Myfile;
	vector<stUsers>vUsers = LoadUsersDataFromFile(UsersFile);
	for (stUsers& usr : vUsers) {
		if (User == usr.UserName) {
			return usr.Parmissions;
		}
	}
	return -1;
}

bool IsUserExistInFile(string UserName,string Password, string file) {
	fstream Myfile;
	Myfile.open(file, ios::in);
	if (Myfile.is_open()) {
		string line;
		stUsers User;
		while (getline(Myfile, line)) {
			User = ConvertLineToRecordUser(line);
			if (User.UserName == UserName&&User.Password==Password) {
				Myfile.close();
				return true;
			}
			
		}
		Myfile.close();
	}
	return false;
}
stUsers ReadUserLoginScreen() {
	stUsers User;
	cout << "Enter UserName? ";
	getline(cin >> ws, User.UserName);
	cout << "Enter Password? ";
	getline(cin >> ws, User.Password);
	return User;
}
void LoginHeader() {
	cout << "\n________________________________________________\n\n";
	cout << "\t\t Login Screen.\n";
	cout << "________________________________________________\n\n";
}
stUsers ShowLoginScreen() {
	stUsers User;
	system("cls");
	LoginHeader();
	User = ReadUserLoginScreen();
	while (!IsUserExistInFile(User.UserName,User.Password, UsersFile)) {
		system("cls");
		LoginHeader();
		cout << "Invalid Username/password\n";
		User = ReadUserLoginScreen();
	}
	User.Parmissions = LoadParmissionFromFile(User.UserName,UsersFile);
	return User;
}

// Lunch the system : 

void ShowAccessDeniedToUser() {
	cout << "\n________________________________________________\n\n";
	cout << "\t\tAccess Denied,\n\    You don't Have permission to do this,\n \t  Please Contact Your Admin!\n";
	cout << "_________________________________________________\n\n";
}


void GoBackToMainMenu(short Permissions) {
	cout << "\n\nPress any Key to go back to Main Menu...";
	system("pause>0");
	ShowMainMenu(Permissions);
}

short ReadMainMenuOptions() {
	short Choice = 0;
	cout << "Choose what do you want to do? [1-8] ";
	cin >> Choice;
	return Choice;
}

void PerformMainMenuOptions(enMainMenuOptions MainMenuOptions,short Permissions) {
	switch (MainMenuOptions) {
	case enMainMenuOptions::ShowClient:
	{

		if (Permissions == -1 || (Permissions & 1)) {
			system("cls");
			ShowClientListScreen();
			GoBackToMainMenu(Permissions);
			break;
		}
		else {

			system("cls");
			ShowAccessDeniedToUser();
			GoBackToMainMenu(Permissions);
			break;
		}
	}
	case enMainMenuOptions::AddClient:
	{
		if (Permissions==-1 || (Permissions & 2)) {
			system("cls");
			AddClientsScreen();
			GoBackToMainMenu(Permissions);
			break;
		}
		else {
			system("cls");
			ShowAccessDeniedToUser();
			GoBackToMainMenu(Permissions);
			break;
		}
		
		
	}
	case enMainMenuOptions::DeleteClient:
	{
		
		if (Permissions == -1 || (Permissions & 4)) {
			system("cls");
			DeleteClientScreen();
			GoBackToMainMenu(Permissions);
			break;
		}
		else {
			system("cls");
			ShowAccessDeniedToUser();
			GoBackToMainMenu(Permissions);
			break;
			}
	}




	case enMainMenuOptions::UpdateClient:
	{
		if (Permissions == -1 || (Permissions & 8)) {
			system("cls");
			UpdateClientScreen();
			GoBackToMainMenu(Permissions);
			break;
		}
		else {
			system("cls");
			ShowAccessDeniedToUser();
			GoBackToMainMenu(Permissions);
			break;
			}

	}
	case enMainMenuOptions::FindClient:
	{
		if (Permissions == -1 || (Permissions & 16)) {
			system("cls");
			FindClientScreen();
			GoBackToMainMenu(Permissions);
			break;
		}
		else 
		{
			system("cls");
			ShowAccessDeniedToUser();
			GoBackToMainMenu(Permissions);
			break;
		}
	}case enMainMenuOptions::Transactions:
	{
		if (Permissions == -1 || (Permissions & 32))
		{
			system("cls");
			ShowTransactionsMenu(Permissions);
			break;
		}
		else
		{
			system("cls");
			ShowAccessDeniedToUser();
			GoBackToMainMenu(Permissions);
			break;
		}
	}
	case enMainMenuOptions::Manage_Users:
	{
		if (Permissions == -1 || (Permissions & 64)) {
			system("cls");
			ShowManageUsersMenu(Permissions);
			break;
		}
		else
		{
			system("cls");
			ShowAccessDeniedToUser();
			GoBackToMainMenu(Permissions);
			break;
		}
		



	}
	case enMainMenuOptions::Logout:
	{
		break;
	}

	}

}

void MainMenu() {
	cout << "================================================\n";
	cout << "\t\t Main Menu Screen\n";
	cout << "================================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transactions.\n";
	cout << "\t[7] Manage Users.\n";
	cout << "\t[8] Logout.\n";
	cout << "================================================\n";
}

void ShowMainMenu(short Permission) {
	short choice = 0;
	bool logout = false;
	while (!logout) {
		system("cls");
		MainMenu();
		choice = ReadMainMenuOptions();
		if ((enMainMenuOptions)choice == enMainMenuOptions::Logout) {
			logout = true;
		}
		else {
			PerformMainMenuOptions((enMainMenuOptions)choice, Permission);
		}
	}
}

void Login() {

		system("cls");
		stUsers user = ShowLoginScreen();
		ShowMainMenu(user.Parmissions);

}


int main()
{
	while(true){
		Login();
	}
	system("pause>0");
	return 0;
}


