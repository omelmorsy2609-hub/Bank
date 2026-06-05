#pragma warning(disable : 4996)

#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <fstream>
#include <random>
#include <iomanip>

using namespace std;

const string ClientsFileName = "clients.txt";
const string UsersFileName = "users.txt";

enum enMainMenueOptions
{
	eListClients = 1, eAddNewClient = 2,
	eDeleteClient = 3, eUpdateClient = 4,
	eFindClient = 5, eTransactions = 6, ManageUsers = 7,
	eLogout = 8
};

enum enTransactionsMenuOptions {
	deposit = 1, withdraw = 2,
	totalBalances = 3, mainMenu = 4
};

enum enManageUsersOptions{ eListUsers = 1, eAddNewUser = 2,
eDeleteUser = 3, eUpdateUser = 4, eFindUser = 5, eMainMenu = 6};

struct stClient
{
	string accountNo, pinCode, phone, name;
	float balance = 0;
	bool toBeDeleted = false;
};

struct stUser {
	string name, password;
	short permissions = 0;
	bool toBeDeleted = false;
};

stUser currentUser;

vector<string> mysplit(string s, string delimeter)
{
	vector<string> words;
	int pos = 0;
	string word;
	while ((pos = s.find(delimeter)) != string::npos)
	{
		word = s.substr(0, pos);
		if (word != "")
			words.push_back(word);
		s.erase(0, pos + delimeter.length());
	}
	if (s != "")
		words.push_back(s);
	return words;
}

string recordToLine(stClient client, string separator = "#//#")
{
	return (client.accountNo) + separator + (client.pinCode) + separator + client.name + separator + (client.phone) + separator + to_string(client.balance);
}

stClient lineToRecord(string s, string separator = "#//#")
{
	vector<string> vClients = mysplit(s, separator);

	stClient client;
	client.accountNo = vClients[0];
	client.pinCode = vClients[1];
	client.name = vClients[2];
	client.phone = vClients[3];
	client.balance = stof(vClients[4]);

	return client;
}

void printClientCard(stClient client)
{
	cout << "\naccount no: ";
	cout << client.accountNo << endl;
	cout << "PinCode: ";
	cout << client.pinCode << endl;
	cout << "name: ";
	cout << client.name << endl;
	cout << "phone: ";
	cout << client.phone << endl;
	cout << "balance: ";
	cout << client.balance << endl;
	cout << "\n-----------------------\n";
}

void showFindHeader()
{
	printf("-----------------------------------\n");
	printf("       find client screen\n");
	printf("-----------------------------------\n");
}

bool findClientByAccountNo(string accountNo, stClient& client, vector<stClient>& vClients)
{

	for (stClient c : vClients)
	{
		if (c.accountNo == accountNo)
		{
			client = c;
			return true;
		}
	}
	return false;

}

stClient readClient(vector<stClient>& vClients)
{
	stClient client;
	cout << "enter account no: ";
	getline(cin >> ws, client.accountNo);

	while (findClientByAccountNo(client.accountNo, client, vClients))
	{
		cout << "client with " << client.accountNo << " already exists, enter another account number: ";
		cin >> client.accountNo;
	}

	cout << "enter PinCode: ";
	getline(cin >> ws, client.pinCode);
	cout << "enter name: ";
	getline(cin, client.name);
	cout << "enter phone: ";
	getline(cin, client.phone);
	cout << "enter balance: ";
	string balance;
	getline(cin, balance);
	client.balance = stof(balance);
	return client;
}

void saveClient(stClient client, string fileName, bool append = true)
{
	fstream myfile;
	if (append)
		myfile.open(fileName, ios::app);
	else
		myfile.open(fileName, ios::out);
	if (myfile.is_open())
	{
		myfile << recordToLine(client) << endl;
		myfile.close();
	}

}

