#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <conio.h>
#include "InputLib.h"

using namespace std;

const string ClientsFileName = "Clients.txt";

enum enMenu {enShowClientsList = 1, enAddNewClients = 2, enDeleteClient = 3, enUpdateClientInfo = 4, enFindClient = 5, enTransactions = 6, enExit = 7};

enum enumTransactions {enDeposit = 1, enWithdraw = 2, enTotalBalances = 3, enMainMenu = 4};

struct stClient
{
    string AccountNumber  = "";
    string PinCode = "";
    string Name = "";
    string PhoneNumber = "";
    double AccountBalance = 0;
    bool MarkForDelete = false;
};

string GetValidPIN()
{
    string pin;
    while (true)
    {
        cout << "Enter PinCode?: ";
        cin >> pin;

        if (pin.length() != 4)
        {
            cout << "\nError: PIN must be exactly 4 digits.\n\n";
            continue;
        }

        bool isAllDigits = true;
        for (char c : pin)
        {
            if (!isdigit(c))
            {
                isAllDigits = false;
                break;
            }
        }

        if (isAllDigits)
        {
            return pin; 
        }
        else
        {
            cout << "\nError: PIN must contain numbers only (no letters or signs).\n\n";
        }
    }
}

bool CleintExistsByAccountNumber(const string &AccountNumber, const vector<stClient> &vClientRecord)
{
    for (const stClient &C : vClientRecord)
    {
        if (C.AccountNumber == AccountNumber)
        {
            cout << "\nClient With[" << AccountNumber << "] already exists, Try again.\n\n";
            return true;
        }
    }
    return false;
}

stClient ReadNewClient(const vector<stClient> &vClientRecord)
{
    stClient Client;
    do
    {
        Client.AccountNumber = InputLib::ReadText("Enter Account Number?: ");
    } while (CleintExistsByAccountNumber(Client.AccountNumber, vClientRecord));
    
    Client.PinCode = GetValidPIN();
    Client.Name = InputLib::ReadText("Enter Name?: ");
    Client.PhoneNumber = InputLib::ReadText("Enter Phone?: ");
    Client.AccountBalance = InputLib::ReadFloatPositiveNumber("Enter Account Balance?: ");
    return Client;
}

vector <string> SplitText(string Text, string Delim = " ")
{
    vector <string> vSpText;
    string sWord = "";
    short Pos = 0;
    while ((Pos = Text.find(Delim)) != string::npos)
    {
        sWord = Text.substr(0, Pos);
        if (sWord != "")
        {
            vSpText.push_back(sWord);
        }
        Text.erase(0, Pos + Delim.length());
    }
    if (Text != "")
    {
        vSpText.push_back(Text);
    }
    return vSpText;
}

stClient ConvertLineToClientRecord(string Line, string Seperator = "#//#")
{
    stClient ClientRecords;
    vector <string> vClientData = SplitText(Line, Seperator);

    ClientRecords.AccountNumber = vClientData[0];
    ClientRecords.PinCode = vClientData[1];
    ClientRecords.Name = vClientData[2];
    ClientRecords.PhoneNumber = vClientData[3];
    ClientRecords.AccountBalance = stof(vClientData[4]);
    
    return ClientRecords;
}

string ConvertRecordToOneLine(const stClient &Record, string Seperator = "#//#")
{
    ostringstream ss;
    ss << fixed << setprecision(6) << Record.AccountBalance; 
    
    string Result = "";
    Result += Record.AccountNumber + Seperator;
    Result += Record.PinCode + Seperator;
    Result += Record.Name + Seperator;
    Result += Record.PhoneNumber + Seperator;
    Result += ss.str();
    
    return Result;
}

vector <stClient> LoadFillContentToVector(string FileName)
{
    vector <stClient> vFileContent;
    fstream Fill;
    Fill.open(FileName, ios::in);
    if (Fill.is_open())
    {
        string Line;
        while(getline(Fill, Line))
        {
            if (Line != "")
                vFileContent.push_back(ConvertLineToClientRecord(Line));
        }
        Fill.close();
    }
    return vFileContent;
}

