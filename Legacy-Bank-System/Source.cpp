#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <utility>
#include <limits>

using namespace std;

const string ClientsFileName = "Clients.txt";
const string UsersFileName = "User.txt";

void ShowMainMenu();
void ShowTransactionsMenu();
void ShowManageUsersMenu();
void Login();

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

struct sUser
{
    string UserName;
    string Pass;
    short Permissions;
    bool MarkForDelete = false;
};
sUser CurrentUser;

bool CheckFileExists(string FileName)
{
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        MyFile.close();
        return true;
    }

    MyFile.open(FileName, ios::out);
    if (MyFile.is_open())
    {
        MyFile.close();
        return true;
    }
    return false;
}

vector<string> SplitString(string& S1, string Delim)
{
    vector<string> vString;
    size_t pos = 0, start = 0;
    while ((pos = S1.find(Delim, start)) != string::npos)
    {
        vString.push_back(S1.substr(start, pos - start));
        start = pos + Delim.length();
    }
    if (start < S1.length())
    {
        vString.push_back(S1.substr(start));
    }
    return vString;
}



sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
    sClient Client;
    vector<string> vClientData;
    vClientData = SplitString(Line, Seperator);
    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    try
    {
        Client.AccountBalance = stod(vClientData[4]);
    }
    catch (const exception& e)
    {
        cout << "Error converting balance: " << e.what() << ". Setting to 0.\n";
        Client.AccountBalance = 0;
    }
    return Client;
}

sUser ConvertLinetoRecordUser(string Line, string Seperator = "#//#")
{
    sUser User;
    vector<string> vUser;
    vUser = SplitString(Line, Seperator);
    User.UserName = vUser[0];
    User.Pass = vUser[1];
    try
    {
        User.Permissions = stoi(vUser[2]);
    }
    catch (const exception& e)
    {
        cout << "Error converting permissions: " << e.what() << ". Setting to 0.\n";
        User.Permissions = 0;
    }
    return User;
}

string ConvertRecordToLineUser(sUser User, string Seperator = "#//#")
{
    string stUserRecord = "";
    stUserRecord += User.UserName + Seperator;
    stUserRecord += User.Pass + Seperator;
    stUserRecord += to_string(User.Permissions) + Seperator;
    return stUserRecord;
}
void InitializeFiles()
{
    if (!CheckFileExists(UsersFileName)) {
        fstream MyFile(UsersFileName, ios::out);
        if (MyFile.is_open()) {

            sUser defaultUser;
            defaultUser.UserName = "Admin";
            defaultUser.Pass = "1234";
            defaultUser.Permissions = -1;
            MyFile << ConvertRecordToLineUser(defaultUser) << endl;
            MyFile.close();
        }
    }
    if (!CheckFileExists(ClientsFileName)) {
        fstream MyFile(ClientsFileName, ios::out);
        MyFile.close();
    }
}


bool UserExistsByUserName(string UserName, string FileName)
{
    vector<sUser> vUsers;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        string Line;
        sUser User;
        while (getline(MyFile, Line))
        {
            User = ConvertLinetoRecordUser(Line);
            if (User.UserName == UserName)
            {
                MyFile.close();
                return true;
            }
            vUsers.push_back(User);
        }
        MyFile.close();
    }
    return false;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
    vector<sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        string Line;
        sClient Client;
        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return false;
}

sClient ReadNewClient()
{
    sClient Client;
    cout << "Enter Account Number? ";
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }
    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;
}