void AddNewClientsScreen(string fileName, vector<stClient>& vClients)
{
	printf("-----------------------------------\n");
	printf("       add new clients screen\n");
	printf("-----------------------------------\n");

	char again = 'y';
	while (tolower(again) == 'y')
	{
		cout << "\nadding new client...\n" << endl;
		stClient client = readClient(vClients);
		vClients.push_back(client);
		saveClient(client, fileName);
		cout << "client added successfully, do you want to add more clients? ";
		cin >> again;
	}
}

vector<stClient> loadClientsToVector(string fileName)
{
	vector<stClient> vClients;
	fstream myfile;
	myfile.open(fileName, ios::in);
	if (myfile.is_open())
	{
		string line;
		while (getline(myfile, line))
		{
			vClients.push_back(lineToRecord(line));
		}
		myfile.close();
	}
	return vClients;
}

void printRecordInClientList(stClient client)
{
	cout << "| " << setw(15) << left << client.accountNo;
	cout << "| " << setw(10) << left << client.pinCode;
	cout << "| " << setw(40) << left << client.name;
	cout << "| " << setw(12) << left << client.phone;
	cout << "| " << setw(12) << left << client.balance;
}

bool markClientsForDeletion(vector<stClient>& vClients, string accountNo)
{
	for (stClient& client : vClients)
	{
		if (client.accountNo == accountNo)
		{
			client.toBeDeleted = true;
			return true;
		}
	}
	return false;
}

void saveClientsToFile(vector<stClient> vClients)
{
	fstream myfile;
	myfile.open(ClientsFileName, ios::out);
	if (myfile.is_open())
	{
		for (stClient& client : vClients)
		{
			if (client.toBeDeleted == false) {
				myfile << recordToLine(client) << endl;
			}
		}
		myfile.close();
	}
}

string readAccountNo()
{
	string accountNo;
	cout << "please enter account number: ";
	getline(cin >> ws, accountNo);
	return accountNo;
}

void deleteClientScreen(vector<stClient>& vClients)
{
	printf("-----------------------------------\n");
	printf("       delete client screen\n");
	printf("-----------------------------------\n");
	stClient client;
	char del;
	string accountNo = readAccountNo();
	if (findClientByAccountNo(accountNo, client, vClients))
	{
		printClientCard(client);
		cout << "\nare you sure you want to delete: ";
		cin >> del;
		if (tolower(del) == 'y')
		{
			markClientsForDeletion(vClients, accountNo);
			saveClientsToFile(vClients);
			vClients = loadClientsToVector(ClientsFileName);
			cout << "\nclient deleted successfully" << endl;
		}
	}
	else
		cout << "\nclient with account Number " << accountNo << " not found";
}

stClient updateClient(string accountNo)
{
	stClient client;
	client.accountNo = accountNo;
	cout << "\nenter PinCode: ";
	getline(cin >> ws, client.pinCode);
	cout << "enter name: ";
	getline(cin, client.name);
	cout << "enter phone: ";
	getline(cin, client.phone);
	cout << "enter balance: ";
	string balance;
	getline(cin, balance);
	client.balance = stof(balance);
	return client;
}

void replaceClient(stClient client, vector<stClient>& vClients)
{
	for (stClient& c : vClients)
	{
		if (c.accountNo == client.accountNo)
		{
			c = client;
			break;
		}
	}
}

void updateClientByAccountNoScreen(vector<stClient>& vClients)
{
	printf("-----------------------------------\n");
	printf("     update client info screen\n");
	printf("-----------------------------------\n");
	stClient client;
	char confirm;
	string accountNo = readAccountNo();
	if (findClientByAccountNo(accountNo, client, vClients))
	{
		cout << "\nthe following is the client details: " << endl;
		printClientCard(client);
		cout << "\nare you sure you want to update this client? ";
		cin >> confirm;
		if (tolower(confirm) == 'y')
		{
			for (stClient& c : vClients)
			{
				if (c.accountNo == accountNo)
				{
					c = updateClient(accountNo);
					break;
				}
			}

			saveClientsToFile(vClients);
			cout << "\nclient updated successfully\n" << endl;
		}
	}
	else
		cout << "\nclient with account Number " << accountNo << " not found\n";
}

