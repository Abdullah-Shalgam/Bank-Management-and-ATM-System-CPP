#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <conio.h>
#include <regex>
#include "InputLib.h"

using namespace std;

// ==========================================
// 1. CONSTANTS & ENUMERATIONS
// ==========================================

const string ClientsFileName = "Clients.txt";
const short ConsoleWidth = 80;

enum enMainMenuOptions 
{ 
    eQuickWithdraw    = 1, 
    eNormalWithdraw   = 2, 
    eDeposit          = 3, 
    eCheckBalance     = 4,
    eMyInfo           = 5,
    eUpdateInfoMenu   = 6,
    eLogout           = 7
};

enum enUpdateInfoOptions
{
    eUpdateName  = 1,
    eUpdatePin   = 2,
    eUpdatePhone = 3,
    eMainMenu    = 4
};

enum enQuickWithdrawOptions
{
    eTwenty       = 1,
    eFifty        = 2,
    eOneHundred   = 3,
    eTwoHundred   = 4,
    eFourHundred  = 5,
    eSixHundred   = 6,
    eEightHundred = 7,
    eOneThousand  = 8,
    eExit         = 9
};

// ==========================================
// 2. STRUCTURES
// ==========================================

struct stClient
{
    string AccountNumber  = "";
    string PinCode        = "";
    string Name           = "";
    string PhoneNumber    = "";
    double AccountBalance = 0.0;
    bool MarkForDelete    = false;
};

// Global Variable for Logged-in Client
stClient CurrentClient;

// ==========================================
// 3. FORWARD DECLARATIONS
// ==========================================

void ShowUpdateInfoMenu();
void ShowMainMenu();
void GoBackToMainMenu();
void GoBackToUpdateInfoMenu();
void Login();

// ==========================================
// 4. VISUAL EFFECTS & UTILITIES
// ==========================================

void ResetScreen()
{
    system("color 0F");
    system("cls");
}

void PrintHeaderLine(char symbol = '=', short width = ConsoleWidth)
{
    cout << string(width, symbol) << "\n";
}

void PrintCentered(string text, short width = ConsoleWidth)
{
    if (text.length() >= width)
    {
        cout << text << "\n";
        return;
    }
    short padding = (width - text.length()) / 2;
    cout << string(padding, ' ') << text << "\n";
}

void TypeWriterText(string text, int speedInMs = 15)
{
    for (char c : text)
    {
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(speedInMs));
    }
}

void ShowSpinner(string Message = "Loading", int cycles = 2)
{
    char spinner[] = {'|', '/', '-', '\\'};
    cout << "\n\t" << Message << " ";
    
    for (int i = 0; i < cycles * 4; i++)
    {
        cout << spinner[i % 4] << "\b" << flush;
        this_thread::sleep_for(chrono::milliseconds(120));
    }
    cout << "Done!\n";
}

void ShowProgressBar(string Message = "Processing Transaction")
{
    cout << "\n\n\t" << Message << "\n\t[";
    for (int i = 0; i < 30; i++)
    {
        cout << "=" << flush;
        this_thread::sleep_for(chrono::milliseconds(30));
    }
    cout << "] 100% Completed!\n\n";
    this_thread::sleep_for(chrono::milliseconds(200));
}

void FlashWarningColor()
{
    system("color 4F");
    this_thread::sleep_for(chrono::milliseconds(150));
    system("color 6F");
    this_thread::sleep_for(chrono::milliseconds(150));
    system("color 0F");
}

void ShowATMWelcomeBanner()
{
    PrintHeaderLine('=');
    PrintCentered("A T M   B A N K I N G   S Y S T E M");
    PrintCentered("Automated Teller Machine - Secure Portal");
    PrintHeaderLine('=');
}

void ShowPrintedReceipt(string TransactionType, double Amount)
{
    char Answer = InputLib::getYesNoAnswer("\nWould you like to print a transaction receipt? (y/n): ");
    if (toupper(Answer) != 'Y') return;

    cout << "\n\n\t[ Printing Receipt... Please Wait ]\n";
    this_thread::sleep_for(chrono::milliseconds(400));

    stringstream ss;
    ss << "\n\t===================================="
       << "\n\t          ATM TRANSACTION RECEIPT   "
       << "\n\t===================================="
       << "\n\t Date/Time : " << __DATE__ << " " << __TIME__
       << "\n\t Account   : " << CurrentClient.AccountNumber
       << "\n\t Client    : " << CurrentClient.Name
       << "\n\t Type      : " << TransactionType
       << "\n\t Amount    : $" << fixed << setprecision(2) << Amount
       << "\n\t Balance   : $" << fixed << setprecision(2) << CurrentClient.AccountBalance
       << "\n\t===================================="
       << "\n\t   Thank you for banking with us!   "
       << "\n\t====================================\n\n";

    TypeWriterText(ss.str(), 10);
}

