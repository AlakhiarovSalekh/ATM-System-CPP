#pragma once
#include <iostream>
#include "clsCurrency.h"
#include <iomanip>


using namespace std;

class clsListCurrenciesScreen : protected clsScreen
{
private:
	static void PrintListCurrenciesScreen(clsCurrency Currency)
	{
        {
            cout << setw(8) << left << "" << "| " << setw(30) << left << Currency.Country();
            cout << "| " << setw(10) << left << Currency.CurrencyCode();
            cout << "| " << setw(45) << left << Currency.CurrencyName();
            cout << "| " << setw(10) << left << Currency.Rate();
        }
	}
public:

    static void ShowCurrencyList() 
    {

        vector <clsCurrency> vCurrencies = clsCurrency::GetCurrenciesList();
        string Title = "\t  Currency List Screen";
        string SubTitle = "\t    (" + to_string(vCurrencies.size()) + ") Currency(s).";

        _DrawScreenHeader(Title, SubTitle);

        cout << setw(8) << left << "" << "\n\t_______________________________________________________";
        cout << "_________________________________________\n" << endl;

        cout << setw(8) << left << "" << "| " << left << setw(30) << "Country";
        cout << "| " << left << setw(10) << "Code";
        cout << "| " << left << setw(45) << "Name";
        cout << "| " << left << setw(10) << "Rate(1$)";
        cout << setw(8) << left << "" << "\n\t_______________________________________________________";
        cout << "_________________________________________\n" << endl;

        if (vCurrencies.size() == 0)
            cout << "\t\t\t\tNo Currencies Available In the System!";
        else

            for (clsCurrency Currency : vCurrencies)
            {
                PrintListCurrenciesScreen(Currency);
                cout << endl;
            }
        cout << setw(8) << left << "" << "\n\t_______________________________________________________";
        cout << "_________________________________________\n" << endl;

    }
};

