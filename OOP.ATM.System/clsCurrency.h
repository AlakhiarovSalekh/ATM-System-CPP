#pragma once
#include <iostream>
#include <string>
#include "clsstring.h"
#include <vector>
#include <fstream>

using namespace std;

class clsCurrency
{
	enum enMode { EmptyMode , UpdateMode};
	enMode _Mode;
	string _Country, _CurrencyCode, _CurrencyName;
	float _Rate;

	static clsCurrency _ConvertLineToCurrencyObject(string Line, string Seperator = "#//#")
	{
		vector <string> vCurrencyDate;
		vCurrencyDate = clsString::Split(Line, Seperator);
		return clsCurrency(enMode::UpdateMode, vCurrencyDate[0], vCurrencyDate[1],
			vCurrencyDate[2], stod(vCurrencyDate[3]));
	}

	static string _ConverCurrencyObjectToLine(clsCurrency Currency, string Seperator = "#//#")
	{
		string stCurrencyRecord = "";
		stCurrencyRecord += Currency.Country() + Seperator;
		stCurrencyRecord += Currency.CurrencyCode() + Seperator;
		stCurrencyRecord += Currency.CurrencyName() + Seperator;
		stCurrencyRecord += to_string(Currency.Rate());
		return stCurrencyRecord;
	}

	static  vector <clsCurrency> _LoadCurrencysDataFromFile()
	{
		vector <clsCurrency> vCurrencys;
		fstream Myfile;
		Myfile.open("Currencies.txt", ios::in);
		if (Myfile.is_open())
		{
			string Line;
			while (getline(Myfile, Line))
			{
				clsCurrency Currency = _ConvertLineToCurrencyObject(Line);

				vCurrencys.push_back(Currency);
			}
			Myfile.close();
		}
		return vCurrencys;
	}
	static void _SaveCurrencyDataToFile(vector <clsCurrency> vCurrencys)
	{
		string DateLine;
		fstream Myfile;
		Myfile.open("Currencies.txt", ios::in);

		if (Myfile.is_open())
		{
			for (clsCurrency C : vCurrencys)
			{
				DateLine = _ConverCurrencyObjectToLine(C);
				Myfile << DateLine << endl;
			}
			Myfile.close();
		}

	}

	void _Update()
	{
		vector <clsCurrency> _vCurrencys;
		_vCurrencys = _LoadCurrencysDataFromFile();
		for (clsCurrency &C : _vCurrencys)
		{
			if (C.CurrencyCode() == CurrencyCode())
			{
				C = *this;
				break;
			}
		}
		_SaveCurrencyDataToFile(_vCurrencys);

	}



public:
	static clsCurrency GetEmptyCurrencyObject()
	{
		return clsCurrency(enMode::EmptyMode, "", "", "", 0);
	}
	clsCurrency(enMode Mode , string Country, string CurrencyCode, string CurrencyName, float Rete)
	{
		_Mode = Mode;
		_Country = Country;
		_CurrencyCode = CurrencyCode;
		_CurrencyName = CurrencyName;
		_Rate = Rete;
	}

	static vector <clsCurrency> GetAllUSDRates()
	{
		return _LoadCurrencysDataFromFile();
	}

	bool IsEmpty()
	{
		return (_Mode == enMode::EmptyMode);
	}

	string Country()
	{
		return _Country;
	}
	string CurrencyCode()
	{
		return _CurrencyCode;
	}	
	string CurrencyName()
	{
		return _CurrencyName;
	}
	void UpdateRate(float NewRate)
	{
		_Rate = NewRate;
		_Update();
	}
	float Rate()
	{
		return _Rate;
	}

	static clsCurrency FindByCode(string CurrencyCode)
	{
		CurrencyCode = clsString::UpperAllString(CurrencyCode);
		fstream Myfile;
		Myfile.open("Currencies.txt", ios::in);
		if (Myfile.is_open())
		{
			string Line;
			while (getline(Myfile, Line))
			{
				clsCurrency Currency = _ConvertLineToCurrencyObject(Line);
				if (Currency.CurrencyCode() == CurrencyCode)
				{
					Myfile.close();
					return Currency;
				}
			}
		}
	}

	static clsCurrency FindByCountry(string Country)
	{
		Country = clsString::UpperAllString(Country);
		fstream Myfile;
		Myfile.open("Currencies.txt", ios::in);
		if (Myfile.is_open())
		{
			string Line;
			while (getline(Myfile, Line))
			{
				clsCurrency Currency = _ConvertLineToCurrencyObject(Line);
				if (clsString::UpperAllString(Currency.Country()) == Country)
				{
					Myfile.close();
					return Currency;
				}
			}
		}
		return GetEmptyCurrencyObject();
	}

	static bool IsCurrencyExist(string CurrencyCode)
	{
		clsCurrency C1 = clsCurrency::FindByCode(CurrencyCode);
		return (!C1.IsEmpty());

	}

	static vector <clsCurrency> GetCurrenciesList()
	{
		return _LoadCurrencysDataFromFile();
	}

	float ConvertToUSD(float Amount)
	{
		return (float)(Amount / Rate());
	}

	float ConvertToOtherCurrency(float Amount, clsCurrency Currency2)
	{
		float AmountInUSD = ConvertToUSD(Amount);

		if (Currency2.CurrencyCode() == "USD")
		{
			return AmountInUSD;
		}

		return (float)(AmountInUSD * Currency2.Rate());

	}
};

