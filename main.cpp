#include <bits/stdc++.h>
using namespace std;

class Stack {
private:
    int* stack;
    int size;
    int top;

public:

    Stack(int size) {
        this->size = size;
        stack = new int[size];
        top = -1;
    }
    ~Stack() {
        delete[] stack;
    }
    bool isEmptyStack() {
        return (top == -1);
    }
    bool isFullStack() {
        return (top == (size - 1));
    }
    void Push(int newItem) {
        if (isFullStack()) {
            return; 
        }
        stack[++top] = newItem;
    }

    int Pop() {
        if (isEmptyStack()) {
            return 0;
        }
        return stack[top--];
    }
    int getTop() {
        if (isEmptyStack()) {
            return -1;
        } else {
            return stack[top];
        }
    }
    void display() {
        if (isEmptyStack()) {
            cout << "Stack is empty!" << endl;
        } else {
            cout << "Stack elements (top to bottom): ";
            for (int i = top; i >= 0; i--) {
                cout << stack[i] << " ";
            }
            cout << endl;
        }
    }
};

void saveTransaction(const string &username, int amount) {
    string filename = username + "_transactions.txt";
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << amount << endl;
        file.close();
    } else {
        cout << "Error: Unable to save transaction for " << username << endl;
    }
}

void loadTransactions(const string &username, Stack &transactionStack, int &Balance, int &loanBalance) {
    string filename = username + "_transactions.txt";
    ifstream file(filename);
    if (file.is_open()) {
        int amount;
        while (file >> amount) {
            transactionStack.Push(amount);
            Balance += (amount > 0) ? amount : 0;
            loanBalance += (amount > 0) ? amount : 0;
        }
        file.close();
    }
}


string hashPassword(const string &password) {
    int hash = 0;
    for (char ch : password) {
        hash += ch;
    }
    return to_string(hash);
}
void registerUser() {
    string username, password;
    cout << "Enter a username: ";
    cin.ignore();
    getline(cin, username);
    cout << "Enter a password: ";
    cin >> password;

    string hashedPassword = hashPassword(password);

    ofstream file("users.txt", ios::app);
    if (file.is_open()) {
        file << username << " " << hashedPassword << endl;
        file.close();
        cout << "Registration successful!\n";
    } else {
        cout << "Error: Unable to open file.\n";
    }
}

string loginUser() {
    string username, password, storedUser, storedPass;
    cout << "Enter username: ";
    cin.ignore();
    getline(cin, username);
    cout << "Enter password: ";
    cin >> password;

    string hashedPassword = hashPassword(password);

    ifstream file("users.txt");
    if (file.is_open()) {
        while (file >> storedUser >> storedPass) {
            if (storedUser == username && storedPass == hashedPassword) {
                cout << "Login successful! Welcome, " << username << "!\n";
                file.close();
                return username;
            }
        }
        file.close();
    }

    cout << "Error: Invalid username or password.\n";
    return "";
}


Stack transactionStack(100);
int Balance = 0;
int loanBalance = 0;
string password = "1234";

bool authenticate() {
    string inputPass;
    cout << "Enter your password: ";
    cin >> inputPass;
    return inputPass == password;
}


void Deposit(int amount,const string username) {
    if (!authenticate()) {
        cout << "Failed to Deposit....! Try Again\n";
        return;
    }
    else {
        if (amount <= 0) {
            cout << "Error: Invalid amount to deposit.\n";
            return;
        } else {
            transactionStack.Push(amount);
            Balance += amount;
            saveTransaction(username,amount);
            cout << "Deposit successful! Amount: $" << amount << endl;
        }
    }
}

void Withdraw(int amount,const string username){
    if (!authenticate()) {
        cout << "Failed to Withdraw....! Try Again\n";
        return;
    }

    if (amount > Balance) {
        cout << "Error: Insufficient balance!\n";
        return;
    }

    transactionStack.Push(-amount);
    Balance -= amount;
    saveTransaction(username,-amount);
    cout << "Withdraw successful! Amount: $" << amount << endl;
}

