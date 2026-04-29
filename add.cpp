#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <cctype>
using namespace std; 

bool is_numeric(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), [](unsigned char c) {
        return std::isdigit(c);
    });
}

int main(){
    map<string,string> file_obj; //FILENAME, FILENO
    bool file=false,struc=false;
    string i,name="",final,k;
    ifstream ipf("structure.txt");
    while(getline(ipf,i)){
        if(!struc){
            if(i=="struct:"){
                struc=true;
            }
            continue;
        }
        if(is_numeric(i)){
            file=true;
            final="python load.py GFN \"" + i + "\"";
            system(final.c_str());
            ifstream inpFile("temp.txt");
            getline(inpFile,k);
            for(char j:k){
                name+=j;
                if(j=='/'){
                    name="";
                    continue;
                }
            }
        }
        if(file){
            file_obj[name]=i;
            file=false;
        }
    }
    cout << "Enter commit message: \n"; 
    string msg; 
    getline(cin, msg);
    cout << "All files present in current working directory: \n\n";
    for (auto pair:file_obj){
        cout << pair.first << "\n"; 
    }
    cout << "\n"; 
    string n; 
    cout << "Enter number of files to add (. for all): ";
    cin >> n;
    if (n == "."){ 
        for (auto pair:file_obj){ 
            string obj = pair.second;
            string obj2; 
            cout << obj ;
            final="python load.py CC \"" + obj + "\"";
            int code = system(final.c_str());  
            if (code == 1){ 
                cout << "You cannot push at this time. Previous conflicts have to be merged before new pushes are allowed. "; 
                string final2="python load.py GFN "+ obj;
                system(final2.c_str());
                ifstream inpFile("temp.txt");
                getline(inpFile,obj2);
                string final3 = "python load.py GF2 \"" + obj2 + "\" \"" + obj + "\"";
                system(final3.c_str());
                system("g++ update.cpp -o update && .\\update");
                cout << obj; 
                string command = "python load.py A \"" + obj + "\" \"" + "merged.txt" + "\"";
                system(command.c_str());
            }
            else if (code == 2){ 
                cout << "Unknown error occurred. "; 
            }
            else{
                string obj3 = "C:/MusicPlayer/"; 
                final="python load.py GFN" + obj;
                system(final.c_str());
                ifstream inpFile("temp.txt");
                getline(inpFile,obj);
                obj3 = obj3 + obj; 
                ifstream file(obj3); 
                string filecontent; 
                if (!file.is_open()){ 
                    cout << "Failed to open file. "; 
                    filecontent = "";  
                } 
                else{
                    stringstream buffer; 
                    buffer << file.rdbuf(); 
                    filecontent = buffer.str(); 
                    file.close(); 
                }
                ofstream temp("tempcontent.txt");
                temp << filecontent;
                temp.close(); 
                cout << pair.first; 
                string command = "python load.py A \"" + obj + "\" \"" + pair.second + "\" \"" + "tempcontent.txt" + "\"";
                system(command.c_str());
                
                ifstream login("LoggedIn.txt"); 
                string username; 
                if (login.is_open()) {
                    getline(login, username);
                    login.close();
                } else {
                    cout << "Failed to open the file." << std::endl;
                } 
                command = "python load.py PU \"" + pair.first + "\" \"" + "tempcontent.txt" + "\" \"" + msg + "\" \"" + username + "\"";
                system(command.c_str());
            }
        }
    }
    else{
        string filename;
        int num = stoi(n);
        while (num--){
            bool chosen = false;
            while (!chosen){ 
                cout << "Enter filename to add/update: "; 
                cin >> filename; 
                auto it = file_obj.find(filename);
                if (it != file_obj.end()) {
                    chosen = true; 
                } 
                else {
                    cout << "File does not exist in current working directory, Try again. \n"; 
                }
            }
            ifstream file(file_obj[filename]); 
            cout<<filename;
            string filecontent; 
            if (!file.is_open()){ 
                cout << "Failed to open file. "; 
                filecontent = "";  
            } 
            else{
                stringstream buffer; 
                buffer << file.rdbuf(); 
                filecontent = buffer.str(); 
                file.close(); 
            }
            ofstream temp("tempcontent.txt");
            temp << filecontent;
            temp.close();
        }
    }
    return 0; 
}