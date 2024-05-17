#include <iostream>
#include <algorithm>
#include <string>
#include <cmath>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <iomanip>
#include <cassert>
#include <cstring>
#include <sstream>
#include <numeric>
#include <cstdio>
#include <bitset>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <random>
#include <unordered_map>

using namespace std;

typedef long long ll;
typedef long double ld;
typedef pair <int, int> pii;
typedef int(*sigma_t)(string);

#define pb push_back
#define F first
#define S second
#define _sz(x) ((int)x.size())

const int LG = 28;
// String size n and algorithm parameters
int n, k, w, sigma_opt = 1;

// Setting up different sigma functions to define ordering
vector <int> rnd;
bool rnd_set = false;

// Character order
inline int get(char c) {
		if (c == 'A') return 0;
		if (c == 'C') return 1;
		if (c == 'G') return 2;
		if (c == 'T') return 3;
		return -1;
}

// Lexicographical ordering
inline int sigma_lexi(string t) {
		int res = 0;
		for (int i = 0; i < t.size(); i++)
				res = 4 * res + get(t[i]);
		return res;
}

inline int sigma_lexi_rev(string t) {
		int res = 0;
		for (int i = 0; i < t.size(); i++)
				res = 4 * res + (3 - get(t[i]));
		return res;
}

// Randomized permutation order
inline int sigma_rand(string t) {

	if (!rnd_set){
		rnd_set = true;
		rnd.resize((1 << (2 * k)));
		iota(rnd.begin(), rnd.end(), 0);
		random_device rd;
		mt19937 g(rd());
		shuffle(rnd.begin(), rnd.end(), g);
	}
	return rnd[sigma_lexi(t)];
}

// list of implemented sigma functions
sigma_t sigmas[] = {sigma_lexi,sigma_rand,sigma_lexi_rev};

inline int sigma(string t){
	return (sigmas[sigma_opt])(t);
}
void read_dna_sequence(const std::string& filename, std::string& dna_sequence) {
    std::ifstream infile(filename);
    std::string line;

    // Skip the header line
    if (std::getline(infile, line)) {
        while (std::getline(infile, line)) {
            if (line[0] == '>') {
                continue; // skip header lines in the middle of the file
            }
            std::string line_filtered = "";
            for(auto c:line)
                if(c == 'A' || c == 'C' || c=='G' || c == 'T')
                    line_filtered += c;

            if(dna_sequence.size() > 1000 * 1000)
                break;
            dna_sequence += line_filtered;
        }
    }
    infile.close();
}

// Non-sparse suffix array
vector<int> rnk, ord;
// string s
string s;

// sparse suffix array
vector <int> sparse_suff;


vector <pii> vals;
set <pair<int, int>> st;
set <int> inds;

// finds minimizers of input string using parameters k, w, sigma_opt
void find_kmers() {
	for (int i = 0; i <= n - k; i++) vals.pb({sigma(s.substr(i, k)), i});

	for (int i = 0; i < w - k; i++) st.insert({vals[i].F, i});

	for (int i = w - k; i <= n - k; i++) {
			st.insert({vals[i].F, i});
			inds.insert(st.begin()->second);
			st.erase({vals[i - (w - k)].F, i - (w - k)});
	}
}

// builds full suffix array
void build_suff(){
	rnk.resize(n);
	ord.resize(n);

	vector <pair <pii, int>> p;

	for (int i = 0; i < n; i++) rnk[i] = s[i];

	for (int t = 0; t < LG; t++) {
			p.clear();

			for (int i = 0; i < n; i++) {
					if (i + (1 << t) < n)
							p.push_back({{rnk[i], rnk[i + (1 << t)]}, i});
					else
							p.pb({{rnk[i], -1}, i});
			}

			sort(p.begin(), p.end());

			rnk[p[0].S] = 0;
			for (int i = 1; i < n; i++) {
					rnk[p[i].S] = rnk[p[i - 1].S] + (p[i - 1].F != p[i].F);
			}
	}

	for (int i = 0; i < n; i++)
			ord[rnk[i]] = i;
}

inline bool cmp(int i, int j) {
		return rnk[i] < rnk[j];
}

// restricts suffix array to samled indices
void sparsify(){
	for (int i: inds) {
			sparse_suff.pb(i);
	}
	sort(sparse_suff.begin(), sparse_suff.end(), cmp);
}

unordered_map<string,pair<int,int>> ptable;

void construct_prefix_table(){
	for(int i=0;i<sparse_suff.size();i++){
        int ind = sparse_suff[i];
        
        string sub = s.substr(ind,k);
        if (!ptable.count(sub)){
            ptable[sub] = {i,i+1};
        }
        else{
            ptable[sub].S = i + 1;
        }
    }
}

// finds minimal kmer in the first w characters of pattern p
inline int get_best_kmer(string p) {
		set<pii> st;
		for (int i = 0; i <= w - k; i++)
				st.insert({sigma(p.substr(i, k)), i});
		int ind = st.begin()->second;
		return ind;
}

// compare pattern and suffix starting at idx
// start_ind is a guiding variable showing a lower bound on the lcp of p and the suffix
// first element of return is 1 if p is greater, -1 if the suffix is greater, and 0 if p is a prefix of the suffix
// second element is the LCP of the inputs
inline pii cmp_str(string p, int idx, int start_ind=0) {
		for (int i = start_ind; i < min((int)p.size(), n - idx); i++) {
				if (p[i] < s[idx + i]) return {1,i};
				if (p[i] > s[idx + i]) return {-1,i};
		}
		if (p.size() <= n - idx) 
			return {0,(int)p.size()};
		else{
			return {-1,n-idx};
		} 
}

