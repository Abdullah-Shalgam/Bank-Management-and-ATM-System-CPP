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

// ==========================================
// CONSTANTS & CONFIGURATIONS
// ==========================================
const string ClientsFileName = "Clients.txt";
const string UsersFileName   = "Users.txt";

// ==========================================
// ENUMS & STRUCTURES
// ==========================================
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
    eListUsers        = 1,
    eAddNewUser       = 2,
    eDeleteUser       = 3,
    eUpdateUser       = 4,
    eFindUser         = 5,
    eReturnToMainMenu = 6
};

enum enMainMenuPermissions
{
    eAll          = -1,
    pListClients  = 1,
    pAddNewClient = 2,
    pDeleteClient = 4,
    pUpdateClient = 8,
    pFindClient   = 16,
    pTransactions = 32,
    pManageUsers  = 64
};

struct stClient
{
    string AccountNumber = "";
    string PinCode        = "";
    string Name          = "";
    string PhoneNumber   = "";
    double AccountBalance = 0.0;
    bool MarkForDelete   = false;
};

struct stUser
{
    string Name           = "";
    string PassWord       = "";
    int Permission        = 0;
    bool MarkForDelete    = false;
};

// Global Logged-in User Context
stUser CurrentUser;

// ==========================================
// FORWARD DECLARATIONS
// ==========================================
void ShowMainMenu();
void ShowTransactionsMenu();
void ShowManageUsersMenu();
void GoBackToMainMenu();
void GoBackToTransactionsMenu();
void GoBackToManageUsersMenu();
void Login();

// ==========================================
// UTILITY & HELPER FUNCTIONS
// ==========================================
void ResetScreen()
{
    system("color 0F");
    system("cls");
}

string GetTabs(short count)
{
    string result = "";
    for (short i = 1; i <= count; i++) 
        result += "\t";
    return result;
}

vector<string> SplitString(string str, string delim = "#//#")
{
    vector<string> vTokens;
    size_t pos = 0;
    string token;
    while ((pos = str.find(delim)) != string::npos)
    {
        token = str.substr(0, pos);
        if (!token.empty()) vTokens.push_back(token);
        str.erase(0, pos + delim.length());
    }
    if (!str.empty()) vTokens.push_back(str);
    return vTokens;
}

string GetValidPIN()
{
    string pin;
    while (true)
    {
        cout << "Enter PinCode (4 digits): ";
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
        else cout << "\nError: PIN must contain digits only.\n\n";
    }
}

void AddDataLineToFile(string fileName, string recordLine)
{
    fstream MyFile;
    MyFile.open(fileName, ios::out | ios::app);
    if (MyFile.is_open())
    {
        MyFile << recordLine << endl;
        MyFile.close();
    }
}

// ==========================================
// PERMISSION MANAGEMENT
// ==========================================
bool CheckAccessPermission(enMainMenuPermissions permission)
{
    if (CurrentUser.Permission == enMainMenuPermissions::eAll) 
        return true;

    return ((permission & CurrentUser.Permission) == permission);
}

void ShowAccessDeniedMessage()
{
    cout << "\n====================================";
    cout << "\nAccess Denied!";
    cout << "\nYou do not have permission to perform this action.";
    cout << "\nPlease contact your administrator.";
    cout << "\n====================================\n";
}

int ReadPermissionsToSet()
{
    char answer = InputLib::getYesNoAnswer("\nDo you want to give full access? (y/n): ");
    if (toupper(answer) == 'Y') return enMainMenuPermissions::eAll;

    int permissions = 0;
    cout << "\nSelect granular permissions:\n\n";

    if (toupper(InputLib::getYesNoAnswer("Show Clients List? (y/n): ")) == 'Y') 
        permissions |= enMainMenuPermissions::pListClients;

    if (toupper(InputLib::getYesNoAnswer("\nAdd New Client? (y/n): ")) == 'Y') 
        permissions |= enMainMenuPermissions::pAddNewClient;

    if (toupper(InputLib::getYesNoAnswer("\nDelete Client? (y/n): ")) == 'Y') 
        permissions |= enMainMenuPermissions::pDeleteClient;

    if (toupper(InputLib::getYesNoAnswer("\nUpdate Client? (y/n): ")) == 'Y') 
        permissions |= enMainMenuPermissions::pUpdateClient;

    if (toupper(InputLib::getYesNoAnswer("\nFind Client? (y/n): ")) == 'Y') 
        permissions |= enMainMenuPermissions::pFindClient;

    if (toupper(InputLib::getYesNoAnswer("\nTransactions Access? (y/n): ")) == 'Y') 
        permissions |= enMainMenuPermissions::pTransactions;

    if (toupper(InputLib::getYesNoAnswer("\nManage Users Access? (y/n): ")) == 'Y') 
        permissions |= enMainMenuPermissions::pManageUsers;

    return permissions;
}

