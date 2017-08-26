/*
Zachary Parker
zbp0003@auburn.edu
Project2.cpp

Compiled using g++ compiler on University TUX machine. 
Compiled with Boost's string algorithms (http://www.boost.org/).
Only compile this .cpp file if the provided 3 files (client-info, account-info, and staff) are in the same directory.

I used references from http://stackoverflow.com and http://cplusplus.com to obtain various pieces of information,
such as how to convert strings to integers or how to use cin twice in order to avoid skipped input. 
Additionally, I included a boost library in order to effectively trim white space out of strings.

I also used files posted on Canvas for clarification of syntax in order to complete this assignment.
*/

#include <fstream> 
#include <iostream> 
#include <cstdlib>
#include <string>
#include <sstream>
#include <boost/algorithm/string.hpp>
using namespace std;

const int ARRAY_MAX = 100;

class Menu;
class Staff;
class Admin;
class FileTracker;
struct Client;
struct Account;

struct Client {
   Client();
   string name;
   string address;
   string ssn;
   string employer;
   string income;
};

struct Account {
   Account();
   string name;
   string acctNum;
   string acctType;
   string balance;
};

struct User {
   User();
   string username;
   string password;
   string role;
};

class Staff {
public:
   Staff();
   int setUsername(string usernameIn);
   int setPassword(string usernameIn);
   string getUsername();
   string getPassword();
   int addClient(FileTracker &filetracker);
   int addAccount(FileTracker &filetracker);
   int editClientInfo(FileTracker &filetracker);
   int manageAccount(FileTracker &filetracker);
   int saveInfo(FileTracker &filetracker);
   int changePassword(string passwordIn, FileTracker &filetracker);
private:
   string username;
   string password;
   Client client;
   Account account;
};

class Admin : public Staff { // inherits from staff
public:
   Admin();
   int addUser(FileTracker &filetracker);
   int deleteUser(FileTracker &filetracker);
   int displayUsers(FileTracker &filetracker);
private:
   string username;
   string password;
   User user;
};

class FileTracker {
public:
   FileTracker();
   bool onlyWhiteSpace(string stringIn);
   bool clientHasAccount(string nameIn);
   bool isAdmin(string usernameIn, string passwordIn);
   bool isStaff(string usernameIn, string passwordIn);
   bool isClient(string clientNameIn);
   bool isAccount(string acctNumIn);
   bool isUser(string usernameIn);
   string getNameOfClient(string acctNumIn);
   int deposit(string acctNumIn, int amount);
   int withdraw(string acctNumIn, int amount);
   int printUserArray();
   int fillClientArray();
   int fillAccountArray();
   int fillUserArray();
   int clientWritefile();
   int accountWritefile();
   int userWritefile();
   int printClientInfo(string nameIn);
   int updateClientInfo(string nameIn);
   int addClientToArray(Client client);
   int addAccountToArray(Account account);
   int addUserToArray(User user);
   int deleteUserFromArray(string nameIn);
   int changePasswordInArray(string usernameIn, string passwordIn);
private:
   Client clientArray[ARRAY_MAX];
   Account accountArray[ARRAY_MAX];
   User userArray[ARRAY_MAX];
};

class Menu {
public:
   Menu();
   int initializeArrays();
   int loginOptions();
   int login();
   int quit();
   int adminOptions();
   int clientManagement();
   int backToAdminOptions(); 
   int backToLogin();
   int staffOptions();
   int backToStaffOptions();
private:
   FileTracker filetracker;
   Staff staff;
   Admin admin;
};

/*
CONSTRUCTORS
*/

Client::Client() {
   name = "";
   address = "";
   ssn = "";
   employer = "";
   income = "";
}

Account::Account() {
   name = "";
   acctNum = "";
   acctType = "";
   balance = "";
}

User::User() {
   username = "";
   password = "";
   role = "";
}

Menu::Menu() {
	//empty
}

Staff::Staff() {
   username = "";
   password = "";
}

Admin::Admin() {
   username = "";
   password = "";
}

FileTracker::FileTracker() {
	//empty
}

/*
Menu member functions 
*/

int Menu::initializeArrays() {
   filetracker.fillClientArray();
   filetracker.fillAccountArray();
   filetracker.fillUserArray();

   loginOptions();

   return 0;
}