// binary search to find matches
inline pii get_range(string p, int lo = -1, int hi = sparse_suff.size()) {
		int low = lo, high = hi;

		while (high - low > 1) {
				int mid = (low + high) / 2;
				if (cmp_str(p, sparse_suff[mid]).F >= 0) high = mid;
				else low = mid;
		}

		int l = high;

		low = lo, high = hi;

		while (high - low > 1) {
				int mid = (low + high) / 2;
				if (cmp_str(p, sparse_suff[mid]).F > 0) high = mid;
				else low = mid;
		}

		int r = high;

		return {l, r}; // answer is range [l,r)
}

// binary search, improved using lcp values
inline pii get_range_lcp(string p, int lo = -1, int hi = sparse_suff.size()) {
		int low = lo, high = hi;
		int lcp_l = 0, lcp_h = 0;
		while (high - low > 1) {
				int mid = (low + high) / 2;
				int lcp_m = min(lcp_l,lcp_h);
				pii cmp_res = cmp_str(p, sparse_suff[mid],lcp_m);
				if (cmp_res.F >= 0) {
					high = mid;
					lcp_h = cmp_res.S;
				}
				else {
					low = mid;
					lcp_l = cmp_res.S;
				}
		}

		int l = high;

		low = lo, high = hi;
		lcp_l = 0, lcp_h = 0;

		while (high - low > 1) {
				int mid = (low + high) / 2;
				int lcp_m = min(lcp_l,lcp_h);
				pii cmp_res = cmp_str(p, sparse_suff[mid],lcp_m);
				if (cmp_res.F > 0) {
					high = mid;
					lcp_h = cmp_res.S;
				}
				else {
					low = mid;
					lcp_l = cmp_res.S;
				}
		}

		int r = high;

		return {l, r}; // answer is range [l,r)
}

int main(int argc, char* argv[]) {

    srand(time(NULL));

    
    k = atoi(argv[1]);
    w = atoi(argv[2]);
    if (argc > 3)
        sigma_opt = atoi(argv[3]);
    int use_p_table =0; 
    if (argc>4)
        use_p_table = atoi(argv[4]);
    string ref = "Homo_sapiens.GRCh38.dna.alt.fa"; 
    if (argc > 5)
        ref = argv[5];
    read_dna_sequence(ref, s);
    n = s.size();
    string qref = "sampled_substrings_1M.txt"; 
    if (argc > 6)
        qref = argv[6];

    ifstream qin(qref);

    build_suff();
    find_kmers();
    sparsify();
    if (use_p_table){
        construct_prefix_table();
    }

    // string bin_name="";
    // bin_name += "bin_out/data_";
    // bin_name += argv[1];
    // bin_name += "_";
    // bin_name += argv[2];
    // bin_name += "_";
    // bin_name += argv[3];
    // bin_name += ".bin";

    // std::ofstream outputFile(bin_name, std::ios::binary);

    // // Check if the file was opened successfully
    // if (outputFile.is_open()) {
    //     // Write the size of the vector
    //     size_t vectorSize = sparse_suff.size();
    //     outputFile.write(reinterpret_cast<char*>(&vectorSize), sizeof(size_t));

    //     // Write the elements of the vector
    //     outputFile.write(reinterpret_cast<char*>(sparse_suff.data()), vectorSize * sizeof(int));

    //     // Close the file
    //     outputFile.close();
    //     std::cout << "Vector saved to file successfully." << std::endl;
    // } else {
    //     std::cout << "Unable to open file for writing." << std::endl;
    // }

    // return 0;

    ld avg_time = 0;
    int cnt_q = 0;
    int gt_idx;
    string p;
    while(qin >> gt_idx >> p)
    {
        clock_t begin_time = clock();
        cnt_q ++;
        int best_kmer = get_best_kmer(p);
        string bkmer = p.substr(best_kmer, (int)p.size() - best_kmer);
        pii range;
        if (use_p_table){
            if (ptable.count(p.substr(best_kmer,k))){
                range = ptable[p.substr(best_kmer,k)];
                range = get_range(bkmer,range.F-1,range.S);
            }else{
                range = {0,0};
            }
        }
        else{
            range = get_range(bkmer);
        }
        

        int l = range.first, r = range.second;

        vector <int> vec_ans;

        bool okay = false;

        for (int i = l; i < r; i++) {
                int ind = sparse_suff[i];

                int start = ind - best_kmer;

                bool flag = true;
                for (int j = 0; j < best_kmer; j++)
                        if (s[start + j] != p[j]) {
                                flag = false;
                                break;
                        }

                if (flag && start == gt_idx) {
                        okay = true;
                        // break;
                }
        }

        avg_time += ld(clock() - begin_time);

        if (!okay) cerr << "Wrong" << endl;
        // else cerr << "Correct!" << endl;
    }
    cout<<"K: "<<k<<", W: "<<w<<endl;
    cout << "Density: " << (ld)sparse_suff.size() / ord.size() << endl;
    cout << "Avg. Query Time: " << avg_time / cnt_q << " ms"<< endl;
    cout<<endl;
}

/*

Test 1:

AACAGTAATACTGCATACT
3 5
ATACT

*/