// ==========================================
// USER MANAGEMENT (DATA & LOGIC)
// ==========================================
stUser ConvertUserLineToRecord(string line, string separator = "#//#")
{
    stUser user;
    vector<string> vUserData = SplitString(line, separator);

    user.Name       = vUserData[0];
    user.PassWord   = vUserData[1];
    user.Permission = stoi(vUserData[2]);

    return user;
}

string ConvertUserRecordToLine(stUser user, string separator = "#//#")
{
    return user.Name + separator + user.PassWord + separator + to_string(user.Permission);
}

vector<stUser> LoadUsersDataFromFile(string fileName)
{
    vector<stUser> vUsers;
    fstream MyFile(fileName, ios::in);

    if (MyFile.is_open())
    {
        string line;
        while (getline(MyFile, line))
        {
            if (!line.empty())
                vUsers.push_back(ConvertUserLineToRecord(line));
        }
        MyFile.close();
    }
    return vUsers;
}

void SaveUsersDataToFile(string fileName, const vector<stUser>& vUsers)
{
    fstream MyFile(fileName, ios::out);
    if (MyFile.is_open())
    {
        for (const stUser& user : vUsers)
        {
            if (!user.MarkForDelete)
            {
                MyFile << ConvertUserRecordToLine(user) << endl;
            }
        }
        MyFile.close();
    }
}

bool FindUserByUsername(string username, const vector<stUser>& vUsers, stUser& user)
{
    for (const stUser& u : vUsers)
    {
        if (u.Name == username)
        {
            user = u;
            return true;
        }
    }
    return false;
}

bool FindUserByUserNameAndPassWord(string username, string password, stUser& user)
{
    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    for (const stUser& u : vUsers)
    {
        if (u.Name == username && u.PassWord == password)
        {
            user = u;
            return true;
        }
    }
    return false;
}

bool UserExistsByUserName(string username, string fileName)
{
    stUser user;
    vector<stUser> vUsers = LoadUsersDataFromFile(fileName);
    return FindUserByUsername(username, vUsers, user);
}