int Menu::loginOptions() {
   string input;
   int inputInt;
   bool repeat;

   cout << "\n\t===========================================================\n";
   cout << "\t        Welcome to the Auburn Branch of Tiger Bank!        \n";
   cout << "\t===========================================================\n";

   cout << "1) Login\n";
   cout << "2) Quit\n";
   cout << "\tPlease choose an option: ";

   do {
      cin >> input;
      inputInt = atoi(input.c_str());
      repeat = false;
   
      switch (inputInt) {
         case 1:
            login();
            break;
         case 2:
            quit();
            break;
      
         default:
            cout << "Your input was invalid. Please enter a valid input (1 or 2): ";
            repeat = true;
      }
   } while (repeat);

   return 0;
}


int Menu::login() {
   string name, pw;
   bool repeat;

   cout << "\n\t===========================================================\n";
   cout << "\t        Login to Access the Teller Terminal System         \n";
   cout << "\t===========================================================\n";

   do {
      repeat = false;
      cout << "Username: ";
      cin >> name;
      cout << "Password: ";
      cin >> pw;
   
      if (filetracker.isAdmin(name, pw)) {
         staff = Staff();
         admin.setUsername(name);
         admin.setPassword(pw);
         adminOptions();
      }
      else if (filetracker.isStaff(name, pw)) {
         admin = Admin();
         staff.setUsername(name);
         staff.setPassword(pw);
         staffOptions();
      }
      else {
         cout << "The username or password is incorrect. Try again!\n\n";
         repeat = true;
      }
   
   } while (repeat);

   return 0;
}

int Menu::quit() {
   string input;
   bool repeat;

   cout << "\nAre you sure you want to quit? (Y/N): ";

   do {
      repeat = false;
      cin >> input;
   
      if (input == "Y" || input == "y") {
         cout << "\nThank you for using the Teller Terminal System!\n";
         cout << "Exiting program... \n";
         exit(1);
      }
      else if (input == "N" || input == "n") {
         loginOptions();
      }
      else {
         cout << "Please enter a valid input (Y or N): ";
         repeat = true;
      }
   } while (repeat);

   return 0;
}

int Menu::adminOptions() {
   string input;
   int inputInt;
   bool repeat;

   cout << "\n\t===========================================================\n";
   cout << "\t       Teller Terminal System - System Administration      \n";
   cout << "\t===========================================================\n";
   cout << "1) Client and Account Management\n";
   cout << "2) Add a branch staff member\n";
   cout << "3) Delete a branch staff member\n";
   cout << "4) Display branch staff\n";
   cout << "5) Change password\n";
   cout << "6) Save and logout\n";
   cout << "\tPlease choose an option: ";

   do {
      cin >> input;
      inputInt = atoi(input.c_str());
      repeat = false;
   
      switch (inputInt) {
         case 1:
            clientManagement();
            break;
         case 2:
            admin.addUser(filetracker);
            repeat = true;
            cout << "1) Client and Account Management\n";
            cout << "2) Add a branch staff member\n";
            cout << "3) Delete a branch staff member\n";
            cout << "4) Display branch staff\n";
            cout << "5) Change password\n";
            cout << "6) Save and logout\n";
            cout << "\tPlease choose an option: ";
            break;
         case 3:
            admin.deleteUser(filetracker);
            repeat = true;
            cout << "1) Client and Account Management\n";
            cout << "2) Add a branch staff member\n";
            cout << "3) Delete a branch staff member\n";
            cout << "4) Display branch staff\n";
            cout << "5) Change password\n";
            cout << "6) Save and logout\n";
            cout << "\tPlease choose an option: ";
            break;
         case 4:
            admin.displayUsers(filetracker);
            repeat = true;
            cout << "1) Client and Account Management\n";
            cout << "2) Add a branch staff member\n";
            cout << "3) Delete a branch staff member\n";
            cout << "4) Display branch staff\n";
            cout << "5) Change password\n";
            cout << "6) Save and logout\n";
            cout << "\tPlease choose an option: ";
            break;
         case 5:
			 admin.changePassword(admin.getPassword(), filetracker); 
            repeat = true;
            cout << "1) Client and Account Management\n";
            cout << "2) Add a branch staff member\n";
            cout << "3) Delete a branch staff member\n";
            cout << "4) Display branch staff\n";
            cout << "5) Change password\n";
            cout << "6) Save and logout\n";
            cout << "\tPlease choose an option: ";
            break;
         case 6:
            backToLogin();
            repeat = true;
            cout << "1) Client and Account Management\n";
            cout << "2) Add a branch staff member\n";
            cout << "3) Delete a branch staff member\n";
            cout << "4) Display branch staff\n";
            cout << "5) Change password\n";
            cout << "6) Save and logout\n";
            cout << "\tPlease choose an option: ";
            break;
         default:
            cout << "Your input was invalid. Please enter a valid input (1, 2, 3, 4, 5, or 6): ";
            repeat = true;
      }
   } while (repeat);

   return 0;
}

