#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <conio.h>
#include "InputLib.h"

using namespace std;

const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";

void ShowMainMenu();
void ShowTransactionsMenu();
void ShowManageUsersMenu();
void GoBackToMainMenu();
void GoBackToTransactionsMenu();
void GoBackToManageUsersMenu();

enum enMainMenuOptions 
{ 
    eListClients          = 1, 
    eAddNewClient         = 2, 
    eDeleteClient         = 3, 
    eUpdateClient         = 4, 
    eFindClient           = 5, 
    eShowTransactionsMenu = 6, 
    eManageUsers          = 7, 
    eLogout               = 8
};

enum enTransactionsMenuOptions 
{ 
    eDeposit          = 1, 
    eWithdraw         = 2, 
    eShowTotalBalance = 3, 
    eShowMainMenu     = 4 
};

enum enManageUsersMenuOptions 
{ 
    eListUsers    = 1,
    eAddNewUser   = 2,
    eDeleteUser   = 3,
    eUpdateUser    = 4,
    eFindUser      = 5,
    eReturnToMainMenu = 6
};

enum enMainMenuePermissions
{
    eAll                   = -1,
    pListClients           = 1,
    pAddNewClient          = 2,
    pDeleteClient          = 4,
    pUpdateClient          = 8,
    pFindClient            = 16,
    pTranactions           = 32,
    pManageUsers           = 64
};

struct stClient
{
    string AccountNumber = "";
    string PinCode = "";
    string Name = "";
    string PhoneNumber = "";
    double AccountBalance = 0;
    bool MarkForDelete = false;
};

struct stUser
{
    string Name = "";
    string PassWord = "";
    int Permission = 0;
    bool MarkForDelete = false;
};

stUser CurrentUser;

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

        if (isAllDigits) return pin;
        else cout << "\nError: PIN must contain numbers only (no letters or signs).\n\n";
    }
}

void ResetTheScreen()
{
    system("color 0F");
    system("cls");
}

string Tabs(short Num)
{
    string Result = "";
    for (short i = 1; i <= Num; i++) Result += "\t";
    return Result;
}

vector<string> SplitString(string S1, string Delim = "#//#")
{
    vector<string> vString;
    short pos = 0;
    string sWord;
    while ((pos = S1.find(Delim)) != string::npos)
    {
        sWord = S1.substr(0, pos);
        if (sWord != "") vString.push_back(sWord);
        S1.erase(0, pos + Delim.length());
    }
    if (S1 != "") vString.push_back(S1);
    return vString;
}

stClient ConvertClientLineToRecord(string Line, string Seperator = "#//#")
{
    stClient Client;
    vector<string> vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.PhoneNumber = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);

    return Client;
}

string ConvertClientRecordToLine(stClient Client, string Seperator = "#//#")
{
    ostringstream ss;
    ss << fixed << setprecision(6) << Client.AccountBalance;
    
    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.PhoneNumber + Seperator;
    stClientRecord += ss.str();

    return stClientRecord;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            stClient Client = ConvertClientLineToRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
        }
        MyFile.close();
    }
    return false;
}

vector<stClient> LoadClientsDataFromFile(string FileName)
{
    vector<stClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            if (Line != "")
                vClients.push_back(ConvertClientLineToRecord(Line));
        }
        MyFile.close();
    }
    return vClients;
}

void SaveClientsDataToFile(string FileName, vector<stClient> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    if (MyFile.is_open())
    {
        for (stClient C : vClients)
        {
            if (!C.MarkForDelete)
            {
                MyFile << ConvertClientRecordToLine(C) << endl;
            }
        }
        MyFile.close();
    }
}

