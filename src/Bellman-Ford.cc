#include "Bellman-Ford.h"

#include <iostream>
using std::cout;
using std::find;
using std::string;
using std::vector;

void BellmanFord::RelaxVtoU(TrafficMap &graph, int u, int *dis,
                            vector<AdjacentEdge> *precursors) {
  for (auto v : graph.graph_[u]) {
    int &vd = dis[v.vertx];
    const int &ud = dis[u];
    if (vd >= ud + v.weight) {
      if (vd > ud + v.weight) vector<AdjacentEdge>().swap(precursors[v.vertx]);
      vd = ud + v.weight;
      AdjacentEdge buf;
      buf.vertx = u;
      buf.line = v.line;
      buf.weight = v.weight;
      if (find(precursors[v.vertx].begin(), precursors[v.vertx].end(), buf) ==
          precursors[v.vertx].end()) {
        precursors[v.vertx].push_back(buf);
      }
    }
  }
}

bool BellmanFord::FindShortestPath(TrafficMap &graph, string start, string end,
                                   vector<ResultPath> &answer) {
  const int n = graph.station_str_to_int_.size();

  int *dis = new int[n];
  vector<AdjacentEdge> *precursors = new vector<AdjacentEdge>[n];
  vector<AdjacentEdge> temp;
  AdjacentEdge startself;
  int st = graph.station_str_to_int_[start];
  startself.vertx = st;
  for (int i = 0; i < n; i++) {
    dis[i] = INT_MAX - 100;
    precursors[i] = temp;
  }
  dis[st] = 0;
  for (int k = 1; k < n; k++) {
    for (int u = 0; u < n; u++) {
      RelaxVtoU(graph, u, dis, precursors);
    }
  }
  for (int u = 0; u < n; u++) {
    for (auto v : graph.graph_[u]) {
      if (dis[v.vertx] > dis[u] + v.weight) {
        return false;
      }
    }
  }
  RecursiveAnswer(graph, answer, start, end, precursors);
  delete[] precursors;
  delete[] dis;
  return true;
}