int Menu::clientManagement() {
   string input;
   int inputInt;
   bool repeat;

   cout << "\n\t===========================================================\n";
   cout << "\t   Teller Terminal System - Client and Account Management  \n";
   cout << "\t===========================================================\n";
   cout << "1) Add a client\n";
   cout << "2) Add an account\n";
   cout << "3) Edit client information\n";
   cout << "4) Manage an account\n";
   cout << "5) Save client and account information\n";
   cout << "6) Exit\n";
   cout << "\tPlease choose an option: ";

   do {
      cin >> input;
      inputInt = atoi(input.c_str());
      repeat = false;
   
      switch (inputInt) {
         case 1:
            staff.addClient(filetracker);
            repeat = true;
            cout << "1) Add a client\n";
            cout << "2) Add an account\n";
            cout << "3) Edit client information\n";
            cout << "4) Manage an account\n";
            cout << "5) Save client and account information\n";
            cout << "6) Exit\n";
            cout << "\tPlease choose an option: ";
            break;
         case 2:
            staff.addAccount(filetracker);
            repeat = true;
            cout << "1) Add a client\n";
            cout << "2) Add an account\n";
            cout << "3) Edit client information\n";
            cout << "4) Manage an account\n";
            cout << "5) Save client and account information\n";
            cout << "6) Exit\n";
            cout << "\tPlease choose an option: ";
            break;
         case 3:
            staff.editClientInfo(filetracker);
            repeat = true;
            cout << "1) Add a client\n";
            cout << "2) Add an account\n";
            cout << "3) Edit client information\n";
            cout << "4) Manage an account\n";
            cout << "5) Save client and account information\n";
            cout << "6) Exit\n";
            cout << "\tPlease choose an option: ";
            break;
         case 4:
            staff.manageAccount(filetracker);
            repeat = true;
            cout << "1) Add a client\n";
            cout << "2) Add an account\n";
            cout << "3) Edit client information\n";
            cout << "4) Manage an account\n";
            cout << "5) Save client and account information\n";
            cout << "6) Exit\n";
            cout << "\tPlease choose an option: ";
            break;
         case 5:
            staff.saveInfo(filetracker);
            repeat = true;
            cout << "1) Add a client\n";
            cout << "2) Add an account\n";
            cout << "3) Edit client information\n";
            cout << "4) Manage an account\n";
            cout << "5) Save client and account information\n";
            cout << "6) Exit\n";
            cout << "\tPlease choose an option: ";
            break;
         case 6:
            if (filetracker.isAdmin(admin.getUsername(), admin.getPassword())) {
               backToAdminOptions();
            }
            else {
               backToStaffOptions();
            }
         
            repeat = true;
            cout << "1) Add a client\n";
            cout << "2) Add an account\n";
            cout << "3) Edit client information\n";
            cout << "4) Manage an account\n";
            cout << "5) Save client and account information\n";
            cout << "6) Exit\n";
            cout << "\tPlease choose an option: ";
            break;
         default:
            cout << "Your input was invalid. Please enter a valid input (1, 2, 3, 4, 5, or 6): ";
            repeat = true;
      }
   } while (repeat);

   return 0;
}

int Menu::backToAdminOptions() {
   string input;
   bool repeat;

   cout << "\nAre you sure you want to go back to the system administrator options menu? (Y/N): ";

   do {
      repeat = false;
      cin >> input;
   
      if (input == "Y" || input == "y") {
         adminOptions();
      }
      else if (input == "N" || input == "n") {
         cout << "\n";
         return 0;
      }
      else {
         cout << "Please enter a valid input (Y or N): ";
         repeat = true;
      }
   } while (repeat);

   return 0;
}

