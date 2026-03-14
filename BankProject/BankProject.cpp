#include <cstddef>
#include <iostream>
#include <fstream>
#include <string>
#include <system_error>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <stdlib.h>
#include <Windows.h>
using namespace std;
void SetColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";
enum EnTransactions { eDeposit = 1, eWithDraw = 2, eTotalBalances = 3, emain_menu = 4 };
enum EnPerformanceMainMenuOptions { eShowClientList = 1, eAddNewClient = 2, eDeleteClient = 3, eUpDateClient = 4, eFindClient = 5, enlTransactions = 6, eManagUsers = 7,eATMsystem=8, eExit = 9 };

enum enMainMenuPermission { pAll = -1, pShowClientList = 1, pAddNewClient = 2, pDeleteClient = 4, pUpDateClient = 8, pFindClient = 16, pTransactions = 32, pManageUsers = 64 };
enum enUsers { ShowUserList = 1, Add_New_User = 2, DeleteUser = 3, UpDateUser = 4, FindUser = 5, MainMenu = 6 };
enum enATMPerformanceMainMenuOption { enQuickWithdraw = 1, enNormalWithdraw = 2,enDeposit=3, enCheckBalance = 4,enATMLogout=5,enSystemLogout=6 };
struct stClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance = 0;
    bool MarkForDelete = false;
};
stClient CurrentClient;
struct stUsers
{
    string UserName;
    string Password;
    int Permission = 0;//one for admin two for users
    char MarkForDelete = false;
};
stUsers CurrentUser;
void ShowManageUsersMainMenuScreen();
void ShowTransactionsMenuScreen();
void ATMLogin();
bool  CheckAccessPermissions(enMainMenuPermission Permission);
void ShowATMMainMenuScreen(string AccountNumber);
void CheckClientBalance(string AccountNumber, vector<stClient>& vClients);
void ShowTheMainMenuScreen();
void Login();
void ShowAccessDeniedScreen();

void ShowTheMainMenuScreen();
//bool CheckAccessPermissions(enUsersMainMenuPermission);
/* */
vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord = ""; // define a string variable
    // use find() function to get the position of the delimiters
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word
        if (sWord != "")
        {
            vString.push_back(sWord);
        }
        S1.erase(0, pos + Delim.length());
    }
    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }
    return vString;
}
stClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
    stClient Client;
    vector<string> vClientData;
    vClientData = SplitString(Line, Seperator);
    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);

    return Client;

}
bool ClientExistByAccountNumber(string AccountNumber, string FileName)
{
    vector<stClient> vClient;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        string Line;
        stClient Client;
        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
            vClient.push_back(Client);
        }
        MyFile.close();
    }
    return false;
}
stClient ReadNewClient()
{
    stClient Client;

    cout << "Enter Account Number? ";

    // Usage of std::ws will extract allthe whitespace character
    getline(cin >> ws, Client.AccountNumber);

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
string ConvertRecordToLine(stClient Client, string Seperator = "#//#")
{
    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;
}
void AddDataLineToFile(string FileName, string  stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {

        MyFile << stDataLine << endl;
        cout << " Data added successfully\n";
        MyFile.close();
    }

}
void ShowAddNewClientScreen()
{
    if (!CheckAccessPermissions(enMainMenuPermission::pAddNewClient))
    {
        ShowAccessDeniedScreen();
    }
    cout << "\n==================================================\n ";
    cout << "\t\t\t Add New Client Screen \n";
    cout << "\n==================================================\n ";
    stClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));

}
/* hh*/
vector <stClient> LoadCleintsDataFromFile(string FileName)
{
    vector <stClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode
    if (MyFile.is_open())
    {
        string Line;
        stClient Client;
        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}
/* ji*/
void PrintClientCard(stClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "**********************************************\n";
    cout << "\nAccout Number: " << Client.AccountNumber;
    cout << "\nPin Code : " << Client.PinCode;
    cout << "\nName : " << Client.Name;
    cout << "\nPhone : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "***********************************************\n";
}
/* yu*/
bool FindClientByAccountNumber(string AccountNumber, stClient& Client)
{
    vector <stClient> vClients =
        LoadCleintsDataFromFile(ClientsFileName);
    for (stClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }

    }
    return false;

}