string ReadPinMasked(string Msg = "Enter PIN Code: ")
{
    string pin = "";
    char ch;

    cout << Msg;

    while (true)
    {
        ch = _getch();

        if (ch == 13) 
        {
            if (!pin.empty()) 
                break;
        }
        else if (ch == 8) 
        {
            if (!pin.empty())
            {
                pin.pop_back();
                cout << "\b \b";
            }
        }
        else if (isdigit(ch) && pin.length() < 4)
        {
            pin += ch;
            cout << '*';
        }
    }

    cout << endl;
    return pin;
}

string GetValidPIN(string Msg)
{
    string pin;
    while (true)
    {
        pin = ReadPinMasked(Msg);

        if (pin.length() != 4)
        {
            FlashWarningColor();
            cout << "\nError: PIN must be exactly 4 digits.\n\n";
            continue;
        }

        return pin;
    }
}

bool isValidAccountName(const string& name)
{
    if (name.length() < 8 || name.length() > 60) return false;
    if (name.front() == ' ' || name.back() == ' ') return false;

    for (size_t i = 0; i < name.length(); ++i) {
        if (i >= 3 && name[i] == name[i-1] && name[i] == name[i-2] && name[i] == name[i-3]) return false;
        if (i >= 1 && name[i] == ' ' && name[i-1] == ' ') return false;
    }

    std::regex bank_name_pattern("^[a-zA-Z]+([ '\\-\\.]?[a-zA-Z]+)+$");
    return std::regex_match(name, bank_name_pattern);
}

// ==========================================
// 5. FILE HANDLING & DATA ACCESS LAYER
// ==========================================

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