int Menu::backToLogin() {
   string input;
   bool repeat;

   cout << "Are you sure you want to logout? (Y/N): ";

   do {
      repeat = false;
      cin >> input;
   
      if (input == "Y" || input == "y") {
         if (filetracker.isAdmin(admin.getUsername(), admin.getPassword())) {
            cout << "User information has been saved in the staff file.\n";
         }
         cout << "Logging off...\n";
         filetracker.userWritefile();
         loginOptions();
      }
      else if (input == "N" || input == "n") {
         cout << "\n";
         return 0;
      }
      else {
         cout << "Please enter a valid input (Y or N): ";
         repeat = true;
      }
   } while (repeat);

   return 0;
}

int Menu::staffOptions() {
   string input;
   int inputInt;
   bool repeat;

   cout << "\n\t===========================================================\n";
   cout << "\t           Teller Terminal System - Branch Staff           \n";
   cout << "\t===========================================================\n";
   cout << "1) Client and Account Management\n";
   cout << "2) Change Password\n";
   cout << "3) Save and logout\n";
   cout << "\tPlease choose an option: ";

   do {
      cin >> input;
      inputInt = atoi(input.c_str());
      repeat = false;
   
      switch (inputInt) {
         case 1:
            clientManagement();
            repeat = true; 
            break;
         case 2:
            staff.changePassword(staff.getPassword(), filetracker);
            repeat = true;
            cout << "1) Client and Account Management\n";
            cout << "2) Change Password\n";
            cout << "3) Save and logout\n";
            cout << "\tPlease choose an option: ";
            break;
         case 3:
            backToLogin();
            repeat = true;
            cout << "1) Client and Account Management\n";
            cout << "2) Change Password\n";
            cout << "3) Save and logout\n";
            cout << "\tPlease choose an option: ";
            break;
         default:
            cout << "Your input was invalid. Please enter a valid input (1, 2 or 3): ";
            repeat = true;
      }
   } while (repeat);

   return 0;
}

int Menu::backToStaffOptions() {
   string input;
   bool repeat;

   cout << "\nAre you sure you want to go back to the branch staff options menu? (Y/N): ";

   do {
      repeat = false;
      cin >> input;
   
      if (input == "Y" || input == "y") {
         staffOptions();
      }
      else if (input == "N" || input == "n") {
         return 0;
      }
      else {
         cout << "Please enter a valid input (Y or N): ";
         repeat = true;
      }
   } while (repeat);

   return 0;
}

/*
Staff member functions 
*/

int Staff::setUsername(string usernameIn) {
   username = usernameIn;
   return 0;
}

int Staff::setPassword(string passwordIn) {
   password = passwordIn;
   return 0;
}

string Staff::getUsername() {
   return username;
}

string Staff::getPassword() {
   return password;
}

int Staff::addClient(FileTracker &filetracker) {
   cout << "\nA new client will be added...";
   cout << "\nClient name: ";
   getline(cin, client.name);
   getline(cin, client.name);
   boost::trim_right(client.name);
   if (filetracker.isClient(client.name)) {
      cout << "ERROR: There is already a client with this name!\n\n";
      return 0;
   }

   cout << "Address: ";
   getline(cin, client.address);
   cout << "Social security number: ";
   getline(cin, client.ssn);
   cout << "Employer: ";
   getline(cin, client.employer);
   cout << "Annual income: ";
   getline(cin, client.income);

   filetracker.addClientToArray(client);

   return 0;
}

int Staff::addAccount(FileTracker &filetracker) {
   string name;
   cout << "\nChoose a client: ";
   getline(cin, name);
   getline(cin, name);
   boost::trim_right(name);
   if (filetracker.isClient(name) && !filetracker.clientHasAccount(name)) {
      account.name = name;
      cout << "A new account will be created for " << account.name << "...\n";
      cout << "Account number: ";
      getline(cin, account.acctNum);
      cout << "Account type: ";
      getline(cin, account.acctType);
      cout << "Balance: ";
      getline(cin, account.balance);
   
      filetracker.addAccountToArray(account);
   }
   else {
      cout << "ERROR: Either client was not found in the system or client already has an account!\n\n";
   }

   return 0;
}

