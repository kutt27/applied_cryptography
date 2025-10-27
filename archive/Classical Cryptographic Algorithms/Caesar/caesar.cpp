#include <iostream>
#include <string>

using namespace std;

char caesar(char c, char k){
    if(isalpha(c)){
        char base = islower(c) ? 'a' : 'A';
        c = (((c - base) + k + 26) % 26) + base;
    }
    return c;
}

string getMessage(){
    string message;
    cout << "Enter the message(alphabet only): ";
    getline(cin, message);
    return message;
}

int getkey(){
    int key;
    cout << "Enter the key (1-25): ";
    cin >> key;
    cin.ignore();
    return key;
}

bool isValidMessage(const string &message){
    for(char c : message){
        // reads through each character in message
        if(!isalpha(c)){
            // if any character is not an alphabet is return false
            return false;
        }
    }
    return true;
}

bool isValidKey(int key){
    return key >= 1 && key <= 25;
}

string processMessage(const string &message, int key){
    string result = "";
    for(char c : message){
        result += caesar(c, key);
    }
    return result;
}

int main(){
    int choice;
    do{
        cout << "1. Encrypt/Decrypt\n2. Quit \nEnter your choice: ";
        cin >> choice;
        cin.ignore();
        if(choice == 1){
            string message = getMessage();
            if(!isValidMessage(message)){
                cout << "Invalid message. Only alphabets allowed \n";
                continue;
            }

            int key = getkey();
            if(!isValidKey(key)){
                cout << "Invalid key. Key must be between 1 and 25. \n";
                continue;
            }

            string result = processMessage(message, key);
            cout << "Result: " << result << endl;
        }
        else if (choice == 2){
            cout << "Quitting.\n";
        }
        else{
            cout << "Invalid action.\n";
        }
    }
    while(choice != 2);
}