void PrintClientRecord(const stClient &Client)
{
    cout << "\n";
    cout << "| " << left << setw(15) << Client.AccountNumber << "| " << left << setw(9) << Client.PinCode << "| " << left << setw(55) << Client.Name << "| " << left << setw(13) << Client.PhoneNumber << "| " << left << setw(13) << Client.AccountBalance << "|";
}

void PrintClientBalanceDetails(const stClient &Client)
{
    cout << "\n";
    cout << "| " << left << setw(15) << Client.AccountNumber << "| " << left << setw(53) << Client.Name << "| " << left << setw(41) << Client.AccountBalance << "|";
}

bool FindClientByAccountNumber(const string &AccountNumber, const vector <stClient> &vClientRecord, stClient &Client)
{
    for (const stClient &Record : vClientRecord)
    {
        if (Record.AccountNumber == AccountNumber)
        {
            Client = Record;
            return true;
        }
    }
    return false;
}

short FindClientIndexByAccountNumber(const string &AccountNumber, const vector<stClient> &vClientRecord)
{
    for (short i = 0; i < vClientRecord.size(); i++)
    {
        if (vClientRecord[i].AccountNumber == AccountNumber)
        {
            return i;
        }
    }
    return -1;
}

bool MarkClientForDeleteByAccountNumber(const string &AccountNumber, vector <stClient> &vClientRecord)
{
    for (stClient &Client : vClientRecord)
    {
        if (Client.AccountNumber == AccountNumber)
        {
            Client.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

stClient ChangeClientRecord(string AccountNumber)
{
    stClient Client;

    Client.AccountNumber = AccountNumber;
    Client.PinCode = InputLib::ReadText("Enter PinCode?: ");
    Client.Name = InputLib::ReadText("Enter Name?: ");
    Client.PhoneNumber = InputLib::ReadText("Enter Phone?: ");
    Client.AccountBalance = InputLib::ReadFloatPositiveNumber("Enter Account Balance?: ");

    return Client;
}

void PrintClientDetails(const stClient &Client)
{
    cout << "\nThe following are the client details:-";
    cout << "\n---------------------------------------------------\n";
    cout << "Account Number : " << Client.AccountNumber;
    cout << "\nPin Code       : " << Client.PinCode << endl;
    cout << "Name           : " << Client.Name;
    cout << "\nPhone Number   : " << Client.PhoneNumber;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n---------------------------------------------------";
    cout << "\n";
    cout << endl;
}

bool AddDataLineToFile(string FileName, string Line)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app); 
    if (MyFile.is_open())
    {
        MyFile << Line << endl;
        MyFile.close();
        return true;
    }
    return false;
}

bool SaveCleintsDataToFile(string FileName, const vector<stClient> &vClientRecord)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    if (MyFile.is_open())
    {
        string DataLine = "";
        for (const stClient &Client : vClientRecord)
        {
            if (!Client.MarkForDelete)
            {
                DataLine = ConvertRecordToOneLine(Client);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
        return true;
    }
    return false;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector <stClient> &vClientRecord)
{
    char Answer = 'n';
    stClient Client;
    if (FindClientByAccountNumber(AccountNumber, vClientRecord, Client))
    {
        PrintClientDetails(Client);
        Answer = InputLib::getYesNoAnswer("Are you sure you want delete this client? (y/n): ");
        if (Answer == 'y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClientRecord);
            SaveCleintsDataToFile(ClientsFileName, vClientRecord);
            vClientRecord = LoadFillContentToVector(ClientsFileName);
            cout << "\n\nClient Deleted Successfully.";
            return true;
        }
        return false;
    }
    else
    {
        printf("\nClient With Account Number (%s) Not Found!.", AccountNumber.c_str());
        return false;
    }
}

bool UpdateClientByAccountNumber(string AccountNumber, vector <stClient> &vClientRecord)
{
    char Answer = 'n';
    stClient Client;
    if (FindClientByAccountNumber(AccountNumber, vClientRecord, Client))
    {
        PrintClientDetails(Client);
        Answer = InputLib::getYesNoAnswer("Are you sure you want update this client? (y/n): ");
        if (Answer == 'y')
        {
            cout << "\n\n";
            for (stClient &C : vClientRecord)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }
            SaveCleintsDataToFile(ClientsFileName, vClientRecord);
            cout << "\n\nClient Updated Successfully.";
            return true;
        }
        return false;
    }
    else
    {
        printf("\nClient With Account Number (%s) Not Found!.", AccountNumber.c_str());
        return false;
    }
}

bool DepositBalanceToClientAccount(float Amount, stClient &Client, vector<stClient> &vClientRecord, string Msg)
{
    char Answer = InputLib::getYesNoAnswer("\n\nAre you sure you want to perform this transaction? (y/n): ");
    if (Answer == 'y')
    {
        Client.AccountBalance += Amount;
        SaveCleintsDataToFile(ClientsFileName, vClientRecord);
        cout << Msg << Client.AccountBalance;
        return true;
    }
    return false;
}

void AddNewClient(vector <stClient> &vClientRecord)
{
    stClient Client;
    Client = ReadNewClient(vClientRecord);
    vClientRecord.push_back(Client);
    AddDataLineToFile(ClientsFileName, ConvertRecordToOneLine(Client));
}

string Tabs(short Num)
{
    string Result = "";
    for (short i = 1; i <= Num; i++)
    {
        Result += "\t";
    }
    return Result;
}

void ResetTheScreen()
{
    system("color 0F");
    system("cls");
}

void ShowClientsList(const vector<stClient> &vClientRecord)
{
    cout << setw(116) << "Client List (" + to_string(vClientRecord.size()) + ") Client(s).                                                             \n";
    cout << "____________________________________________________________________________________________________________________\n\n";
    cout << "| " << left << setw(15) << "Account Number" << "| " << left << setw(9) << "Pin Code" << "| " << left << setw(55) << "Client Name" << "| " << left << setw(13) << "Phone" << "| " << left << setw(13) << "Balance" << "|";
    cout << "\n____________________________________________________________________________________________________________________\n";
    for (const stClient &Record : vClientRecord)
    {
        PrintClientRecord(Record);
    }
    cout << "\n____________________________________________________________________________________________________________________";
}

void AddNewClients(vector <stClient> &vClientRecord)
{
    char AddNew = ' ';
    do
    {
        ResetTheScreen();
        cout << "----------------------------------------\n";
        cout << Tabs(1) << "Add New Clients Screen" << Tabs(1);
        cout << "\n----------------------------------------\n";
        cout << "Adding New Client:-\n\n";
        AddNewClient(vClientRecord);
        AddNew = InputLib::getYesNoAnswer("\nClient Added Successfully, do you want to add more clients?: ");
    } while (AddNew == 'y');
}

void DeleteClient(vector <stClient> &vClientRecord)
{
    cout << "----------------------------------------\n";
    cout << Tabs(1) << "Delete Client Screen" << Tabs(1);
    cout << "\n----------------------------------------\n\n";
    DeleteClientByAccountNumber(InputLib::ReadText("Please Enter Account Number?: "), vClientRecord);
}

void UpdateClientInfo(vector <stClient> &vClientRecord)
{
    cout << "-------------------------------------------\n";
    cout << Tabs(1) << "Update Client Info Screen" << Tabs(1);
    cout << "\n-------------------------------------------\n\n";
    UpdateClientByAccountNumber(InputLib::ReadText("Please Enter Account Number?: "), vClientRecord);
}

void FindClient(const vector <stClient> &vClientRecord)
{
    stClient Client;
    cout << "--------------------------------------\n";
    cout << Tabs(1) << "Find Client Screen" << Tabs(1);
    cout << "\n--------------------------------------\n\n";
    string AccountNumber = InputLib::ReadText("Please Enter Account Number?: ");
    if (FindClientByAccountNumber(AccountNumber, vClientRecord, Client))
    {
        PrintClientDetails(Client);
    }
    else
    {
        printf("\nClient With Account Number (%s) Not Found!.", AccountNumber.c_str());
    }
}

void Deposit(vector<stClient> &vClientRecord)
{
    string AccountNumber = "";
    cout << "-------------------------------\n";
    cout << Tabs(1) << "Deposit Screen" << Tabs(1);
    cout << "\n-------------------------------\n\n";
    AccountNumber = InputLib::ReadText("Please Enter Account Number?: ");
    short ClientIndex = FindClientIndexByAccountNumber(AccountNumber, vClientRecord);
    
    while (ClientIndex == -1)
    {
        printf("\nClient With Account Number (%s) Not Found!.\n\n", AccountNumber.c_str());
        AccountNumber = InputLib::ReadText("Please Enter Account Number?: ");
        ClientIndex = FindClientIndexByAccountNumber(AccountNumber, vClientRecord);
    }

    PrintClientDetails(vClientRecord[ClientIndex]);

    float DepositAmount = InputLib::ReadFloatPositiveNumber("Please enter deposit amount?: ");
    DepositBalanceToClientAccount(DepositAmount, vClientRecord[ClientIndex], vClientRecord, "\n\nDeposit Done Successfully New Balance is: ");
}

void Withdraw(vector<stClient> &vClientRecord)
{
    string AccountNumber = "";
    cout << "--------------------------------\n";
    cout << Tabs(1) << "Withdraw Screen" << Tabs(1);
    cout << "\n--------------------------------\n\n";
    AccountNumber = InputLib::ReadText("Please Enter Account Number?: ");
    short ClientIndex = FindClientIndexByAccountNumber(AccountNumber, vClientRecord);
    
    while (ClientIndex == -1)
    {
        printf("\nClient With Account Number (%s) Not Found!.\n\n", AccountNumber.c_str());
        AccountNumber = InputLib::ReadText("Please Enter Account Number?: ");
        ClientIndex = FindClientIndexByAccountNumber(AccountNumber, vClientRecord);
    }

    PrintClientDetails(vClientRecord[ClientIndex]);

    float WithdrawAmount = InputLib::ReadFloatPositiveNumber("Please enter withdraw amount?: ");
     while (vClientRecord[ClientIndex].AccountBalance < WithdrawAmount)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << vClientRecord[ClientIndex].AccountBalance;
        WithdrawAmount = InputLib::ReadFloatPositiveNumber("\n\nPlease enter withdraw amount?: ");
    }
    DepositBalanceToClientAccount(WithdrawAmount * -1, vClientRecord[ClientIndex], vClientRecord, "\n\nWithdraw Done Successfully New Balance is: ");
}