bool FindClientByAccountNumber(string AccountNumber, vector<stClient> &vClients, stClient &Client)
{
    for (stClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

short FindClientIndexByAccountNumber(string AccountNumber, const vector<stClient> &vClients)
{
    for (short i = 0; i < vClients.size(); i++)
    {
        if (vClients[i].AccountNumber == AccountNumber) return i;
    }
    return -1;
}

void PrintClientRecordLine(stClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.PhoneNumber;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintClientCard(stClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccount Number: " << Client.AccountNumber;
    cout << "\nPin Code      : " << Client.PinCode;
    cout << "\nName          : " << Client.Name;
    cout << "\nPhone         : " << Client.PhoneNumber;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";
}

stUser ConvertUserLineToRecord(string Line, string Seperator = "#//#")
{
    stUser User;
    vector<string> vUserData = SplitString(Line, Seperator);

    User.Name = vUserData[0];
    User.PassWord = vUserData[1];
    User.Permission = stoi(vUserData[2]);

    return User;
}

string ConvertUserRecordToLine(stUser User, string Seperator = "#//#")
{
    string stUserRecord = "";

    stUserRecord += User.Name + Seperator;
    stUserRecord += User.PassWord + Seperator;
    stUserRecord += to_string(User.Permission);

    return stUserRecord;
}

vector<stUser> LoadUsersDataFromFile(string FileName)
{
    vector<stUser> vUsers;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            if (Line != "")
                vUsers.push_back(ConvertUserLineToRecord(Line));
        }
        MyFile.close();
    }
    return vUsers;
}

bool FindUserByUserNameAndPassWord(string UserName, string PassWord, stUser &User)
{
    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    for (stUser U : vUsers)
    {
        if (U.Name == UserName && U.PassWord == PassWord)
        {
            User = U;
            return true;
        }
    }
    return false;
}

void ShowAccessDeniedMessage()
{
    cout << "\n====================================";
    cout << "\nAccessDenied,";
    cout << "\nYou dont have permission To Do this,";
    cout << "\nPlease Contact Your Admin.";
    cout << "\n====================================";
}

void ShowLoginScreen()
{
    ResetTheScreen();
    cout << "===========================================\n";
    cout << Tabs(2) << "Login Screen" << Tabs(2) << "\n";
    cout << "===========================================\n";
}

void PrintUserRecordLine(stUser User)
{
    cout << "| " << setw(20) << left << User.Name;
    cout << "| " << setw(10) << left << User.PassWord;
    cout << "| " << setw(12) << left << User.Permission;
}

void ShowAllUsersScreen()
{
    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    cout << "\n\t\t\t\t\tUsers List (" << vUsers.size() << ") User(s).";
    cout << "\n_________________________________________________________________________________________\n" << endl;
    cout << "| " << left << setw(20) << "User Name" 
         << "| " << left << setw(10) << "Password" 
         << "| " << left << setw(12) << "Permissions";
    cout << "\n_________________________________________________________________________________________\n" << endl;

    for (stUser User : vUsers)
    {
        PrintUserRecordLine(User);
        cout << endl;
    }

    cout << "\n_________________________________________________________________________________________\n" << endl;
}

bool UserExistsByUserName(string UserName, string FileName)
{
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            stUser User = ConvertUserLineToRecord(Line);
            if (User.Name == UserName)
            {
                MyFile.close();
                return true;
            }
        }
        MyFile.close();
    }
    return false;
}

int ReadPermissionsToSet()
{
    char Answer = 'N';

    Answer = InputLib::getYesNoAnswer("\nDo you want to give full access? (y/n): ");
    if (toupper(Answer) == 'Y') return enMainMenuePermissions::eAll;

    int Permission = 0;

    cout << "\nDo you want to give access to :\n\n";

    Answer = InputLib::getYesNoAnswer("Show Clients List? (y/n): ");
    if (toupper(Answer) == 'Y') Permission |= enMainMenuePermissions::pListClients;

    Answer = InputLib::getYesNoAnswer("\nAdd New Client? (y/n): ");
    if (toupper(Answer) == 'Y') Permission |= enMainMenuePermissions::pAddNewClient;

    Answer = InputLib::getYesNoAnswer("\nDelete Client? (y/n): ");
    if (toupper(Answer) == 'Y') Permission |= enMainMenuePermissions::pDeleteClient;

    Answer = InputLib::getYesNoAnswer("\nUpdate Client? (y/n): ");
    if (toupper(Answer) == 'Y') Permission |= enMainMenuePermissions::pUpdateClient;

    Answer = InputLib::getYesNoAnswer("\nFind Client? (y/n): ");
    if (toupper(Answer) == 'Y') Permission |= enMainMenuePermissions::pFindClient;

    Answer = InputLib::getYesNoAnswer("\nTranactions? (y/n): ");
    if (toupper(Answer) == 'Y') Permission |= enMainMenuePermissions::pTranactions;

    Answer = InputLib::getYesNoAnswer("\nManageUsers? (y/n): ");
    if (toupper(Answer) == 'Y') Permission |= enMainMenuePermissions::pManageUsers;

    return Permission;
}

bool CheckAccessPermission(enMainMenuePermissions Permission)
{
    if (CurrentUser.Permission == enMainMenuePermissions::eAll) return true;

    return ((Permission & CurrentUser.Permission) == Permission);
}

