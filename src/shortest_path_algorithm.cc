#include "shortest_path_algorithm.h"

#include <iostream>
using std::cout;
using std::find;
using std::string;
using std::vector;

ResultPath::ResultPath() {
  time = 0;
  transfer_times = 0;
}

void ResultPath::Copy(const ResultPath &a) {
  for (auto i : a.path) {
    path.push_back(i);
  }
  for (auto i : a.lines) {
    lines.push_back(i);
  }
  time = a.time;
  transfer_times = a.transfer_times;
}

bool operator==(const ResultPath &a, const ResultPath &b) {
  if (a.lines == b.lines && a.path == b.path)
    return true;
  else
    return false;
}

void ShortestPathAlgorithm::RecursiveAnswer(
    TrafficMap &graph, vector<ResultPath> &answer, string start, string end,
    vector<AdjacentEdge> *precursors) {  //将合适的answer后push入end与其前驱
  if (end == start) return;
  vector<ResultPath> temp;
  for (auto i : answer) {
    if (i.path.back() == end) {
      temp.push_back(i);
    }
  }
  for (auto i : precursors[graph.station_str_to_int_[end]]) {
    if (temp.size() == 0) {
      ResultPath buf;
      buf.path.push_back(end);
      buf.path.push_back(graph.station_int_to_str_[i.vertx]);
      buf.lines.push_back(i.line);
      buf.time += i.weight;
      answer.push_back(buf);
      RecursiveAnswer(graph, answer, start, graph.station_int_to_str_[i.vertx],
                      precursors);
      continue;
    }
    for (auto j : temp) {
      ResultPath buf = j;
      int line1 = buf.lines.back();
      int line2 = i.line;
      if (line1 != line2) {
        buf.transfer_times += 1;
        buf.lines.push_back(i.line);
      } else {
        buf.path.pop_back();
      }
      buf.path.push_back(graph.station_int_to_str_[i.vertx]);
      buf.time += i.weight;
      answer.push_back(buf);
    }
    RecursiveAnswer(graph, answer, start, graph.station_int_to_str_[i.vertx],
                    precursors);
  }
  for (auto i : temp) {
    vector<ResultPath>::iterator temp = find(answer.begin(), answer.end(), i);
    if (temp != answer.end()) answer.erase(temp);
  }
}