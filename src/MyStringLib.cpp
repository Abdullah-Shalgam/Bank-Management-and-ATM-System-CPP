#include <iostream>
#include <string>
#include "MyStringLib.h"

using namespace std;

string MyStringLib::ConvertTextToLowerCase(string Text)
{
    for (char &Letter : Text) 
    {
        Letter = tolower(Letter);
    }
    return Text;
}

string MyStringLib::ConvertTextToUpperCase(string Text)
{
    for (char &Letter : Text) 
    {
        Letter = toupper(Letter);
    }
    return Text;
}

int MyStringLib::GetNumberLength(int Num)
{
    return to_string(Num).length();
}
