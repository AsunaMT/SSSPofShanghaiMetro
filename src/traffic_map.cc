#include "traffic_map.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::ifstream;
using std::ios;
using std::istringstream;
using std::ostream;
using std::pair;
using std::string;
using std::vector;

bool operator==(const AdjacentEdge &a, const AdjacentEdge &b) {
  if (a.line == b.line && a.vertx == b.vertx && a.weight == b.weight)
    return true;
  else
    return false;
}

bool TrafficMap::GenerateMapByFile(string name) {
  ifstream inFile(name, ios::in);
  if (!inFile) {
    cout << "Fail!" << endl;
    return false;
  }
  string temp1;
  int n;
  getline(inFile, temp1);
  n = atoi(temp1.c_str());
  for (int i = 0; i < n; i++) {
    string temp2;
    int sta, line;
    getline(inFile, temp2, ' ');
    line = atoi(temp2.c_str());
    getline(inFile, temp2, '\n');
    sta = atoi(temp2.c_str());
    vector<string> stations;
    vector<string> time;
    for (int j = 0; j < sta; j++) {
      string line;
      getline(inFile, line);
      line += "\n";
      istringstream sin(line);
      string station;
      getline(sin, station, '\t');
      stations.push_back(station);
      string ti;
      getline(sin, ti, '\n');
      time.push_back(ti);
    }
    if (station_str_to_int_.find(stations[0]) == station_str_to_int_.end()) {
      station_str_to_int_.insert(
          pair<string, int>(stations[0], station_str_to_int_.size()));
      station_int_to_str_[station_str_to_int_.size() - 1] = stations[0];
    }
    for (int j = 0; j < sta - 1; j++) {
      string sta1 = stations[j];
      int m = j + 1;
      string sta2 = stations[m];
      string t1 = time[j];
      string t2 = time[m];
      int dt = atoi(t2.substr(3, 2).c_str()) - atoi(t1.substr(3, 2).c_str());
      int dtime = dt > 0 ? dt : dt + 60;
      if (station_str_to_int_.find(sta2) == station_str_to_int_.end()) {
        station_str_to_int_.insert(
            pair<string, int>(sta2, station_str_to_int_.size()));
        station_int_to_str_[station_str_to_int_.size() - 1] = sta2;
      }
      AdjacentEdge adja1, adja2;
      adja1.vertx = station_str_to_int_[sta2];
      adja1.line = line;
      adja1.weight = dtime;
      adja2.vertx = station_str_to_int_[sta1];
      adja2.line = line;
      adja2.weight = dtime;
      graph_[adja2.vertx].push_back(adja1);
      graph_[adja1.vertx].push_back(adja2);
    }
  }
  inFile.close();
  return true;
}