void SaveCurrentClientChanges()
{
    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    bool isUpdated = false;

    for (stClient& C : vClients)
    {
        if (C.AccountNumber == CurrentClient.AccountNumber)
        {
            C = CurrentClient;
            isUpdated = true;
            break;
        }
    }

    if (isUpdated)
    {
        SaveClientsDataToFile(ClientsFileName, vClients);
    }
}

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, stClient& Client)
{
    vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    for (const stClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

bool LoadClientInfo(string AccountNumber, string PinCode)
{
    return FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient);
}

// ==========================================
// 6. BUSINESS LOGIC & TRANSACTIONS
// ==========================================

short GetQuickWithdrawAmount(enQuickWithdrawOptions option)
{
    switch(option)
    {
    case enQuickWithdrawOptions::eTwenty:       return 20;
    case enQuickWithdrawOptions::eFifty:        return 50;
    case enQuickWithdrawOptions::eOneHundred:   return 100;
    case enQuickWithdrawOptions::eTwoHundred:   return 200;
    case enQuickWithdrawOptions::eFourHundred:  return 400;
    case enQuickWithdrawOptions::eSixHundred:   return 600;
    case enQuickWithdrawOptions::eEightHundred: return 800;
    case enQuickWithdrawOptions::eOneThousand:  return 1000;
    default:                                    return 0;
    }
}

bool IsValidWithdraw(double Amount)
{
    return (CurrentClient.AccountBalance >= Amount);
}

bool PerformWithdraw(double WithdrawAmount)
{
    if (!IsValidWithdraw(WithdrawAmount))
    {
        FlashWarningColor();
        cout << "\nError: The amount exceeds your available balance.";
        cout << "\nPress any key to continue...";
        _getch();
        return false;
    }

    char Answer = InputLib::getYesNoAnswer("\n\nAre you sure you want to perform this transaction? (y/n): ");
    
    if (toupper(Answer) == 'Y')
    {
        ShowProgressBar("Processing Cash Dispensing");
        CurrentClient.AccountBalance -= WithdrawAmount;
        SaveCurrentClientChanges();
        
        system("color 0A");
        cout << "\nDone Successfully. New Balance is: $" << fixed << setprecision(2) << CurrentClient.AccountBalance << "\n";
        this_thread::sleep_for(chrono::milliseconds(400));
        system("color 0F");

        ShowPrintedReceipt("Withdrawal", WithdrawAmount);
    }
    else
    {
        cout << "\n\nOperation Cancelled.";
    }

    return true;
}

bool PerformDeposit(double DepositAmount)
{
    char Answer = InputLib::getYesNoAnswer("\n\nAre you sure you want to perform this transaction? (y/n): ");
    
    if (toupper(Answer) == 'Y')
    {
        ShowProgressBar("Accepting Notes and Updating Balance");
        CurrentClient.AccountBalance += DepositAmount;
        SaveCurrentClientChanges();

        system("color 0A");
        cout << "\nDone Successfully. New Balance is: $" << fixed << setprecision(2) << CurrentClient.AccountBalance << "\n";
        this_thread::sleep_for(chrono::milliseconds(400));
        system("color 0F");

        ShowPrintedReceipt("Deposit", DepositAmount);
    }
    else
    {
        cout << "\n\nOperation Cancelled.";
    }

    return true;
}

bool PerformUpdateAccountName(string AccountName)
{
    if (!isValidAccountName(AccountName))
    {
        FlashWarningColor();
        cout << "\nError: Invalid name! Must be 8-60 characters, contain first and last name, no symbols/numbers.";
        cout << "\nPress any key to try again...";
        _getch();
        return false;
    }

    if (AccountName == CurrentClient.Name)
    {
        FlashWarningColor();
        cout << "\nError: The new name is identical to your current name.";
        cout << "\nPress any key to try again...";
        _getch();
        return false;
    }

    char Answer = InputLib::getYesNoAnswer("\n\nAre you sure you want to perform this update? (y/n): ");
    
    if (toupper(Answer) == 'Y')
    {
        ShowProgressBar("Updating Profile");
        CurrentClient.Name = AccountName;
        SaveCurrentClientChanges();
        cout << "\n\nDone Successfully. New Account Name is: " << CurrentClient.Name;
    }
    else
    {
        cout << "\n\nOperation Cancelled.";
    }
    
    return true;
}

bool PerformUpdatePinCode(string NewPin)
{
    char Answer = InputLib::getYesNoAnswer("\n\nAre you sure you want to perform this update? (y/n): ");
    
    if (toupper(Answer) == 'Y')
    {
        ShowProgressBar("Encrypting and Updating PIN");
        CurrentClient.PinCode = NewPin;
        SaveCurrentClientChanges();
        cout << "\n\nDone Successfully. PIN Code updated successfully.";
    }
    else
    {
        cout << "\n\nOperation Cancelled.";
    }

    return true;
}

bool PerformUpdatePhoneNumber(string PhoneNumber)
{
    char Answer = InputLib::getYesNoAnswer("\n\nAre you sure you want to perform this update? (y/n): ");
    
    if (toupper(Answer) == 'Y')
    {
        ShowProgressBar("Updating Phone Record");
        CurrentClient.PhoneNumber = PhoneNumber;
        SaveCurrentClientChanges();
        cout << "\n\nDone Successfully. New Phone Number is: " << CurrentClient.PhoneNumber;
    }
    else
    {
        cout << "\n\nOperation Cancelled.";
    }

    return true;
}

// ==========================================
// 7. PRESENTATION LAYER (SCREENS & MENUS)
// ==========================================

void GoBackToMainMenu()
{
    cout << "\n\nPress any key to go back to Main Menu...";
    _getch();
    ShowMainMenu();
}

void GoBackToUpdateInfoMenu()
{
    cout << "\n\nPress any key to go back to Update Info Menu...";
    _getch();
    ShowUpdateInfoMenu(); 
}

void ShowSystemLockedScreen()
{
    ResetScreen();
    FlashWarningColor();
    system("color 4F");
    PrintHeaderLine('=');
    PrintCentered("SYSTEM LOCKED / ACCESS DENIED");
    PrintHeaderLine('=');
    cout << "\nYou have entered an invalid PIN 3 times consecutively.\n";
    cout << "Your session has been terminated for security reasons.\n\n";
    
    for (int i = 5; i > 0; i--)
    {
        cout << "\rSystem shutting down in: " << i << " second(s)... " << flush;
        this_thread::sleep_for(chrono::seconds(1));
    }
    cout << "\n";
    PrintHeaderLine('=');
}

void ShowLoginScreen()
{
    ResetScreen();
    ShowATMWelcomeBanner();
}

void ShowQuickWithdrawScreen()
{
    short Choice = 0, WithdrawAmount = 0;

    do
    {
        ResetScreen();
        PrintHeaderLine('=');
        PrintCentered("QUICK WITHDRAW SCREEN");
        PrintHeaderLine('=');
        cout << "\t[1] $20\t\t\t[2] $50\n";
        cout << "\t[3] $100\t\t[4] $200\n";
        cout << "\t[5] $400\t\t[6] $600\n";
        cout << "\t[7] $800\t\t[8] $1000\n";
        cout << "\t[9] Exit\n";
        PrintHeaderLine('=');
        cout << "Your Balance is: $" << fixed << setprecision(2) << CurrentClient.AccountBalance << "\n\n";

        Choice = InputLib::ReadIntNumberInRange(1, 9, "Choose what to withdraw [1 to 9]: ");

        if (Choice == 9) return;

        WithdrawAmount = GetQuickWithdrawAmount((enQuickWithdrawOptions)Choice);

        if (PerformWithdraw(WithdrawAmount))
            break;

    } while (true);
}

void ShowNormalWithdrawScreen()
{
    double WithdrawAmount = 0;

    do
    {
        ResetScreen();
        PrintHeaderLine('=');
        PrintCentered("NORMAL WITHDRAW SCREEN");
        PrintHeaderLine('=');
        cout << "Your Balance is: $" << fixed << setprecision(2) << CurrentClient.AccountBalance << "\n\n";

        WithdrawAmount = InputLib::ReadFloatPositiveNumber("Enter an amount multiple of 5's: ");

        if ((int)WithdrawAmount % 5 != 0)
        {
            FlashWarningColor();
            cout << "\nError: The amount must be a multiple of 5. Please try again.";
            cout << "\nPress any key to continue...";
            _getch();
        }
        else
        {
            if (PerformWithdraw(WithdrawAmount))
                break;
        }

    } while (true);
}

void ShowDepositScreen()
{
    ResetScreen();
    PrintHeaderLine('=');
    PrintCentered("DEPOSIT SCREEN");
    PrintHeaderLine('=');
    cout << "Your Balance is: $" << fixed << setprecision(2) << CurrentClient.AccountBalance << "\n\n";

    double DepositAmount = InputLib::ReadFloatPositiveNumber("Enter Deposit Amount: ");
    PerformDeposit(DepositAmount);
}

void ShowCheckBalanceScreen()
{
    ResetScreen();
    ShowSpinner("Fetching Account Balance from Mainframe", 2);
    
    ResetScreen();
    PrintHeaderLine('=');
    PrintCentered("CHECK BALANCE SCREEN");
    PrintHeaderLine('=');
    cout << " Your Current Balance is: $" << fixed << setprecision(2) << CurrentClient.AccountBalance << "\n";
    PrintHeaderLine('=');
}

void ShowMyInfoScreen()
{
    ResetScreen();
    ShowSpinner("Retrieving Secure Customer Profile", 2);

    ResetScreen();
    PrintHeaderLine('=');
    PrintCentered("MY ACCOUNT DETAILS");
    PrintHeaderLine('=');
    
    stringstream ss;
    ss << " Account Number : " << CurrentClient.AccountNumber << "\n"
       << " Account Name   : " << CurrentClient.Name << "\n"
       << " Phone Number   : " << CurrentClient.PhoneNumber << "\n"
       << " PIN Code       : ****\n"
       << " Balance        : $" << fixed << setprecision(2) << CurrentClient.AccountBalance << "\n";

    TypeWriterText(ss.str(), 10);
    PrintHeaderLine('=');
}

void ShowUpdateAccountNameScreen()
{
    string AccountName = "";

    do
    {
        ResetScreen();
        PrintHeaderLine('=');
        PrintCentered("CHANGE ACCOUNT NAME");
        PrintHeaderLine('=');
        cout << "Current Account Name: " << CurrentClient.Name << "\n\n";

        AccountName = InputLib::ReadText("Enter New Account Name: ");

        if (PerformUpdateAccountName(AccountName))
            break;

    } while (true);
}

void ShowUpdateAccountPinScreen()
{
    string CurrentPin = "";
    string NewPin = "";
    short FailedAttempts = 0;
    const short MaxAttempts = 3;
    short RemainingAttempts = 0;

    do
    {
        ResetScreen();
        PrintHeaderLine('=');
        PrintCentered("CHANGE ACCOUNT PIN");
        PrintHeaderLine('=');

        CurrentPin = GetValidPIN("Enter your current PIN Code (4 digits): ");

        if (CurrentPin == CurrentClient.PinCode)
        {
            cout << "\nCorrect PIN.\n\n";
            NewPin = GetValidPIN("Enter The New PIN Code (4 digits): ");

            PerformUpdatePinCode(NewPin);
            break;
        }
        else
        {
            FailedAttempts++;
            RemainingAttempts = MaxAttempts - FailedAttempts;
            FlashWarningColor();

            if (RemainingAttempts > 0)
            {
                cout << "\nError: Invalid PIN Code! You have " << RemainingAttempts << " attempt(s) left.";
                cout << "\nPress any key to try again...";
                _getch();
            }
            else
            {
                cout << "\nError: You have exceeded the maximum number of attempts (" << MaxAttempts << ").";
                cout << "\nOperation Cancelled for security reasons.";
                break;
            }
        }
    } while (FailedAttempts < MaxAttempts);
}

void ShowUpdatePhoneNumberScreen()
{
    ResetScreen();
    PrintHeaderLine('=');
    PrintCentered("CHANGE PHONE NUMBER");
    PrintHeaderLine('=');
    cout << "Current Phone Number: " << CurrentClient.PhoneNumber << "\n\n";

    string PhoneNumber = InputLib::ReadText("Enter New Phone Number: ");
    PerformUpdatePhoneNumber(PhoneNumber);
}

void PerformUpdateInfoOption(enUpdateInfoOptions option)
{
    switch (option)
    {
    case enUpdateInfoOptions::eUpdateName:
        ShowUpdateAccountNameScreen();
        GoBackToUpdateInfoMenu();
        break;

    case enUpdateInfoOptions::eUpdatePin:
        ShowUpdateAccountPinScreen();
        GoBackToUpdateInfoMenu();
        break;

    case enUpdateInfoOptions::eUpdatePhone:
        ShowUpdatePhoneNumberScreen();
        GoBackToUpdateInfoMenu();
        break;

    case enUpdateInfoOptions::eMainMenu:
        ShowMainMenu();
        break;
    }
}

void ShowUpdateInfoMenu()
{
    ResetScreen();
    PrintHeaderLine('=');
    PrintCentered("UPDATE ACCOUNT INFO MENU");
    PrintHeaderLine('=');
    cout << "\t[1] Change Account Name.\n";
    cout << "\t[2] Change Account PIN.\n";
    cout << "\t[3] Change Phone Number.\n";
    cout << "\t[4] Back to ATM Main Menu.\n";
    PrintHeaderLine('=');

    short choice = InputLib::ReadIntNumberInRange(1, 4, "Choose what do you want to do? [1 to 4]: ");
    PerformUpdateInfoOption((enUpdateInfoOptions)choice);
}

void PerformMainMenuOption(enMainMenuOptions option)
{
    switch (option)
    {
    case enMainMenuOptions::eQuickWithdraw:
        ShowQuickWithdrawScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eNormalWithdraw:
        ShowNormalWithdrawScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eDeposit:
        ShowDepositScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eCheckBalance:
        ShowCheckBalanceScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eMyInfo:
        ShowMyInfoScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eUpdateInfoMenu:
        ShowUpdateInfoMenu();
        break;
    case enMainMenuOptions::eLogout:
        ResetScreen();
        ShowSpinner("Logging out safely", 2);
        Login();
        break;
    }
}

void ShowMainMenu()
{
    ResetScreen();
    PrintHeaderLine('=');
    PrintCentered("ATM MAIN MENU SCREEN");
    PrintHeaderLine('=');
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] My Info.\n";
    cout << "\t[6] Update Account Info.\n";
    cout << "\t[7] Logout.\n";
    PrintHeaderLine('=');
    
    short choice = InputLib::ReadIntNumberInRange(1, 7, "Choose what do you want to do? [1 to 7]: ");
    PerformMainMenuOption((enMainMenuOptions)choice);
}

void Login()
{
    string AccountNumber = "", PinCode = "";
    bool loginFailed = false;
    short FailedAttempts = 0;
    const short MaxAttempts = 3;
    short RemainingAttempts = 0;

    do
    {
        ShowLoginScreen();

        if (loginFailed)
        {
            FlashWarningColor();
            RemainingAttempts = MaxAttempts - FailedAttempts;
            cout << "Invalid Account Number or PIN Code!\n";
            cout << "Warning: You have " << RemainingAttempts << " attempt(s) left before account lockout!\n\n";
        }

        AccountNumber = InputLib::ReadText("Enter Account Number: ");
        PinCode       = GetValidPIN("Enter PIN Code (4 digits): ");

        ShowSpinner("Authenticating Credentials", 2);

        if (LoadClientInfo(AccountNumber, PinCode))
        {
            loginFailed = false;
            ShowMainMenu();
            return;
        }
        else
        {
            loginFailed = true;
            FailedAttempts++;
        }

    } while (FailedAttempts < MaxAttempts);

    ShowSystemLockedScreen();
}

// ==========================================
// 8. MAIN FUNCTION
// ==========================================

int main()
{
    Login();
    return 0;
}