/* i  */
string ReadClientAccountNumber()
{
    string AccountNumber = "";
    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;
}
void ShowSearchForClientScreen()
{
    if (!CheckAccessPermissions(enMainMenuPermission::pFindClient))
    {
        ShowAccessDeniedScreen();
    }
    cout << "\n==================================================\n ";
    cout << "\t\t\t Seach Client Screen \n";
    cout << "\n==================================================\n ";
    stClient Client;
    string AccountNumber = ReadClientAccountNumber();
    if (FindClientByAccountNumber(AccountNumber, Client))
    {
        PrintClientCard(Client);
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber <<
            ") is Not Found!";
    }
}

string Tabs(short NumberOfTabs)
{
    string Tab = " ";
    for (int i = 1; i < NumberOfTabs; i++)
    {
        Tab = Tab + "\t";
        // cout << Tab;
    }
    return Tab;
}
/* i*/
void PrintClientRecord(stClient Client)
{
    cout << "||" << left << setw(16) << Client.AccountNumber;
    cout << "||" << left << setw(13) << Client.PinCode;
    cout << "||" << left << setw(21) << Client.Name;
    cout << "||" << left << setw(13) << Client.Phone;
    cout << "||" << left << setw(11) << Client.AccountBalance << endl;
}
void ShowAllClientsList(vector <stClient> vClients)
{
    if (!CheckAccessPermissions(enMainMenuPermission::pShowClientList))
    {
        ShowAccessDeniedScreen();
    }
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).\n";
    cout << "\n***********************************************";
    cout << "*******************************************||\n" << endl;
    cout << "|| " << left << setw(15) << "Accout Number";
    cout << "|| " << left << setw(12) << "Pin Code";
    cout << "|| " << left << setw(20) << "Client Name";
    cout << "|| " << left << setw(12) << "Phone";
    cout << "|| " << left << setw(12) << "Balance";
    cout << "\n*****************************************";
    cout << "*******************************************||\n" << endl;
    for (stClient Client : vClients)
    {
        PrintClientRecord(Client);

    }
    cout << " || " << endl;
    cout << "\n*****************************************";
    cout << "**********************************************\n" << endl;
}
/* i */
stClient ChangeClientRecord(string AccountNumber)
{
    stClient Client;

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

vector <stClient> SaveCleintsDataToFile(string FileName, vector <stClient> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {
        for (stClient C : vClients)
        {
            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }
    return vClients;

}
bool FindClientByAccountNumber(string AccountNumber, vector <stClient> vClients, stClient& Client)
{
    for (stClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

bool ShowUpdateClientByAccountNumberScreen(string AccountNumber, vector <stClient>& vClients)
{
    if (!CheckAccessPermissions(enMainMenuPermission::pUpDateClient))
    {
        ShowAccessDeniedScreen();
    }

    cout << "\n==================================================\n ";
    cout << Tabs(3) << " Update Client Screen \n";
    cout << "\n==================================================\n ";
    AccountNumber = ReadClientAccountNumber();
    stClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);
        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (stClient& C : vClients)
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
    }
    return false;

}   ///////////////////////////////////////////////////////////////////////////////
/* */
void GoBackToTheMainMenu()
{
    cout << "\n press any key to reurn to the main menu......";
    system("pause>0");
    system("cls");
    ShowTheMainMenuScreen();
}
void GoBackToTransactionMenu()
{
    cout << "\n press any key to go to transactions screen.....";
    system("pause>0");
    system("cls");
    ShowTransactionsMenuScreen();

}
/* i */
bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <stClient>& vClients)
{
    for (stClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;             return true;
        }

    }
    return false;
}
bool ShowDeleteClientByAccountNumberScreen(string AccountNumber, vector <stClient>& vClients)
{
    if (!CheckAccessPermissions(enMainMenuPermission::pDeleteClient))
    {
        ShowAccessDeniedScreen();
    }
    SetColor(4);
    cout << "\n==================================================\n ";
    cout << Tabs(3) << " Delete Screen \n";
    cout << "\n==================================================\n ";
    AccountNumber = ReadClientAccountNumber();
    stClient Client;     char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients,
        Client))
    {

        PrintClientCard(Client);

        cout << "\n\nAre you sure you want delete this client? y/n ? ";         cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);

            //Refresh Clients
            vClients = LoadCleintsDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }
    }
    else {
        cout << "\nClient with Account Number (" << AccountNumber
            << ") is Not Found!";
        return false;
    }
    return false;
    SetColor(7);
}
/* */

short ReadPerformanceMainMenuOption()
{
    short PerformanceNumber;
    cout << " what do you want to do (1——6)\n";
    cin >> PerformanceNumber;
    return PerformanceNumber;
}

