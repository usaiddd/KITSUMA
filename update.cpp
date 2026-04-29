#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define DIM     "\033[2m"
#define RED     "\033[38;5;203m"
#define GREEN   "\033[38;5;114m"
#define YELLOW  "\033[38;5;221m"
#define BLUE    "\033[38;5;75m"
#define CYAN    "\033[38;5;87m"

enum Op { KEEP, INS, DEL };
struct Edit { Op op; string s; };

vector<Edit> myersDiff(const vector<string>& a, const vector<string>& b) {
    int N = (int)a.size(), M = (int)b.size(), MAX = N + M;
    if (N == 0 && M == 0) return {};
    if (N == 0) { vector<Edit> r; for (auto& l:b) r.push_back({INS,l}); return r; }
    if (M == 0) { vector<Edit> r; for (auto& l:a) r.push_back({DEL,l}); return r; }

    vector<int>         V(2*MAX+2, -1);
    vector<vector<int>> Vs;
    V[1+MAX] = 0;

    for (int D = 0; D <= MAX; D++) {
        Vs.push_back(V);
        for (int k = -D; k <= D; k += 2) {
            bool down = (k == -D || (k != D && V[k-1+MAX] < V[k+1+MAX]));
            int x = down ? V[k+1+MAX] : V[k-1+MAX] + 1;
            int y = x - k;
            while (x < N && y < M && a[x] == b[y]) { x++; y++; }
            V[k+MAX] = x;
            if (x == N && y == M) goto done;
        }
    }
done:
    vector<Edit> ops;
    int x = N, y = M;
    for (int D = (int)Vs.size()-1; D >= 0; D--) {
        auto& pV = Vs[D];
        int k    = x - y;
        bool down = (k == -D || (k != D && pV[k-1+MAX] < pV[k+1+MAX]));
        int pk   = down ? k+1 : k-1;
        int px   = pV[pk+MAX], py = px - pk;
        // diagonal snake going back
        while (x > px + (!down?1:0) && y > py + (down?1:0)) {
            ops.push_back({KEEP, a[x-1]}); x--; y--;
        }
        if (D == 0) break;
        if (down) { ops.push_back({INS, b[y-1]}); y--; }
        else      { ops.push_back({DEL, a[x-1]}); x--; }
    }
    reverse(ops.begin(), ops.end());
    return ops;
}

vector<string> readFile(const string& path) {
    ifstream f(path);
    vector<string> lines;
    if (!f.is_open()) {
        cerr << RED << "Error: cannot open '" << path << "'" << RESET << "\n";
        return lines;
    }
    string line;
    while (getline(f, line)) lines.push_back(line);
    return lines;
}

bool writeFile(const string& path, const vector<string>& lines) {
    ofstream f(path);
    if (!f.is_open()) return false;
    for (auto& l : lines) f << l << "\n";
    return true;
}


struct BranchMap {
    int                    n;          
    vector<bool>           deleted;
    vector<vector<string>> ins_before; 
    vector<string>         ins_after;  
};

BranchMap buildBranchMap(const vector<string>& base, const vector<Edit>& diff) {
    BranchMap bm;
    bm.n = (int)base.size();
    bm.deleted.assign(bm.n, false);
    bm.ins_before.resize(bm.n);
    vector<string> pending;
    int bi = 0;
    for (auto& e : diff) {
        if (e.op == KEEP) {
            bm.ins_before[bi] = pending; pending.clear(); bi++;
        } else if (e.op == DEL) {
            bm.ins_before[bi] = pending; pending.clear();
            bm.deleted[bi] = true; bi++;
        } else {
            pending.push_back(e.s);
        }
    }
    bm.ins_after = pending;
    return bm;
}

enum RegionType { UNCHANGED, ONLY_A, ONLY_B, CONFLICT };

struct MergeRegion {
    RegionType     type;
    vector<string> base_lines, a_lines, b_lines;
};