short ReadPermissions()
{
    short Permissions = 0;
    char answer;

    cout << "Do you want FullAccess? (y/n): ";
    cin >> answer;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (answer == 'y' || answer == 'Y')
    {
        Permissions |= -1;
        return Permissions;
    }

    cout << "Do you want showClientList? (y/n): ";
    cin >> answer;
    while (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N')
    {
        cout << "Please enter y or n: ";
        cin >> answer;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (answer == 'y' || answer == 'Y')
        Permissions |= 1;

    cout << "Do you want AddNewClient? (y/n): ";
    cin >> answer;
    while (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N')
    {
        cout << "Please enter y or n: ";
        cin >> answer;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (answer == 'y' || answer == 'Y')
        Permissions |= 2;

    cout << "Do you want DeleteClient? (y/n): ";
    cin >> answer;
    while (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N')
    {
        cout << "Please enter y or n: ";
        cin >> answer;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (answer == 'y' || answer == 'Y')
        Permissions |= 4;

    cout << "Do you want UpdateClient? (y/n): ";
    cin >> answer;
    while (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N')
    {
        cout << "Please enter y or n: ";
        cin >> answer;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (answer == 'y' || answer == 'Y')
        Permissions |= 8;

    cout << "Do you want FindClient? (y/n): ";
    cin >> answer;
    while (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N')
    {
        cout << "Please enter y or n: ";
        cin >> answer;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (answer == 'y' || answer == 'Y')
        Permissions |= 16;

    cout << "Do you want Transactions? (y/n): ";
    cin >> answer;
    while (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N')
    {
        cout << "Please enter y or n: ";
        cin >> answer;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (answer == 'y' || answer == 'Y')
        Permissions |= 32;

    cout << "Do you want ManageUser? (y/n): ";
    cin >> answer;
    while (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N')
    {
        cout << "Please enter y or n: ";
        cin >> answer;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (answer == 'y' || answer == 'Y')
        Permissions |= 64;

    return Permissions;
}

sUser ReadNewUser()
{
    sUser User;
    cout << "Enter UserName? ";
    getline(cin >> ws, User.UserName);

    while (UserExistsByUserName(User.UserName, UsersFileName))
    {
        cout << "\nUser with [" << User.UserName << "] already exists, Enter another UserName? ";
        getline(cin >> ws, User.UserName);
    }
    cout << "Enter Pass? ";
    getline(cin, User.Pass);

    User.Permissions = ReadPermissions();
    return User;
}

vector<sClient> LoadCleintsDataFromFile(string FileName)
{
    vector<sClient> vClients;
    if (!CheckFileExists(FileName))
    {
        cout << "Error: Could not access or create file " << FileName << ".\n";
        return vClients;
    }

    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            vClients.push_back(ConvertLinetoRecord(Line));
        }
        MyFile.close();
    }
    return vClients;
}

vector<sUser> LoadUsersDataFromFile(string FileName) {
    vector<sUser> vUsers;
    if (!CheckFileExists(FileName)) {
        cout << "Error: Could not access or create file " << FileName << ".\n";
        return vUsers;
    }

    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (!MyFile.is_open()) {
        cout << "Error: Unable to open file " << FileName << " for reading.\n";
        return vUsers;
    }

    string Line;
    while (getline(MyFile, Line)) {
        if (!Line.empty()) { 
            vUsers.push_back(ConvertLinetoRecordUser(Line));
        }
    }
    MyFile.close();
    return vUsers;
}
void PrintClientRecordLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintUserRecordLine(sUser User)
{
    cout << "| " << setw(15) << left << User.UserName;
    cout << "| " << setw(10) << left << User.Pass;
    cout << "| " << setw(10) << left << User.Permissions;
}

void ShowAllClientsScreen()
{
    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else
        for (sClient Client : vClients)
        {
            PrintClientRecordLine(Client);
            cout << endl;
        }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

void ShowAllUsersScreen()
{
    vector<sUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    cout << "\n\t\t\t\t\tUser List (" << vUsers.size() << ") User(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "UserName";
    cout << "| " << left << setw(10) << "Pass";
    cout << "| " << left << setw(10) << "Permissions";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vUsers.size() == 0)
        cout << "\t\t\t\tNo Users Available In the System!";
    else
        for (sUser User : vUsers)
        {
            PrintUserRecordLine(User);
            cout << endl;
        }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccount Number: " << Client.AccountNumber;
    cout << "\nPin Code     : " << Client.PinCode;
    cout << "\nName         : " << Client.Name;
    cout << "\nPhone        : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";
}

void PrintUserCard(sUser User)
{
    cout << "\nThe following are the User details:\n";
    cout << "-----------------------------------";
    cout << "\nUserName  : " << User.UserName;
    cout << "\nPass      : " << User.Pass;
    cout << "\nPermissions      : " << User.Permissions;
    cout << "\n-----------------------------------\n";
}

bool FindClientByAccountNumber(string AccountNumber, vector<sClient> vClients, sClient& Client)
{
    for (sClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

bool FindUserByUserName(string UserName, vector<sUser> vUsers, sUser& User)
{
    for (sUser C : vUsers)
    {
        if (C.UserName == UserName)
        {
            User = C;
            return true;
        }
    }
    return false;
}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;
    return Client;
}

sUser ChangeUserRecord(string UserName)
{
    sUser User;
    User.UserName = UserName;
    cout << "\n\nEnter Pass? ";
    getline(cin >> ws, User.Pass);
    User.Permissions = ReadPermissions();
    return User;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    for (sClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

bool MarkUserForDeleteByUserName(string UserName, vector<sUser>& vUsers)
{
    for (sUser& C : vUsers)
    {
        if (C.UserName == UserName)
        {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

vector<sClient> SaveCleintsDataToFile(string FileName, vector<sClient> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);

    string DataLine;

    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {
            if (!C.MarkForDelete)
                MyFile << ConvertRecordToLine(C) << endl;
        }
        MyFile.close();
    }
    return vClients;
}

vector<sUser> SaveUsersDataToFile(string FileName, vector<sUser> vUsers)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out); 

    string DataLine;

    if (MyFile.is_open())
    {
        for (sUser C : vUsers)
        {
            if (!C.MarkForDelete)
                MyFile << ConvertRecordToLineUser(C) << endl;
        }
        MyFile.close();
    }
    return vUsers;
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
    sUser User;
    User = ReadNewUser();
    AddDataLineToFile(UsersFileName, ConvertRecordToLineUser(User));
}

void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        cout << "Adding New Client:\n\n";
        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}

void AddNewUsers()
{
    char AddMore = 'Y';
    do
    {
        cout << "Adding New User:\n\n";
        AddNewUser();
        cout << "\nUser Added Successfully, do you want to add more Users? Y/N? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);
            vClients = LoadCleintsDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
}

bool DeleteUserByUserName(string UserName, vector<sUser>& vUsers)
{
    sUser User;
    char Answer = 'n';

    if (FindUserByUserName(UserName, vUsers, User))
    {
        if (User.UserName != "Admin")
        {
            PrintUserCard(User);
            cout << "\n\nAre you sure you want delete this User? y/n ? ";
            cin >> Answer;
            if (Answer == 'y' || Answer == 'Y')
            {
                MarkUserForDeleteByUserName(UserName, vUsers);
                SaveUsersDataToFile(UsersFileName, vUsers);
                vUsers = LoadUsersDataFromFile(UsersFileName);

                cout << "\n\nUser Deleted Successfully.";
                return true;
            }
        }
        else
            cout << "\nUser with UserName (" << UserName << ") cannot be deleted; it is the leader!";
        return false;
    }
    else
    {
        cout << "\nUser with UserName (" << UserName << ") is Not Found!";
        return false;
    }
}

bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }
            SaveCleintsDataToFile(ClientsFileName, vClients);
            cout << "\n\nClient Updated Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
}

bool UpdateUserByUserName(string UserName, vector<sUser>& vUsers)
{
    sUser User;
    char Answer = 'n';

    if (FindUserByUserName(UserName, vUsers, User))
    {
        PrintUserCard(User);
        cout << "\n\nAre you sure you want update this User? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (sUser& C : vUsers)
            {
                if (C.UserName == UserName)
                {
                    C = ChangeUserRecord(UserName);
                    break;
                }
            }
            SaveUsersDataToFile(UsersFileName, vUsers);
            cout << "\n\nUser Updated Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nUser with UserName (" << UserName << ") is Not Found!";
        return false;
    }
}

string ReadClientAccountNumber() {
    string AccountNumber = "";
    cout << "\nPlease enter AccountNumber? ";
    getline(cin >> ws, AccountNumber);
    while (AccountNumber.empty()) {
        cout << "\nAccount Number cannot be empty. Please enter a valid AccountNumber: ";
        getline(cin >> ws, AccountNumber);
    }
    return AccountNumber;
}

string ReadUserUserName()
{
    string UserName = "";
    cout << "\nPlease enter UserName? ";
    cin >> UserName;
    return UserName;
}

void ShowDeleteClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ShowDeleteUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete User Screen";
    cout << "\n-----------------------------------\n";

    vector<sUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    string UserName = ReadUserUserName();
    DeleteUserByUserName(UserName, vUsers);
}

void ShowUpdateClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate User Info Screen";
    cout << "\n-----------------------------------\n";

    vector<sUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    string UserName = ReadUserUserName();
    UpdateUserByUserName(UserName, vUsers);
}

void ShowAddNewClientsScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";

    AddNewClients();
}

void ShowAddNewUsersScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Users Screen";
    cout << "\n-----------------------------------\n";

    AddNewUsers();
}

void ShowFindClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";
}

void ShowFindUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind User Screen";
    cout << "\n-----------------------------------\n";

    vector<sUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    sUser User;
    string UserName = ReadUserUserName();
    if (FindUserByUserName(UserName, vUsers, User))
        PrintUserCard(User);
    else
        cout << "\nUser with UserName[" << UserName << "] is not found!";
}

sClient DepositClientsScreen()
{
    sClient Client;
    double dep = 0;
    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);

        cout << "Please enter deposit amount: ";
        cin >> dep;
        while (dep <= 0) 
        {
            cout << "\nPlease enter a positive amount: ";
            cin >> dep;
        }

        cout << "Are you sure you want to perform this transaction? (Y/N): ";
        char An = 'n';
        cin >> An;

        if (An == 'y' || An == 'Y')
        {
            Client.AccountBalance += dep;
            cout << "\nTransaction completed successfully.\n";
            cout << "Current Balance: " << Client.AccountBalance << "\n";
        }
        else
        {
            cout << "\nTransaction canceled.\n";
        }

        for (sClient& c : vClients)
        {
            if (c.AccountNumber == Client.AccountNumber)
            {
                c = Client;
                break;
            }
        }
        SaveCleintsDataToFile(ClientsFileName, vClients);
    }
    else
    {
        cout << "\nClient with account number [" << AccountNumber << "] does not exist.";
    }

    return Client;
}

void ShowDepositClientsScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Client Screen";
    cout << "\n-----------------------------------\n";
    DepositClientsScreen();
}

sClient WithdrawClientsScreen()
{
    sClient Client;
    double With = 0;
    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);

        cout << "Please enter Withdraw amount: ";
        cin >> With;
        while (With > Client.AccountBalance) 
        {
            cout << "\nThe amount exceeds your balance. You can withdraw up to " << Client.AccountBalance << " only.\n";
            cout << "Please enter another amount: ";
            cin >> With;
        }

        cout << "Are you sure you want to perform this transaction? (Y/N): ";
        char An = 'n';
        cin >> An;

        if (An == 'y' || An == 'Y')
        {
            Client.AccountBalance -= With;
            cout << "\nTransaction completed successfully.\n";
            cout << "Current Balance: " << Client.AccountBalance << "\n";
        }
        else
        {
            cout << "\nTransaction canceled.\n";
        }

        for (sClient& c : vClients)
        {
            if (c.AccountNumber == Client.AccountNumber)
            {
                c = Client;
                break;
            }
        }
        SaveCleintsDataToFile(ClientsFileName, vClients);
    }
    else
    {
        cout << "\nClient with account number [" << AccountNumber << "] does not exist.";
    }

    return Client;
}

void ShowWithdrawClientsScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\nWithdraw Client Screen";
    cout << "\n-----------------------------------\n";
    WithdrawClientsScreen();
}

