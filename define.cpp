#include<iostream>
#include<cstdlib>
#include<vector>
#include<fstream>
#include<string>
#include <algorithm>
#include <cctype>
using namespace std;

class user{
private:
    string authority;
    string id;
public:
    user(string authority,string id){
        this->authority=authority;
        this->id=id;
    }
};

bool is_numeric(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), [](unsigned char c) {
        return std::isdigit(c);
    });
}

int main(){
    system("g++ run.cpp -o run && .\\run");
    vector<string> files,filepath;
    int index=0;
    user *u;
    string base="C:/testing";
    ifstream inputFile("structure.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: structure.txt" << std::endl;
        return 1;
    }
    bool auth=false,emps=false,structure=false,file=false,folder=true;
    string line,id,final="",file_name="";
    bool first=true;
    while(getline(inputFile,line)){
        if(first){
            id=line;
            first=false;
            continue;
        }
        if(line=="auth:"){
            auth=true;
            continue;
        }
        else if(line=="emps:"){
            auth=false;
            emps=true;
            continue;
        }
        else if(line=="struct:"){
            emps=false;
            structure=true;
            continue;
        }
        if(auth){
            u=new user(line,id);
        }
        if(emps){
        }
        if(structure){
            if(is_numeric(line)){
                file=true;
                folder=false;
            }
            if(folder){
                final = "mkdir \"" + base + "/" + line + "\"";
                system(final.c_str());
            }
            if(file){
                final="python load.py GFN \"" + line + "\"";
                system(final.c_str());
                ifstream inpFile("temp.txt");
                getline(inpFile,line);
                final = "type nul > "+ base + "/" + line;
                system(final.c_str());
                final= "python load.py GF \"" + line + "\" \"" + base + "\" \"" + file_name + "\"";
                system(final.c_str());
                files.push_back(file_name);
                filepath.push_back(base+'/'+line);
                file=false;
                folder=true;
            }
        }
    }
    inputFile.close();
    // ofstream ofs("structure.txt", ofstream::out | ofstream::trunc);
    // for (string i : files){
    //     ofs<<i<<endl;
    //     ofs<<filepath[index]<<endl;
    //     index++;
    // }
    // ofs.close();
    return 0;
}
