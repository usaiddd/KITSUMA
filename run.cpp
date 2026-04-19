#include <bits/stdc++.h>
#include <fstream>
using namespace std;

int main(){
    system("cls");
    cout << "WELCOME TO KITSUMA - HIERARCHY BASED VERSION CONTROL SYSTEM\n";
    bool chosen = false;
    string user; 
    while (!chosen) {
        char choice;
        cout << "Do you want to Signup(S) or Login(L): ";
        cin >> choice;
        if (choice == 's' || choice == 'S') {
            chosen = true;
            while (true) {
                string sign_up, new_pass;
                cout << "Enter your AdminID: ";
                cin >> sign_up;
                cout << "Enter your AdminPass: ";
                cin >> new_pass;
                string command = "python load.py S \"" + sign_up + "\" \"" + new_pass + "\"";
                int res = system(command.c_str());
                if (res == 0) {
                    cout << "Signup successful!\n";
                    user = sign_up;
                    cout << "Enter container name for your project: "; 
                    string conname; 
                    cin >> conname;
                    int employeecount; 
                    cout << "Enter number of employees in your project: "; 
                    cin >> employeecount; 
                    vector <pair<string, string>> employees; 
                    for (int i=0; i<employeecount; i++){ 
                        string loginID, pass;
                        cout << "   Enter LoginID of employee " << i+1 << ": ";
                        cin >> loginID; 
                        cout << "   Enter Password of employee " << i+1 << ": ";
                        cin >> pass;
                        employees.push_back({loginID, pass}); 
                    }
                    command = "python load.py S \"" + sign_up + "\" \"" + new_pass + "\"";
                    break;
                }
                else if (res == 1) {
                    cout << "AdminID already exists. Try again.\n";
                }
                else {
                    cout << "Error occurred. Try again.\n";
                }
            }
        }
        else if (choice == 'l' || choice == 'L') {
            chosen = true;
            while (true) {
                string login_id, password;
                cout << "Enter your LoginID: ";
                cin >> login_id;
                cout << "Enter your Password: ";
                cin >> password;
                string command = "python load.py L \"" + login_id + "\" \"" + password + "\"";
                int res = system(command.c_str());
                if (res == 0) {
                    cout << "\nWelcome to KITSUMA, " << login_id << endl;
                    user = login_id; 
                    break;
                }
                else if (res == 1) {
                    cout << "Invalid LoginID or Password. Try again.\n";
                }
                else {
                    cout << "Error occurred. Try again.\n";
                }
            }
        }
        else {
            cout << "Invalid option chosen. Try again.\n";
        }
    }
    string command = "python load.py G \"" + user + "\"";
    int res = system(command.c_str());
    ofstream login("LoggedIn.txt");
    login << user; 
    login.close();  
    return 0;
}