short ReadMainMenueOption()
{
	cout << "Choose what do you want to do? [1 to 8]? ";
	short Choice = 0;
	cin >> Choice;

	return Choice;
}

short ReadManageUsersOption()
{
	cout << "Choose what do you want to do? [1 to 6]? ";
	short Choice = 0;
	cin >> Choice;
	return Choice;
}

void showClientListScreen(vector<stClient>& vClients)
{

	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	for (stClient Client : vClients)
	{
		printRecordInClientList(Client);
		cout << endl;
	}
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}

void showEndHeader()
{
	printf("-----------------------------------\n");
	printf("        program ends :-)\n");
	printf("-----------------------------------\n");
}

void GoBackToMainMenue();

float readDepositAmount()
{
	float amount;
	cout << "\nplease enter deposit amount: ";
	cin >> amount;
	return amount;
}

bool areYouSure(string s)
{
	char answer;
	cout << s;
	cin >> answer;
	if (tolower(answer == 'y'))
		return true;
	return false;
}

void showTransactionsMenuScreen();

void goBackToTransactionsMenu()
{
	cout << "\n\nPress any key to go back to Transactions Menue...";
	system("pause>0");
	system("cls");
	showTransactionsMenuScreen();
}

void depositInBalance(vector<stClient>& vClients)
{
	printf("-----------------------------------\n");
	printf("         Deposit Screen\n");
	printf("-----------------------------------\n");

	stClient client;
	string accountNo = readAccountNo();
	while (!findClientByAccountNo(accountNo, client, vClients))
	{
		cout << "\nclient with account number " << accountNo << " does not exist.\n";
		accountNo = readAccountNo();
	}
	cout << "\nthe following is the client details: \n";
	printClientCard(client);
	float depositAmount = readDepositAmount();
	if (areYouSure("\nare you sure you want to perform this transaction? y/n ? "))
	{
		client.balance += depositAmount;
		cout << "\ntransaction performed successfully\n";
		cout << "\nnew balance: " << client.balance << endl;
		replaceClient(client, vClients);
		saveClientsToFile(vClients);
	}


}

void printRecordInBalanceList(stClient client)
{
	cout << "| " << setw(15) << left << client.accountNo;
	cout << "| " << setw(40) << left << client.name;
	cout << "| " << setw(12) << left << client.balance;
}

float calculateTotalBalances(vector<stClient>& vClients)
{
	float sum = 0;
	for (stClient& c : vClients)
	{
		sum += c.balance;
	}
	return sum;
}

void showTotalBalancesScreen(vector<stClient>& vClients)
{
	cout << "\n\t\t\t\t\tBalance List (" << vClients.size() << ") Client(s).";
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Balance";
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	for (stClient Client : vClients)
	{
		printRecordInBalanceList(Client);
		cout << endl;
	}
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	cout << "\nTotal Balances = " << calculateTotalBalances(vClients) << endl;
}

float readWithdrawAmount(float clientBalance)
{
	float amount;
	cout << "\nenter withdraw amount: ";
	cin >> amount;
	while (amount > clientBalance)
	{
		cout << "\namount exceeds balance, you can withdraw up to : " << clientBalance << endl;
		cout << "\nenter withdraw amount: ";
		cin >> amount;
	}
	return amount;

}

void withdrawFromBalance(vector<stClient>& vClients)
{
	printf("-----------------------------------\n");
	printf("         Withdraw Screen\n");
	printf("-----------------------------------\n");

	stClient client;
	string accountNo = readAccountNo();
	while (!findClientByAccountNo(accountNo, client, vClients))
	{
		cout << "\nclient with account number " << accountNo << " does not exist.\n";
		accountNo = readAccountNo();
	}
	cout << "\nthe following is the client details: \n";
	printClientCard(client);
	float withdrawAmount = readWithdrawAmount(client.balance);
	if (areYouSure("\nare you sure you want to perform this transaction? y/n ? "))
	{
		client.balance -= withdrawAmount;
		cout << "\ntransaction performed successfully\n";
		cout << "\nnew balance: " << client.balance << endl;
		replaceClient(client, vClients);
		saveClientsToFile(vClients);
	}


}

