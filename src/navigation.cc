#include "navigation.h"

#include <cstdlib>
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
using std::string;
using std::to_string;
using std::vector;

Navigation::Navigation() {
  two_stations_out_ = TRANSFER_TIMES_MIN;
  mulitple_stations_out_ = TRANSFER_TIMES_MIN;
  cout << "请选择最短路算法:\n"
       << "Bellam-Ford算法---请输入1\n"
       << "Dijkstra算法:---请输入2\n"
       << "P.S.非法输入将会默认使用通常性能更好的Dijkstra算法!\n";
  int temp;
  cin >> temp;
  if (temp == 1)
    algo_ = new BellmanFord;
  else
    algo_ = new Dijkstra;
}

Navigation::~Navigation() { delete algo_; }

void Navigation::WelcomeScreen() {
  cout << "********************************" << endl;
  cout << "********************************" << endl;
  cout << "**         上海地铁导航       **" << endl;
  cout << "********************************" << endl;
  cout << "**  Plaease input your order  **" << endl;
  cout << "********************************" << endl;
  cout << "**                            **" << endl;
  cout << "**        #1 添加地图文件     **" << endl;
  cout << "**        #2 文件输入模式     **" << endl;
  cout << "**        #3 自由输入模式     **" << endl;
  cout << "**        #4 输出设置         **" << endl;
  cout << "**        #5 退出导航         **" << endl;
  cout << "**                            **" << endl;
  cout << "**                            **" << endl;
  cout << "********************************" << endl;
  cout << "********************************" << endl;
  cout << "          #";
}

bool Navigation::AddMaPFile() {
  cout << "添加地图文件---请输入1\n"
       << "P.S.否则将使用系统默认地图!\n";
  string order, name;
  cin >> order;
  if (1) {
    static BellmanFord a;
    algo_ = &a;
  }
  if (order == "1") {
    cout << "请输入文件名:\n";
    cin >> name;
    return map_.GenerateMapByFile(name);
  } else
    return map_.GenerateMapByFile(MAP_NAME);
}

void Navigation::FileInputMode() {
  cout << "选择新的输入文件---请输入1\n"
       << "P.S.否则将使用默认测试文件!\n";
  string order, name;
  cin >> order;
  if (order == "1") {
    cout << "请输入文件名:\n";
    cin >> name;
  } else {
    name = TEST_INPUT_NAME;
  }
  HandleFile(name);
}

void Navigation::HandleFile(string name) {
  ifstream inFile(name, ios::in);
  if (!inFile) {
    cout << "Fail!" << endl;
    return;
  }
  string line;
  while (getline(inFile, line)) {
    line += "\t\n";
    istringstream sin(line);
    vector<string> stations;
    for (int i = 0; line[i] != '\n';) {
      string temp;
      getline(sin, temp, '\t');
      if (i == 0 || temp != stations.back()) stations.push_back(temp);
      i += (temp.size() + 1);
    }
    HandleStations(stations);
  }
  inFile.close();
}

void Navigation::ManualInputMode() {
  string temp;
  vector<string> stations;
  cout << "请输入出发站点:\n";
  cin >> temp;
  while (map_.station_str_to_int_.find(temp) ==
         map_.station_str_to_int_.end()) {
    cout << "该站点不存在，请重新输入:\n";
    cin >> temp;
  }
  stations.push_back(temp);
  while (1) {
    cout << "请输入下一个想要到达的站点(结束输入请输入0):\n";
    cin >> temp;
    if (temp == "0") break;
    while (map_.station_str_to_int_.find(temp) ==
           map_.station_str_to_int_.end()) {
      cout << "该站点不存在，请重新输入:\n";
      cin >> temp;
    }
    while (temp == stations.back()) {
      cout << "该站点与前一个站点相同,您不需要额外乘车到达，请重新输入:\n";
      cin >> temp;
    }
    stations.push_back(temp);
  }
  if (stations.size() < 2) {
    cout << "您至少需要输入两个站点才可以得到路线。\n";
    return;
  }
  HandleStations(stations);
}