vector<MergeRegion> threeWayMerge(const vector<string>& base,
                                   const vector<string>& A,
                                   const vector<string>& B) {
    auto bm_a = buildBranchMap(base, myersDiff(base, A));
    auto bm_b = buildBranchMap(base, myersDiff(base, B));

    vector<MergeRegion> out;

    auto emit = [&](RegionType t,
                    const vector<string>& bl,
                    const vector<string>& al,
                    const vector<string>& bbl) {
        if (!out.empty() && out.back().type == t) {
            auto& r = out.back();
            for (auto& l : bl)  r.base_lines.push_back(l);
            for (auto& l : al)  r.a_lines   .push_back(l);
            for (auto& l : bbl) r.b_lines   .push_back(l);
        } else {
            out.push_back({t, bl, al, bbl});
        }
    };

    for (int i = 0; i < bm_a.n; i++) {
        const auto& ia = bm_a.ins_before[i];
        const auto& ib = bm_b.ins_before[i];
        if (!ia.empty() || !ib.empty()) {
            if      (!ia.empty() && !ib.empty())
                emit(ia == ib ? UNCHANGED : CONFLICT, {}, ia, ib);
            else if (!ia.empty()) emit(ONLY_A, {}, ia, {});
            else                  emit(ONLY_B, {}, {}, ib);
        }

        bool da = bm_a.deleted[i], db = bm_b.deleted[i];
        if      (!da && !db) emit(UNCHANGED, {base[i]}, {base[i]}, {base[i]});
        else if ( da &&  db) /* both deleted: auto, emit nothing */ ;
        else if ( da)        emit(ONLY_A, {base[i]}, {}, {base[i]});
        else                 emit(ONLY_B, {base[i]}, {base[i]}, {});
    }

    const auto& ta = bm_a.ins_after, &tb = bm_b.ins_after;
    if (!ta.empty() || !tb.empty()) {
        if      (!ta.empty() && !tb.empty())
            emit(ta == tb ? UNCHANGED : CONFLICT, {}, ta, tb);
        else if (!ta.empty()) emit(ONLY_A, {}, ta, {});
        else                  emit(ONLY_B, {}, {}, tb);
    }

    return out;
}

void rule(const string& s, int n = 66) {
    for (int i = 0; i < n; i++) cout << s;
    cout << "\n";
}

string prompt(const string& msg) {
    string in;
    while (true) {
        cout << BOLD << YELLOW << "  > " << msg << " " << RESET;
        getline(cin, in);
        if (!in.empty()) return in;
    }
}

vector<string> resolveConflict(const vector<string>& a,
                                const vector<string>& b, int n) {
    cout << "\n";
    rule("~");
    cout << BOLD << YELLOW << "  CONFLICT #" << n << RESET << "\n";
    rule("~");

    cout << "\n" << BOLD << "  Branch A (file1):\n" << RESET;
    if (a.empty()) cout << RED << DIM << "  (deleted)\n" << RESET;
    else for (auto& l : a) cout << RED   << "  - " << RESET << l << "\n";

    cout << "\n" << BOLD << "  Branch B (file2):\n" << RESET;
    if (b.empty()) cout << RED << DIM << "  (deleted)\n" << RESET;
    else for (auto& l : b) cout << GREEN << "  + " << RESET << l << "\n";

    cout << "\n"
         << "  [a]   keep A\n"
         << "  [b]   keep B\n"
         << "  [ab]  keep both (A then B)\n"
         << "  [ba]  keep both (B then A)\n"
         << "  [e]   enter custom text\n"
         << "  [s]   skip (drop both)\n\n";

    while (true) {
        string c = prompt("Choice:");
        if (c == "a")  return a;
        if (c == "b")  return b;
        if (c == "s")  return {};
        if (c == "ab") { auto r=a; r.insert(r.end(),b.begin(),b.end()); return r; }
        if (c == "ba") { auto r=b; r.insert(r.end(),a.begin(),a.end()); return r; }
        if (c == "e") {
            cout << "  Type lines, empty line to finish:\n";
            vector<string> custom;
            string l;
            while (true) { cout << "  | "; getline(cin,l); if(l.empty()) break; custom.push_back(l); }
            return custom;
        }
        cout << RED << "  Unknown. Use: a / b / ab / ba / e / s\n" << RESET;
    }
}