stUser ReadNewUser()
{
    stUser User;
    User.Name = InputLib::ReadText("Enter UserName: ");
    while (UserExistsByUserName(User.Name, UsersFileName))
    {
        cout << "\nUser with [" << User.Name << "] already exists, Enter another UserName.";
        User.Name = InputLib::ReadText("Enter UserName: ");
    }
    
    User.PassWord = InputLib::ReadText("Enter PassWord: ");
    User.Permission = ReadPermissionsToSet();

    return User;
}

void AddDataLineToFile(string FileName, string stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);
    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;
        MyFile.close();
    } 
}

void AddNewUser()
{
    stUser User = ReadNewUser();
    AddDataLineToFile(UsersFileName, ConvertUserRecordToLine(User));
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber,vector <stClient> &vClients)
{
    for (stClient &C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}
bool MarkUserForDeleteByUsername(string Username, vector <stUser> &vUsers)
{
    for (stUser &U : vUsers)
    {
        if (U.Name == Username)
        {
            U.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

void ShowAddNewUsersScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Users Screen";
    cout << "\n-----------------------------------\n";
    
    char AddMore = 'Y';
    do
    {
        cout << "Adding New User:\n\n";

        AddNewUser();
        AddMore = InputLib::getYesNoAnswer("\nUser Added Successfully, do you want to add more Users? (y/n): ");
    } while (toupper(AddMore) == 'Y');
}

void PrintUserCard(stUser User)
{
    cout << "\nThe following are the User details:\n";
    cout << "-----------------------------------";
    cout << "\nUserName    : " << User.Name;
    cout << "\nPassWord    : " << User.PassWord;
    cout << "\nPermissions : " << User.Permission;
    cout << "\n-----------------------------------\n";
}

void SaveUsersDataToFile(string FileName, vector<stUser> vUsers)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    if (MyFile.is_open())
    {
        for (stUser &U : vUsers)
        {
            if (!U.MarkForDelete)
            {
                MyFile << ConvertUserRecordToLine(U) << endl;
            }
        }
        MyFile.close();
    }
}

bool FindUserByUsername(string Username, vector <stUser> vUsers,stUser& User)
{
    for (stUser U : vUsers)
    {
        if (U.Name == Username)
        {
            User = U;
            return true;
        }
    }
    return false;
}

void ShowDeleteUserScreen()
{
    cout << "\n---------------------------------\n";
    cout << "\tDelete User Screen";
    cout << "\n---------------------------------\n";

    string UserName = InputLib::ReadText("Please enter UserName:  ");

    if (UserName == "Admin")
    {
        cout << "\n\nYou Can't Delete This User.";
        return;
    }

    if (UserName == CurrentUser.Name)
    {
        cout << "\n\nYou Can't Delete Your Own Logged-in Account!";
        return;
    }

    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    stUser User;

    if (FindUserByUsername(UserName, vUsers, User))
    {
        PrintUserCard(User);
        char Answer = InputLib::getYesNoAnswer("\n\nAre you sure you want delete this User? (y/n): ");
        if (Answer == 'y')
        {
            MarkUserForDeleteByUsername(UserName, vUsers);
            SaveUsersDataToFile(UsersFileName, vUsers);
            vUsers = LoadUsersDataFromFile(UsersFileName);
            cout << "\n\nUser Deleted Successfully.";
        }
    }
    else cout << "\nUser with UserName (" << UserName << ") is Not Found!";
}

stUser ChangeUserRecord(string UserName)
{
    stUser User;

    User.Name = UserName;
    User.PassWord = InputLib::ReadText("Enter Password: ");
    User.Permission = ReadPermissionsToSet();

    return User;
}

void ShowUpdateUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate User Info Screen";
    cout << "\n-----------------------------------\n";

    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    string UserName = InputLib::ReadText("Please enter UserName: ");
    stUser User;

    if (FindUserByUsername(UserName, vUsers, User))
    {
        PrintUserCard(User);
        char Answer = InputLib::getYesNoAnswer("\n\nAre you sure you want update this User? (y/n): ");
        if (Answer == 'y')
        {
            for (stUser &U : vUsers)
            {
                if (U.Name == UserName)
                {
                    U = ChangeUserRecord(UserName);
                    break;
                }
            }
            SaveUsersDataToFile(UsersFileName, vUsers);
            cout << "\n\nUser Updated Successfully.";
        }
    }
    else cout << "\nUser with UserName (" << UserName << ") is Not Found!";
}

void ShowFindUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind User Screen";
    cout << "\n-----------------------------------\n";

    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    string UserName = InputLib::ReadText("Please enter UserName: ");
    stUser User;

    if (FindUserByUsername(UserName, vUsers, User)) PrintUserCard(User);
    else cout << "\nUser with UserName (" << UserName << ") is Not Found!";
}

void ShowAllClientsScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pListClients))
    {
        ShowAccessDeniedMessage();
        return;
    }

    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_____________________________________________________________________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Account Number" << "| " << left << setw(10) << "Pin Code" << "| " << left << setw(40) << "Client Name" << "| " << left << setw(12) << "Phone" << "| " << left << setw(12) << "Balance";
    cout << "\n_____________________________________________________________________________________________________\n" << endl;

    if (vClients.size() == 0) cout << "\t\t\t\tNo Clients Available In the System!";
    else
    {
        for (stClient Client : vClients)
        {
            PrintClientRecordLine(Client);
            cout << endl;
        }
    }
    cout << "\n_____________________________________________________________________________________________________\n" << endl;
}

stClient ReadNewClient()
{
    stClient Client;
    Client.AccountNumber = InputLib::ReadText("Enter Account Number? ");
    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        Client.AccountNumber = InputLib::ReadText("Enter Account Number? ");
    }
    Client.PinCode = GetValidPIN();
    Client.Name = InputLib::ReadText("Enter Name? ");
    Client.PhoneNumber = InputLib::ReadText("Enter Phone? ");
    Client.AccountBalance = InputLib::ReadFloatPositiveNumber("Enter AccountBalance? ");
    return Client;
}

void AddNewClient()
{
    stClient Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertClientRecordToLine(Client));
}

void ShowAddNewClientsScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pAddNewClient))
    {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";
    
    char AddMore = 'Y';
    do {
        cout << "Adding New Client:\n\n";
        
        AddNewClient();
        AddMore = InputLib::getYesNoAnswer("\nClient Added Successfully, do you want to add more clients? (y/n): ");
    } while (toupper(AddMore) == 'Y');
}

void ShowDeleteClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pDeleteClient))
    {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = InputLib::ReadText("Please enter AccountNumber? ");
    stClient Client;

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        char Answer = InputLib::getYesNoAnswer("\n\nAre you sure you want delete this client? (y/n): ");
        if (Answer == 'y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveClientsDataToFile(ClientsFileName, vClients);
            vClients = LoadClientsDataFromFile(ClientsFileName);
            cout << "\n\nClient Deleted Successfully.";
        }
    }
    else cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
}

stClient ChangeClientRecord(string AccountNumber)
{
    stClient Client;

    Client.AccountNumber = AccountNumber;
    Client.PinCode = GetValidPIN();
    Client.Name = InputLib::ReadText("Enter Name? ");
    Client.PhoneNumber = InputLib::ReadText("Enter Phone? ");
    Client.AccountBalance = InputLib::ReadFloatPositiveNumber("Enter AccountBalance? ");

    return Client;
}

void ShowUpdateClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pUpdateClient))
    {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = InputLib::ReadText("Please enter AccountNumber? ");
    stClient Client;

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        char Answer = InputLib::getYesNoAnswer("\n\nAre you sure you want update this client? (y/n): ");
        if (Answer == 'y')
        {
            for (stClient &C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }
            SaveClientsDataToFile(ClientsFileName, vClients);
            cout << "\n\nClient Updated Successfully.";
        }
    }
    else cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
}

void ShowFindClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pFindClient))
    {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = InputLib::ReadText("Please enter AccountNumber? ");
    stClient Client;

    if (FindClientByAccountNumber(AccountNumber, vClients, Client)) PrintClientCard(Client);
    else cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";
}

void ShowDepositScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";

    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = InputLib::ReadText("Please enter AccountNumber? ");
    short ClientIndex = FindClientIndexByAccountNumber(AccountNumber, vClients);

    while (ClientIndex == -1)
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = InputLib::ReadText("Please enter AccountNumber? ");
        ClientIndex = FindClientIndexByAccountNumber(AccountNumber, vClients);
    }

    PrintClientCard(vClients[ClientIndex]);
    double Amount = InputLib::ReadFloatPositiveNumber("\nPlease enter deposit amount? ");

    char Answer = InputLib::getYesNoAnswer("\n\nAre you sure you want perform this transaction? (y/n): ");
    if (Answer == 'y')
    {
        vClients[ClientIndex].AccountBalance += Amount;
        SaveClientsDataToFile(ClientsFileName, vClients);
        cout << "\n\nDone Successfully. New balance is: " << vClients[ClientIndex].AccountBalance;
    }
}

void ShowWithDrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";

    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = InputLib::ReadText("Please enter AccountNumber? ");
    short ClientIndex = FindClientIndexByAccountNumber(AccountNumber, vClients);

    while (ClientIndex == -1)
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = InputLib::ReadText("Please enter AccountNumber? ");
        ClientIndex = FindClientIndexByAccountNumber(AccountNumber, vClients);
    }

    PrintClientCard(vClients[ClientIndex]);
    double Amount = InputLib::ReadFloatPositiveNumber("\nPlease enter withdraw amount? ");

    while (Amount > vClients[ClientIndex].AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << vClients[ClientIndex].AccountBalance << endl;
        Amount = InputLib::ReadFloatPositiveNumber("Please enter another amount? ");
    }

    char Answer = InputLib::getYesNoAnswer("\n\nAre you sure you want perform this transaction? (y/n): ");
    if (Answer == 'y')
    {
        vClients[ClientIndex].AccountBalance -= Amount;
        SaveClientsDataToFile(ClientsFileName, vClients);
        cout << "\n\nDone Successfully. New balance is: " << vClients[ClientIndex].AccountBalance;
    }
}

void ShowTotalBalances()
{
    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n___________________________________________________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Account Number" << "| " << left << setw(40) << "Client Name" << "| " << left << setw(12) << "Balance";
    cout << "\n___________________________________________________________________________________\n" << endl;
    
    double TotalBalances = 0;
    if (vClients.size() == 0) cout << "\t\t\t\tNo Clients Available In the System!";
    else
    {
        for (stClient Client : vClients)
        {
            cout << "| " << setw(15) << left << Client.AccountNumber;
            cout << "| " << setw(40) << left << Client.Name;
            cout << "| " << setw(12) << left << Client.AccountBalance << endl;
            TotalBalances += Client.AccountBalance;
        }
    }
    cout << "\n___________________________________________________________________________________\n" << endl;
    printf("%sTotal Balances = %.3f", Tabs(3).c_str(), TotalBalances);
}

void ShowEndScreen()
{
    cout << "\n-----------------------------------\n";
    cout << Tabs(1) << "Program Ends :-)" << Tabs(1);
    cout << "\n-----------------------------------\n";
    exit(0);
}

void GoBackToMainMenu()
{
    cout << "\n\nPress any key to go back to Main Menu...";
    _getch();
    ShowMainMenu();
}

void GoBackToTransactionsMenu()
{
    cout << "\n\nPress any key to go back to Transactions Menu...";
    _getch();
    ShowTransactionsMenu();
}

void GoBackToManageUsersMenu()
{
    cout << "\n\nPress any key to go back to Manage Users Menu...";
    _getch();
    ShowManageUsersMenu();
}

bool LoadUserInfo(string UserName, string PassWord)
{
    return (FindUserByUserNameAndPassWord(UserName, PassWord, CurrentUser));
}

void Login()
{
    string UserName = "", PassWord = "";
    bool LoginField = false;

    do
    {
        ShowLoginScreen();

        if (LoginField)
        {
            cout << "Invalid UserName/Password!\n";
        }

        UserName = InputLib::ReadText("Enter UserName: ");
        PassWord = InputLib::ReadText("Enter PassWord: ");

        LoginField = !LoadUserInfo(UserName, PassWord);

    } while(LoginField);
    
    ShowMainMenu();
}

void PerformTransactionsMenuOption(enTransactionsMenuOptions TransactionMenuOption)
{
    switch (TransactionMenuOption)
    {
    case enTransactionsMenuOptions::eDeposit:
        ResetTheScreen();
        ShowDepositScreen();
        GoBackToTransactionsMenu();
        break;
    case enTransactionsMenuOptions::eWithdraw:
        ResetTheScreen();
        ShowWithDrawScreen();
        GoBackToTransactionsMenu();
        break;
    case enTransactionsMenuOptions::eShowTotalBalance:
        ResetTheScreen();
        ShowTotalBalances();
        GoBackToTransactionsMenu();
        break;
    case enTransactionsMenuOptions::eShowMainMenu:
        ShowMainMenu();
        break;
    }
}