void Navigation::HandleStations(vector<string> stations) {
  // cout << "请输入输出文件名:\n";
  // string name;
  // cin >> name;
  vector<ResultPath> answer;
  if (stations.size() == 2) {
    algo_->FindShortestPath(map_, stations[0], stations[1], answer);
    switch (two_stations_out_) {
      case TRANSFER_TIMES_MIN:
        Output(answer[GetTransferTimesMinPath(answer)]);
        break;
      case ALL_PATHS:
        for (auto i : answer) {
          Output(i);
        }
        break;
      default:
        break;
    }
  } else {
    vector<vector<ResultPath>> answers;
    for (int i = 0; i < stations.size() - 1; i++) {
      vector<ResultPath> ans;
      algo_->FindShortestPath(map_, stations[i], stations[i + 1], ans);
      answers.push_back(ans);
    }
    ResultPath res;
    switch (mulitple_stations_out_) {
      case TRANSFER_TIMES_MIN:
        res.Copy(answers[0][GetTransferTimesMinPath(answers[0])]);
        for (int i = 1; i < answers.size(); i++) {
          res = MergePath(res, answers[i][GetTransferTimesMinPath(answers[i])]);
        }
        Output(res);
        break;
      case ALL_PATHS:
        GetAllPaths(answers, answer);
        for (auto i : answer) {
          Output(i);
        }
        break;
      case SEGMENTED:
        for (int i = 0; i < answers.size(); i++) {
          for (auto j : answers[i]) {
            Output(j);
          }
        }
        break;
      default:
        break;
    }
  }
}

string Navigation::Output(ResultPath& result) {
  string out;
  out += result.path.back();
  for (int i = result.lines.size() - 1; i >= 0; i--) {
    out = out + "-Line" + to_string(result.lines[i]) + "-" + result.path[i];
  }
  out = out + "\n预计所需时间: " + to_string(result.time) +
        " 分钟\n预计换乘数: " + to_string(result.transfer_times) + " 次\n";
  cout << out;
  return out;
}

void Navigation::OutputSetting() {
  cout << "请选择只输入起点终点(即无中间站点)时的输出方式:\n"
       << "1-----输出一条换乘次数最少的路线\n"
       << "2-----输出所有路线\n"
       << "P.S.无效输入将会默认选择1\n";
  int a, b;
  cin >> a;
  switch (a) {
    case 1:
    case 2:
      two_stations_out_ = a;
      break;
    default:
      two_stations_out_ = 1;
      break;
  }
  cout << "请选择有中间站点输入时的输出方式:\n"
       << "1-----输出一条换乘次数最少的路线\n"
       << "2-----输出所有路线\n"
       << "3-----分段输出，每段与只输入起点终点(即无中间站点)"
          "时的输出方式相同\n"
       << "P.S.无效输入将会默认选择1\n";
  ;
  cin >> b;
  switch (b) {
    case 1:
    case 2:
    case 3:
      mulitple_stations_out_ = b;
      break;
    default:
      mulitple_stations_out_ = 1;
      break;
  }
}

int Navigation::GetTransferTimesMinPath(vector<ResultPath>& answer) {
  int min = 0;
  for (int i = 0; i < answer.size(); i++) {
    if (answer[i].transfer_times < answer[min].transfer_times) {
      min = i;
    }
  }
  return min;
}

ResultPath Navigation::MergePath(ResultPath a, ResultPath b) {
  ResultPath out;
  // if (a.path.size() > 0)
  b.path.pop_back();
  out.path.insert(out.path.end(), b.path.begin(), b.path.end());
  out.path.insert(out.path.end(), a.path.begin(), a.path.end());
  out.lines.insert(out.lines.end(), b.lines.begin(), b.lines.end());
  out.lines.insert(out.lines.end(), a.lines.begin(), a.lines.end());
  out.time = a.time + b.time;
  out.transfer_times = a.transfer_times + b.transfer_times;
  if (a.path.size() > 0 && a.lines[0] != b.lines.back())
    out.transfer_times += 1;
  return out;
}

void Navigation::GetAllPaths(vector<vector<ResultPath>>& answers,
                             vector<ResultPath>& out) {
  for (auto i : answers[0]) {
    out.push_back(i);
  }
  for (int i = 1; i < answers.size() - 1; i++) {
    vector<ResultPath> buf;
    for (auto j : out) {
      for (auto k : answers[i + 1]) {
        buf.push_back(MergePath(j, k));
      }
    }
    out.clear();
    out = buf;
  }
}

void Navigation::RunNavigation() {
  string order;
  for (bool startif = true; startif;) {
    fflush(stdin);
    WelcomeScreen();
    for (bool buf = true; buf;) {
      cin >> order;
      if (order.size() > 1) {
        cout << "Invalid order, please enter again!" << endl;
        cout << "          #";
        continue;
      }
      buf = false;
    }
    switch (order[0]) {
      case '1':
        AddMaPFile();
        break;
      case '2':
        FileInputMode();
        getchar();
        break;
      case '3':
        ManualInputMode();
        getchar();
        getchar();
        break;
      case '4':
        OutputSetting();
        break;
      case '5':
        startif = false;
        break;
      default:
        cout << "Invalid order, please enter again!" << endl;
        break;
    }
  }
}