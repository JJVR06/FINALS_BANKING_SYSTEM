#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime> //date/time functionality

using namespace std;

struct Transaction {
    string type;
    double amount = 0.0; 
    string date;
};

class Customer {
private:
    string username;
    double balance;
    vector<Transaction> transactionHistory;

public:
    Customer(string username) {
        this->username = username;
        balance = 0.0;
    }

    string getUsername() const {
        return username;
    }

    double getBalance() const {
        return balance;
    }

    void deposit(double amount) {
        balance += amount;
        addTransaction("Deposit", amount);
    }

    bool withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            addTransaction("Withdraw", -amount); // Negative amount indicates a withdrawal
            return true;
        }
        else {
            return false;
        }
    }

    bool transferFunds(Customer& recipient, double amount) {
        if (balance >= amount) {
            balance -= amount;
            recipient.balance += amount;

            addTransaction("Transfer Sent", -amount);
            recipient.addTransaction("Transfer Received", amount);
            return true;
        }
        else {
            return false;
        }
    }

    void showTransactionHistory() {
        sortTransactionHistory();
        cout << "Transaction History for " << username << ":" << endl;
        for (const auto& transaction : transactionHistory) {
            cout << "Type: " << transaction.type << ", Amount: " << transaction.amount << ", Date: " << transaction.date << endl;
        }
    }

private:
    void addTransaction(const string& type, double amount) {
        Transaction transaction;
        transaction.type = type;
        transaction.amount = amount;
        transaction.date = getCurrentDate();
        transactionHistory.push_back(transaction);
    }

    void sortTransactionHistory() {
        sort(transactionHistory.begin(), transactionHistory.end(), [](const Transaction& t1, const Transaction& t2) {
            return t1.date < t2.date;
            });
    }

    string getCurrentDate() {
        time_t now = time(0);
        tm currentTime;
        localtime_s(&currentTime, &now);
        char dateString[11];
        strftime(dateString, sizeof(dateString), "%m-%d-%Y", &currentTime);
        return dateString;
    }
};

void displayCustomers(const vector<Customer>& customers) {
    cout << "Existing Customers:" << endl;
    for (const auto& customer : customers) {
        cout << customer.getUsername() << endl;
    }
}

int main() {
    vector<Customer> customers;

    //Available accounts
    customers.push_back(Customer("Michael"));
    customers.push_back(Customer("Jerome"));
    customers.push_back(Customer("Raymond"));
    customers.push_back(Customer("Andrei"));
    customers.push_back(Customer("Xandrew"));

    cout << "Welcome to My Banking System!" << endl;

    while (true) {
       
        cout << "1. Login" << endl;
        cout << "2. Display existing customers" << endl;
        cout << "3. Exit" << endl;
        cout << "Please select an option:";

        int choice;
        cin >> choice;

        if (choice == 1) {
            cout << "Please enter your username:";

            string username;
            cin >> username;

            auto customerIterator = find_if(customers.begin(), customers.end(), [&](const Customer& customer) {
                return customer.getUsername() == username;
                });

            if (customerIterator != customers.end()) {
                Customer& customer = *customerIterator;

                while (true) {
                    cout << "Welcome, " << customer.getUsername() << "!" << endl;
                  
                    cout << "1. Deposit" << endl;
                    cout << "2. Withdraw" << endl;
                    cout << "3. Transfer funds" << endl;
                    cout << "4. Show transaction history" << endl;
                    cout << "5. Logout" << endl;
                    cout << "Please select an option:";

                    int userChoice;
                    cin >> userChoice;

                    if (userChoice == 1) {
                        double depositAmount;
                        cout << "Please enter the deposit amount: "<<"PHP ";
                        cin >> depositAmount;

                        customer.deposit(depositAmount);

                        cout << "Deposit successful. New balance: " << "PHP " << customer.getBalance() << endl;
                    }
                    else if (userChoice == 2) {
                        double withdrawalAmount;
                        cout << "Please enter the withdrawal amount: " << "PHP ";
                        cin >> withdrawalAmount;

                        bool success = customer.withdraw(withdrawalAmount);

                        if (success) {
                            cout << "Withdrawal successful. New balance: PHP " << customer.getBalance() << endl;
                        }
                        else {
                            cout << "Insufficient funds." << endl;
                        }
                    }
                    else if (userChoice == 3) {
                        string recipientUsername;
                        double transferAmount;

                        cout << "Please enter the recipient's username:" << endl;
                        cin >> recipientUsername;

                        auto recipientIterator = find_if(customers.begin(), customers.end(), [&](const Customer& recipient) {
                            return recipient.getUsername() == recipientUsername && &recipient != &customer; 
                            });

                        if (recipientIterator != customers.end()) {
                            Customer& recipient = *recipientIterator;

                            cout << "Please enter the transfer amount: " << "PHP " ;
                            cin >> transferAmount;

                            bool success = customer.transferFunds(recipient, transferAmount);

                            if (success) {
                                cout << "Transfer successful. New balance: PHP " << customer.getBalance() << endl;
                            }
                            else {
                                cout << "Insufficient funds." << endl;
                            }
                        }
                        else {
                            cout << "Recipient not found." << endl;
                        }
                    }
                    else if (userChoice == 4) {
                        customer.showTransactionHistory();
                    }
                    else if (userChoice == 5) {
                        break; // Logout and return to main menu
                    }
                    else {
                        cout << "Invalid choice." << endl;
                    }
                }
            }
            else {
                cout << "User not found." << endl;
            }
        }
        else if (choice == 2) {
            displayCustomers(customers);
        }
        else if (choice == 3) {
            cout << "Goodbye.... ";
            break;
        }
        else {
            cout << "Invalid choice." << endl;
        }
    }

    return 0;
}
