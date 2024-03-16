#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

const string FileName = "Clients.txt";

void PrintMenu();
void ShowMenu();
void ShowTransactionMenu();

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDeletion = false;
};

void GoBackToMainMenu()
{
    cout << "\n\nPress any key to go back to the Main Menu...";
    system("pause>0");
}

void GoBackToTransactionMenu()
{
    cout << "\n\nPress any key to go back to the Transaction Menu...";
    system("pause>0");
}

vector <string> SplitWords(string S1, string delim)
{
    vector <string> vWords;

    short pos = 0;
    string sWord;

    while ((pos = S1.find(delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos);
        if (sWord != "")
        {
            vWords.push_back(sWord);
        }

        S1.erase(0, pos + delim.length());
    }

    if (S1 != "")
    {
        vWords.push_back(S1);
    }
    return vWords;
}

sClient ConvertFromLineToRecord(string Line, string delim = "#//#")
{
    sClient Client;
    vector <string> vWords;

    vWords = SplitWords(Line, delim);

    Client.AccountNumber = vWords[0];
    Client.PinCode = vWords[1];
    Client.Name = vWords[2];
    Client.Phone = vWords[3];
    Client.AccountBalance = stod(vWords[4]);

    return Client;
}

vector <sClient> LoadClientDataFromFile(string FileName)
{
    vector <sClient> vClients;
    fstream myFile;

    myFile.open(FileName, ios::in);

    if (myFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(myFile, Line))
        {
            Client = ConvertFromLineToRecord(Line);
            vClients.push_back(Client);

        }

        myFile.close();
    }

    return vClients;
}

void PrintClientRecord(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintAllClientsRecord(vector <sClient> vClients)
{
    system("cls");

    cout << "\t\t\t\t\tClient List(" << vClients.size() << ") client(s) \n";
    cout << "_______________________________________________________________________________________________________" << endl;
    cout << "| " << setw(15) << left << "AccountNumber";
    cout << "| " << setw(10) << left << "PinCode";
    cout << "| " << setw(40) << left << "Name";
    cout << "| " << setw(12) << left << "Phone";
    cout << "| " << setw(12) << left << "AccountBalance";
    cout << "\n_______________________________________________________________________________________________________" << endl;

    for (sClient Client : vClients)
    {
        PrintClientRecord(Client);
        cout << endl;
    }
    cout << "_______________________________________________________________________________________________________" << endl;
}

void ShowClientList()
{
    vector <sClient> vClients = LoadClientDataFromFile(FileName);
    PrintAllClientsRecord(vClients);
}

bool ThereIsOne(vector <sClient> vClients, sClient Client)
{
    for (sClient C : vClients)
    {
        if (C.AccountNumber == Client.AccountNumber)
        {
            return true;
        }
    }
    return false;
}

sClient ReadNewClient ()
{
    vector <sClient> vClients;
    vClients = LoadClientDataFromFile(FileName);

    sClient Client;

    cout << "Enter Account Number? ";
    getline(cin >> ws, Client.AccountNumber); // ignore the white spaces that are in the buffer 

    for (sClient C : vClients)
    {
        while (ThereIsOne(vClients, Client))
        {
            cout << "You can't choose this account number, because there's one. Enter another one? ";
            getline(cin >> ws, Client.AccountNumber);
            ThereIsOne(vClients, Client);
        }
    }

    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone)
        ;
    cout << "Enter Account Balance? ";
    cin >> Client.AccountBalance;

    return Client;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;
}

void AddClientDataToFile(string Filename, string DataLine)
{
    fstream MyFile;

    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << DataLine << endl;

        MyFile.close();
    }


}

void AddClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddClientDataToFile(FileName, ConvertRecordToLine(Client));
}

void AddNewClients()
{
    char more = 'Y';
    sClient Client;

    do
    {
        system("cls");

        AddClient();

        cout << "Client added seuccessfully, do you want to add more clients? ";
        cin >> more;

    } while (toupper(more) == 'Y');
}

string ReadAccountNumber()
{
    string accountNumber = "";

    cout << "Please enter Account number? ";
    cin >> accountNumber;

    return accountNumber;
}

void PrintClientInfo(sClient Client)
{
    cout << "\nThe information of the Client:\n\n";
    cout << "\nAccount number   : " << Client.AccountNumber << endl;
    cout << "Pin code         : " << Client.PinCode << endl;
    cout << "Name             : " << Client.Name << endl;
    cout << "Phone            : " << Client.Phone << endl;
    cout << "Account balance  : " << Client.AccountBalance << endl;
}

