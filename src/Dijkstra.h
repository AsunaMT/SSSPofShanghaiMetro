#ifndef PROJRCT2_DIJKSTRA_H
#define PROJRCT2_DIJKSTRA_H

#include <queue>

#include "shortest_path_algorithm.h"

struct DijkstraNode {
  int vertx;
  int dis;
};

bool operator<(const DijkstraNode &a, const DijkstraNode &b);
bool operator>(const DijkstraNode &a, const DijkstraNode &b);

class Dijkstra : public ShortestPathAlgorithm {
  std::priority_queue<DijkstraNode, std::vector<DijkstraNode>,
                      std::greater<DijkstraNode>> *q;
  void RelaxVtoU(TrafficMap &graph, int u, int *dis,
                 std::vector<AdjacentEdge> *precursors);

 public:
  bool FindShortestPath(TrafficMap &graph, std::string start, std::string end,
                        std::vector<ResultPath> &answer);
};

#endif