#ifndef PROJRCT2_NAVIGATION_H
#define PROJRCT2_NAVIGATION_H

#include "Bellman-Ford.h"
#include "Dijkstra.h"
#include "traffic_map.h"

const std::string MAP_NAME = "ds-pj2/subwaylinesofshanghai.txt";
const std::string TEST_INPUT_NAME = "ds-pj2/performance-benchmark.txt";
const int TRANSFER_TIMES_MIN = 1;
const int ALL_PATHS = 2;
const int SEGMENTED = 3;

class Navigation {
 private:
  int two_stations_out_;
  int mulitple_stations_out_;
  TrafficMap map_;
  ShortestPathAlgorithm* algo_;
  void WelcomeScreen();
  bool AddMaPFile();
  void FileInputMode();
  void HandleFile(std::string name);
  void ManualInputMode();
  void HandleStations(std::vector<std::string> stations);
  int GetTransferTimesMinPath(std::vector<ResultPath>& answer);
  std::string Output(ResultPath& result);
  void OutputSetting();
  ResultPath MergePath(ResultPath a, ResultPath b);
  void GetAllPaths(std::vector<std::vector<ResultPath>>& answers,
                   std::vector<ResultPath>& out);

 public:
  Navigation();
  ~Navigation();
  void RunNavigation();
};

#endif