void ShowTransactionsMenu()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pTranactions))
    {
        ShowAccessDeniedMessage();
        GoBackToMainMenu();
        return;
    }

    ResetTheScreen();
    cout << "===========================================\n";
    cout << Tabs(1) << "Transactions Menu Screen" << Tabs(1) << "\n";
    cout << "===========================================\n";
    cout << Tabs(1) << "[1] Deposit.\n";
    cout << Tabs(1) << "[2] Withdraw.\n";
    cout << Tabs(1) << "[3] Total Balances.\n";
    cout << Tabs(1) << "[4] Main Menu.\n";
    cout << "===========================================\n";
    
    short Choice = InputLib::ReadIntNumberInRange(1, 4, "Choose what do you want to do? [1 to 4]: ");
    PerformTransactionsMenuOption((enTransactionsMenuOptions)Choice);
}

void PerformManageUsersMenuOption(enManageUsersMenuOptions ManageUsersMenuOption)
{
    switch (ManageUsersMenuOption)
    {
    case enManageUsersMenuOptions::eListUsers:
        ResetTheScreen();
        ShowAllUsersScreen();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersMenuOptions::eAddNewUser:
        ResetTheScreen();
        ShowAddNewUsersScreen();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersMenuOptions::eDeleteUser:
        ResetTheScreen();
        ShowDeleteUserScreen();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersMenuOptions::eUpdateUser:
        ResetTheScreen();
        ShowUpdateUserScreen();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersMenuOptions::eFindUser:
        ResetTheScreen();
        ShowFindUserScreen();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersMenuOptions::eReturnToMainMenu:
        ResetTheScreen();
        ShowMainMenu();
        break;
    }
}

void ShowManageUsersMenu()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pManageUsers))
    {
        ShowAccessDeniedMessage();
        GoBackToMainMenu();
        return;
    }

    ResetTheScreen();
    cout << "=======================================================\n";
    cout << Tabs(2) << "Manage Users Menu Screen" << Tabs(2) << "\n";
    cout << "=======================================================\n";
    cout << Tabs(1) << "[1] List Users.\n";
    cout << Tabs(1) << "[2] Add New User.\n";
    cout << Tabs(1) << "[3] Delete User.\n";
    cout << Tabs(1) << "[4] Update User.\n";
    cout << Tabs(1) << "[5] Find User.\n";
    cout << Tabs(1) << "[6] Main Menu.\n";
    cout << "===========================================\n";
    
    short Choice = InputLib::ReadIntNumberInRange(1, 6, "Choose what do you want to do? [1 to 6]: ");
    PerformManageUsersMenuOption((enManageUsersMenuOptions)Choice);
}

void PerformMainMenuOption(enMainMenuOptions MainMenuOption)
{
    switch (MainMenuOption)
    {
    case enMainMenuOptions::eListClients:
        ResetTheScreen(); 
        ShowAllClientsScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eAddNewClient:
        ResetTheScreen();
        ShowAddNewClientsScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eDeleteClient:
        ResetTheScreen();
        ShowDeleteClientScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eUpdateClient:
        ResetTheScreen();
        ShowUpdateClientScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eFindClient:
        ResetTheScreen();
        ShowFindClientScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eShowTransactionsMenu:
        ResetTheScreen();
        ShowTransactionsMenu();
        break;
    case enMainMenuOptions::eManageUsers:
        ResetTheScreen();
        ShowManageUsersMenu();
        break;
    case enMainMenuOptions::eLogout:
        ResetTheScreen();
        Login();
        break;
    }
}

void ShowMainMenu()
{
    ResetTheScreen();
    cout << "===========================================\n";
    cout << Tabs(2) << "Main Menu Screen" << Tabs(2) << "\n";
    cout << "===========================================\n";
    cout << Tabs(1) << "[1] Show Client List.\n";
    cout << Tabs(1) << "[2] Add New Client.\n";
    cout << Tabs(1) << "[3] Delete Client.\n";
    cout << Tabs(1) << "[4] Update Client Info.\n";
    cout << Tabs(1) << "[5] Find Client.\n";
    cout << Tabs(1) << "[6] Transactions.\n";
    cout << Tabs(1) << "[7] Manage Users.\n";
    cout << Tabs(1) << "[8] Logout.\n";
    cout << "===========================================\n";
    
    short Choice = InputLib::ReadIntNumberInRange(1, 8, "Choose what do you want to do? [1 to 8]: ");
    PerformMainMenuOption((enMainMenuOptions)Choice);
}

int main()
{
    Login();
    return 0;
}