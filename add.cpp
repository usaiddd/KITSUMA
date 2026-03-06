#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <map>
using namespace std; 

int main(){
    map<string,string> file_obj;
    bool file=true;
    string i,name;
    ifstream ipf("structure.txt");
    while(getline(ipf,i)){
        if(file){
            name=i;
            file_obj[i]="";
            file=false;
        }
        else{
            file_obj[name]=i;
            file=true;
        }
    }
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
            ifstream file(obj); 
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
            string command = "python load.py A \"" + pair.first + "\" tempcontent.txt";
            system(command.c_str());
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
            string command = "python load.py A \"" + filename + "\" tempcontent.txt";
            system(command.c_str());
        }
    }
    
    return 0; 
}