void showMainMenuScreen();

void performTransactionsMenuOption(enTransactionsMenuOptions transactionsMenuOption)
{
	vector<stClient> vClients = loadClientsToVector(ClientsFileName);
	switch (transactionsMenuOption)
	{
	case enTransactionsMenuOptions::deposit:
	{
		system("cls");
		depositInBalance(vClients);
		goBackToTransactionsMenu();
		break;
	}
	case enTransactionsMenuOptions::withdraw:
	{
		system("cls");
		withdrawFromBalance(vClients);
		goBackToTransactionsMenu();
		break;
	}
	case enTransactionsMenuOptions::totalBalances:
	{
		system("cls");
		showTotalBalancesScreen(vClients);
		goBackToTransactionsMenu();
		break;
	}
	case enTransactionsMenuOptions::mainMenu:
	{
		system("cls");
		showMainMenuScreen();
	}
	}
}

short ReadTransactionsMenueOption()
{
	cout << "Choose what do you want to do? [1 to 4]? ";
	short Choice = 0;
	cin >> Choice;

	return Choice;
}

void showTransactionsMenuScreen()
{
	printf("=================================================\n");
	printf("             Transactions Menu Screen\n");
	printf("=================================================\n");
	printf("          [1] Deposit.\n");
	printf("          [2] Withdraw.\n");
	printf("          [3] Total Balances.\n");
	printf("          [4] Main Menu.\n");
	printf("=================================================\n");
	performTransactionsMenuOption((enTransactionsMenuOptions)ReadTransactionsMenueOption());
}

void showLoginScreen();

void printRecordInUsersList(stUser user)
{
	cout << "| " << setw(20) << left << user.name;
	cout << "| " << setw(15) << left << user.password;
	cout << "| " << setw(15) << left << user.permissions;
}

void showUsersList(vector<stUser> vUsers)
{

	cout << "\n\t\t\t\t\tUsers List (" << vUsers.size() << ") User(s).";
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(20) << "User Name";
	cout << "| " << left << setw(15) << "Password";
	cout << "| " << left << setw(15) << "Permissions";
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	for (stUser user : vUsers)
	{
		printRecordInUsersList(user);
		cout << endl;
	}
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}

void showManageUsersScreen();

void goBackToManageUsersScreen()
{
	cout << "\n\nPress any key to go back to Manage Users Screen...";
	system("pause>0");
	system("cls");
	showManageUsersScreen();
}

stUser lineToUser(string s, string separator = "#//#")
{
	vector<string> vUser = mysplit(s, separator);

	stUser user;
	user.name = vUser[0];
	user.password = vUser[1];
	user.permissions = stof(vUser[2]);

	return user;
}

string userToLine(stUser user, string separator = "#//#")
{
	return (user.name) + separator + (user.password) + separator + to_string(user.permissions);
}

vector<stUser> loadUsers(string fileName)
{
	vector<stUser> vUsers;
	fstream myfile;
	myfile.open(fileName, ios::in);
	if (myfile.is_open())
	{
		string line;
		while (getline(myfile, line))
		{
			vUsers.push_back(lineToUser(line));
		}
		myfile.close();
	}
	return vUsers;
}

bool findUser(string name,stUser &user,vector<stUser> vUsers, bool returnUserIfFound = true)
{
	for (stUser& u : vUsers)
	{
		if (u.name == name)
		{
			if (returnUserIfFound)
				user = u;
			return true;
		}
	}
	return false;
}

