#include <cstddef>
#include <iostream>
#include <fstream>
#include <string>
#include <system_error>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <stdlib.h>
using namespace std;
enum EnTransactions { eDeposit = 1, eWithDraw = 2, eTotalBalances = 3, emain_menu = 4 };
enum EnPerformanceOptions { eShowClientList = 1,
    eAddNewClient = 2, eDeleteClient = 3,
    eUpDateClient = 4, eFindClient = 5, 
    enlTransactions = 6, eExit = 7 
};
void ShowTransactionsMenuScreen();
const string ClientsFileName = "Clients.txt";
void StartMainMenu();
struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};
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
sClient ConvertLinetoRecord(string Line, string Seperator ="#//#")
{
    sClient Client;
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
    vector<sClient> vClient;
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
            vClient.push_back(Client);
        }
        MyFile.close();
    }
}
sClient ReadNewClient()
{
    sClient Client;

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
string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;     stClientRecord += Client.PinCode + Seperator;     stClientRecord += Client.Name + Seperator;     stClientRecord += Client.Phone + Seperator;     stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;

}
void AddDataLineToFile(string FileName, string  stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {

        MyFile << stDataLine << endl;
        cout << " Client added successfully\n";
        MyFile.close();
    }

}
void AddNewClientRecord()
{
    sClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));

}
/* hh*/
vector <sClient> LoadCleintsDataFromFile(string FileName)
{
    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode
    if (MyFile.is_open())
    {
        string Line;
        sClient Client;
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
void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "\nAccout Number: " << Client.AccountNumber;
    cout << "\nPin Code : " << Client.PinCode;
    cout << "\nName : " << Client.Name;
    cout << "\nPhone : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
}
/* yu*/
bool FindClientByAccountNumber(string AccountNumber, sClient& Client)
{
    vector <sClient> vClients =
        LoadCleintsDataFromFile(ClientsFileName);
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

/* i  */
string ReadClientAccountNumber()
{
    string AccountNumber = "";
    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;
}
void SearchForClient()
{
    sClient Client;
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
void PrintClientRecord(sClient Client)
{
    cout << "| " << left << setw(15) << Client.AccountNumber;
    cout << "| " << left << setw(10) << Client.PinCode;
    cout << "| " << left << setw(20) << Client.Name;
    cout << "| " << left << setw(12) << Client.Phone;
    cout << "| " << left << setw(12) << Client.AccountBalance << endl;
}
void PrintAllClientsData(vector <sClient> vClients)
{
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).\n";
    cout << "\n__________________________________________";
    cout << "_________________________________________ \n" << endl;
    cout << "| " << left << setw(15) << "Accout Number ";
    cout << "| " << left << setw(10) << "Pin Code ";
    cout << "| " << left << setw(20) << "Client Name ";
    cout << "| " << left << setw(10) << "Phone ";
    cout << "| " << left << setw(12) << "Balance \n";
    cout <<"\n__________________________________________________";
    cout << "_________________________________________\n" << endl;
    for (sClient Client : vClients)
    {
        PrintClientRecord(Client);
    }
    cout << endl;
}
/* i */
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

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {
        for (sClient C : vClients)
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
bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
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

bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    AccountNumber = ReadClientAccountNumber();
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
                    C = ChangeClientRecord(AccountNumber);                     break;
                }

            }

            SaveCleintsDataToFile(ClientsFileName, vClients);

            cout << "\n\nClient Updated Successfully.";
            return true;
        }
    }
    else {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }

}   ///////////////////////////////////////////////////////////////////////////////
/* */
void GoBackToTheMainMenu()
{
    cout << "\n press any key to reurn to the main menu......";
    system("pause>0");
    system("cls");
    StartMainMenu();
}
void GoBackToTransactionMenu()
{
    cout << "\n press any key to go to transactions screen.....";
    system("pause>0");
    system("cls");
    ShowTransactionsMenuScreen();

}
void EndProgram()
{
    cout << " press enter to End Program....";
    system("pause>0");
    exit(0);
}
/* i */
bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    for (sClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;             return true;
        }

    }
    return false;
}
bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    AccountNumber = ReadClientAccountNumber();
    sClient Client;     char Answer = 'n';

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

            cout << "\n\nClient Deleted Successfully.";             return true;
        }
    }
    else {
        cout << "\nClient with Account Number (" << AccountNumber
            << ") is Not Found!";         return false;
    }
}
/* */

short ReadPerformanceOption()
{
    short PerformanceNumber;
    cout << " what do you want to do (1——6)\n";
    cin >> PerformanceNumber;
    return PerformanceNumber;
}

