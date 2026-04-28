#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set> 
#include <fstream>
#include <limits>
#include <algorithm>
#include <conio.h>
#include <sstream>
using namespace std;
string getPassword() {
    string pass;
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') {
            if (!pass.empty()) {
                pass.pop_back();
                cout << "\b \b";  
            }
        } else {
            pass += ch;
            cout << '*';
        }
    }
    cout << endl;
    return pass;
}
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
                cout << "Enter container name for your project: "; 
                string conname;
                cin.ignore();
                getline(cin, conname);
                sign_up = sign_up + "@" + conname; 
                ofstream out("conname.txt");
                out << conname << "\n"; 
                string command = "python load.py S \"" + sign_up + "\" \"" + new_pass + "\"";
                int res = system(command.c_str());
                if (res == 0) {
                    cout << "Signup successful!\n";
                    user = sign_up;
                    int employeecount; 
                    cout << "Enter number of employees in your project: "; 
                    cin >> employeecount; 
                    vector <pair<string, string>> employees; 
                    for (int i=0; i<employeecount; i++){ 
                        string loginID, pass;
                        cout << "   Enter LoginID of employee " << i+1 << ": ";
                        cin >> loginID; 
                        loginID = loginID + "@" + conname;
                        cout << "   Enter Password of employee " << i+1 << ": ";
                        pass = getPassword();
                        employees.push_back({loginID, pass}); 
                        cout << "\n";
                    }
                    ofstream out("data.txt"); 
                    for (auto&p: employees){ 
                        out << p.first << "|" << p.second << "\n"; 
                    }
                    out.close(); 
                    command = "python load.py PD \"" + conname + "\" \"" + user + "\"";
                    int res2 = system(command.c_str()); 
                    if (res2 == 0){ 
                        cout << "Login ID's successfully created.";
                        system("cls");
                        cout << "DEFINE CONTAINER STRUCTURE";
                        vector<string> structure, pathStack;
                        while (true) {
                            string currentPath = "";
                            for (int i = 0; i < pathStack.size(); i++) {
                                currentPath += pathStack[i];
                                if (i != pathStack.size() - 1) currentPath += "/";
                            }
                            cout << "\nCurrent path: /" << currentPath << endl;
                            cout << "1. Add Folder\n";
                            cout << "2. Add File\n";
                            cout << "3. Go Inside Folder\n";
                            cout << "4. Go Back\n";
                            cout << "5. Finish\n";
                            cout << "Choice: ";
                            int choice;
                            cin >> choice;
                            if (choice == 1) {
                                string name;
                                cout << "Enter folder name: ";
                                cin >> name;
                                string fullPath = currentPath.empty() ? name : currentPath + "/" + name;
                                structure.push_back(fullPath);
                            } 
                            else if (choice == 2) {
                                string name;
                                cout << "Enter file name (with extension): ";
                                cin >> name;
                                string fullPath = currentPath.empty() ? name : currentPath + "/" + name;
                                structure.push_back(fullPath);
                            } 
                            else if (choice == 3) {
                                vector<string> possible;
                                for (auto &s : structure) {
                                    if (currentPath.empty()) {
                                        int pos = s.find('/');
                                        string folder = (pos == string::npos) ? s : s.substr(0, pos);
                                        if (folder.find('.') == string::npos) { 
                                            if (find(possible.begin(), possible.end(), folder) == possible.end()) {
                                                possible.push_back(folder);
                                            }
                                        }
                                    } 
                                    else if (s.find(currentPath + "/") == 0) {
                                        string remaining = s.substr(currentPath.size() + 1);
                                        int pos = remaining.find('/');

                                        if (pos != string::npos) {
                                            string folder = remaining.substr(0, pos);

                                            if (find(possible.begin(), possible.end(), folder) == possible.end()) {
                                                possible.push_back(folder);
                                            }
                                        }
                                    }
                                }
                                if (possible.empty()) {
                                    cout << "No folders available here.\n";
                                    continue;
                                }
                                cout << "Available folders:\n";
                                for (int i = 0; i < possible.size(); i++) {
                                    cout << i + 1 << ". " << possible[i] << endl;
                                }
                                cout << "Enter choice: ";
                                int idx;
                                cin >> idx;
                                if (idx >= 1 && idx <= possible.size()) {
                                    pathStack.push_back(possible[idx - 1]);
                                } else {
                                    cout << "Invalid choice\n";
                                }
                            }
                            else if (choice == 4) {
                                if (!pathStack.empty()) {
                                    pathStack.pop_back();
                                } else {
                                    cout << "Already at root.\n";
                                }
                            } 
                            else if (choice == 5) {
                                break;
                            }
                            else {
                                cout << "Invalid choice\n";
                            }
                        }
                        string name = ""; 
                        bool file = false;  
                        ofstream out("structure.txt");
                        for (auto &s : structure) {
                            for (char i: s){  
                                name+=i;  
                                if (i == '.'){ 
                                    file = true; 
                                }
                            }
                            if (file){ 
                                command = "python load.py A \"" + name + "\" \"" + name + "\"";
                            }
                            out << s << "\n";
                        }
                        out.close();
                        system("cls"); 
                        cout << "ALL MAIN FOLDERS AVAILABLE IN THE CONTAINER\n"; 
                        vector<string> rootFolders;
                        for (auto &s : structure) {
                            int pos = s.find('/');
                            string top = (pos == string::npos) ? s : s.substr(0, pos);
                            if (top.find('.') == string::npos) {
                                if (find(rootFolders.begin(), rootFolders.end(), top) == rootFolders.end()) {
                                    rootFolders.push_back(top);
                                }
                            }
                        }
                        for (int i = 0; i < rootFolders.size(); i++) {
                            cout << i + 1 << ". " << rootFolders[i] << endl;
                        }
                        cout << "\nLIST OF EMPLOYEES AVAILABLE IN CONTAINER\n"; 
                        for (int i=0; i<employees.size(); i++){ 
                            cout << i+1 << ". " << employees[i].first << endl; 
                        }
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        vector<vector<int>> folderemployees; 
                        cout << "\nCHOOSE EMPLOYEES FOR EACH FOLDER (Input corresponding employee numbers)\n"; 
                        for (int i=0; i<rootFolders.size(); i++){
                            cout << i+1 << ". " << rootFolders[i] << ": "; 
                            string line;
                            getline(cin, line);
                            vector<int> x;
                            stringstream ss(line);
                            int num;
                            while (ss >> num) {
                                if (num >= 1 && num <= employees.size()) {
                                    x.push_back(num);
                                } else {
                                    cout << "Invalid employee number ignored: " << num << endl;
                                }
                            }
                            folderemployees.push_back(x);
                        }
                        cout << "\nEMPLOYEES ASSIGNED TO EACH FOLDER\n";
                        for (int i = 0; i < folderemployees.size(); i++) {
                            cout << rootFolders[i] << ": ";
                            for (int num : folderemployees[i]) {
                                cout << num << " ";
                            }
                            cout << endl;
                        }
                        cout << "\nDECLARE AN EMPLOYEE AS FOLDER HEAD";
                        vector<int> folderHead(rootFolders.size(), -1);  
                        set<int> used;  
                        for (int i=0; i<rootFolders.size(); i++){
                            while (true) {
                                cout << "\n" << i+1 << ". " << rootFolders[i] << "\n";
                                cout << "Available Employees for this folder:\n";
                                for (int j = 0; j < folderemployees[i].size(); j++) {
                                    int empNum = folderemployees[i][j];
                                    if (!used.count(empNum)) {
                                        cout << empNum << ". " << employees[empNum - 1].first << endl; 
                                    }
                                }
                                cout << "Enter employee number: ";
                                int choice;
                                cin >> choice;
                                bool isInFolder = false;
                                for (int num : folderemployees[i]) {
                                    if (num == choice) {
                                        isInFolder = true;
                                        break;
                                    }
                                }
                                if (!isInFolder) {
                                    cout << "Invalid choice. You must pick an employee assigned to this folder.\n";
                                    continue;
                                }
                                if (used.count(choice)) {
                                    cout << "Employee already assigned. Choose another.\n";
                                    continue;
                                }
                                folderHead[i] = choice;
                                used.insert(choice);
                                break;
                            }
                        }
                        cout << "\nFINAL FOLDER HEADS:\n";
                        for (int i = 0; i < rootFolders.size(); i++) {
                            cout << rootFolders[i] << ": " << employees[folderHead[i] - 1].first << endl;
                        }
                        system("cls"); 
                        cout << "\nDECLARE HIERARCHY OF EACH FOLDER\n";
                        vector<vector<int>> folderhierarchy; 
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                        for (int i=0; i<rootFolders.size(); i++){ 
                            cout << i+1 << ". " << rootFolders[i] << endl;
                            cout << "AVAILABLE EMPLOYEES IN FOLDER: \n";
                            for (int j=0; j<folderemployees[i].size(); j++){ 
                                cout << "   " << j+1 << ". " << employees[folderemployees[i][j] - 1].first << endl; 
                            }
                            cout << "Enter hierarchy of folder (corresponding numbers): "; 
                            string line;
                            getline(cin, line);
                            vector<int> x;
                            stringstream ss(line);
                            int num;
                            while (ss >> num) {
                                if (num >= 1 && num <= folderemployees[i].size()) {
                                    x.push_back(folderemployees[i][num - 1]);
                                } else {
                                    cout << "Invalid employee number ignored: " << num << endl;
                                }
                            }
                            folderhierarchy.push_back(x);
                            cout << endl; 
                        }
                        cout << "\nFOLDER HIERARCHY MAPPING\n";
                        for (int i = 0; i < rootFolders.size(); i++) {
                            cout << "\n" << rootFolders[i] << ":\n";
                            if (folderhierarchy[i].empty()) {
                                cout << "   None\n";
                                continue;
                            }
                            for (int j = 0; j < folderhierarchy[i].size(); j++) {
                                int empIndex = folderhierarchy[i][j] - 1;
                                cout << "   " << j+1 << ". "
                                    << employees[empIndex].first << endl;
                            }
                        }
                        ofstream hout("hierarchy_data.txt");
                        for (auto &s : structure) {
                            hout << "PATH|" << s << "\n";
                        }
                        for (int i = 0; i < rootFolders.size(); i++) {
                            for (int num : folderemployees[i]) {
                                hout << "EMP|" << rootFolders[i] << "|" << employees[num - 1].first << "\n";
                            }
                            if (folderHead[i] != -1) {
                                hout << "HEAD|" << rootFolders[i] << "|" << employees[folderHead[i] - 1].first << "\n";
                            }
                            for (int j = 0; j < folderhierarchy[i].size(); j++) {
                                hout << "HIER|" << rootFolders[i] << "|" 
                                     << employees[folderhierarchy[i][j] - 1].first << "|" << (j + 1) << "\n";
                            }

                        }
                        hout.close();
                        string ph_command = "python load.py PH \"" + conname + "\"";
                        int ph_res = system(ph_command.c_str());
                        if (ph_res == 0) {
                            cout << "\n[Success] Container hierarchy saved to database!\n";
                        } else {
                            cout << "\n[Error] Failed to save hierarchy to database.\n";
                        }
                    }
                    else if (res2 == 1){ 
                        cout << "One of the login credentials created already exists, Try again.";
                        command = "python load.py X \"" + user + "\"";
                        int res4 = system(command.c_str());
                    }
                    else if (res2 == 2){ 
                        cout << "Error occurred, Try again. ";
                    }
                    remove("data.txt");
                    remove("hierarchy_data.txt");
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