void ShowBalanceList(const vector<stClient> &vClientRecord)
{
    float TotalBalances = 0;
    cout << setw(116) << "Balance List (" + to_string(vClientRecord.size()) + ") Client(s).                                             \n";
    cout << "____________________________________________________________________________________________________________________\n\n";
    cout << "| " << left << setw(15) << "Account Number" << "| " << left << setw(53) << "Client Name" << "| " << left << setw(41) << "Balance" << "|";
    cout << "\n____________________________________________________________________________________________________________________\n";
    for (const stClient &Record : vClientRecord)
    {
        PrintClientBalanceDetails(Record);
        TotalBalances += Record.AccountBalance;
    }
    cout << "\n____________________________________________________________________________________________________________________\n\n";
    printf("%sTotal Balances = %.3f", Tabs(5).c_str(), TotalBalances);
}

void Exit()
{
    cout << "-----------------------------------\n";
    cout << Tabs(1) << "Program Ends :-)" << Tabs(1);
    cout << "\n-----------------------------------\n";
    exit(0);
}

void GoBackToTransactionsMenu()
{
    cout << "\n\nPress any key to go back to Transactions menu screen...";
    _getch();
}

void GoBackToMainMenu()
{
    cout << "\n\nPress any key to go back to main menu screen...";
    _getch();
}