void TakeLoan(int amount,const string username) {
    if (!authenticate()) {
        cout << "Loan Request Failed....! Try Again\n";
        return;
    }

    if (amount <= 0) {
        cout << "Error: Invalid loan amount!\n";
        return;
    }

    transactionStack.Push(amount);
    Balance += amount;
    loanBalance += amount;
    saveTransaction(username,amount);
    cout << "Loan Approved! Amount: $" << amount << endl;
}


void LoanPayment(int amount,const string username) {
    if (!authenticate()) {
        cout << "Loan Payment Failed....! Try Again\n";
        return;
    }

    if (amount <= 0) {
        cout << "Error: Invalid payment amount!\n";
        return;
    }

    if (amount > Balance) {
        cout << "Error: Insufficient balance to pay loan!\n";
        return;
    }

    if (amount > loanBalance) {
        cout << "You are overpaying! Loan Balance is $" << loanBalance << endl;
        return;
    }

    transactionStack.Push(-amount);
    Balance -= amount;
    loanBalance -= amount;
    saveTransaction(username,-amount);
    cout << "Loan Payment Successful! Paid: $" << amount << endl;
    cout << "Remaining Loan Balance: $" << loanBalance << endl;
}

void TransactionDetails(const string username) {
    if (!authenticate()) {
        cout << "Failed to get Transaction Details\n";
    }
    else {
        Stack tempStack = transactionStack;
        cout << "Transactions History:"<<endl;
        int i=1;
        while (!tempStack.isEmptyStack()) {
            cout <<i++<<". "<< tempStack.Pop() <<endl;
        }

    }
}


void showMenu(const string username) {
    int choice;
    do {
        cout << "\n===== Main Menu =====" << endl;
        cout << "1. Deposit" << endl;
        cout << "2. Withdraw" << endl;
        cout << "3. Loans" << endl;
        cout << "4. Balance Check" << endl;
        cout << "5. View Transactions Details" << endl;
        cout << "6. Exit" << endl;
        cout << "Choose an option: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int amount;
                cout << "Enter amount to deposit: ";
                cin >> amount;
                Deposit(amount,username);
                break;
            }
            case 2: {
                int amount;
                cout << "Enter amount to withdraw: ";
                cin >> amount;
                Withdraw(amount,username);
                break;
            }
            case 3: {
                int choose;
                cout << "Loans\n";
                cout << "1. Take Loan\n";
                cout << "2. Pay Loan\n";
                cout << "3. Loans Balance\n";
                cout<<"Choose: "<<endl;
                cin>>choose;
                switch (choose) {
                    case 1: {
                        int amount;
                        cout << "Enter loan amount: ";
                        cin >> amount;
                        TakeLoan(amount,username);
                        break;
                    }
                    case 2: {
                        int amount;
                        cout << "Enter loan payment amount: ";
                        cin >> amount;
                        LoanPayment(amount,username);
                        break;
                    }
                    case 3: {
                        cout << "Loan Balance: $" << loanBalance << endl;
                        break;
                    }
                }
                break;
            }
            case 4:
                cout << "Current Balance: $" << Balance << endl;
                break;
            case 5: {
                if (transactionStack.isEmptyStack()) {
                    cout << "History is empty!" << endl;
                } else {
                    TransactionDetails(username);
                }
                break;
            }
            case 6:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice! Try again.\n";
        }
    }while (choice != 6);

}

int main(){
    int choice;
    cout << "1. Register\n2. Login\nChoose an option: ";
    cin >> choice;

    string username;
    if (choice == 1) {
        registerUser();
        username = loginUser();
    } else if (choice == 2) {
        username = loginUser();
    } else {
        cout << "Invalid option!\n";
    }

    if(!username.empty()) {
        loadTransactions(username, transactionStack, Balance, loanBalance);
        showMenu(username);
    }

    return 0;
}