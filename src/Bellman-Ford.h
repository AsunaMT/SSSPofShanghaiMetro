#ifndef PROJRCT2_BELLMANFORD_H
#define PROJRCT2_BELLMANFORD_H

#include "shortest_path_algorithm.h"

class BellmanFord : public ShortestPathAlgorithm {
  void RelaxVtoU(TrafficMap &graph, int u, int *dis,
                 std::vector<AdjacentEdge> *precursors);

 public:
  virtual bool FindShortestPath(TrafficMap &graph, std::string start,
                                std::string end,
                                std::vector<ResultPath> &answer);
};

#endif