#include <iostream>
#include <cstdlib>
#include "MathLib.h"

using namespace std;

int MathLib::RandomNumber(int From, int To)
{
    return rand() % (To - From + 1) + From;
}

int MathLib::ReversedNumber(int Num)
{
    int Remainder = 0, Reversed = 0;
    while (Num > 0)
    {
        Remainder = Num % 10;
        Num /= 10;
        Reversed = Reversed * 10 + Remainder;
    }
    return Reversed;
}

void MathLib::Swap2Numbers(int& num1, int& num2) 
{
    int temp = num1;
    num1 = num2;
    num2 = temp;
}

bool MathLib::isPrime(int i)
{
    if (i <= 1)
        return false;
    for (int j = 2; j * j <= i; j++)
    {
        if (i % j == 0)
            return false;
    }
    return true;
}

bool MathLib::isPerfect(int Num)
{
    int Sum = 1;
    for (int i = 2; i <= Num / 2; i++)
    {
        if (Num % i == 0)
            Sum += i;
    }
    return (Sum == Num);
}

bool MathLib::IsPalindrome(int Num)
{
    return (Num == ReversedNumber(Num)); 
}