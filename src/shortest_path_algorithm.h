#ifndef PROJRCT2_SHORTESTPATHALGORITHM_H
#define PROJRCT2_SHORTESTPATHALGORITHM_H

#include <algorithm>

#include "traffic_map.h"

class ResultPath {
 public:
  std::vector<std::string> path;
  std::vector<int> lines;
  int time;
  int transfer_times;
  ResultPath();
  void Copy(const ResultPath &a);
};
bool operator==(const ResultPath &a, const ResultPath &b);

class ShortestPathAlgorithm {
 protected:
  void RecursiveAnswer(TrafficMap &graph, std::vector<ResultPath> &answer,
                       std::string start, std::string end,
                       std::vector<AdjacentEdge> *precursors);

 public:
  virtual bool FindShortestPath(TrafficMap &graph, std::string start,
                                std::string end,
                                std::vector<ResultPath> &answer) = 0;
};

#endif