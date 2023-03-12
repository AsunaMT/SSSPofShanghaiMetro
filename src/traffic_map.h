#ifndef PROJRCT2_TRAFFICMAP_H
#define PROJRCT2_TRAFFICMAP_H

#include <map>
#include <string>
#include <vector>

const int MAX = 500;

struct AdjacentEdge {
  int vertx = -1;
  int line = -1;
  int weight = 0;
};

bool operator==(const AdjacentEdge &a, const AdjacentEdge &b);

class TrafficMap {
 private:
 public:
  std::string station_int_to_str_[MAX];
  std::map<std::string, int> station_str_to_int_;
  std::vector<AdjacentEdge> graph_[MAX];

  bool GenerateMapByFile(std::string name);
};

#endif