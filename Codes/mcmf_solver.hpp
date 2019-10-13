#include <bits/stdc++.h>
#include "constants.hpp"
using namespace std;

///////////////////// MCMF Part ////////////////////

class mcmf_solver {
	public:

		int n, upper;
		vector < vector <int> > qs;
		set < vector <int> > removed_edges;
		vector < pair <int, int> > removed_queries;

		mcmf_solver() {};

		mcmf_solver(int _n, set < vector <int> > list_of_edges, vector < vector <int> > query_start, int up_cost) {
			n = _n;
			upper = up_cost;
			qs = query_start;
			for(auto ee : list_of_edges) {
				add_edge(ee[0], ee[1], 1, ee[2]);
				add_edge(ee[1], ee[0], 1, ee[2]);
			}
		};

		void find_mcmf(int start) {
			for(int i=0; i<(int)(qs[start].size()); i++) {
				for(int j=0; j<qs[start][i]; j++) {
					add_edge(i, n, 1, 0);
				}
			}
			auto mcmf = min_cost_flow(n+1, inf-1, start, n);
			removed_edges.clear();
			removed_queries.clear();
			for(auto ed : e) {
				if(ed.cost > 0 && ed.cap == 0) {
					removed_edges.insert({min(ed.a, ed.b), max(ed.a, ed.b), (int) ed.cost});
				}
				if(ed.cost == 0 && ed.cap == 0 && ed.b == n) {
					removed_queries.push_back({ed.a, start});
				}
			}
		}

	private:


		struct edge {
			int a, b;
			long long cap, cost;
		};
		 
		vector <edge> e;
		vector <int> g[max_n];

		void add_edge(int a, int b, int cap, long long cost) {
			edge e1 = { a, b, cap, cost };
			edge e2 = { b, a, 0, -cost };
			g[a].push_back((int)e.size());
			e.push_back(e1);
			g[b].push_back((int)e.size());
			e.push_back(e2);
		}

		// dijkstra or bellman-fored
		void shortest_paths(int n, int v0, vector <long long> &d, vector <int> &p) {
			d.assign(n, inf+1);
			d[v0] = 0;
			deque <int> q;
			q.push_back(v0);
			p.assign(n, -1);
			vector <int> m(n, 2);
			while(q.size()) {
				int u = q.front();
				q.pop_front();
				m[u] = 0;
				for(int i : g[u]) {
					int v = e[i].b;
					if(e[i].cap > 0 && d[v] > d[u] + e[i].cost) {
						d[v] = d[u] + e[i].cost;
						p[v] = i;
						if(m[v] == 2) {
							m[v] = 1;
							q.push_back(v);
						}
						else if(m[v] == 0) {
							m[v] = 1;
							q.push_front(v);
						}
					}
				}
			}
		}

		pair <long long, long long> min_cost_flow(int n, int needed_flow, int s, int t) {
			int flow = 0;
			long long cost = 0;
			vector <int> p;
			vector <long long> d;
			while(flow < needed_flow) {
				shortest_paths(n, s, d, p);
				if(d[t] > upper) break;
				long long f = needed_flow - flow;
				long long cur = t;
				while(cur != s) {
					f = min(f, e[p[cur]].cap);
					cur = e[p[cur]].a;
				}
				flow += f;
				assert(f == 1);
				cost += f * d[t];
				cur = t;
				while(cur != s) {
					e[p[cur]].cap -= f;
					e[p[cur]^1].cap += f;
					cur = e[p[cur]].a;
				}
			}
			return {flow, cost};
		}

};

////////////////////////////////////////////////////////////////////////////////////