bool DepositBalanceByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients)
{
    char Answer = 'y';
    cout << "\n\n are you sure want to deposit [" << Amount << "] into [" << AccountNumber << "] Account\n";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        for (sClient& C : vClients)
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
}
void showDepositScreen()
{
    system("cls");
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    sClient Client;

    if (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with Account Number [" << AccountNumber << "] is Not Found!\n";
        AccountNumber = ReadClientAccountNumber();
    }
    cout << "\n==================================================\n ";
    cout << Tabs(3) << " Deposit Screen \n";
    cout << "\n==================================================\n ";
    PrintClientCard(Client);
    double Amount = 0;  
    cout << "\n enter Amount to deposit :\n";
    cin >> Amount;
    DepositBalanceByAccountNumber(AccountNumber, Amount, vClients);
}   
void ShowWithDrawScreen()
{

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    sClient Client;

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
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    double TotalBalance = 0;
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).\n";
    cout << "\n__________________________________________\n";
    cout << "\n_________________________________________ \n" << endl;
    cout << "| " << left << setw(15) << "Accout Number ";
    cout << "| " << left << setw(40) << "Client Name ";
    cout << "| " << left << setw(12) << "Balance ";
    cout <<"\n__________________________________________________\n";
    cout << "_________________________________________\n" << endl;
    for (sClient Client : vClients)
    {

        PrintClientRecord(Client);
        TotalBalance += Client.AccountBalance;
    }
    cout << Tabs(3) << " Total Balance = " << TotalBalance << endl;

}
void PerformanceTransactionsOption(EnTransactions TransactionsOption)
{
    vector <sClient> vClients;
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
        GoBackToTransactionMenu;
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
        StartMainMenu();
        break;
    }
    }
}
void ShowTransactionsMenuScreen()
{
    cout << "\n=================================================\n ";
    cout << Tabs(3) << "Transactions Menu Screen \n";
    cout << "\n=================================================\n ";
    cout << Tabs(3) << " [1] Deposit \n";
    cout << Tabs(3) << " [2] WithDraw \n";
    cout << Tabs(3) << " [3] TotalBalances \n";
    cout << Tabs(3) << " [4] Main Menu \n";
    cout << "=================================================\n ";
    PerformanceTransactionsOption(EnTransactions(ReadTransactionOption()));

}
void PerformanceOptions(EnPerformanceOptions PerformanceOption)
{
    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber;
    if (PerformanceOption >= 1 && PerformanceOption <= 7)
    {
        switch (PerformanceOption)
        {
        case EnPerformanceOptions::eShowClientList:
            system("cls");
            PrintAllClientsData(vClients);
            GoBackToTheMainMenu();
            break;
        case EnPerformanceOptions::eAddNewClient:
            system("cls");
            AddNewClientRecord();
            GoBackToTheMainMenu();
            break;
        case EnPerformanceOptions::eDeleteClient:
            system("cls");
            DeleteClientByAccountNumber(AccountNumber, vClients);
            GoBackToTheMainMenu();
            break;
        case EnPerformanceOptions::eUpDateClient:
            system("cls");
            UpdateClientByAccountNumber(AccountNumber, vClients);
            GoBackToTheMainMenu();
            break;
        case EnPerformanceOptions::eFindClient:
            system("cls");
            SearchForClient();
            GoBackToTheMainMenu();
            break;
        case EnPerformanceOptions::enlTransactions:
            system("cls");
            ShowTransactionsMenuScreen();
            GoBackToTheMainMenu();
            break;
        case EnPerformanceOptions::eExit:
            system("cls");
            //exit(0);
            EndProgram();
            break;

        }
    }
    else {
        cout << "\n AN ERROR CHOICE \n";
        system("cls");
        StartMainMenu();
    }
}
void StartMainMenu()

{
    cout << "--------------------------------------------------\n ";
    cout << Tabs(3) << "Main Menu Screen \n";
    cout << "----------------------------------—---------------\n ";
    cout << Tabs(3) << " [1] ShowClientList \n";
    cout << Tabs(3) << " [2] AddNewClient \n";
    cout << Tabs(3) << " [3] DeleteClient \n";
    cout << Tabs(3) << " [4] UpDateClient \n";
    cout << Tabs(3) << " [5] FindClient  \n";
    cout << Tabs(3) << " [6] Transactions\n";
    cout << Tabs(3) << " [7] Exit \n";
    PerformanceOptions(EnPerformanceOptions(ReadPerformanceOption()));
}

int main()
{

    StartMainMenu();

    return 0;
}