int Staff::editClientInfo(FileTracker &filetracker) {
   string name, input;
   bool repeat = true;
	
   cout << "\nChoose a client: ";
   getline(cin, name);
   getline(cin, name);
   boost::trim_right(name);
   if (!filetracker.isClient(name)) {
      cout << "ERROR: Client not found!\n\n";
      return 0;
   }

   cout << "\nDisplaying " << name << "'s Information: ";

   filetracker.printClientInfo(name);

   cout << "\nClient " << name << "'s information will be updated...";
   cout << "\n1) Confirm";
   cout << "\n2) Cancel";
   cout << "\n\tPlease choose an option: ";

   do {
      repeat = false;
      cin >> input;
   	
      int inputInt = atoi(input.c_str());
   
      switch (inputInt) {
         case 1:
            if (filetracker.isClient(name)) {
               filetracker.updateClientInfo(name);
            }
            else {
               cout << "ERROR: Client not found!\n\n";
            }
            break;
         case 2:
            cout << "Going back to client management menu...\n\n";
            return 0;
            break;
         default:
            cout << "Your input was invalid. Please enter a valid input (1 or 2): ";
            repeat = true;
      }
   
   } while (repeat);

   return 0;
}

int Staff::manageAccount(FileTracker &filetracker) {
   string acctNum, input;
   int inputInt, amount;
   bool repeat;

   cout << "\nEnter the number of the account to be managed: ";
   cin >> acctNum;
   boost::trim_right(acctNum);

   if (!filetracker.isAccount(acctNum)) {
      cout << "ERROR: There is no account with this number in the database!\n\n";
      return 0;
   }

   cout << "\nNow managing account " << acctNum << " for " << filetracker.getNameOfClient(acctNum) << ": \n";
   cout << "1) Deposit\n";
   cout << "2) Withdraw\n";
   cout << "3) Cancel\n";
   cout << "\tPlease choose an option: ";

   do {
      repeat = false;
      amount = 0;
   
      cin >> input;
      inputInt = atoi(input.c_str());
   
      switch (inputInt) {
         case 1:
            cout << "Deposit amount: ";
            cin >> amount;
            filetracker.deposit(acctNum, amount);
            repeat = true;
            cout << "1) Deposit\n";
            cout << "2) Withdraw\n";
            cout << "3) Cancel\n";
            cout << "\tPlease choose an option: ";
            break;
         case 2:
            cout << "Withdraw amount: ";
            cin >> amount;
            filetracker.withdraw(acctNum, amount);
            repeat = true;
            cout << "1) Deposit\n";
            cout << "2) Withdraw\n";
            cout << "3) Cancel\n";
            cout << "\tPlease choose an option: ";
            break;
         case 3:
            cout << "Returning to menu...\n\n";
            return 0;
            break;
         default:
            cout << "Your input was invalid. Please enter a valid input (1, 2 or 3): ";
            repeat = true;
      }
   } while (repeat);

   return 0;
}

int Staff::saveInfo(FileTracker &filetracker) {
   filetracker.clientWritefile();
   cout << "\nClient information has been saved in the client-info file.\n";
   filetracker.accountWritefile();
   cout << "Account information has been saved in the account-info file.\n\n";
   return 0;
}

int Staff::changePassword(string passwordIn, FileTracker &filetracker) {
   string newPassword;
   bool repeat = true;

   do {
      cout << "New password (use any character except spaces): ";
      cin >> newPassword;
      boost::trim_right(newPassword);
   
      if (newPassword == passwordIn) {
         cout << "ERROR: Your new password can't be the same as your old password!\n\n";
      }
      else {
         setPassword(newPassword);
         filetracker.changePasswordInArray(getUsername(), getPassword());
         cout << "Password was changed!\n\n";
         repeat = false;
      }
   } while (repeat);

   return 0;
}

/*
Admin member functions 

*/

int Admin::addUser(FileTracker &filetracker) {
   string name, pw, role;
   bool repeat = true;

   cout << "\nUsername: ";
   getline(cin, name);
   getline(cin, name);
   boost::trim_right(name);
   if (filetracker.isUser(name)) {
      cout << "ERROR: User is already in the system!\n\n";
      return 0;
   }

   cout << "Password (any character except spaces): ";
   getline(cin, pw);

   user.username = name;
   user.password = pw;

   do {
      cout << "Role (1 - Administrator | 2 - Branch Staff): ";
      getline(cin, role);
      int roleInt = atoi(role.c_str());
   
      switch (roleInt) {
         case 1:
            user.role = "System Administrator";
            filetracker.addUserToArray(user);
            cout << "A new system administrator was added!\n\n";
            repeat = false;
            break;
         case 2:
            user.role = "Branch Staff";
            filetracker.addUserToArray(user);
            cout << "A new branch staff member was added!\n\n";
            repeat = false;
            break;
         default:
            cout << "Invalid input! Please enter 1 or 2.\n";
      }
   } while (repeat);

   return 0;
}