bool checkUserForLogin(stUser &user, vector<stUser> vUsers)
{
	for (stUser& u : vUsers)
	{
		if (u.name == user.name && u.password==user.password)
		{
			user = u;
			return true;
		}
	}
	return false;
}

string readUsername()
{
	string username;
	cout << "enter Username: ";
	cin >> username;
	return username;
}

string readUserPassword()
{
	string password;
	cout << "enter password: ";
	cin >> password;
	return password;
}

void givePermissionTo(stUser& user,enMainMenueOptions option ,string message)
{
	char confirm;
	cout << message;
	cin >> confirm;
	if (tolower(confirm) == 'y')
		user.permissions += pow(2, option - 1);
}

stUser readFullUser(vector<stUser> vUsers, bool startFromPassword = false)
{
	char giveFullAccess;
	stUser user;
	if (!startFromPassword) {
		user.name = readUsername();

		while (findUser(user.name, user, vUsers, false))
		{
			cout << "user with " << user.name << " already exists, enter another username: ";
			cin >> user.name;
		}
	}
	user.password = readUserPassword();
	cout << "\ndo you want to give full access? (y/n) ? ";
	cin >> giveFullAccess;
	if (tolower(giveFullAccess) == 'y')
	{
		user.permissions = -1;
	}
	else
	{
		cout << "\ndo you want to give access to: \n\n";
		givePermissionTo(user, enMainMenueOptions::eListClients, "show client list? y/n? ");
		givePermissionTo(user, enMainMenueOptions::eAddNewClient, "\nadd new client? y/n? ");
		givePermissionTo(user, enMainMenueOptions::eDeleteClient, "\ndelete client? y/n? ");
		givePermissionTo(user, enMainMenueOptions::eUpdateClient, "\nupdate client? y/n? ");
		givePermissionTo(user, enMainMenueOptions::eFindClient, "\nfind client? y/n? ");
		givePermissionTo(user, enMainMenueOptions::eTransactions, "\nTransactions? y/n? ");
		givePermissionTo(user, enMainMenueOptions::ManageUsers, "\nmanage users? y/n? ");
	}

	return user;

}

void saveUsersToFile(vector<stUser> vUsers)
{
	fstream myfile;
	myfile.open(UsersFileName, ios::out);
	if (myfile.is_open())
	{
		for (stUser& u : vUsers)
		{
			if (u.toBeDeleted == false) {
				myfile << userToLine(u) << endl;
			}
		}
		myfile.close();
	}
}

void addNewUser(string fileName, vector<stUser> &vUsers)
{
	printf("-----------------------------------\n");
	printf("       add new user screen\n");
	printf("-----------------------------------\n");
	cout << "\nadding new user...\n" << endl;

	char again = 'y';
	while (tolower(again) == 'y')
	{
		stUser user = readFullUser(vUsers);
		vUsers.push_back(user);
		cout << "\nuser added successfully, do you want to add more users? ";
		cin >> again;
		cout << endl;
	}
	saveUsersToFile(vUsers);
}

void printUserCard(stUser user)
{
	cout << "\n-----------------------\n";
	cout << "Username: ";
	cout << user.name << endl;
	cout << "Password: ";
	cout << user.password << endl;
	cout << "permissions: ";
	cout << user.permissions << endl;
	cout << "-----------------------\n\n";
}

bool markUserForDeletion(vector<stUser>& vUsers, string username)
{
	for (stUser& user : vUsers)
	{
		if (user.name == username)
		{
			user.toBeDeleted = true;
			return true;
		}
	}
	return false;
}