void ShowTransactionsMenuScreen()
{
    ResetTheScreen();
    cout << "-------------------------------------------\n";
    cout << Tabs(1) << "Transactions Menu Screen" << Tabs(1);
    cout << "\n-------------------------------------------\n";
    cout << Tabs(1) << "[1] Deposit.\n";
    cout << Tabs(1) << "[2] Withdraw.\n";
    cout << Tabs(1) << "[3] Total Balances.\n";
    cout << Tabs(1) << "[4] Main Menu.\n";
    cout << "-------------------------------------------\n";
}

void TransactionsDoUserChoice(enumTransactions Choice, vector<stClient> &vClientRecord)
{
    ResetTheScreen();
    switch (Choice)
    {
        case enumTransactions::enDeposit:
            Deposit(vClientRecord);
            GoBackToTransactionsMenu();
            break;
        case enumTransactions::enWithdraw:
            Withdraw(vClientRecord);
            GoBackToTransactionsMenu();
            break;
        case enumTransactions::enTotalBalances:
            ShowBalanceList(vClientRecord);
            GoBackToTransactionsMenu();
            break;
        case enumTransactions::enMainMenu:
            break;
    }
}

void TransactionsMenu(vector<stClient> &vClientRecord)
{
    short Answer = 0;
    do
    {
        ShowTransactionsMenuScreen();
        Answer = InputLib::ReadIntNumberInRange(1, 4, "Choose what do you want to do? [1 to 4]: ");
        TransactionsDoUserChoice((enumTransactions) Answer, vClientRecord);
    } while (Answer != 4);
}

