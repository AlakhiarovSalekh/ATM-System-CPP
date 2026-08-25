#pragma once
#include <iostream>
#include "clsCurrency.h"
#include <iomanip>
#include "clsScreen.h"
#include "clsInputValidate.h"
using namespace std;

class clsFindCurrencyScreen : protected clsScreen
{
private:
    enum enFind { eCode = 1, eCountry = 2 };
    static void _PrintCurrency(clsCurrency Currency)
    {
        cout << "\nCurrency Card:";
        cout << "\n___________________";
        cout << "\nCountry   : " << Currency.Country();
        cout << "\nCode      : " << Currency.CurrencyCode();
        cout << "\nName      : " << Currency.CurrencyName();
        cout << "\nRate(1USD): " << Currency.Rate();
        cout << "\n___________________\n";
    }

    static enFind _ReadFindOption()
    {
        cout << "Find by: [1] Code, [2] Country? ";
        int choice = clsInputValidate::ReadIntNumberBetween(1, 2, "Enter a number between 1 and 2: ");
        return (enFind)choice;
    }

public:
    static void ShowFindCurrencyScreen()
    {
        _DrawScreenHeader("\tFind Currency Screen");


        enFind findOption = _ReadFindOption();

        string searchValue;
        clsCurrency Currency = clsCurrency::GetEmptyCurrencyObject();

        if (findOption == eCode)
        {
            cout << "Please Enter Currency Code: ";
            searchValue = clsInputValidate::ReadString();
            Currency = clsCurrency::FindByCode(searchValue);


            while (Currency.IsEmpty())
            {
                cout << "\nCurrency with Code [" << searchValue << "] not found, choose another one: ";
                searchValue = clsInputValidate::ReadString();
                Currency = clsCurrency::FindByCode(searchValue);
            }
        }
        else
        {
            cout << "Please Enter Country Name: ";
            searchValue = clsInputValidate::ReadString();
            Currency = clsCurrency::FindByCountry(searchValue);
            while (Currency.IsEmpty())
            {
                cout << "\nCurrency for Country [" << searchValue << "] not found, choose another one: ";
                searchValue = clsInputValidate::ReadString();
                Currency = clsCurrency::FindByCountry(searchValue);
            }
        }
        _PrintCurrency(Currency);
    }
};