void deleteUser(vector<stUser>& vUsers)
{
	printf("-----------------------------------\n");
	printf("       delete user screen\n");
	printf("-----------------------------------\n");
	string username = readUsername();
	if (username != "Admin") {
		stUser user;
		char del;
		if (findUser(username, user, vUsers))
		{
			cout << "\nThe following are the user details: " << endl;
			printUserCard(user);
			cout << "\nare you sure you want to delete this user: ";
			cin >> del;
			if (tolower(del) == 'y')
			{
				markUserForDeletion(vUsers, user.name);
				saveUsersToFile(vUsers);
				vUsers = loadUsers(UsersFileName);
				cout << "\nUser deleted successfully" << endl;
			}
		}
		else
			cout << "\user with Username " << username << " not found";
	}
	else
	{
		cout << "you cannot delete this user.\n";
	}
}

void updateUser(vector<stUser>& vUsers)
{
	printf("-----------------------------------\n");
	printf("     update users screen\n");
	printf("-----------------------------------\n");
	stUser user;
	char confirm;
	string username = readUsername();
	if (findUser(username, user, vUsers))
	{
		cout << "\nthe following is the user details: " << endl;
		printUserCard(user);
		cout << "\nare you sure you want to update this client? ";
		cin >> confirm;
		if (tolower(confirm) == 'y')
		{
			for (stUser& u : vUsers)
			{
				if (u.name == username)
				{
					u = readFullUser(vUsers, true);
					u.name = username;
					break;
				}
			}

			saveUsersToFile(vUsers);
			cout << "\nuser updated successfully\n" << endl;
		}
	}
	else
		cout << "\nuser with username " << username << " not found\n";
}

void findUserScreen(vector<stUser>& vUsers)
{
	printf("-----------------------------------\n");
	printf("       find user screen\n");
	printf("-----------------------------------\n");
	stUser user;
	string username = readUsername();
	if (findUser(username, user, vUsers))
		printUserCard(user);
	else
		cout << "\user with username " << username << " not found\n";
}

void performManageUsersOption(enManageUsersOptions muoption)
{
	vector<stUser> vUsers = loadUsers(UsersFileName);
	switch (muoption)
	{
	case enManageUsersOptions::eListUsers:
	{
		system("cls");
		showUsersList(vUsers);
		goBackToManageUsersScreen();
		break;
	}
	case enManageUsersOptions::eAddNewUser:
	{
		system("cls");
		addNewUser(UsersFileName, vUsers);
		goBackToManageUsersScreen();
		break;
	}

	case enManageUsersOptions::eDeleteUser:
	{
		system("cls");
		deleteUser(vUsers);
		goBackToManageUsersScreen();
		break;
	}
	case enManageUsersOptions::eUpdateUser:
	{
		system("cls");
		updateUser(vUsers);
		goBackToManageUsersScreen();
		break;
	}
	case enManageUsersOptions::eFindUser:
	{
		system("cls");
		findUserScreen(vUsers);
		goBackToManageUsersScreen();
		break;
	}
	case enManageUsersOptions::eMainMenu:
	{
		system("cls");
		showMainMenuScreen();
	}
	}
}

void showManageUsersScreen()
{
	system("cls");
	printf("=================================================\n");
	printf("             Manage Users Menu Screen\n");
	printf("=================================================\n");
	printf("          [1] List Users.\n");
	printf("          [2] Add New User.\n");
	printf("          [3] Delete User.\n");
	printf("          [4] Update User.\n");
	printf("          [5] Find User.\n");
	printf("          [6] Main Menu.\n");
	printf("=================================================\n");
	performManageUsersOption((enManageUsersOptions)ReadManageUsersOption());
}

bool doesUserHasPermission(enMainMenueOptions option)
{
	if (currentUser.permissions == -1)
		return true;
	//return 
	//for (short i = permissions;i >= 1;i--)
	//{
	//	float index = log2(i) + 1;
	//	if (option == index)
	//		return true;
	//}
	//return false;
	return currentUser.permissions & int(pow(2, option - 1));
}

void showAccessDeniedScreen()
{
	printf("-----------------------------------\n");
	printf("Access Denied,\nYou do not have permission to do this,\nPlease contact you Admin\n");
	printf("-----------------------------------\n\n");
}

