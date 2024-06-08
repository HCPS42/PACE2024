#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

string to_string(string a) { return '"' + a + '"'; }
string to_string(char a) { return "'" + string(1, a) + "'"; }
string to_string(const char* a) { return to_string((string) a); }
string to_string(bool a) { return a ? "true" : "false"; }
template <class T1, class T2>
string to_string(pair<T1, T2> a) {
    return "(" + to_string(a.first) + ", " + to_string(a.second) + ")";
}
template <class T>
string to_string(T a) {
    bool first = true; string res = "{";
    for (const auto& i : a) {
        if (!first) res += ", ";
        first = false;
        res += to_string(i);
    }
    res += "}";
    return res;
}
void debug_out() { cerr << endl; }
template <class T1, class... T2>
void debug_out(T1 a, T2... b) {
    cerr << " " << to_string(a);
    debug_out(b...);
}

#ifdef LOCAL
#define out(...) cerr << "[" << #__VA_ARGS__ << "]:", debug_out(__VA_ARGS__)
#else
#define out(...) 42
#endif

clock_t start_time; void start_timer() { start_time = clock(); }
double get_time() { return (double) (clock() - start_time) / CLOCKS_PER_SEC; }

void Solve();

int main() {
    ios_base::sync_with_stdio(false); cin.tie(nullptr);
#ifdef LOCAL
    //freopen("input.txt", "r", stdin);
#endif
    start_timer();
    Solve();
#ifdef LOCAL
    cerr << fixed << setprecision(3);
    cerr << endl << "Time spent: " << get_time() << endl;
#endif
    return 0;
}

// do something, stay focused
// look for stupid bugs
// guess, slow, stress
// don't overgeneralize
// don't rush

// don't waste time on standings

// SOLVE THE PROBLEM OR DIE TRYING
// THE SOLUTION IS ALWAYS SIMPLE
// THE CODE IS ALWAYS SHORT

volatile sig_atomic_t tle = 0;
void term(int signum) { tle = 1; }

void solve_tiny(int n, int m, vector<pair<int, int>> es);
void solve_heuristics(int n, int m, vector<pair<int, int>> es);

void Solve() {
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = term;
    sigaction(SIGTERM, &action, NULL);

    string line;
    while (getline(cin, line)) {
        if (line[0] == 'c') {
            continue;
        }
        else if (line[0] == 'p') {
            stringstream s(line);
            string useless;
            string id;
            int n0, n1, m;
            s >> useless >> id >> n0 >> n1 >> m;
            vector<pair<int, int>> es;
            for (int i = 1; i <= m; i++) {
                getline(cin, line);
                stringstream t(line);
                int u, v;
                t >> u >> v;
                es.emplace_back(u, v);
            }
            //solve_tiny(n0, n1, es);
            solve_heuristics(n0, n1, es);
        }
        else {
            assert(0);
        }
    }
}

int calc_crossings_tiny(const vector<pair<int, int>>& es, const vector<int>& p) {
    map<int, int> whe;
    for (int i = 0; i < p.size(); i++) {
        whe[p[i]] = i;
    }
    int res = 0;
    for (int i = 0; i < es.size(); i++) {
        for (int j = i + 1; j < es.size(); j++) {
            auto [u1, v1] = es[i];
            auto [u2, v2] = es[j];
            if (u1 < u2 && whe[v1] > whe[v2]) {
                res++;
            }
            else if (u1 > u2 && whe[v1] < whe[v2]) {
                res++;
            }
        }
    }
    return res;
}

void solve_tiny(int n, int m, vector<pair<int, int>> es) {
    vector<int> p(m);
    iota(p.begin(), p.end(), n + 1);
    vector<int> ans = p;
    int best = calc_crossings_tiny(es, p);
    do {
        int cur = calc_crossings_tiny(es, p);
        if (cur < best) {
            best = cur;
            ans = p;
        }
    } while (next_permutation(p.begin(), p.end()));
    for (int i : ans) {
        cout << i << "\n";
    }
}

struct E {
    int id;
    ll cnt;
    ll sum;
};

bool operator<(const E& a, const E& b) {
    return a.sum * b.cnt < b.sum * a.cnt;
}

void solve_heuristics(int n, int m, vector<pair<int, int>> es) {
    sort(es.begin(), es.end());

    vector<E> a;
    for (int i = 0; i < m; i++) {
        a.push_back({i + n + 1, 0, 0});
    }
    for (auto [u, v] : es) {
        a[v - n - 1].cnt++;
        a[v - n - 1].sum += u;
    }
    sort(a.begin(), a.end());

    for (auto [id, cnt, sum] : a) {
        cout << id << "\n";
    }
}
