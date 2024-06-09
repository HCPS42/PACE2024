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
            int cw;
            s >> useless >> id >> n0 >> n1 >> m >> cw;
            vector<int> cw_order(n0 + n1);
            for (int i = 0; i < n0 + n1; i++) {
                getline(cin, line);
                stringstream t(line);
                t >> cw_order[i];
            }
            vector<pair<int, int>> es;
            for (int i = 1; i <= m; i++) {
                getline(cin, line);
                stringstream t(line);
                int u, v;
                t >> u >> v;
                es.emplace_back(u, v);
            }
            solve_heuristics(n0, n1, es);
        }
        else {
            assert(0);
        }
    }
}

void answer(vector<int>& a, const vector<vector<int>>& c) {
    for (int i : a) {
#ifndef LOCAL
        cout << i << "\n";
#endif
    }
    if (c.empty()) {
        return;
    }
    int n = *min_element(a.begin(), a.end()) - 1;
    ll num_of_intersection = 0;
    for (int i = 0; i < a.size(); i++) {
        for (int j = i + 1; j < a.size(); j++) {
            num_of_intersection += c[a[i] - n - 1][a[j] - n - 1];
        }
    }
    out(num_of_intersection);

    int m = (int) c.size();
    assert(a.size() == c.size());
    sort(a.begin(), a.end());
    for (int i = 0; i < m; i++) {
        assert(a[i] == n + 1 + i);
    }
}

void dfs_inv(int v, vector<bool>& used, vector<int>& post, const vector<vector<int>>& g_inv) {
    used[v] = true;
    for (int u : g_inv[v]) {
        if (!used[u]) {
            dfs_inv(u, used, post, g_inv);
        }
    }
    post.push_back(v);
}

void dfs(int v, vector<bool>& used, vector<int>& comp, const vector<vector<int>>& g) {
    used[v] = true;
    comp.push_back(v);
    for (int u : g[v]) {
        if (!used[u]) {
            dfs(u, used, comp, g);
        }
    }
}

vector<vector<int>> kosaraju(const vector<vector<int>> (&g)[2]) {
    int m = (int) g[0].size();
    vector<bool> used(m, false);
    vector<int> post;
    for (int i = 0; i < m; i++) {
        if (!used[i]) {
            dfs_inv(i, used, post, g[1]);
        }
    }
    reverse(post.begin(), post.end());
    used.assign(m, false);
    vector<vector<int>> comps;
    for (int v : post) {
        if (used[v]) continue;
        vector<int> comp;
        dfs(v, used, comp, g[0]);
        comps.push_back(comp);
    }
    return comps;
}

ll calc_cost(const vector<int>& a, const vector<vector<int>>& c) {
    ll res = 0;
    for (int i = 0; i < a.size(); i++) {
        for (int j = i + 1; j < a.size(); j++) {
            res += c[a[i]][a[j]];
        }
    }
    return res;
}

const int B = 10;

//int perms = 0;

vector<int> divide_and_conquer(vector<int> comp, const vector<vector<int>>& c) {
    if (tle) {
        return comp;
    }
    if (comp.size() <= B) {
        sort(comp.begin(), comp.end());
        auto best = comp;
        ll best_cost = calc_cost(comp, c);
        do {
            ll cost = calc_cost(comp, c);
            if (cost < best_cost) {
                best_cost = cost;
                best = comp;
            }
            //perms++;
        } while (next_permutation(comp.begin(), comp.end()));
        return best;
    }
    vector<int> lef;
    vector<int> rig;
    int m = comp.size() / 2;
    for (int i = 0; i < comp.size(); i++) {
        if (i < m) {
            lef.push_back(comp[i]);
        }
        else {
            rig.push_back(comp[i]);
        }
    }
    lef = divide_and_conquer(lef, c);
    rig = divide_and_conquer(rig, c);
    comp.clear();
    for (int i : lef) comp.push_back(i);
    for (int i : rig) comp.push_back(i);
    vector<int> rev_comp;
    for (int i : rig) rev_comp.push_back(i);
    for (int i : lef) rev_comp.push_back(i);
    if (calc_cost(comp, c) < calc_cost(rev_comp, c)) {
        return comp;
    }
    else {
        return rev_comp;
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

vector<int> improve_comp(vector<int> comp, const vector<vector<int>>& c, const vector<pair<int, int>>& es, int n) {
    if (comp.size() <= 2) {
        return comp;
    }

    if (comp.size() > 100) {
        out(comp.size());
    }

    // barycenters
    
    vector<E> p;
    map<int, int> whe;
    for (int i = 0; i < comp.size(); i++) {
        p.push_back({comp[i], 0, 0});
        whe[comp[i]] = i;
    }
    for (auto [u, v] : es) {
        p[whe[v - n - 1]].cnt++;
        p[whe[v - n - 1]].sum += u;
    }
    sort(p.begin(), p.end());
    comp.clear();
    for (auto [id, cnt, sum] : p) {
        comp.push_back(id);
    }
     
    vector<int> res;

    int BLOCKS = 4;

    for (int i = 0; i < comp.size(); i += BLOCKS * B) {
        vector<int> cur;
        for (int j = 0; j < BLOCKS * B && i + j < comp.size(); j++) {
            cur.push_back(comp[i + j]);
        }
        cur = divide_and_conquer(cur, c);
        for (int v : cur) {
            res.push_back(v);
        }
    }

    //out(perms);

    return res;
}

void solve_heuristics(int n, int m, vector<pair<int, int>> es) {
    out(n, m, es.size());

    vector<vector<int>> c;
    if (m > 15'000) { // maybe increase???
        vector<E> a;
        for (int i = 0; i < m; i++) {
            a.push_back({i + n + 1, 0, 0});
        }
        for (auto [u, v] : es) {
            a[v - n - 1].cnt++;
            a[v - n - 1].sum += u;
        }
        sort(a.begin(), a.end());
        vector<int> ans;
        for (auto [id, cnt, sum] : a) {
            ans.push_back(id);
        }
        answer(ans, c);
        return;
    }
    vector<vector<int>> G(m);
    for (auto [u, v] : es) {
        G[v - n - 1].push_back(u);
    }
    for (auto& s : G) {
        sort(s.begin(), s.end());
    }
    c.assign(m, vector<int>(m, 0));
    
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            if (i == j) continue;
            int cnt = 0;
            int ptr = -1;
            for (int k = 0; k < G[i].size(); k++) {
                while (ptr + 1 < G[j].size() && G[j][ptr + 1] < G[i][k]) {
                    ptr++;
                }
                cnt += ptr + 1;
            }
            c[i][j] = cnt;
        }
    }

    G.clear();

    ll trivial_lower_bound = 0;
    for (int i = 0; i < m; i++) {
        for (int j = i + 1; j < m; j++) {
            trivial_lower_bound += min(c[i][j], c[j][i]);
        }
    }

    out(trivial_lower_bound);

    vector<vector<int>> g[2];
    g[0].resize(m);
    g[1].resize(m);

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            if (i == j) continue;
            if (c[i][j] < c[j][i]) {
                g[0][i].push_back(j);
                g[1][j].push_back(i);
            }
        }
    }

    auto comps = kosaraju(g);

    reverse(comps.begin(), comps.end());
    vector<int> ans;
    for (auto& comp : comps) {
        if (!tle) {
            comp = improve_comp(comp, c, es, n);
        }
        for (int v : comp) {
            ans.push_back(v + n + 1);
        }
    }
    answer(ans, c);
}

