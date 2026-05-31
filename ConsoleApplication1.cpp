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

enum enMainMenueOptions
{
	eListClients = 1, eAddNewClient = 2,
	eDeleteClient = 3, eUpdateClient = 4,
	eFindClient = 5, eTransactions = 6, eExit = 7
};

enum enTransactionsMenuOptions{ 
	deposit = 1, withdraw = 2, 
	totalBalances = 3, mainMenu = 4};

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
		s.erase(0,pos + delimeter.length());
	}
	if (s != "")
		words.push_back(s);
	return words;
}

struct stClient
{
	string accountNo, pinCode , phone, name;
	float balance = 0;
	bool toBeDeleted = false;
};

string recordToLine(stClient client, string separator="#//#")
{
	return (client.accountNo) + separator + (client.pinCode) + separator + client.name + separator + (client.phone) + separator + to_string(client.balance);
}

stClient lineToRecord(string s, string separator="#//#")
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

bool findClientByAccountNo(string accountNo, stClient &client, vector<stClient> &vClients)
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

stClient readClient(vector<stClient> &vClients)
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
	getline(cin>>ws, client.pinCode);
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

void AddNewClients(string fileName, vector<stClient> &vClients)
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
		for (stClient &client : vClients)
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

void showDeleteHeader()
{
	printf("-----------------------------------\n");
	printf("       delete client screen\n");
	printf("-----------------------------------\n");
}

void deleteClient(string accountNo, vector<stClient> &vClients)
{
	
	stClient client;
	char del;
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
	getline(cin>>ws, client.pinCode);
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


void showUpdateHeader()
{
	printf("-----------------------------------\n");
	printf("     update client info screen\n");
	printf("-----------------------------------\n");
}

void updateClientByAccountNo(string accountNo, vector<stClient>& vClients)
{
	stClient client;
	char confirm;
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
	cout << "Choose what do you want to do? [1 to 6]? ";
	short Choice = 0;
	cin >> Choice;

	return Choice;
}

void showClientList(vector<stClient> &vClients)
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

void depositInBalance(vector<stClient> &vClients)
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

void PerformMainMenueOption(enMainMenueOptions MainMenueOption)
{
	vector<stClient> vClients = loadClientsToVector(ClientsFileName);
	switch (MainMenueOption)
	{
	case enMainMenueOptions::eListClients:
	{
		system("cls");
		showClientList(vClients);
		GoBackToMainMenue();
		break;
	}
	case enMainMenueOptions::eAddNewClient:
	{
		system("cls");
		AddNewClients(ClientsFileName, vClients);
		GoBackToMainMenue();
		break;
	}

	case enMainMenueOptions::eDeleteClient:
	{
		system("cls");
		showDeleteHeader();
		deleteClient(readAccountNo(), vClients);
		GoBackToMainMenue();
		break;
	}
	case enMainMenueOptions::eUpdateClient:
	{
		system("cls");
		showUpdateHeader();
		updateClientByAccountNo(readAccountNo(), vClients);
		GoBackToMainMenue();
		break;
	}
	case enMainMenueOptions::eFindClient:
	{
		system("cls");
		showFindHeader();
		stClient client;
		string accountNo = readAccountNo();
		if (findClientByAccountNo(accountNo, client, vClients))
			printClientCard(client);
		else
			cout << "\nclient with account number " << accountNo << " not found\n";
		GoBackToMainMenue();
		break;
	}
	case enMainMenueOptions::eTransactions:
	{
		system("cls");
		showTransactionsMenuScreen();
		break;
	}
	case enMainMenueOptions::eExit:
	{
		system("cls");
		showEndHeader();
		break;
	}
	}
}

void showMainMenuScreen()
{
	printf("=================================================\n");
	printf("                  Main Menu Screen\n");
	printf("=================================================\n");
	printf("          [1] show client list.\n");
	printf("          [2] add new client.\n");
	printf("          [3] delete client.\n");
	printf("          [4] update client info.\n");
	printf("          [5] find client.\n");
	printf("          [6] Transactions.\n");
	printf("          [7] Exit.\n");
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

void start()
{
	
	string answer;
	vector<stClient> vClients = loadClientsToVector(ClientsFileName);
	
	while (true)
	{
		system("cls");
		showMainMenuScreen();
		cout << "choose what do you want to do? [1 to 6]? ";
		cin >> answer;

		if (answer == "1")
		{
			system("cls");
			showClientList(vClients);

		}
		else if (answer == "2")
		{
			system("cls");
			AddNewClients(ClientsFileName, vClients);

		}
		else if (answer == "3")
		{
			system("cls");
			showDeleteHeader();
			deleteClient(readAccountNo(), vClients);
		}
		else if (answer == "4")
		{
			system("cls");
			showUpdateHeader();
			updateClientByAccountNo(readAccountNo(), vClients);
		}
		else if (answer == "5") 
		{
			system("cls");
			showFindHeader();
			stClient client;
			string accountNo = readAccountNo();
			if (findClientByAccountNo(accountNo, client, vClients))
				printClientCard(client);
			else
				cout << "\nclient with account number " << accountNo << " not found\n";
		}
		else if (answer == "6")
		{
			system("cls");
			showEndHeader();
			break;
		}

		cout << "\npress any key to go back to main menu...";
		system("pause > nul");

	}
}

int main()
{
	srand((unsigned)time(NULL));
	showMainMenuScreen();
	
}