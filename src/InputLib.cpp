#include <iostream>
#include <string>
#include <limits>
#include <cctype>
#include <cstdlib>
#include "InputLib.h"

using namespace std;

int InputLib::ReadIntNumber(string Msg)
{
    string inputLine;
    long validatedNumber;
    while (true)
    {
        cout << Msg;
        if (!getline(cin >> ws, inputLine))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid Input, Try again.\n\n";
            continue;
        }
        char* processedCharactersEnd;
        validatedNumber = strtol(inputLine.c_str(), &processedCharactersEnd, 10);
        if (processedCharactersEnd != inputLine.c_str() && *processedCharactersEnd == '\0')
        {
            return static_cast<int>(validatedNumber);
        }
        cout << "\nInvalid Input, Try again.\n\n";
    }
}

float InputLib::ReadFloatNumber(std::string Msg)
{
    string inputLine;
    float validatedNumber;
    while (true)
    {
        cout << Msg;
        if (!getline(cin >> ws, inputLine))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid Input, Try again.\n\n";
            continue;
        }
        char* processedCharactersEnd;
        validatedNumber = strtof(inputLine.c_str(), &processedCharactersEnd);
        if (processedCharactersEnd != inputLine.c_str() && *processedCharactersEnd == '\0')
        {
            return validatedNumber;
        }
        cout << "\nInvalid Input, Try again.\n\n";
    }
}

int InputLib::ReadIntPositiveNumber(string Msg)
{
    string inputLine;
    long validatedNumber;
    while (true)
    {
        cout << Msg;
        if (!getline(cin >> ws, inputLine))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid Input, Try again.\n\n";
            continue;
        }
        char* processedCharactersEnd;
        validatedNumber = strtol(inputLine.c_str(), &processedCharactersEnd, 10);
        if (processedCharactersEnd != inputLine.c_str() && *processedCharactersEnd == '\0' && validatedNumber >= 0)
        {
            return static_cast<int>(validatedNumber);
        }
        cout << "\nInvalid Input, Try again.\n\n";
    }
}

float InputLib::ReadFloatPositiveNumber(std::string Msg)
{
    string inputLine;
    float validatedNumber;
    while (true)
    {
        cout << Msg;
        if (!getline(cin >> ws, inputLine))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nnvalid Input, Try again.\n\n";
            continue;
        }
        char* processedCharactersEnd;
        validatedNumber = strtof(inputLine.c_str(), &processedCharactersEnd);
        if (processedCharactersEnd != inputLine.c_str() && *processedCharactersEnd == '\0' && validatedNumber >= 0.0f)
        {
            return validatedNumber;
        }
        cout << "\nInvalid Input, Try again.\n\n";
    }
}

int InputLib::ReadIntNumberInRange(int From, int To, string Msg)
{
    string inputLine;
    long validatedNumber;
    while (true)
    {
        cout << Msg;
        if (!getline(cin >> ws, inputLine))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid Input, Try again.\n\n";
            continue;
        }
        char* processedCharactersEnd;
        validatedNumber = strtol(inputLine.c_str(), &processedCharactersEnd, 10);
        if (processedCharactersEnd != inputLine.c_str() && *processedCharactersEnd == '\0' && validatedNumber >= From && validatedNumber <= To)
        {
            return static_cast<int>(validatedNumber);
        }
        cout << "\nInvalid Input, Try again.\n\n";
    }
}

float InputLib::ReadFloatNumberInRange(float From, float To, std::string Msg)
{
    string inputLine;
    float validatedNumber;
    while (true)
    {
        cout << Msg;
        if (!getline(cin >> ws, inputLine))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid Input, Try again.\n\n";
            continue;
        }
        char* processedCharactersEnd;
        validatedNumber = strtof(inputLine.c_str(), &processedCharactersEnd);
        if (processedCharactersEnd != inputLine.c_str() && *processedCharactersEnd == '\0' && validatedNumber >= From && validatedNumber <= To)
        {
            return validatedNumber;
        }
        cout << "\nInvalid Input, Try again.\n\n";
    }
}

string InputLib::ReadText(string Msg)
{
    string textContent;
    while (true)
    {
        cout << Msg;
        if (getline(cin >> ws, textContent))
        {
            return textContent;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nInvalid Input, Try again.\n\n";
    }
}

char InputLib::ReadCharacter(string Msg)
{
    string inputLine;
    while (true)
    {
        cout << Msg;
        if (!getline(cin >> ws, inputLine))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid Input, Try again.\n\n";
            continue;
        }
        if (inputLine.length() == 1)
        {
            return inputLine[0];
        }
        cout << "\nInvalid Input, Try again.\n\n";
    }
}

char InputLib::getYesNoAnswer(string Msg)
{
    string inputLine;
    while (true)
    {
        cout << Msg;
        if (!getline(cin >> ws, inputLine))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid Input, Try again.\n\n";
            continue;
        }
        if (inputLine.length() == 1)
        {
            char targetChar = tolower(static_cast<unsigned char>(inputLine[0]));
            if (targetChar == 'y' || targetChar == 'n')
            {
                return inputLine[0];
            }
        }
        cout << "\nInvalid Input, Try again.\n\n";
    }
}