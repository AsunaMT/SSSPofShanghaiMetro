#include "Dijkstra.h"

#include <iostream>
#include <queue>
using std::cout;
using std::find;
using std::greater;
using std::priority_queue;
using std::string;
using std::vector;

bool operator<(const DijkstraNode &a, const DijkstraNode &b) {
  return a.dis < b.dis;
}
bool operator>(const DijkstraNode &a, const DijkstraNode &b) {
  return a.dis > b.dis;
}

void Dijkstra::RelaxVtoU(TrafficMap &graph, int u, int *dis,
                         vector<AdjacentEdge> *precursors) {
  for (auto v : graph.graph_[u]) {
    int &vd = dis[v.vertx];
    const int &ud = dis[u];
    if (vd >= ud + v.weight) {
      if (vd > ud + v.weight) {
        vector<AdjacentEdge>().swap(precursors[v.vertx]);
        q->push({v.vertx, vd});
      }
      vd = ud + v.weight;
      AdjacentEdge buf;
      buf.vertx = u;
      buf.line = v.line;
      buf.weight = v.weight;
      if (find(precursors[v.vertx].begin(), precursors[v.vertx].end(), buf) ==
          precursors[v.vertx].end()) {
        precursors[v.vertx].push_back(buf);
        // cout << v.vertx << "sqsc" << buf.vertx << "\n";
      }
    }
  }
}

bool Dijkstra::FindShortestPath(TrafficMap &graph, string start, string end,
                                vector<ResultPath> &answer) {
  q = new priority_queue<DijkstraNode, vector<DijkstraNode>,
                         greater<DijkstraNode>>;
  const int n = graph.station_str_to_int_.size();
  int *dis = new int[n];
  int *vis = new int[n];
  vector<AdjacentEdge> *precursors = new vector<AdjacentEdge>[n];
  vector<AdjacentEdge> temp;
  AdjacentEdge startself;
  int st = graph.station_str_to_int_[start];
  startself.vertx = st;
  for (int i = 0; i < n; i++) {
    vis[i] = 0;
    dis[i] = INT_MAX - 100;
    precursors[i] = temp;
  }
  q->push({st, 0});
  dis[st] = 0;
  // precursors[st].push_back(startself);
  while (!q->empty()) {
    int u = q->top().vertx;
    q->pop();
    if (vis[u]) continue;
    vis[u] = 1;
    RelaxVtoU(graph, u, dis, precursors);
  }
  RecursiveAnswer(graph, answer, start, end, precursors);
  delete[] precursors;
  delete[] dis;
  delete[] vis;
  delete[] q;
  return true;
}