bool DepositBalanceByAccountNumber(string AccountNumber, double Amount, vector <stClient>& vClients)
{
    char Answer = 'y';
    cout << "\n\n are you sure want to make this actions [" << Amount << "] into [" << AccountNumber << "] Account \n";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        for (stClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully.your New Balance Is = " << C.AccountBalance << "\n";
                return true;
            }
        }
        return false;
    }
    else {
        cout << " sorry your balance [ " << Amount << " ] Not Added successfully";
        ShowTransactionsMenuScreen();
    }
    return false;
}
void showDepositScreen()
{
    SetColor(2);
    cout << "\n==================================================\n ";
    cout << Tabs(3) << " Deposit Screen \n";
    cout << "\n==================================================\n ";
    system("cls");
    vector <stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    stClient Client;

    if (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with Account Number [" << AccountNumber << "] is Not Found!\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);
    double Amount = 0;
    cout << "\n enter Amount to deposit :\n";
    cin >> Amount;
    DepositBalanceByAccountNumber(AccountNumber, Amount, vClients);
    SetColor(7);
}
void ShowWithDrawScreen()
{
    SetColor(4);
    cout << "\n==================================================\n ";
    cout << Tabs(3) << " WithDraw Screen \n";
    cout << "\n==================================================\n ";
    vector <stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    stClient Client;

    if (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        AccountNumber = ReadClientAccountNumber();

    }
    cout << "\n==================================================\n ";
    cout << Tabs(3) << " WithDraw Screen \n";
    cout << "\n==================================================\n ";

    PrintClientCard(Client);
    double Amount = 0;
    cout << " \n\n enter Amount to WithDraw :\n";
    cin >> Amount;
    while (Amount > Client.AccountBalance)
    {

        cout << "\n Amount Exceeds .you can withdraw up to " << Client.AccountBalance;
        cin >> Amount;
    }
    DepositBalanceByAccountNumber(AccountNumber, Amount * -1, vClients);
    SetColor(7);
}
short ReadTransactionOption()
{
    short Option;
    cout << "\n choose what do you want to do [1 to 4]: ";
    cin >> Option;
    return Option;
}
void ShowClientsTotalBalanceScreen()
{
    SetColor(14);
    vector <stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    double TotalBalance = 0;
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).\n";
    cout << "\n__________________________________________";
    cout << "_________________________________________ \n" << endl;
    cout << "| " << left << setw(15) << "Accout Number ";
    cout << "| " << left << setw(40) << "Client Name ";
    cout << "| " << left << setw(12) << "Balance ";
    cout << "\n__________________________________________________\n";
    cout << "_________________________________________\n" << endl;
    for (stClient Client : vClients)
    {
        PrintClientRecord(Client);
        TotalBalance += Client.AccountBalance;
    }
    cout << Tabs(3) << " Total Balance = " << TotalBalance << endl;
    SetColor(7);
}
void PerformanceTransactionsOption(EnTransactions TransactionsOption)
{
    vector <stClient> vClients;
    string AccountNumber;
    switch (TransactionsOption)
    {
    case EnTransactions::eDeposit:
    {
        system("cls");
        showDepositScreen();
        GoBackToTransactionMenu();
        break;
    }
    case EnTransactions::eWithDraw:
    {
        system("cls");
        ShowWithDrawScreen();
        GoBackToTransactionMenu();
        break;
    }
    case EnTransactions::eTotalBalances:
    {
        system("cls");
        ShowClientsTotalBalanceScreen();
        GoBackToTransactionMenu();
        break;
    }
    case EnTransactions::emain_menu:
    {
        system("cls");
        ShowTheMainMenuScreen();
        break;
    }
    }
}
bool  CheckAccessPermission(int UserPermission, enMainMenuPermission Permission)
{
    // Permission = ConvertOptionToPermission(Permission);
    if (UserPermission == enMainMenuPermission::pAll)
        return true;
    return (UserPermission & Permission) != 0;
}
bool CheckAccessPermissions(enMainMenuPermission Permission)
{
    return CheckAccessPermission(CurrentUser.Permission, Permission);
}
void ShowTransactionsMenuScreen()
{
    enMainMenuPermission Permission = {};
    if (!CheckAccessPermissions(enMainMenuPermission::pTransactions))
    {
        ShowAccessDeniedScreen();
    }
    SetColor(14);
    cout << "\n";
    cout << "||********************************************||\n ";
    cout << "||" << Tabs(2) << "Transactions Menu Screen   ||\n";
    cout << "||********************************************||\n ";
    cout << "||" << Tabs(2) << " [1] Deposit               ||\n";
    cout << "||" << Tabs(2) << " [2] WithDraw              ||\n";
    cout << "||" << Tabs(2) << " [3] TotalBalances         ||\n";
    cout << "||" << Tabs(2) << " [4] Main Menu             ||\n";
    cout << "||********************************************||\n ";
    PerformanceTransactionsOption(EnTransactions(ReadTransactionOption()));
    SetColor(7);
}
void PerformanceMainMenuOptions(EnPerformanceMainMenuOptions PerformanceOption)
{
    vector<stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber;
    if (PerformanceOption >= 1 && PerformanceOption <= 8)
    {
        switch (PerformanceOption)
        {
        case EnPerformanceMainMenuOptions::eShowClientList:
            system("cls");
            ShowAllClientsList(vClients);
            GoBackToTheMainMenu();
            break;
        case EnPerformanceMainMenuOptions::eAddNewClient:
            system("cls");
            ShowAddNewClientScreen();
            GoBackToTheMainMenu();
            break;
        case EnPerformanceMainMenuOptions::eDeleteClient:
            system("cls");
            ShowDeleteClientByAccountNumberScreen(AccountNumber, vClients);
            GoBackToTheMainMenu();
            break;
        case EnPerformanceMainMenuOptions::eUpDateClient:
            system("cls");
            ShowUpdateClientByAccountNumberScreen(AccountNumber, vClients);
            GoBackToTheMainMenu();
            break;
        case EnPerformanceMainMenuOptions::eFindClient:
            system("cls");
            ShowSearchForClientScreen();
            GoBackToTheMainMenu();
            break;
        case EnPerformanceMainMenuOptions::enlTransactions:
            system("cls");
            ShowTransactionsMenuScreen();
            GoBackToTheMainMenu();
            break;
        case EnPerformanceMainMenuOptions::eManagUsers:
        {
            system("cls");
            ShowManageUsersMainMenuScreen();
        }
        case EnPerformanceMainMenuOptions::eATMsystem:
        {
            system("cls");
            ATMLogin();
            GoBackToTheMainMenu();
        }
        case EnPerformanceMainMenuOptions::eExit:
            system("cls");
            Login();
            break;

        }
    }
    else {
        cout << "\n AN ERROR CHOICE \n";
        system("cls");
        // ShowTheMainMenuScreen();
        return;
    }

}
/////////////////////////////////////////////////////////////////////////////////
/* this function give user permissions to use pragram; */
int ReadPermission()
{
    short Permissions = 0;
    char Answer = 'Y';
    cout << "\n\n do you want to give this User full access [y/n]:";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        return -1;
    }
    else {
        cout << " \n do you want to access [show client] (y/n):";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            Permissions |= enMainMenuPermission::pShowClientList;
        }
        cout << " \n do you want to access [Add New Client](y/n):";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            Permissions |= enMainMenuPermission::pAddNewClient;
        }
        cout << " \n do you want to access [Delete_Client](y/n):";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            Permissions |= enMainMenuPermission::pDeleteClient;
        }
        cout << " \n do you want to access [UpDate_Client](y/n):";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            Permissions |= enMainMenuPermission::pUpDateClient;
        }
        cout << " \n do you want to access [Find_Client](y/n):";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            Permissions |= enMainMenuPermission::pFindClient;
        }
        cout << " \n do you want to access [Transactions](y/n):";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            Permissions |= enMainMenuPermission::pTransactions;
        }
        cout << " \n do you want to access [Manage_Users](y/n):";
        cin >> Answer;
        if (Answer == 'Y' || Answer == 'y')
        {
            Permissions |= enMainMenuPermission::pManageUsers;
        }
    }
    return Permissions;
}
stUsers CovertUsersDataToRecord(string UsersDataline, string Delim = "#//#")
{
    stUsers User;
    vector <string> vUers;
    vUers = SplitString(UsersDataline, Delim);
    User.UserName = vUers[0];
    User.Password = vUers[1];
    User.Permission = stoi(vUers[2]);
    return User;
}
vector <stUsers> LoadUserDataFromFile(string FileName)
{
    vector <stUsers> vUsers;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode
    if (MyFile.is_open())
    {
        string Line;
        stUsers User;
        while (getline(MyFile, Line))
        {
            User = CovertUsersDataToRecord(Line);
            vUsers.push_back(User);
        }
        MyFile.close();
    }
    return vUsers;
}
bool UserExistByUserName(string UserName, string FileName)
{
    vector<stUsers> vUsers;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        string Line;
        stUsers User;
        while (getline(MyFile, Line))
        {
            User = CovertUsersDataToRecord(Line);
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
stUsers ReadNewUsers()
{
    cout << "\n==================================================\n ";
    cout << Tabs(3) << " Add New User Screen \n";
    cout << "\n==================================================\n ";
    stUsers User;
    string UserName, Password;
    cout << "\n enter User Name: ";
    getline(cin >> ws, User.UserName);
    while (UserExistByUserName(User.UserName, UsersFileName))
    {
        cout << "\n user with username [" << UserName << "] Already exist, please enter another username:";
        getline(cin >> ws, UserName);
    }
    cout << " \n enter password: ";
    getline(cin, User.Password);
    User.Permission = ReadPermission();
    return User;

}
bool FindUserByUserNameRecord(string UserName, stUsers& User)
{
    vector <stUsers> vUsers = LoadUserDataFromFile(UsersFileName);
    for (stUsers U : vUsers)
    {
        if (U.UserName == UserName)
        {
            User = U;
            return true;
        }

    }
    return false;

}

void ShowAccessDeniedScreen()
{
    cout << "\n==================================================\n ";
    cout << Tabs(3) << " Access Denied Screen \n";
    cout << "\n================================================== ";
    cout << "\n\t Sorry you don't have permission to access this option\n";
    GoBackToTheMainMenu();
}
string ReadUserName()
{
    string UserName;
    cout << " enter User Name? ";
    cin >> UserName;
    return UserName;
}
string ConvertUserRecordToLine(stUsers User, string Seperator = "#//#")
{
    string stUserRecord = "";

    stUserRecord += User.UserName + Seperator;
    stUserRecord += User.Password + Seperator;
    stUserRecord += to_string(User.Permission);

    return stUserRecord;
}
void ShowAddNewUserScreen()
{

    stUsers User;
    User = ReadNewUsers();
    AddDataLineToFile(UsersFileName, ConvertUserRecordToLine(User));


}
void PrintUserCard(stUsers User)
{
    cout << " the following are the Uer Details:\n";
    cout << "\n***********************************\n";
    cout << "||" << " User Name : " << User.UserName << Tabs(3) << "||\n";
    cout << "||" << " Password  : " << User.Password << Tabs(3) << "||\n";
    cout << "||" << " Permission: " << User.Permission << Tabs(3) << "||\n";
    cout << " ***********************************\n";
}
void PrintUserRecord(stUsers User)
{
    cout << "\t||" << left << setw(14) << User.UserName;
    cout << "||" << left << setw(14) << User.Password;
    cout << "||" << left << setw(12) << User.Permission << endl;
}
void ShowAllUsersList(vector <stUsers> vUser)
{


    cout << "\n\t\t\t\t Users List [" << vUser.size() << "] User[s]\n";
    cout << "\n\t*******************************************\n";
    cout << "\t||" << left << setw(10) << "UserName:";
    cout << "\t||" << left << setw(10) << "Password:";
    cout << "\t||" << left << setw(12) << "Permission:";
    cout << "\n\t*********************************************\n";
    for (stUsers User : vUser)
    {
        PrintUserRecord(User);
    }
    cout << endl;
}
bool FindUserByUserName(string UserName, vector <stUsers> vUsers, stUsers& User)
{
    for (stUsers U : vUsers)
    {
        if (U.UserName == UserName)
        {
            User = U;
            return true;
        }
    }
    return false;
}
bool MarkUserForDeleteByUserName(string UserName, vector <stUsers>& vUsers)
{
    for (stUsers& U : vUsers)
    {
        if (U.UserName == UserName)
        {
            U.MarkForDelete = true;
            return true;
        }

    }
    return false;
}
stUsers ChangeUserRecord(string UserName)
{
    stUsers User;

    User.UserName = UserName;
    cout << " enter Password :";
    cin >> User.Password;
    User.Permission = ReadPermission();
    return User;

}
vector <stUsers> SaveUserDataToFile(string FileName, vector <stUsers> vUsers)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {
        for (stUsers U : vUsers)
        {
            if (U.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.
                DataLine = ConvertUserRecordToLine(U);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }
    return vUsers;

}
bool ShowDeleteUserByUserNameScreen(string UserName, vector <stUsers>& vUsers)
{

    UserName = ReadUserName();
    stUsers User;
    char Answer = 'n';

    if (FindUserByUserName(UserName, vUsers, User))
    {

        PrintUserCard(User);

        cout << "\n\nAre you sure you want delete this User? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            if ((UserName == "Admin" || UserName == "ADMIN") && User.Permission == -1)
            {
                cout << " sorry you cannot delete this client because has full access to the pragram";
                return false;
            }
            MarkUserForDeleteByUserName(UserName, vUsers);
            SaveUserDataToFile(UsersFileName, vUsers);

            //Refresh Clients
            vUsers = LoadUserDataFromFile(UsersFileName);

            cout << "\n\nUser Deleted Successfully.";
            return true;
        }
    }
    else {
        cout << "\nuser with UserName (" << UserName
            << ") is Not Found!";
        return false;
    }
    return false;
}
bool ShowUpdateUserByUserNameScreen(string UserName, vector <stUsers>& vUsers)
{

    UserName = ReadUserName();
    stUsers User;
    char Answer = 'n';
    if (FindUserByUserName(UserName, vUsers, User))
    {

        PrintUserCard(User);
        cout << "\n\nAre you sure you want update this User? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (stUsers& U : vUsers)
            {
                if (U.UserName == UserName)
                {
                    U = ChangeUserRecord(UserName);
                    break;
                }

            }

            SaveUserDataToFile(UsersFileName, vUsers);

            cout << "\n\nUser Updated Successfully.";
            return true;
        }
    }
    else {
        cout << " User with UerName: " << UserName << " is Not Found!";
    }
    return false;
}   ///////////////////////////////////////////////////////////////////////////////
void ShowSearchForUserScreen()
{

    stUsers User;
    string UserName = ReadUserName();
    if (FindUserByUserNameRecord(UserName, User))
    {
        PrintUserCard(User);
    }
    else
    {
        cout << "User With UserName [" << UserName << "] is Not Found?\n";
    }
}
int ReadUserOption()
{
    int Option;
    cout << Tabs(4) << " Selection :";
    cin >> Option;
    return Option;
}
void GoBackToManageUsersScreen()
{
    cout << " press any key to return to Manage User Main Menu Screen.....";
    system("pause>0");
    system("cls");
    ShowManageUsersMainMenuScreen();
}
void PerformanceUsersOption(enUsers Permission)
{
    stUsers User;
    vector <stUsers> vUsers = LoadUserDataFromFile(UsersFileName);
    switch (Permission)
    {
    case enUsers::ShowUserList:
    {
        system("cls");
        ShowAllUsersList(vUsers);
        GoBackToManageUsersScreen();
        break;
    }
    case enUsers::Add_New_User:
    {
        system("cls");
        ShowAddNewUserScreen();
        GoBackToManageUsersScreen();
        break;
    }
    case enUsers::DeleteUser:
    {
        system("cls");
        ShowDeleteUserByUserNameScreen(UsersFileName, vUsers);
        GoBackToManageUsersScreen();
        break;
    }
    case enUsers::UpDateUser:
    {
        system("cls");
        ShowUpdateUserByUserNameScreen(UsersFileName, vUsers);
        GoBackToManageUsersScreen();
        break;
    }
    case enUsers::FindUser:
    {
        system("cls");
        ShowSearchForUserScreen();
        GoBackToManageUsersScreen();
        break;
    }
    case enUsers::MainMenu:
    {
        system("cls");
        ShowTheMainMenuScreen();
        break;
    }

    }
}

void ShowManageUsersMainMenuScreen()
{
    if (!CheckAccessPermissions(enMainMenuPermission::pManageUsers))
    {
        ShowAccessDeniedScreen();
        return;

    }
    SetColor(14);
    cout << "\t||***********************************************||\n ";
    cout << "\t||" << Tabs(2) << "Manage Users Menu Screen      ||\n";
    cout << "\t||***********************************************||\n ";
    cout << "\t||" << Tabs(2) << " [1] Show Users List          ||\n";
    cout << "\t||" << Tabs(2) << " [2] Add New User             ||\n";
    cout << "\t||" << Tabs(2) << " [3] Delete User              ||\n";
    cout << "\t||" << Tabs(2) << " [4] UpDate User              ||\n";
    cout << "\t||" << Tabs(2) << " [6] Main Menu                ||\n";
    cout << "\t****************************************************\n";
    PerformanceUsersOption(enUsers(ReadUserOption()));
    SetColor(7);
}
void ShowTemporaryMessage(int Color)
{
    //here you can set the color you want..
    SetColor(Color);
    //sleep functiion included from windows library ,,delete the message after 10 second with \r message \r mark..
   // Sleep(Seconds*1000);
    //here is the message
   // cout << "\r" << string(Message.length(), ' ') << "\r";
    //\r Message \r delete the middle message..
}
void ShowTheMainMenuScreen()
{
    system("cls");
    ShowTemporaryMessage(2);
    cout << "\n\t||*************************************||\n ";
    cout << "\t||" << Tabs(2) << "Main Menu Screen               ||\n ";
    cout << "\t||*************************************||\n ";
    cout << "\t||" << Tabs(2) << " [1] ShowClientList            ||\n";
    cout << "\t||" << Tabs(2) << " [2] AddNewClient              ||\n";
    cout << "\t||" << Tabs(2) << " [3] DeleteClient              ||\n";
    cout << "\t||" << Tabs(2) << " [4] UpDateClient              ||\n";
    cout << "\t||" << Tabs(2) << " [5] FindClient                ||\n";
    cout << "\t||" << Tabs(2) << " [6] Transactions              ||\n";
    cout << "\t||" << Tabs(2) << " [7] Manage Users              ||\n";
    cout << "\t||" << Tabs(2) << " [8] ATM system                ||\n";
    cout << "\t||" << Tabs(2) << " [9] LogOut                    ||\n";
    cout << "\t||*************************************||\n";
    PerformanceMainMenuOptions(EnPerformanceMainMenuOptions(ReadPerformanceMainMenuOption()));
}
bool FindUserByUserNameAndPassword(string UserName, string Password, stUsers& User)
{
    vector <stUsers> vUsers = LoadUserDataFromFile(UsersFileName);
    for (stUsers &U : vUsers)
    {
        if (U.UserName == UserName && U.Password == Password)
        {
            User = U;
            return true;

        }
    }
    return false;
}
bool LoadUserInfo(string UserName, string Password)
{
    if (FindUserByUserNameAndPassword(UserName, Password, CurrentUser))
    {
        return true;
    }
    return false;
}
/*Here is the starting of ATM Management Systen*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
short ReadQuickWithdrawOption()
{
    short Option;
    cout << "\n what do you want to do [1--8] ";
    cin >> Option;
    return Option;
}
int PerformanceQuickWithdrawOption(short Option)
{
    switch (Option)
    {
    case 1:
        return 100;
    case 2:
        return 200;
    case 3:
        return 300;
    case 4:
        return 400;
    case 5:
        return 500;
    case 6:
        return 600;
    case 7:
        return 900;
    case 8:
        return 1000;
    default:
        return 0;
    }
}
void ShowQuickWithdrawScreen(string &AccountNumber, vector <stClient> vClients)
{
    stClient Client;
    system("cls");
    SetColor(7);
    cout << "\n[1] 100 \t\t [2] 200\n";
    cout << "[3] 300 \t\t [4] 400\n";
    cout << "[5] 500 \t\t [6] 600\n";
    cout << "[7] 900 \t\t [8] 1000\n";
    DepositBalanceByAccountNumber(AccountNumber, PerformanceQuickWithdrawOption(ReadQuickWithdrawOption())* -1, vClients);
}
void ShowNormalWithdrawScreen(string &AccountNumber, vector <stClient> &vClients)
{
    stClient Client;
    
    system("cls");
    SetColor(7);
    cout << "||***************************************||\n";
    cout << "||\t Normal Withdraw Screen              ||\n";
    cout << "||***************************************||\n";
    double Amount = 0;
    cout << " \nEnter amout to  Withdraw ";
    cin >> Amount;
    for (stClient &C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            break;
        }
    }
    while (Amount > Client.AccountBalance)
    {
        cout << " \nAmount Exceeds,You can withdraw up to " << Client.AccountBalance << " ";
        cin >> Amount;
    }
    DepositBalanceByAccountNumber(AccountNumber, Amount * -1, vClients);
}
void ShowATMDepositScreen(string &AccountNumber, vector <stClient> &vClients)
{
    system("cls");
    stClient Client;
    
    SetColor(2);
    cout << "\n";
    cout << "||*************************************************||\n\n";
    cout << "||" << Tabs(3) << " Deposit Screen                   ||\n\n";
    cout << "||*************************************************||\n\n";
    CheckClientBalance(AccountNumber,vClients);
    int Amount = 0;
    cout << "\nEnter Amount To Deposit abstract /5 ";
    cin >> Amount;
    
    if ((Amount >= 20 )&& Amount % 5 == 0)
    {
        DepositBalanceByAccountNumber(AccountNumber, Amount, vClients);
    }
    else
    {
        system("cls");
        ShowATMDepositScreen(AccountNumber,vClients);
        return;
    }
        

}
short ReadATMMenuOption()
{
    short Option = 0;
    cout << " what do you want to do [1-3] ";
    cin >> Option;
    return Option;
}
void GoBackToATMMainMenuScreen(string AccountNumber)
{
    
    cout << "press any key to return to ATM Main M,enu Screen....";
    system("pause>0");
    system("cls");    
    ShowATMMainMenuScreen(AccountNumber);

}
void CheckClientBalance(string AccountNumber, vector<stClient> &vClients)
{
        stClient Client;
        
        
        for (stClient &C : vClients)
        {
          
            if (C.AccountNumber == AccountNumber)
            {

                cout << "\n your Balance is " << C.AccountBalance << endl << endl;
            }
            return;
        }
    
}
void ShowCheckBalanceScreen(string AccountNumber,vector <stClient> &vClients)
{
   
    cout << "||**********************************||\n";
    cout << "||\t Check Balance Screen           ||\n";
    cout << "||**********************************||\n";
    CheckClientBalance(AccountNumber,vClients);
}
bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, stClient &Client)
{
    vector <stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    for (stClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
        {
            Client = C;
            return true;

        }
    }
    return false;
}
bool LoadClientInfo(string UserName, string Password)
{
    if (FindClientByAccountNumberAndPinCode(UserName, Password, CurrentClient))
    {
        return true;
    }
    return false;
}
void PerformanceATMSystemOption(short Option, string &AccountNumber)
{
    stClient Client;
    vector <stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    switch (Option)
    {
    case enATMPerformanceMainMenuOption::enQuickWithdraw:
    {
        system("cls");
        ShowQuickWithdrawScreen(AccountNumber, vClients);
        GoBackToATMMainMenuScreen(AccountNumber);
        break;
    }
    case enATMPerformanceMainMenuOption::enNormalWithdraw:
    {
        system("cls");
        ShowNormalWithdrawScreen(AccountNumber,vClients);
        GoBackToATMMainMenuScreen(AccountNumber);
        break;
    }
    case enATMPerformanceMainMenuOption::enDeposit:
    {
        system("cls");
        ShowATMDepositScreen(AccountNumber,vClients);
        GoBackToATMMainMenuScreen(AccountNumber);
    }
    case enATMPerformanceMainMenuOption::enCheckBalance:
    {
        system("cls");
        ShowCheckBalanceScreen(AccountNumber,vClients);
        GoBackToATMMainMenuScreen(AccountNumber);
        break;
    }
    case enATMPerformanceMainMenuOption::enATMLogout:
    {
        system("cls");
        ATMLogin();
        break;
    }
    case enATMPerformanceMainMenuOption::enSystemLogout:
    {
        system("cls");
        Login();
        break;
    }
    }
}
void ShowATMMainMenuScreen(string AccountNumber)
{
    SetColor(2);
    system("cls");
    cout << "|| *********************||\n";
    cout << "|| [1] Quick WithDraw   ||\n";
    cout << "|| [2] Normal WithDraw  ||\n";
    cout << "|| [3] Deposit          ||\n";
    cout << "|| [4] Check Balance    ||\n";
    cout << "|| [5] ATM Logout       ||\n";
    cout << "|| [6] systen Logout    ||\n";
    cout << "|| *********************||\n";
    PerformanceATMSystemOption(enATMPerformanceMainMenuOption(ReadATMMenuOption()), AccountNumber);
}
void ATMLogin()
{
    string AccountNumber, PinCode;
    bool LoginFaileld = false;
    do {
        system("cls");
        cout << "\n";
        cout << Tabs(2) << "*******************\n";
        cout << Tabs(2) << "|| Login Page    ||\n";
        cout << Tabs(2) << "*******************\n";

        if (LoginFaileld)
        {

            cout << "\n\n" << Tabs(2) << " Login failed,please try again: \n";

        }
        cout << "\n\tAccount Number:";
        getline(cin >> ws, AccountNumber);
        cout << "\n\tPassword:";
        getline(cin, PinCode);

        LoginFaileld = !LoadClientInfo(AccountNumber, PinCode);
    } while (LoginFaileld);
    ShowATMMainMenuScreen(AccountNumber);
}
/*Here is the end of ATM Management System*/ 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void  Login()
{
    string UserName, Password;
    bool LoginFaileld = false;
    do {
        system("cls");
        cout << "\n";
        cout << Tabs(2) << "*******************\n";
        cout << Tabs(2) << "|| Login Page    ||\n";
        cout << Tabs(2) << "*******************\n";

        if (LoginFaileld)
        {
            cout << "\n\n" << Tabs(2) << " Login failed,please try again: \n";

        }
        cout << "\n\tUserName:";
        getline(cin >> ws, UserName);
        cout << "\n\tPassword:";
        getline(cin, Password);

        LoginFaileld = !LoadUserInfo(UserName, Password);
    } while (LoginFaileld);
    SetColor(7);
    ShowTheMainMenuScreen();
}
int main()
{

    Login();
    return 0;
}