#pragma once
#include <string>

class InputLib 
{
public:
    static int ReadIntNumber(std::string Msg);
    static int ReadIntPositiveNumber(std::string Msg);
    static int ReadIntNumberInRange(int From, int To, std::string Msg);
    static float ReadFloatNumber(std::string Msg);
    static float ReadFloatPositiveNumber(std::string Msg);
    static float ReadFloatNumberInRange(float From, float To, std::string Msg);
    static std::string ReadText(std::string Msg);
    static char ReadCharacter(std::string Msg);
    static char getYesNoAnswer(std::string Msg);
};