int Admin::deleteUser(FileTracker &filetracker) {
   string name, input;
   bool repeat = true;

   cout << "\nPlease enter username of user to be deleted: ";
   getline(cin, name);
   getline(cin, name);
   boost::trim_right(name);

   if (name == "admin") {
      cout << "ERROR: The main system administrator cannot be deleted!\n\n";
      return 0;
   }

   if (name == getUsername()) {
	   cout << "ERROR: You cannot delete the user that is currently using the system!\n\n";
	   return 0;
   }

   if (!filetracker.isUser(name)) {
	   cout << "ERROR: User is not in the system!\n\n";
	   return 0;
   }

   cout << "1) Confirm\n";
   cout << "2) Cancel\n";
   cout << "\tPlease choose an option: ";
   do {
      getline(cin, input);
      int inputInt = atoi(input.c_str());
   
      switch (inputInt) {
         case 1:
            filetracker.deleteUserFromArray(name);
            cout << "User was deleted from the system.\n\n";
            repeat = false;
            break;
         case 2:
            cout << "Returning to system administration main menu...\n\n";
            return 0;
         default:
            cout << "Your input was invalid. Please enter a valid input (1 or 2): ";
      }
   } while (repeat);

   return 0;
}

int Admin::displayUsers(FileTracker &filetracker) {
   filetracker.printUserArray();
   return 0;
}

/*
FileTracker member functions 
*/

bool FileTracker::onlyWhiteSpace(string stringIn) {
   return (stringIn.find_first_not_of(' ') == stringIn.npos);
}

bool FileTracker::clientHasAccount(string nameIn) {
   for (int i = 0; i < ARRAY_MAX; i++) {
      if (accountArray[i].name == nameIn) {
         return true;
      }
   }
   return false;
}

bool FileTracker::isAdmin(string usernameIn, string passwordIn) {
   for (int i = 0; i < ARRAY_MAX; i++) {
      string u = userArray[i].username;
      string p = userArray[i].password;
      string r = userArray[i].role;
   
      boost::trim_right(u);
      boost::trim_right(p);
      boost::trim_right(r);
   
      if (u == usernameIn && p == passwordIn) {
         if (r == "System Administrator") {
            return true;
         }
      }
   }

   return false;
}

bool FileTracker::isStaff(string usernameIn, string passwordIn) {
   for (int i = 0; i < ARRAY_MAX; i++) {
      string u = userArray[i].username;
      string p = userArray[i].password;
      string r = userArray[i].role;
      boost::trim_right(u);
      boost::trim_right(p);
      boost::trim_right(r);
      if (u == usernameIn && p == passwordIn) {
         if (r == "Branch Staff") {
            return true;
         }
      }
   }
   return false;
}

bool FileTracker::isClient(string clientNameIn) {
   for (int i = 0; i < ARRAY_MAX; i++) {
      string n = clientArray[i].name;
      boost::trim_right(n);
      if (n == clientNameIn) {
         return true;
      }
   }
   return false;
}

bool FileTracker::isAccount(string acctNumIn) {
   for (int i = 0; i < ARRAY_MAX; i++) {
      string n = accountArray[i].acctNum;
      boost::trim_right(n);
      if (n == acctNumIn) {
         return true;
      }
   }
   return false;
}

bool FileTracker::isUser(string usernameIn) {
   for (int i = 0; i < ARRAY_MAX; i++) {
      if (userArray[i].username == usernameIn) {
         return true;
      }
   }
   return false;
}

string FileTracker::getNameOfClient(string acctNumIn) {
   for (int i = 0; i < ARRAY_MAX; i++) {
      if (accountArray[i].acctNum == acctNumIn) {
         return accountArray[i].name;
      }
   }
   return "UNKNOWN";
}

int FileTracker::deposit(string acctNumIn, int amount) {
   int balanceInt;
   for (int i = 0; i < ARRAY_MAX; i++) {
   
      if (accountArray[i].acctNum == acctNumIn) {
         balanceInt = atoi(accountArray[i].balance.c_str());
         balanceInt += amount;
  
         accountArray[i].balance = static_cast<ostringstream*>(&(ostringstream() << balanceInt))->str();
         cout << "Balance of account " << accountArray[i].acctNum << " is: " << accountArray[i].balance << "\n\n";
         return 0;
      }
   }
   return 0;
}

