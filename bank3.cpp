#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>

using namespace std;

class BankAccount {
private:
    int account_number;
    string name;
    string address;
    string contact_details;
    double balance;
    string currency;
    string email;
    string password;
    vector<string> transaction_history;

public:
    BankAccount(int acc_number, const string& n, const string& addr, const string& contact, const string& mail, const string& pass, double initial_balance, const string& curr)
        : account_number(acc_number), name(n), address(addr), contact_details(contact), balance(initial_balance), currency(curr), email(mail), password(pass) {}

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            transaction_history.push_back("Deposited " + to_string(amount) + " " + currency);

            // Print deposit statement to terminal
            cout << "Deposited " << amount << " " << currency << " to account " << account_number << "." << endl;

            // Send an email notification
            send_email("Deposit Notification", "You have made a deposit of " + to_string(amount) + " " + currency + " to your account.");
        } else {
            cout << "Invalid deposit amount." << endl;
        }
    }

    void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            transaction_history.push_back("Withdrew " + to_string(amount) + " " + currency);
        } else {
            cout << "Invalid withdrawal amount or insufficient funds." << endl;
        }
    }

    void display_info() {
        cout << "Account Information:" << endl;
        cout << "Account Number: " << account_number << endl;
        cout << "Account Holder's Name: " << name << endl;
        cout << "Address: " << address << endl;
        cout << "Contact Details: " << contact_details << endl;
        cout << "Email: " << email << endl;
        cout << "Current Balance: " << balance << " " << currency << endl;
        cout << "Transaction History:" << endl;
        for (const string& transaction : transaction_history) {
            cout << transaction << endl;
        }
    }

    bool authenticate(const string& pass) const {
        return password == pass;
    }

    void send_email(const string& subject, const string& message) {
        cout << "Email sent to " << email << " with subject: " << subject << endl;
        cout << "Message: " << message << endl;
    }

    int get_account_number() const {
        return account_number;
    }

    double get_balance() const {
        return balance;
    }

    const string& get_currency() const {
        return currency;
    }
};

void deposit_cash(BankAccount& account) {
    double amount;
    cout << "Enter the deposit amount in " << account.get_currency() << ": ";
    cin >> amount;
    account.deposit(amount);
    cout << "Deposit completed." << endl;
}

void withdraw_cash(BankAccount& account) {
    double amount;
    cout << "Enter the withdrawal amount in " << account.get_currency() << ": ";
    cin >> amount;
    account.withdraw(amount);
    cout << "Withdrawal completed." << endl;
}

void transfer_funds(BankAccount& sender_account, BankAccount& recipient_account, const string& sender_password) {
    double amount;
    cout << "Enter the transfer amount in " << sender_account.get_currency() << ": ";
    cin >> amount;

    if (sender_account.get_balance() >= amount && sender_account.authenticate(sender_password)) {
        sender_account.withdraw(amount);
        recipient_account.deposit(amount);
        cout << "Transfer completed." << endl;
    } else {
        cout << "Transfer failed. Either insufficient funds or incorrect password." << endl;
    }
}

void create_account(vector<BankAccount>& accounts) {
    int account_number = accounts.size() + 1;
    string name, address, contact_details, currency = "INR", email, password;
    double initial_deposit;

    cout << "Enter your name: ";
    cin >> name;
    cout << "Enter your address: ";
    cin.ignore();
    getline(cin, address);
    cout << "Enter your contact details: ";
    cin.ignore();
    getline(cin, contact_details);
    cout << "Enter your email: ";
    cin >> email;
    cout << "Enter your password: ";
    cin >> password;
    cout << "Enter the initial deposit amount in INR (minimum 1000): ";
    cin >> initial_deposit;

    if (initial_deposit >= 1000) {
        BankAccount new_account(account_number, name, address, contact_details, email, password, initial_deposit, currency);
        accounts.push_back(new_account);
        cout << "Account created successfully." << endl;
    } else {
        cout << "Invalid initial deposit amount. Minimum deposit amount is 1000 INR." << endl;
    }
}

int login(vector<BankAccount>& accounts) {
    int account_number;
    string password;
    cout << "Enter your account number: ";
    cin >> account_number;
    cout << "Enter your password: ";
    cin >> password;

    for (size_t i = 0; i < accounts.size(); i++) {
        if (accounts[i].get_account_number() == account_number && accounts[i].authenticate(password)) {
            return i;
        }
    }

    return -1;
}

int main() {
    vector<BankAccount> accounts;
    int active_account_index = -1;

    while (true) {
        if (active_account_index == -1) {
            cout<<"                                WELCOME                               \n";
            cout<<"****************************BANK OF BHARAT****************************\n";
            cout << "\n\t\t\tBanking System Menu:\n" << endl;
            cout << "1. Create New Account\t\t";
            cout << "2. Log In\t\t";
            cout << "3. Exit\t\t" << endl<<endl;
            string choice;
            cout << "Enter your choice (1/2/3): ";
            cin >> choice;

            if (choice == "1") {
                create_account(accounts);
            } else if (choice == "2") {
                active_account_index = login(accounts);
                if (active_account_index == -1) {
                    cout << "\nAccount not found or password incorrect." << endl;
                } else {
                    cout << "\nLogged in as account " << accounts[active_account_index].get_account_number() << "." << endl;
                }
            } else if (choice == "3") {
                cout << "Exiting." << endl;
                break;
            } else {
                cout << "Invalid choice. Please try again." << endl;
            }
        } else {
            cout << "\nBanking System Menu:\n" << endl;
            cout << "1. Deposit Cash\t\t\t\t";
            cout << "2. Withdraw Cash\t\t"<<endl;
            cout << "3. Transfer Between Accounts\t\t";
            cout << "4. Display Account Information\t\t"<<endl;
            cout << "5. Log Out\n" << endl;
            string choice;
            cout << "Enter your choice (1/2/3/4/5): ";
            cin >> choice;

            if (choice == "1") {
                deposit_cash(accounts[active_account_index]);
            } else if (choice == "2") {
                withdraw_cash(accounts[active_account_index]);
            } else if (choice == "3") {
                cout << "Enter recipient's account number: ";
                int recipient_number;
                cin >> recipient_number;
                bool recipient_found = false;
                BankAccount* recipient_account = nullptr;

                // Find the recipient account
                for (BankAccount& account : accounts) {
                    if (account.get_account_number() == recipient_number) {
                        recipient_account = &account;
                        recipient_found = true;
                        break;
                    }
                }

                if (!recipient_found) {
                    cout << "Recipient account not found." << endl;
                } else {
                    string sender_password;
                    cout << "Enter your password: ";
                    cin >> sender_password;
                    transfer_funds(accounts[active_account_index], *recipient_account, sender_password);
                }
            } else if (choice == "4") {
                accounts[active_account_index].display_info();
            } else if (choice == "5") {
                cout << "Logging out." << endl;
                active_account_index = -1;
            } else {
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

    return 0;
}