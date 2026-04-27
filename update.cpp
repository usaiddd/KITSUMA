#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

struct Edit {
    char type;
    string line;
};

vector<Edit> myersDiff(vector<string>& A, vector<string>& B) {
    int N = A.size();
    int M = B.size();
    int maxD = N + M;

    int offset = maxD;
    vector<int> V(2 * maxD + 1, -1);
    vector<vector<int>> trace;

    V[offset + 1] = 0;

    for (int D = 0; D <= maxD; D++) {
        vector<int> current(2 * maxD + 1, -1);

        for (int k = -D; k <= D; k += 2) {
            int idx = offset + k;
            int x;

            if (k == -D || (k != D && V[idx - 1] < V[idx + 1])) {
                x = V[idx + 1];
            } else {
                x = V[idx - 1] + 1;
            }

            int y = x - k;
            while (x < N && y < M && A[x] == B[y]) {
                x++;
                y++;
            }

            current[idx] = x;

            if (x >= N && y >= M) {
                trace.push_back(current);
                goto BACKTRACK;
            }
        }

        trace.push_back(current);
        V = current;
    }

BACKTRACK:
    vector<Edit> result;
    int x = N, y = M;

    for (int D = trace.size() - 1; D >= 0; D--) {
        auto& V = trace[D];
        int k = x - y;
        int idx = offset + k;

        if (D == 0) break;

        int prev_k;
        if (k == -D || (k != D && V[idx - 1] < V[idx + 1])) {
            prev_k = k + 1;
        } else {
            prev_k = k - 1;
        }

        int prev_idx = offset + prev_k;
        int prev_x = trace[D - 1][prev_idx];
        int prev_y = prev_x - prev_k;

        while (x > prev_x && y > prev_y) {
            result.push_back({' ', A[x - 1]});
            x--;
            y--;
        }

        if (x == prev_x) {
            result.push_back({'+', B[y - 1]});
            y--;
        } else {
            result.push_back({'-', A[x - 1]});
            x--;
        }
    }

    reverse(result.begin(), result.end());
    return result;
}

vector<string> readFile(string filename) {
    ifstream file(filename);
    vector<string> lines;
    string line;

    if (!file.is_open()) {
        cout << "Error opening file: " << filename << endl;
        return lines;
    }

    while (getline(file, line)) {
        lines.push_back(line);
    }

    return lines;
}

int main() {
    vector<string> fileA = readFile("usaid.txt");
    vector<string> fileB = readFile("usaid1.txt");
    vector<Edit> diff = myersDiff(fileA, fileB);
    ofstream myfile("final.txt");
    char decision;
    ifstream file("usaid1.txt");
    string line;

    if (!file.is_open()) {
        cout << "Error opening file" << endl;
    }

    while (getline(file, line)) {
        if(line==diff[0].line){
            break;
        }
        myfile<<line<<endl;
    }
    for (auto& e : diff) {
        if(e.type=='-'){
            cout<<"line deleted - "<<e.line<<endl;
            cout<<"keep the line deleted ? (y/n) : ";
            cin>>decision;
            if(decision=='n'){
                myfile<<e.line<<endl;
            }
        }
        if(e.type=='+'){
            cout<<"line added - "<<e.line<<endl;
            cout<<"keep the line added ? (y/n) : ";
            cin>>decision;
            if(decision=='y'){
                myfile<<e.line<<endl;
            }
        }
        if(e.type==' '){
            myfile<<e.line<<endl;
        }
    }

    return 0;
}