vector<string> showAutoHunk(const MergeRegion& r, int n) {
    bool is_a = (r.type == ONLY_A);
    string branch = is_a ? "A (file1)" : "B (file2)";
    const auto& changed  = is_a ? r.a_lines : r.b_lines;
    const auto& original = r.base_lines;

    cout << "\n";
    rule("-");
    cout << BOLD << BLUE << "  AUTO-MERGE #" << n
         << RESET << DIM << "  only branch " << branch << " changed this\n" << RESET;
    rule("-");

    if (!original.empty()) {
        cout << DIM << "\n  base:\n" << RESET;
        for (auto& l : original) cout << DIM << "    " << l << RESET << "\n";
    }
    cout << "\n  result:\n";
    if (changed.empty()) cout << RED << DIM << "    (line deleted)\n" << RESET;
    else for (auto& l : changed) {
        bool is_new = find(original.begin(), original.end(), l) == original.end();
        if (is_new) cout << GREEN << "  + " << RESET << l << "\n";
        else        cout << DIM   << "    " << l << RESET << "\n";
    }

    cout << "\n  [y] accept   [n] override manually\n\n";
    string c = prompt("Accept auto-merge?");
    if (c == "n") return resolveConflict(r.a_lines, r.b_lines, n);
    return changed;
}

int main(int argc, char* argv[]) {
    string base_path = "not_merged/main.txt";
    string a_path    = "not_merged/file1.txt";
    string b_path    = "not_merged/file2.txt";
    string out_path  = "merged.txt";

    if (argc == 5) {
        base_path = argv[1]; a_path = argv[2];
        b_path    = argv[3]; out_path = argv[4];
    }

    auto base = readFile(base_path);
    auto A    = readFile(a_path);
    auto B    = readFile(b_path);
    if (base.empty() && A.empty() && B.empty()) return 1;

    auto regions = threeWayMerge(base, A, B);

    int n_conflict = 0, n_auto = 0;
    for (auto& r : regions) {
        if      (r.type == CONFLICT)  n_conflict++;
        else if (r.type != UNCHANGED) n_auto++;
    }

    cout << "\n";
    rule("=");
    cout << BOLD << CYAN << "  3-WAY MERGE\n" << RESET;
    cout << DIM  << "  base: " << base_path
                 << "   A: " << a_path
                 << "   B: " << b_path << "\n" << RESET;
    rule("=");
    cout << "\n"
         << "  " << GREEN  << n_auto     << RESET << " auto-mergeable hunk(s)\n"
         << "  " << YELLOW << n_conflict << RESET << " conflict(s) to resolve\n\n";
    rule("-");
    cout << DIM << "  LEGEND:  " << RESET
         << GREEN  << "+ added  " << RESET
         << RED    << "- deleted  " << RESET
         << DIM    << "context unchanged\n" << RESET;
    rule("-");

    vector<string> output;
    int hunk_no = 0;

    for (auto& r : regions) {
        if (r.type == UNCHANGED) {
            for (auto& l : r.a_lines) output.push_back(l);
            continue;
        }
        hunk_no++;
        vector<string> resolved;
        if (r.type == CONFLICT) resolved = resolveConflict(r.a_lines, r.b_lines, hunk_no);
        else                    resolved = showAutoHunk(r, hunk_no);
        for (auto& l : resolved) output.push_back(l);
    }

    cout << "\n";
    rule("=");
    if (writeFile(out_path, output))
        cout << BOLD << GREEN  << "  Merge complete -> " << out_path << RESET << "\n";
    else
        cout << BOLD << RED    << "  Failed to write " << out_path  << RESET << "\n";
    rule("=");
    cout << "\n";
    return 0;
}