int FileTracker::withdraw(string acctNumIn, int amount) {
   int balanceInt;
   for (int i = 0; i < ARRAY_MAX; i++) {
      if (accountArray[i].acctNum == acctNumIn) {
         balanceInt = atoi(accountArray[i].balance.c_str());
         balanceInt -= amount;
      
         if (balanceInt < 0) {
            cout << "ERROR: Withdraw amount exceeds balance!";
            cout << "\nNo money was withdrawn from " << accountArray[i].name << "'s account.\n\n";
            return 0;
         }
      
         accountArray[i].balance = static_cast<ostringstream*>(&(ostringstream() << balanceInt))->str();
         cout << "Balance of account " << accountArray[i].acctNum << " is: " << accountArray[i].balance << "\n\n";
         return 0;
      }
   }
   return 0;
}

int FileTracker::printUserArray() {
   int numUsers = 0;

   for (int i = 0; i < ARRAY_MAX; i++) {
      if (onlyWhiteSpace(userArray[i].username) && onlyWhiteSpace(userArray[i].password)) { // reaches end of text
         break;
      }
      numUsers++;
   }

   cout << "\nThere are " << numUsers << " users in the system.";

   for (int i = 0; i < numUsers; i++) {
      if (i == 0) {
         cout << "\n\t1) Username: admin\t\tRole: System Administrator";
      }
      else {
         cout << "\n\t" << (i + 1) << ") Username: " << userArray[i].username << "\t\tRole: " << userArray[i].role;
      }
   }
	
   cout << "\n\n";
   return 0;
}

int FileTracker::fillClientArray() {
   ifstream inStream;
   inStream.open("client-info.txt");

   if (inStream.fail()) {
      cout << "\nERROR: Client information could not be loaded from file!\n";
      return 0;
   }

   for (int i = 0; i < ARRAY_MAX; i++) {
      getline(inStream, clientArray[i].name);
   
      if (onlyWhiteSpace(clientArray[i].name)) {
         clientArray[i].name = "";
      }
   
      getline(inStream, clientArray[i].address);
      getline(inStream, clientArray[i].ssn);
      getline(inStream, clientArray[i].employer);
      getline(inStream, clientArray[i].income);
   }

   inStream.close();
   return 0;
}

int FileTracker::fillAccountArray() {
   ifstream inStream;
   inStream.open("account-info.txt");

   if (inStream.fail()) {
      cout << "ERROR: Account information could not be loaded from file!\n";
      return 0;
   }

   for (int i = 0; i < ARRAY_MAX; i++) {
      getline(inStream, accountArray[i].name);
   
      if (onlyWhiteSpace(accountArray[i].name)) {
         accountArray[i].name = "";
      }
   
      getline(inStream, accountArray[i].acctNum);
      getline(inStream, accountArray[i].acctType);
      getline(inStream, accountArray[i].balance);
   }

   inStream.close();
   return 0;
}

int FileTracker::fillUserArray() {
   ifstream inStream;
   inStream.open("staff.txt");

   if (inStream.fail()) {
      cout << "ERROR: Staff members could not be loaded from file!\n";
      return 0;
   }

   for (int i = 0; i < ARRAY_MAX; i++) {
      getline(inStream, userArray[i].username);
   
      if (onlyWhiteSpace(userArray[i].username)) {
         userArray[i].username = "";
      }
   
      getline(inStream, userArray[i].password);
      getline(inStream, userArray[i].role);
   }

   inStream.close();
   return 0;
}

int FileTracker::clientWritefile() {
   ofstream outStream;
   outStream.open("client-info.txt");

   for (int i = 0; i < ARRAY_MAX; i++) {
      outStream << clientArray[i].name << endl;
      outStream << clientArray[i].address << endl;
      outStream << clientArray[i].ssn << endl;
      outStream << clientArray[i].employer << endl;
      outStream << clientArray[i].income << endl;
   }
   outStream.close();
   return 0;
}

int FileTracker::accountWritefile() {
   ofstream outStream;
   outStream.open("account-info.txt");

   for (int i = 0; i < ARRAY_MAX; i++) {
      outStream << accountArray[i].name << endl;
      outStream << accountArray[i].acctNum << endl;
      outStream << accountArray[i].acctType << endl;
      outStream << accountArray[i].balance << endl;
   }
   outStream.close();
   return 0;
}

