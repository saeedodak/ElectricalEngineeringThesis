// in the name of Allah

#include <bits/stdc++.h>
#include "mcmf_solver.hpp"
#include "constants.hpp"
using namespace std;

#define sz(x) ((int) (x.size()))

#define fr first
#define se second

int n, m, q;
int max_handled_querys;
vector <int> best_path;
vector < vector <int> > rem_E;
vector < pair <int, int> > best_remove;

void process_step(set < vector <int> > &list_of_edges, vector < vector <int> > &query_start, int up_cost, int step_number, vector <int> &path, vector < pair <int, int> > &pre_removed_queries, vector < vector <int> > &lstE) {
	if(step_number == max_depth) {
		if(sz(pre_removed_queries) > max_handled_querys) {
			best_path = path;
			best_remove = pre_removed_queries;
			max_handled_querys = sz(pre_removed_queries);
			rem_E = lstE;
		}
		return;
	}
	for(int i=0; i<n; i++) {
		bool gone = 0;
		for(int e : path) {
			if(i == e) {
				gone = 1;
				break;
			}
		}
		if(gone) continue;

		mcmf_solver MS(n, list_of_edges, query_start, up_cost);
		MS.find_mcmf(i);

		path.push_back(i);
		for(auto e : MS.removed_edges) {
			list_of_edges.erase(e);
			lstE.push_back(e);
		}
		for(auto e : MS.removed_queries) {
			// assert(query_start[e.fr][e.se] > 0 && query_start[e.se][e.fr] > 0);
			query_start[e.fr][e.se]--;
			query_start[e.se][e.fr]--;
			pre_removed_queries.push_back(e);
		}

		// solve recursive!
		process_step(list_of_edges, query_start, up_cost, step_number+1, path, pre_removed_queries, lstE);

		// update back list_of_edges and query_start
		path.pop_back();
		for(auto e : MS.removed_edges) {
			list_of_edges.insert(e);
			lstE.pop_back();
		}
		for(auto e : MS.removed_queries) {
			query_start[e.fr][e.se]++;
			query_start[e.se][e.fr]++;
			pre_removed_queries.pop_back();
		}
	}
}

int mohmoh = 0;

void do_this_found_path_removes(vector < vector <int> > &query_start) {
	for(auto e : best_remove) {
		// assert(query_start[e.fr][e.se] > 0 && query_start[e.se][e.fr] > 0);
		query_start[e.fr][e.se]--;
		query_start[e.se][e.fr]--;
	}
	if(mohmoh) {
		for(auto ee : rem_E) cout << ee[0] << " " << ee[1] << " " << ee[2] << endl;
	}
	
}

// solve layer by layer for the given upper bound of cost
// return number of required layers
int solve(int up_cost, set < vector <int> > list_of_edges, vector < vector <int> > query_start) {
	int cnt = 0, reqs = 0; // number of layers, number of handled requests
	while(reqs < q) {
		// greedily process and do mcmf for #max_depth verteces 
		// and pick the best set of starting veteces! :)

		cnt++;
		// back track method
		best_path.clear();
		vector <int> path;
		vector < pair <int, int> > rem;
		max_handled_querys = 0;
		vector < vector <int> > hahaha;
		process_step(list_of_edges, query_start, up_cost, 0, path, rem, hahaha);
		reqs += max_handled_querys;
		if(max_handled_querys == 0) return inf;
		if(cnt > upper_bound_of_layers) return cnt;
		// we should remove handled requests
		do_this_found_path_removes(query_start);
	}
	return cnt;
}


int main() {

	set < vector <int> > list_of_edges;
	vector < vector <int> > query_start(N, vector <int> (N, 0));

	// fast input & output
	// ios::sync_with_stdio(0);
	// cin.tie(0); cout.tie(0);

	// read the input (network and demands)
	cin >> n >> m;
	for(int i=0; i<m; i++) {
		int u, v, w;
		cin >> u >> v >> w;
		list_of_edges.insert({min(u, v), max(u, v), w});
	}

	int all = 0;
	for(int i=1; i<n; i++) {
		for(int j=0; j<i; j++) {
			double cnt;
			cin >> cnt;
			int x = ceil(cnt/100);
			cerr << x << " ";
			all += x;
			query_start[i][j] += x;
			query_start[j][i] += x;
		}
		cout << endl;
	}
	q = all;
	cout << q << endl;

	// binary search for min cost

	long long l = 0, r = inf, mid_cost;
	while(l < r) {
		mid_cost = (l + r) >> 1;
		cerr << "m " << mid_cost << " #########################" << endl;
		int layers = solve(mid_cost, list_of_edges, query_start);
		if(layers <= upper_bound_of_layers) r = mid_cost;
		else l = mid_cost + 1;

		// cerr << "####################################" << endl;
	}

	cerr << "&&&&&&&&&&&&&&&&&&&&&&&" << endl;

	mohmoh = 1;
	int LA = solve(r, list_of_edges, query_start);

	cout << "cost " << r << endl;
	cout << "number of layers " << LA << endl;


//////////////////////////////////////////////////////////////

	return 0;
}


// there is lots of trash(unused) space, I should use them,
// but I don't now how to fix it right now!! :(