void ShowMainMenuScreen()
{
    ResetTheScreen();
    cout << "*************************************************\n";
    cout << Tabs(2) << "Main Menu Screen" << Tabs(2);
    cout << "\n*************************************************\n";
    cout << Tabs(1) << "[1] Show Client List.\n";
    cout << Tabs(1) << "[2] Add New Client.\n";
    cout << Tabs(1) << "[3] Delete Client.\n";
    cout << Tabs(1) << "[4] Update Client Info.\n";
    cout << Tabs(1) << "[5] Find Client.\n";
    cout << Tabs(1) << "[6] Transactions.\n";
    cout << Tabs(1) << "[7] Exit.\n";
    cout << "*************************************************\n";
}

void MenuDoUserChoice(enMenu Choice, vector<stClient> &vClientRecord)
{
    ResetTheScreen();
    switch (Choice)
    {
        case enMenu::enShowClientsList:
            ShowClientsList(vClientRecord);
            GoBackToMainMenu();
            break;
        case enMenu::enAddNewClients:
            AddNewClients(vClientRecord);
            GoBackToMainMenu();
            break;
        case enMenu::enDeleteClient:
            DeleteClient(vClientRecord);
            GoBackToMainMenu();
            break;
        case enMenu::enUpdateClientInfo:
            UpdateClientInfo(vClientRecord);
            GoBackToMainMenu();
            break;
        case enMenu::enFindClient:
            FindClient(vClientRecord);
            GoBackToMainMenu();
            break;
        case enMenu::enTransactions:
            TransactionsMenu(vClientRecord);
            break;
        case enMenu::enExit:
            Exit();
            break;
    }
}

void StartTheProgram()
{
    vector <stClient> vClientRecord = LoadFillContentToVector(ClientsFileName);
    short Answer = 0;
    do
    {
        ShowMainMenuScreen();
        Answer = InputLib::ReadIntNumberInRange(1, 7, "Choose what do you want to do? [1 to 7]: ");
        MenuDoUserChoice((enMenu) Answer, vClientRecord);
    } while (Answer != 7);
}

int main()
{
    StartTheProgram();
    return 0;
}