void PrintTotalBalancesClientsCard(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void ShowTotalBalancesClientsCardScreen()
{
    double Balances = 0;
    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Account Number"; 
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else
        for (sClient Client : vClients)
        {
            Balances += Client.AccountBalance;
            PrintTotalBalancesClientsCard(Client);
            cout << endl;
        }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "\t\t\t\t Total Balances = " << Balances << endl;
}

enum enMainMenuOptions {
    eListClients = 1, eAddNewClient = 2, eDeleteClient = 3, eUpdateClient = 4,
    eFindClient = 5, eTransactions = 6, eManageUsers = 7, eLogout = 8, eExit = 9 
};

enum enTransactions { Deposit = 1, Withdraw, TotalBalances, MainMenu };

enum enManageUsers { eListUsers = 1, eAddNewUser, eDeleteUser, eUpdateUser, eFindUser, eMainMenu };

enum enPermissions {
    fullAccess = -1, showClientList = 1, AddNewclient = 2, DeleteClient = 4,
    UpdateClient = 8, FindClient = 16, Transactions = 32, ManageUser = 64,
};

void GoBackToMainMenu()
{
    cout << "\n\nPress any key to go back to Main Menu...";
    system("pause>0");
    ShowMainMenu();
}

void GoBackToTransactionsMenu()
{
    cout << "\n\nPress any key to go to Transactions Menu...";
    system("pause>0");
    ShowTransactionsMenu();
}

void GoToBackManageUsers()
{
    cout << "\n\nPress any key to go to Manage Users Menu...";
    system("pause>0");
    ShowManageUsersMenu();
}

short ReadMainMenuOption()
{
    short Choice = 0;
    bool validInput = false;

    while (!validInput)
    {
        cout << "Choose what do you want to do? [1 to 9]? "; 
        if (cin >> Choice)
        {
            if (Choice >= 1 && Choice <= 9)
            {
                validInput = true;
            }
            else
            {
                cout << "Please enter a number between 1 and 9.\n";
            }
        }
        else
        {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return Choice;
}

short ReadTransactions()
{
    short Choice = 0;
    bool validInput = false;

    while (!validInput)
    {
        cout << "Choose what do you want to do? [1 to 4]? ";
        if (cin >> Choice)
        {
            if (Choice >= 1 && Choice <= 4)
            {
                validInput = true;
            }
            else
            {
                cout << "Please enter a number between 1 and 4.\n";
            }
        }
        else
        {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return Choice;
}

short ReadManageUsers()
{
    short Choice = 0;
    bool validInput = false;

    while (!validInput)
    {
        cout << "Choose what do you want to do? [1 to 6]? ";
        if (cin >> Choice)
        {
            if (Choice >= 1 && Choice <= 6)
            {
                validInput = true;
            }
            else
            {
                cout << "Please enter a number between 1 and 6.\n";
            }
        }
        else
        {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return Choice;
}

void ShowEndScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :-)";
    cout << "\n-----------------------------------\n";
}

bool HasPermission(short Permissions, short PermissionToCheck)
{
    if (Permissions == -1)
        return true;
    return (Permissions & PermissionToCheck) == PermissionToCheck;
}

void PerfromManageUsers(enManageUsers ManageUsers)
{
    if (!HasPermission(CurrentUser.Permissions, ManageUser))
    {
        system("cls");
        cout << "\nAccess Denied! You do not have permission to manage users.\n";
        GoBackToMainMenu();
        return;
    }
    switch (ManageUsers)
    {
    case enManageUsers::eListUsers:
    {
        system("cls");
        ShowAllUsersScreen();
        GoToBackManageUsers();
        break;
    }
    case enManageUsers::eAddNewUser:
    {
        system("cls");
        ShowAddNewUsersScreen();
        GoToBackManageUsers();
        break;
    }
    case enManageUsers::eDeleteUser:
    {
        system("cls");
        ShowDeleteUserScreen();
        GoToBackManageUsers();
        break;
    }
    case enManageUsers::eUpdateUser:
    {
        system("cls");
        ShowUpdateUserScreen();
        GoToBackManageUsers();
        break;
    }
    case enManageUsers::eFindUser:
    {
        system("cls");
        ShowFindUserScreen();
        GoToBackManageUsers();
        break;
    }
    case enManageUsers::eMainMenu:
    {
        system("cls");
        ShowMainMenu();
        break;
    }
    }
}

void PerfromTransactions(enTransactions Transactions)
{
    if (!HasPermission(CurrentUser.Permissions, Transactions))
    {
        system("cls");
        cout << "\nAccess Denied! You do not have permission to access transactions.\n";
        GoBackToMainMenu();
        return;
    }
    switch (Transactions)
    {
    case enTransactions::Deposit:
    {
        system("cls");
        ShowDepositClientsScreen();
        GoBackToTransactionsMenu();
        break;
    }
    case enTransactions::Withdraw:
    {
        system("cls");
        ShowWithdrawClientsScreen();
        GoBackToTransactionsMenu();
        break;
    }
    case enTransactions::TotalBalances:
    {
        system("cls");
        ShowTotalBalancesClientsCardScreen();
        GoBackToTransactionsMenu();
        break;
    }
    case enTransactions::MainMenu:
    {
        system("cls");
        ShowMainMenu();
        break;
    }
    }
}

void PerfromMainMenuOption(enMainMenuOptions MainMenuOption)
{
    switch (MainMenuOption)
    {
    case enMainMenuOptions::eListClients:
    {
        if (!HasPermission(CurrentUser.Permissions, showClientList))
        {
            system("cls");
            cout << "\nAccess Denied! You do not have permission to show clients list.\n";
            GoBackToMainMenu();
            break;
        }
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenu();
        break;
    }
    case enMainMenuOptions::eAddNewClient:
    {
        if (!HasPermission(CurrentUser.Permissions, AddNewclient))
        {
            system("cls");
            cout << "\nAccess Denied! You do not have permission to Add New client.\n";
            GoBackToMainMenu();
            break;
        }
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenu();
        break;
    }
    case enMainMenuOptions::eDeleteClient:
    {
        if (!HasPermission(CurrentUser.Permissions, DeleteClient))
        {
            system("cls");
            cout << "\nAccess Denied! You do not have permission to Delete Client.\n";
            GoBackToMainMenu();
            break;
        }
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenu();
        break;
    }
    case enMainMenuOptions::eUpdateClient:
    {
        if (!HasPermission(CurrentUser.Permissions, UpdateClient))
        {
            system("cls");
            cout << "\nAccess Denied! You do not have permission to Update Client.\n";
            GoBackToMainMenu();
            break;
        }
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenu();
        break;
    }
    case enMainMenuOptions::eFindClient:
    {
        if (!HasPermission(CurrentUser.Permissions, FindClient))
        {
            system("cls");
            cout << "\nAccess Denied! You do not have permission to Find Client.\n";
            GoBackToMainMenu();
            break;
        }
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenu();
        break;
    }
    case enMainMenuOptions::eTransactions:
    {
        if (!HasPermission(CurrentUser.Permissions, Transactions))
        {
            system("cls");
            cout << "\nAccess Denied! You do not have permission to Transactions.\n";
            GoBackToMainMenu();
            break;
        }
        ShowTransactionsMenu();
        GoBackToMainMenu();
        break;
    }
    case enMainMenuOptions::eManageUsers:
    {
        if (!HasPermission(CurrentUser.Permissions, ManageUser))
        {
            system("cls");
            cout << "\nAccess Denied! You do not have permission to Manage User.\n";
            GoBackToMainMenu();
            break;
        }
        ShowManageUsersMenu();
        GoBackToMainMenu();
        break;
    }
    case enMainMenuOptions::eLogout:
    {
        system("cls");
        Login();
        break;
    }
    case enMainMenuOptions::eExit:
    {
        system("cls");
        ShowEndScreen();
        exit(0);
        break;
    }
    }
}

void ShowManageUsersMenu()
{
    system("cls");
    cout << "=================================================\n";
    cout << "\t\t   Manage Users Menu Screen\n";
    cout << "=================================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menu.\n";
    cout << "=================================================\n";
    PerfromManageUsers((enManageUsers)ReadManageUsers());
}

void ShowTransactionsMenu()
{
    system("cls");
    cout << "=================================================\n";
    cout << "\t\t   Transactions Menu Screen\n";
    cout << "=================================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menu.\n";
    cout << "=================================================\n";
    PerfromTransactions((enTransactions)ReadTransactions());
}

void ShowMainMenu()
{
    system("cls");
    cout << "\n Welcome, " << CurrentUser.UserName << "! You have logged in successfully.\n";
    cout << "=================================================\n";
    cout << "\t\t   Main Menu Screen\n";
    cout << "=================================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";
    cout << "\t[9] Exit.\n";
    cout << "=================================================\n";
    PerfromMainMenuOption((enMainMenuOptions)ReadMainMenuOption());
}

void Login()
{
    cout << "=================================================\n";
    cout << "\t\t   Login Screen \n";
    cout << "=================================================\n";
    string username, password;
    bool loginSuccess = false;
    do
    {
        cout << "Enter UserName: ";
        getline(cin >> ws, username);
        cout << "Enter Password: ";
        getline(cin, password);
        vector<sUser> vUsers = LoadUsersDataFromFile(UsersFileName);
        for (sUser& user : vUsers)
        {
            if (user.UserName == username && user.Pass == password)
            {
                CurrentUser = user;
                loginSuccess = true;
                break;
            }
        }
        if (!loginSuccess)
        {
            cout << "Invalid credentials! Please try again.\n";
        }
        else
        {
            system("cls");
            ShowMainMenu();
        }
    } while (!loginSuccess);
}

int main()
{
    InitializeFiles();
    Login();
    return 0;
}