bool MarkUserForDeleteByUsername(string username, vector<stUser>& vUsers)
{
    for (stUser& user : vUsers)
    {
        if (user.Name == username)
        {
            user.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

void PrintUserRecordLine(stUser user)
{
    cout << "| " << setw(20) << left << user.Name;
    cout << "| " << setw(10) << left << user.PassWord;
    cout << "| " << setw(12) << left << user.Permission;
}

void PrintUserCard(stUser user)
{
    cout << "\nUser Details:\n";
    cout << "-----------------------------------";
    cout << "\nUserName    : " << user.Name;
    cout << "\nPassWord    : " << user.PassWord;
    cout << "\nPermissions : " << user.Permission;
    cout << "\n-----------------------------------\n";
}

stUser ReadNewUser()
{
    stUser user;
    user.Name = InputLib::ReadText("Enter UserName: ");

    while (UserExistsByUserName(user.Name, UsersFileName))
    {
        cout << "\nUser [" << user.Name << "] already exists. Enter another UserName: ";
        user.Name = InputLib::ReadText("");
    }
    
    user.PassWord   = InputLib::ReadText("Enter Password: ");
    user.Permission = ReadPermissionsToSet();

    return user;
}

stUser UpdateUserRecord(string username)
{
    stUser user;
    user.Name       = username;
    user.PassWord   = InputLib::ReadText("Enter Password: ");
    user.Permission = ReadPermissionsToSet();

    return user;
}

// ==========================================
// CLIENT MANAGEMENT (DATA & LOGIC)
// ==========================================
stClient ConvertClientLineToRecord(string line, string separator = "#//#")
{
    stClient client;
    vector<string> vClientData = SplitString(line, separator);

    client.AccountNumber  = vClientData[0];
    client.PinCode        = vClientData[1];
    client.Name           = vClientData[2];
    client.PhoneNumber    = vClientData[3];
    client.AccountBalance = stod(vClientData[4]);

    return client;
}

string ConvertClientRecordToLine(stClient client, string separator = "#//#")
{
    ostringstream ss;
    ss << fixed << setprecision(6) << client.AccountBalance;
    
    return client.AccountNumber + separator +
           client.PinCode + separator +
           client.Name + separator +
           client.PhoneNumber + separator +
           ss.str();
}

vector<stClient> LoadClientsDataFromFile(string fileName)
{
    vector<stClient> vClients;
    fstream MyFile(fileName, ios::in);

    if (MyFile.is_open())
    {
        string line;
        while (getline(MyFile, line))
        {
            if (!line.empty())
                vClients.push_back(ConvertClientLineToRecord(line));
        }
        MyFile.close();
    }
    return vClients;
}

void SaveClientsDataToFile(string fileName, const vector<stClient>& vClients)
{
    fstream MyFile(fileName, ios::out);
    if (MyFile.is_open())
    {
        for (const stClient& client : vClients)
        {
            if (!client.MarkForDelete)
            {
                MyFile << ConvertClientRecordToLine(client) << endl;
            }
        }
        MyFile.close();
    }
}

bool FindClientByAccountNumber(string accountNumber, const vector<stClient>& vClients, stClient& client)
{
    for (const stClient& c : vClients)
    {
        if (c.AccountNumber == accountNumber)
        {
            client = c;
            return true;
        }
    }
    return false;
}

short FindClientIndexByAccountNumber(string accountNumber, const vector<stClient>& vClients)
{
    for (short i = 0; i < vClients.size(); i++)
    {
        if (vClients[i].AccountNumber == accountNumber) return i;
    }
    return -1;
}

bool ClientExistsByAccountNumber(string accountNumber, string fileName)
{
    stClient client;
    vector<stClient> vClients = LoadClientsDataFromFile(fileName);
    return FindClientByAccountNumber(accountNumber, vClients, client);
}

bool MarkClientForDeleteByAccountNumber(string accountNumber, vector<stClient>& vClients)
{
    for (stClient& client : vClients)
    {
        if (client.AccountNumber == accountNumber)
        {
            client.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

void PrintClientRecordLine(stClient client)
{
    cout << "| " << setw(15) << left << client.AccountNumber;
    cout << "| " << setw(10) << left << client.PinCode;
    cout << "| " << setw(40) << left << client.Name;
    cout << "| " << setw(12) << left << client.PhoneNumber;
    cout << "| " << setw(12) << left << client.AccountBalance;
}

void PrintClientCard(stClient client)
{
    cout << "\nClient Details:\n";
    cout << "-----------------------------------";
    cout << "\nAccount Number : " << client.AccountNumber;
    cout << "\nPin Code       : " << client.PinCode;
    cout << "\nName           : " << client.Name;
    cout << "\nPhone          : " << client.PhoneNumber;
    cout << "\nAccount Balance: " << client.AccountBalance;
    cout << "\n-----------------------------------\n";
}

stClient ReadNewClient()
{
    stClient client;
    client.AccountNumber = InputLib::ReadText("Enter Account Number: ");

    while (ClientExistsByAccountNumber(client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient [" << client.AccountNumber << "] already exists. Enter another Account Number: ";
        client.AccountNumber = InputLib::ReadText("");
    }

    client.PinCode        = GetValidPIN();
    client.Name           = InputLib::ReadText("Enter Name: ");
    client.PhoneNumber    = InputLib::ReadText("Enter Phone: ");
    client.AccountBalance = InputLib::ReadFloatPositiveNumber("Enter Account Balance: ");

    return client;
}

stClient UpdateClientRecord(string accountNumber)
{
    stClient client;
    client.AccountNumber  = accountNumber;
    client.PinCode        = GetValidPIN();
    client.Name           = InputLib::ReadText("Enter Name: ");
    client.PhoneNumber    = InputLib::ReadText("Enter Phone: ");
    client.AccountBalance = InputLib::ReadFloatPositiveNumber("Enter Account Balance: ");

    return client;
}

// ==========================================
// USER INTERFACE & SCREENS (USERS)
// ==========================================
void ShowAllUsersScreen()
{
    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    cout << "\n\t\t\t\t\tUsers List (" << vUsers.size() << ") User(s).";
    cout << "\n_________________________________________________________________________________________\n" << endl;
    cout << "| " << left << setw(20) << "User Name" 
         << "| " << left << setw(10) << "Password" 
         << "| " << left << setw(12) << "Permissions";
    cout << "\n_________________________________________________________________________________________\n" << endl;

    for (const stUser& user : vUsers)
    {
        PrintUserRecordLine(user);
        cout << endl;
    }
    cout << "\n_________________________________________________________________________________________\n" << endl;
}

void ShowAddNewUsersScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Users Screen";
    cout << "\n-----------------------------------\n";
    
    char addMore = 'Y';
    do
    {
        cout << "Adding New User:\n\n";
        stUser user = ReadNewUser();
        AddDataLineToFile(UsersFileName, ConvertUserRecordToLine(user));
        
        addMore = InputLib::getYesNoAnswer("\nUser Added Successfully, do you want to add more Users? (y/n): ");
    } while (toupper(addMore) == 'Y');
}

void ShowDeleteUserScreen()
{
    cout << "\n---------------------------------\n";
    cout << "\tDelete User Screen";
    cout << "\n---------------------------------\n";

    string username = InputLib::ReadText("Please enter UserName: ");

    if (username == "Admin")
    {
        cout << "\n\nYou cannot delete the primary Admin account.";
        return;
    }

    if (username == CurrentUser.Name)
    {
        cout << "\n\nYou cannot delete your own logged-in account!";
        return;
    }

    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    stUser user;

    if (FindUserByUsername(username, vUsers, user))
    {
        PrintUserCard(user);
        char answer = InputLib::getYesNoAnswer("\n\nAre you sure you want to delete this User? (y/n): ");
        if (toupper(answer) == 'Y')
        {
            MarkUserForDeleteByUsername(username, vUsers);
            SaveUsersDataToFile(UsersFileName, vUsers);
            cout << "\n\nUser Deleted Successfully.";
        }
    }
    else cout << "\nUser with UserName (" << username << ") was Not Found!";
}

void ShowUpdateUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate User Info Screen";
    cout << "\n-----------------------------------\n";

    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    string username = InputLib::ReadText("Please enter UserName: ");
    stUser user;

    if (FindUserByUsername(username, vUsers, user))
    {
        PrintUserCard(user);
        char answer = InputLib::getYesNoAnswer("\n\nAre you sure you want to update this User? (y/n): ");
        if (toupper(answer) == 'Y')
        {
            for (stUser& u : vUsers)
            {
                if (u.Name == username)
                {
                    u = UpdateUserRecord(username);
                    break;
                }
            }
            SaveUsersDataToFile(UsersFileName, vUsers);
            cout << "\n\nUser Updated Successfully.";
        }
    }
    else cout << "\nUser with UserName (" << username << ") was Not Found!";
}

void ShowFindUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind User Screen";
    cout << "\n-----------------------------------\n";

    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    string username = InputLib::ReadText("Please enter UserName: ");
    stUser user;

    if (FindUserByUsername(username, vUsers, user)) PrintUserCard(user);
    else cout << "\nUser with UserName (" << username << ") was Not Found!";
}

// ==========================================
// USER INTERFACE & SCREENS (CLIENTS)
// ==========================================
void ShowAllClientsScreen()
{
    if (!CheckAccessPermission(enMainMenuPermissions::pListClients))
    {
        ShowAccessDeniedMessage();
        return;
    }

    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_____________________________________________________________________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Account Number" 
         << "| " << left << setw(10) << "Pin Code" 
         << "| " << left << setw(40) << "Client Name" 
         << "| " << left << setw(12) << "Phone" 
         << "| " << left << setw(12) << "Balance";
    cout << "\n_____________________________________________________________________________________________________\n" << endl;

    if (vClients.empty()) cout << "\t\t\t\tNo Clients Available In the System!";
    else
    {
        for (const stClient& client : vClients)
        {
            PrintClientRecordLine(client);
            cout << endl;
        }
    }
    cout << "\n_____________________________________________________________________________________________________\n" << endl;
}

void ShowAddNewClientsScreen()
{
    if (!CheckAccessPermission(enMainMenuPermissions::pAddNewClient))
    {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";
    
    char addMore = 'Y';
    do 
    {
        cout << "Adding New Client:\n\n";
        stClient client = ReadNewClient();
        AddDataLineToFile(ClientsFileName, ConvertClientRecordToLine(client));
        
        addMore = InputLib::getYesNoAnswer("\nClient Added Successfully, do you want to add more clients? (y/n): ");
    } while (toupper(addMore) == 'Y');
}

void ShowDeleteClientScreen()
{
    if (!CheckAccessPermission(enMainMenuPermissions::pDeleteClient))
    {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string accountNumber = InputLib::ReadText("Please enter AccountNumber: ");
    stClient client;

    if (FindClientByAccountNumber(accountNumber, vClients, client))
    {
        PrintClientCard(client);
        char answer = InputLib::getYesNoAnswer("\n\nAre you sure you want to delete this client? (y/n): ");
        if (toupper(answer) == 'Y')
        {
            MarkClientForDeleteByAccountNumber(accountNumber, vClients);
            SaveClientsDataToFile(ClientsFileName, vClients);
            cout << "\n\nClient Deleted Successfully.";
        }
    }
    else cout << "\nClient with Account Number (" << accountNumber << ") was Not Found!";
}

void ShowUpdateClientScreen()
{
    if (!CheckAccessPermission(enMainMenuPermissions::pUpdateClient))
    {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string accountNumber = InputLib::ReadText("Please enter AccountNumber: ");
    stClient client;

    if (FindClientByAccountNumber(accountNumber, vClients, client))
    {
        PrintClientCard(client);
        char answer = InputLib::getYesNoAnswer("\n\nAre you sure you want to update this client? (y/n): ");
        if (toupper(answer) == 'Y')
        {
            for (stClient& c : vClients)
            {
                if (c.AccountNumber == accountNumber)
                {
                    c = UpdateClientRecord(accountNumber);
                    break;
                }
            }
            SaveClientsDataToFile(ClientsFileName, vClients);
            cout << "\n\nClient Updated Successfully.";
        }
    }
    else cout << "\nClient with Account Number (" << accountNumber << ") was Not Found!";
}

void ShowFindClientScreen()
{
    if (!CheckAccessPermission(enMainMenuPermissions::pFindClient))
    {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string accountNumber = InputLib::ReadText("Please enter AccountNumber: ");
    stClient client;

    if (FindClientByAccountNumber(accountNumber, vClients, client)) PrintClientCard(client);
    else cout << "\nClient with Account Number [" << accountNumber << "] was Not Found!";
}

// ==========================================
// TRANSACTIONS SCREENS
// ==========================================
void ShowDepositScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";

    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string accountNumber = InputLib::ReadText("Please enter AccountNumber: ");
    short clientIndex = FindClientIndexByAccountNumber(accountNumber, vClients);

    while (clientIndex == -1)
    {
        cout << "\nClient with [" << accountNumber << "] does not exist.\n";
        accountNumber = InputLib::ReadText("Please enter AccountNumber: ");
        clientIndex = FindClientIndexByAccountNumber(accountNumber, vClients);
    }

    PrintClientCard(vClients[clientIndex]);
    double amount = InputLib::ReadFloatPositiveNumber("\nPlease enter deposit amount: ");

    char answer = InputLib::getYesNoAnswer("\n\nAre you sure you want to perform this transaction? (y/n): ");
    if (toupper(answer) == 'Y')
    {
        vClients[clientIndex].AccountBalance += amount;
        SaveClientsDataToFile(ClientsFileName, vClients);
        cout << "\n\nDone Successfully. New balance is: " << vClients[clientIndex].AccountBalance;
    }
}

void ShowWithdrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";

    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string accountNumber = InputLib::ReadText("Please enter AccountNumber: ");
    short clientIndex = FindClientIndexByAccountNumber(accountNumber, vClients);

    while (clientIndex == -1)
    {
        cout << "\nClient with [" << accountNumber << "] does not exist.\n";
        accountNumber = InputLib::ReadText("Please enter AccountNumber: ");
        clientIndex = FindClientIndexByAccountNumber(accountNumber, vClients);
    }

    PrintClientCard(vClients[clientIndex]);
    double amount = InputLib::ReadFloatPositiveNumber("\nPlease enter withdraw amount: ");

    while (amount > vClients[clientIndex].AccountBalance)
    {
        cout << "\nAmount exceeds available balance! Maximum allowed withdraw is: " 
             << vClients[clientIndex].AccountBalance << endl;
        amount = InputLib::ReadFloatPositiveNumber("Please enter another amount: ");
    }

    char answer = InputLib::getYesNoAnswer("\n\nAre you sure you want to perform this transaction? (y/n): ");
    if (toupper(answer) == 'Y')
    {
        vClients[clientIndex].AccountBalance -= amount;
        SaveClientsDataToFile(ClientsFileName, vClients);
        cout << "\n\nDone Successfully. New balance is: " << vClients[clientIndex].AccountBalance;
    }
}

void ShowTotalBalancesScreen()
{
    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n___________________________________________________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Account Number" 
         << "| " << left << setw(40) << "Client Name" 
         << "| " << left << setw(12) << "Balance";
    cout << "\n___________________________________________________________________________________\n" << endl;
    
    double totalBalances = 0.0;
    if (vClients.empty()) cout << "\t\t\t\tNo Clients Available In the System!";
    else
    {
        for (const stClient& client : vClients)
        {
            cout << "| " << setw(15) << left << client.AccountNumber;
            cout << "| " << setw(40) << left << client.Name;
            cout << "| " << setw(12) << left << client.AccountBalance << endl;
            totalBalances += client.AccountBalance;
        }
    }
    cout << "\n___________________________________________________________________________________\n" << endl;
    printf("%sTotal Balances = %.3f", GetTabs(3).c_str(), totalBalances);
}

// ==========================================
// NAVIGATION & MENU CONTROLLERS
// ==========================================
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

void PerformTransactionsMenuOption(enTransactionsMenuOptions option)
{
    switch (option)
    {
    case enTransactionsMenuOptions::eDeposit:
        ResetScreen();
        ShowDepositScreen();
        GoBackToTransactionsMenu();
        break;
    case enTransactionsMenuOptions::eWithdraw:
        ResetScreen();
        ShowWithdrawScreen();
        GoBackToTransactionsMenu();
        break;
    case enTransactionsMenuOptions::eShowTotalBalance:
        ResetScreen();
        ShowTotalBalancesScreen();
        GoBackToTransactionsMenu();
        break;
    case enTransactionsMenuOptions::eShowMainMenu:
        ShowMainMenu();
        break;
    }
}

void ShowTransactionsMenu()
{
    if (!CheckAccessPermission(enMainMenuPermissions::pTransactions))
    {
        ShowAccessDeniedMessage();
        GoBackToMainMenu();
        return;
    }

    ResetScreen();
    cout << "===========================================\n";
    cout << GetTabs(1) << "Transactions Menu Screen" << GetTabs(1) << "\n";
    cout << "===========================================\n";
    cout << GetTabs(1) << "[1] Deposit.\n";
    cout << GetTabs(1) << "[2] Withdraw.\n";
    cout << GetTabs(1) << "[3] Total Balances.\n";
    cout << GetTabs(1) << "[4] Main Menu.\n";
    cout << "===========================================\n";
    
    short choice = InputLib::ReadIntNumberInRange(1, 4, "Choose what do you want to do? [1 to 4]: ");
    PerformTransactionsMenuOption((enTransactionsMenuOptions)choice);
}

void PerformManageUsersMenuOption(enManageUsersMenuOptions option)
{
    switch (option)
    {
    case enManageUsersMenuOptions::eListUsers:
        ResetScreen();
        ShowAllUsersScreen();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersMenuOptions::eAddNewUser:
        ResetScreen();
        ShowAddNewUsersScreen();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersMenuOptions::eDeleteUser:
        ResetScreen();
        ShowDeleteUserScreen();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersMenuOptions::eUpdateUser:
        ResetScreen();
        ShowUpdateUserScreen();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersMenuOptions::eFindUser:
        ResetScreen();
        ShowFindUserScreen();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersMenuOptions::eReturnToMainMenu:
        ResetScreen();
        ShowMainMenu();
        break;
    }
}

void ShowManageUsersMenu()
{
    if (!CheckAccessPermission(enMainMenuPermissions::pManageUsers))
    {
        ShowAccessDeniedMessage();
        GoBackToMainMenu();
        return;
    }

    ResetScreen();
    cout << "=======================================================\n";
    cout << GetTabs(2) << "Manage Users Menu Screen" << GetTabs(2) << "\n";
    cout << "=======================================================\n";
    cout << GetTabs(1) << "[1] List Users.\n";
    cout << GetTabs(1) << "[2] Add New User.\n";
    cout << GetTabs(1) << "[3] Delete User.\n";
    cout << GetTabs(1) << "[4] Update User.\n";
    cout << GetTabs(1) << "[5] Find User.\n";
    cout << GetTabs(1) << "[6] Main Menu.\n";
    cout << "=======================================================\n";
    
    short choice = InputLib::ReadIntNumberInRange(1, 6, "Choose what do you want to do? [1 to 6]: ");
    PerformManageUsersMenuOption((enManageUsersMenuOptions)choice);
}

void PerformMainMenuOption(enMainMenuOptions option)
{
    switch (option)
    {
    case enMainMenuOptions::eListClients:
        ResetScreen(); 
        ShowAllClientsScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eAddNewClient:
        ResetScreen();
        ShowAddNewClientsScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eDeleteClient:
        ResetScreen();
        ShowDeleteClientScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eUpdateClient:
        ResetScreen();
        ShowUpdateClientScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eFindClient:
        ResetScreen();
        ShowFindClientScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eShowTransactionsMenu:
        ResetScreen();
        ShowTransactionsMenu();
        break;
    case enMainMenuOptions::eManageUsers:
        ResetScreen();
        ShowManageUsersMenu();
        break;
    case enMainMenuOptions::eLogout:
        ResetScreen();
        Login();
        break;
    }
}

void ShowMainMenu()
{
    ResetScreen();
    cout << "===========================================\n";
    cout << GetTabs(2) << "Main Menu Screen" << GetTabs(2) << "\n";
    cout << "===========================================\n";
    cout << GetTabs(1) << "[1] Show Client List.\n";
    cout << GetTabs(1) << "[2] Add New Client.\n";
    cout << GetTabs(1) << "[3] Delete Client.\n";
    cout << GetTabs(1) << "[4] Update Client Info.\n";
    cout << GetTabs(1) << "[5] Find Client.\n";
    cout << GetTabs(1) << "[6] Transactions.\n";
    cout << GetTabs(1) << "[7] Manage Users.\n";
    cout << GetTabs(1) << "[8] Logout.\n";
    cout << "===========================================\n";
    
    short choice = InputLib::ReadIntNumberInRange(1, 8, "Choose what do you want to do? [1 to 8]: ");
    PerformMainMenuOption((enMainMenuOptions)choice);
}

// ==========================================
// AUTHENTICATION & LOGIN
// ==========================================
void ShowLoginScreen()
{
    ResetScreen();
    cout << "===========================================\n";
    cout << GetTabs(2) << "Login Screen" << GetTabs(2) << "\n";
    cout << "===========================================\n";
}

void Login()
{
    string username = "", password = "";
    bool loginFailed = false;

    do
    {
        ShowLoginScreen();

        if (loginFailed)
        {
            cout << "Invalid Username/Password!\n\n";
        }

        username = InputLib::ReadText("Enter Username: ");
        password = InputLib::ReadText("Enter Password: ");

        loginFailed = !FindUserByUserNameAndPassWord(username, password, CurrentUser);

    } while (loginFailed);

    ShowMainMenu();
}

// ==========================================
// MAIN ENTRY POINT
// ==========================================
int main()
{
    Login();
    return 0;
}