int FileTracker::userWritefile() {
   ofstream outStream;
   outStream.open("staff.txt");

   for (int i = 0; i < ARRAY_MAX; i++) {
      outStream << userArray[i].username << endl;
      outStream << userArray[i].password << endl;
      outStream << userArray[i].role << endl;
   }
   outStream.close();
   return 0;
}

int FileTracker::printClientInfo(string nameIn) {
   for (int i = 0; i < ARRAY_MAX; i++) {
      if (clientArray[i].name == nameIn) {
         cout << "\n\tAddress: " << clientArray[i].address;
         cout << "\n\tSocial security number: " << clientArray[i].ssn;
         cout << "\n\tEmployer: " << clientArray[i].employer;
         cout << "\n\tAnnual income: " << clientArray[i].income << endl;
      }
   }
   return 0;
}

int FileTracker::updateClientInfo(string nameIn) {
   for (int i = 0; i < ARRAY_MAX; i++) {
      if (clientArray[i].name == nameIn) {
         cout << "\nPlease fill out " << clientArray[i].name << "'s updatd information...\n";
         cout << "Address: ";
         getline(cin, clientArray[i].address);
         getline(cin, clientArray[i].address);
         cout << "Social security number: ";
         getline(cin, clientArray[i].ssn);
         cout << "Employer: ";
         getline(cin, clientArray[i].employer);
         cout << "Annual income: ";
         getline(cin, clientArray[i].income);
         cout << "Client " << clientArray[i].name << "'s information was updated!\n\n";
      }
   }
   return 0;
}

int FileTracker::addClientToArray(Client client) {
   int count = 0;

   for (int i = 0; i < ARRAY_MAX; i++) {
      if (clientArray[i].name != "") {
         count++;
      }
   }

   if (count >= 99) {
      cout << "ERROR: You can't add any more clients because the database is full!\n\n";
      return 0;
   }

   clientArray[count] = client;
   cout << "A new client was added!\n\n";
   return 0;
}

int FileTracker::addAccountToArray(Account account) {
   int count = 0;

   for (int i = 0; i < ARRAY_MAX; i++) {
      if (accountArray[i].name != "") {
         count++;
      }
   }

   if (count >= 99) {
      cout << "ERROR: You can't add any more accounts because the database is full!\n\n";
   }

   accountArray[count] = account;
   cout << "A new account was created for " << account.name << "!\n\n";
   return 0;
}

int FileTracker::addUserToArray(User user) {
   int count = 0;

   for (int i = 0; i < ARRAY_MAX; i++) {
      if (userArray[i].username != "") {
         count++;
      }
   }

   if (count >= 99) {
      cout << "ERROR: You can't add any more users because the database is full!\n\n";
   }

   userArray[count] = user;
   return 0;
}

int FileTracker::deleteUserFromArray(string nameIn) {
   for (int i = 0; i < ARRAY_MAX; i++) {
      if (userArray[i].username == nameIn) {
        userArray[i] = User();
      
         for (int j = i + 1; j < ARRAY_MAX; j++) {
            userArray[j - 1].username = userArray[j].username;
            userArray[j - 1].password = userArray[j].password;
            userArray[j - 1].role = userArray[j].role;
         }
      }
   }
   return 0;
}

int FileTracker::changePasswordInArray(string usernameIn, string passwordIn) {
	if (usernameIn == "admin") {
		userArray[0].password = passwordIn;
		return 0;
	}
	for (int i = 0; i < ARRAY_MAX; i++) {
      if (userArray[i].username == usernameIn) {
		  userArray[i].password = passwordIn;
      }
   }
   return 0;
}

int main() {
   Menu menu = Menu();
   menu.initializeArrays();
   return 0;
}

void test_login_process(void) {
	Menu menu = Menu();
	menu.loginOptions(); // select 1, 2, or an invalid input. if select 1, enter valid login info or invalid login info 
}

void test_system_admin_menu(void) {
	Menu menu = Menu();
	menu.adminOptions(); // select 1, 2, 3, 4, 5, 6, or an invalid input
}

void test_branch_staff_menu(void) {
	Menu menu = Menu();
	menu.staffOptions(); // select 1, 2, 3, or an invalid input
}

void test_client_and_account_management(void) {
	Menu menu = Menu();
	menu.clientManagement(); // select 1, 2, 3, 4, 5, 6, or an invalid input
}