bool FindClientByAccountNum(sClient& Client, string accountNumber, vector <sClient> vClients)
{

    for (sClient C : vClients)
    {
        if (C.AccountNumber == accountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;


}

bool MarkClientForDeletion(string accountNumber, vector <sClient>& vClients)
{
    for (sClient& C : vClients)
    {
        if (C.AccountNumber == accountNumber)
        {
            C.MarkForDeletion = true;
            return true;
        }
    }
    return false;
}

vector <sClient> SaveClientsInfoInFile(string accountNumber, vector <sClient> vClients)
{
    fstream MyFile;

    MyFile.open(FileName, ios::out);

    if (MyFile.is_open())
    {
        string DataLine;

        for (sClient C : vClients)
        {
            if (C.MarkForDeletion == false)
            {
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }

        MyFile.close();
    }
    return vClients;
}

bool DeleteClientByAccountNumber(string accountNumber, vector <sClient> vClients)
{
    sClient Client;
    char answer = 'n';

    if (FindClientByAccountNum(Client, accountNumber, vClients))
    {
        PrintClientInfo(Client);

        cout << "\n\nAre you sure that you want to delete this Client? y/n? ";
        cin >> answer;

        if (answer == 'y' || answer == 'Y')
        {
            MarkClientForDeletion(accountNumber, vClients);
            SaveClientsInfoInFile(accountNumber, vClients);

            vClients = LoadClientDataFromFile(FileName);

            cout << "\nThe deletion was successful ^^";
            return true;
        }
    }
    else
    {
        cout << "The Client with this account number is NOT found!!\n";
        return false;
    }
}

void DeleteClient()
{

    vector <sClient> vClients = LoadClientDataFromFile(FileName);
    string accountNumber = ReadAccountNumber();

    DeleteClientByAccountNumber(accountNumber, vClients);

}

sClient ChangeClientRecord(string accountNumber)
{

    sClient Client;
    Client.AccountNumber = accountNumber;


    cout << "Enter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone)
        ;
    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;
}

bool UpdateClientByAccountNumber(string accountNumber, vector <sClient> vClients)
{
    sClient Client;
    char answer = 'n';

    if (FindClientByAccountNum(Client, accountNumber, vClients))
    {
        PrintClientInfo(Client);

        cout << "\n\nAre you sure that you want to update this Client? y/n? ";
        cin >> answer;

        if (answer == 'y' || answer == 'Y')
        {
            for (sClient& C : vClients)
            {
                if (C.AccountNumber == accountNumber)
                {
                    C = ChangeClientRecord(accountNumber);
                    break;
                }
            }
            SaveClientsInfoInFile(accountNumber, vClients);

            vClients = LoadClientDataFromFile(FileName);

            cout << "\nThe Update was successful ^^";
            return true;
        }
    }
    else
    {
        cout << "The Client with this account number is NOT found!!\n";
        return false;
    }
}

void UpdateClient()
{
    vector <sClient> vClients = LoadClientDataFromFile(FileName);
    string accountNum = ReadAccountNumber();

    UpdateClientByAccountNumber(accountNum, vClients);
}

bool CheckClientAccountNumAndPrint(sClient& Client, string accountNumber)
{

    vector <sClient> vClients = LoadClientDataFromFile(FileName);

    for (sClient C : vClients)
    {
        if (C.AccountNumber == accountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;

}

void FindClient()
{

    string accountNumber = ReadAccountNumber();
    sClient Client;

    if (CheckClientAccountNumAndPrint(Client, accountNumber))
    {
        PrintClientInfo(Client);
    }
    else
    {
        cout << "The Client with account number (" << accountNumber << ") was NOT FOUND! " << endl;
    }

}

short ReadChoice()
{
	short choice; 

	cout << "Choose what do you want to do? [1 to 7]? ";
	cin >> choice;

	return choice;
}

void Exit()
{

	cout << "=========================================\n\n";
	cout << "          The programm ends here ^^\n\n";
	cout << "=========================================\n";
}   

void ShowTheChoice()
{
	short choice = ReadChoice();

	switch (choice)
	{
	case 1: 
        system("cls");
		ShowClientList();
        GoBackToMainMenu();
        system("cls");
        ShowMenu();
		break;
	case 2:
        system("cls");
		AddNewClients();
        GoBackToMainMenu(); 
        system("cls");
        ShowMenu();
		break;
	case 3:
        system("cls");
		DeleteClient();
        GoBackToMainMenu();
        system("cls");
        ShowMenu();
		break;
	case 4:
        system("cls");
		UpdateClient();
        GoBackToMainMenu();
        system("cls");
        ShowMenu();
		break;
	case 5:
        system("cls");
		FindClient();
        GoBackToMainMenu();
        system("cls");
        ShowMenu();
		break;
	case 6:
        system("cls");
        ShowTransactionMenu();
        GoBackToMainMenu();
        system("cls");
        ShowMenu();
		break;
    case 7:
        system("cls");
        Exit();
        GoBackToMainMenu();
        system("cls");
        ShowMenu();
        break;
	}
}

void PrintMenu()
{
	cout << "=========================================\n";
	cout << "             Main menu screen" << endl;
	cout << "=========================================\n";
	cout << "            [1] Show client list." << endl;
	cout << "            [2] Add new client." << endl;
	cout << "            [3] Delete client." << endl;
	cout << "            [4] Update client info." << endl;
	cout << "            [5] Find client." << endl;
    cout << "            [6] Transaction." << endl;
	cout << "            [7] Exit." << endl;
	cout << "=========================================\n";	
}

void ShowMenu()
{
    PrintMenu();
    ShowTheChoice();
}





int ReadNum()
{
    int num;

    cout << "Please enter deposite amount? ";
    cin >> num;

    return num;
}

void Deposit()
{
    sClient Client;
    vector <sClient> vClients = LoadClientDataFromFile(FileName);
    string accountNumber = ReadAccountNumber();

   
    if (FindClientByAccountNum(Client, accountNumber, vClients))
    {
        PrintClientInfo(Client); cout << endl;
        int numPlus = ReadNum();

        for (sClient& C : vClients)
        {
            if (C.AccountNumber == accountNumber)
            {
                C.AccountBalance += numPlus;
            }
        }

        SaveClientsInfoInFile(accountNumber, vClients);

        vClients = LoadClientDataFromFile(FileName);
    }
    else
    {
        cout << "This account number was NOT found !!";
    }
        
    }

void WithDraw()
{
    sClient Client;
    vector <sClient> vClients = LoadClientDataFromFile(FileName);
    string accountNumber = ReadAccountNumber();

    if (FindClientByAccountNum(Client, accountNumber, vClients))
    {
        PrintClientInfo(Client); cout << endl;
        int num = ReadNum();

        for (sClient& C : vClients)
        {
            if (C.AccountNumber == accountNumber)
            {
                if (C.AccountBalance < num)
                {
                    cout << "\nIt is not possible to withdraw money, because you don't have enough !\n";
                    cout << "Enter a new amount: ";
                    cin >> num;

                    if (!(C.AccountBalance < num))
                    {
                        C.AccountBalance -= num;
                    }
                }
                else
                {
                    C.AccountBalance -= num;
                }
                
            }
        }

        SaveClientsInfoInFile(accountNumber, vClients);

        vClients = LoadClientDataFromFile(FileName);
    }
    else
    {
        cout << "This account number was NOT found !!\n";
    }
}

void PrintSingleCleintInfo(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintClientsInfos(vector <sClient> vClients)
{
    cout << "\t\t\t\t\tClient List(" << vClients.size() << ") client(s) \n";
    cout << "_______________________________________________________________________________________________________" << endl;
    cout << "| " << setw(15) << left << "AccountNumber";
    cout << "| " << setw(40) << left << "Name";
    cout << "| " << setw(12) << left << "AccountBalance";
    cout << "\n_______________________________________________________________________________________________________" << endl;

    for (sClient C : vClients)
    {
        PrintSingleCleintInfo(C);
        cout << endl;
    }
}

int CalculateTotalBalance(vector <sClient> vClients)
{
    int total = 0;

    for(sClient C: vClients)
    {
       

        total += (int)C.AccountBalance;
    }

    return total;
}

void TotalBalances()
{
    sClient Client;
    vector <sClient> vClients = LoadClientDataFromFile(FileName);

    PrintClientsInfos(vClients);
    cout << "\n_______________________________________________________________________________________________________" << endl;
    cout << "\nTotal Balance: " << CalculateTotalBalance(vClients); cout << endl;
}

void PrintTransactionHead()
{
    cout << "=============================================\n";
    cout << "             Transaction Menu\n";
    cout << "=============================================\n";
}

void PrintTransactionChoices()
{
    cout << "              [1] Deposit\n";
    cout << "              [2] Withdraw\n";
    cout << "              [3] Total balances\n";
    cout << "              [4] Back To main Menu\n";
}

short ReadChoicess()
{
    short choice;

    cout << "Choose what do you want to do [1 to 4]? ";
    cin >> choice;

    return choice;
}

void TheActionAfterChoice(short choice)
{
    switch (choice)
    {
    case 1:
        system("cls");
        Deposit();
        GoBackToTransactionMenu();
        system("cls");
        ShowTransactionMenu();
        break;
    case 2:
        system("cls");
        WithDraw();
        GoBackToTransactionMenu();
        system("cls");
        ShowTransactionMenu();
        break;
    case 3:
        system("cls");
        TotalBalances();
        GoBackToTransactionMenu();
        system("cls");
        ShowTransactionMenu();
        break;
    case 4:
        system("cls");
        ShowMenu();
        break;
    }
}

void ShowTransactionMenu()
{
    PrintTransactionHead();
    PrintTransactionChoices();
    cout << "=============================================\n";
    short choice = ReadChoicess();
    TheActionAfterChoice(choice);
}

int main()
{
    ShowMenu();
    
    system("pause>0");
    return 0;
}