void findClientScreen(vector<stClient> vClients)
{
	showFindHeader();
	stClient client;
	string accountNo = readAccountNo();
	if (findClientByAccountNo(accountNo, client, vClients))
		printClientCard(client);
	else
		cout << "\nclient with account number " << accountNo << " not found\n";
}

void PerformMainMenueOption(enMainMenueOptions MainMenueOption)
{
	vector<stClient> vClients = loadClientsToVector(ClientsFileName);
	switch (MainMenueOption)
	{
	case enMainMenueOptions::eListClients:
	{
		system("cls");
		if (doesUserHasPermission(enMainMenueOptions::eListClients))
			showClientListScreen(vClients);
		else
			showAccessDeniedScreen();
		GoBackToMainMenue();
		break;
	}
	case enMainMenueOptions::eAddNewClient:
	{
		system("cls");
		if (doesUserHasPermission(enMainMenueOptions::eAddNewClient))
			AddNewClientsScreen(ClientsFileName, vClients);
		else
			showAccessDeniedScreen();
		GoBackToMainMenue();
		break;
	}

	case enMainMenueOptions::eDeleteClient:
	{
		system("cls");
		if (doesUserHasPermission(enMainMenueOptions::eDeleteClient))
			deleteClientScreen(vClients);
		else
			showAccessDeniedScreen();
		GoBackToMainMenue();
		break;
	}
	case enMainMenueOptions::eUpdateClient:
	{
		system("cls");
		if (doesUserHasPermission(enMainMenueOptions::eUpdateClient))
			updateClientByAccountNoScreen(vClients);
		else
			showAccessDeniedScreen();
		GoBackToMainMenue();
		break;
	}
	case enMainMenueOptions::eFindClient:
	{
		system("cls");
		if (doesUserHasPermission(enMainMenueOptions::eFindClient))
			findClientScreen(vClients);
		else
			showAccessDeniedScreen();
		GoBackToMainMenue();
		break;
	}
	case enMainMenueOptions::eTransactions:
	{
		system("cls");
		if (doesUserHasPermission(enMainMenueOptions::eTransactions))
			showTransactionsMenuScreen();
		else {
			showAccessDeniedScreen();
			GoBackToMainMenue();
		}
		break;
	}
	case enMainMenueOptions::ManageUsers:
	{
		system("cls");
		if (doesUserHasPermission(enMainMenueOptions::ManageUsers))
			showManageUsersScreen();
		else {
			showAccessDeniedScreen();
			GoBackToMainMenue();
		}
		break;
	}
	case enMainMenueOptions::eLogout:
	{
		system("cls");
		showLoginScreen();
	}
	}
}

void showMainMenuScreen()
{
	system("cls");
	printf("=================================================\n");
	printf("                  Main Menu Screen\n");
	printf("=================================================\n");
	printf("          [1] show client list.\n");
	printf("          [2] add new client.\n");
	printf("          [3] delete client.\n");
	printf("          [4] update client info.\n");
	printf("          [5] find client.\n");
	printf("          [6] Transactions.\n");
	printf("          [7] Manage Users.\n");
	printf("          [8] Logout.\n");
	printf("=================================================\n");
	PerformMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}

void GoBackToMainMenue()
{
	cout << "\n\nPress any key to go back to Main Menue...";
	system("pause>0");
	system("cls");
	showMainMenuScreen();
}

stUser readUser()
{
	stUser user;
	user.name = readUsername();
	user.password = readUserPassword();
	return user;
}

void showLoginScreen()
{
	printf("-----------------------------------\n");
	printf("          Login Screen\n");
	printf("-----------------------------------\n");

	vector<stUser> vUsers = loadUsers(UsersFileName);
	stUser user = readUser();
	while (!checkUserForLogin(user, vUsers))
	{
		cout << "Invalid username/password!\n";
		user = readUser();
	}
	currentUser = user;
	showMainMenuScreen();

}

int main()
{
	srand((unsigned)time(NULL));
	showLoginScreen();
	
}
