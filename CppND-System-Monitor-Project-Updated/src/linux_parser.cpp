#include <curses.h>
#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  filestream.close();
  return value;
}

string LinuxParser::Kernel() {
  string os;
  string version;
  string kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  stream.close();
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  fs::path proc_dir(kProcDirectory);
  for (auto& p : fs::directory_iterator(proc_dir)) {
    auto filename = p.path().filename().string();
    if (std::all_of(filename.begin(), filename.end(), isdigit)) {
      int pid = stoi(filename);
      pids.push_back(pid);
    }
  }
  return pids;
}

float LinuxParser::MemoryUtilization() {
  string key, unit;
  int value;
  string line;
  int mem_total, mem_free;
  std::ifstream stream(kProcDirectory + LinuxParser::kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value >> unit) {
        if (key == "MemTotal") {
          mem_total = value;
        }
        if (key == "MemFree") {
          mem_free = value;
        }
      }
    }
  }
  stream.close();

  float mem_used = mem_total - mem_free;
  return mem_used / mem_total;
}

long LinuxParser::UpTime() {
  float uptime, idle;
  string line;
  std::ifstream stream(kProcDirectory + LinuxParser::kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idle;
  }
  stream.close();
  return (long)uptime;
}

long LinuxParser::Jiffies() { return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies(); }

long LinuxParser::ActiveJiffies(int pid) {
  // things
  string value;
  std::vector<string> values;
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + LinuxParser::kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) values.push_back(value);
    auto u_time = stol(values.at(kUTime_));
    auto s_time = stol(values.at(kSTime_));
    auto cu_time = stol(values.at(kCUTime_));
    auto cs_time = stol(values.at(kCSTime_));
    // https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
    return u_time + s_time + cu_time + cs_time;
  }
  stream.close();

  return 0;
}

long LinuxParser::ActiveJiffies() {
  auto util = LinuxParser::CpuUtilization();
  auto user = std::stol(util.at(LinuxParser::CPUStates::kUser_));
  auto nice = std::stol(util.at(LinuxParser::CPUStates::kNice_));
  auto system = std::stol(util.at(LinuxParser::CPUStates::kSystem_));
  auto irq = std::stol(util.at(LinuxParser::CPUStates::kIRQ_));
  auto softirq = std::stol(util.at(LinuxParser::CPUStates::kSoftIRQ_));
  auto steal = std::stol(util.at(LinuxParser::CPUStates::kSteal_));
  return user + nice + system + irq + softirq + steal;
}

long LinuxParser::IdleJiffies() {
  auto util = LinuxParser::CpuUtilization();
  auto idle = stol(util.at(LinuxParser::CPUStates::kIdle_));
  auto iowait = stol(util.at(LinuxParser::CPUStates::kIOwait_));
  return idle + iowait;
}

vector<string> LinuxParser::CpuUtilization() {
  std::vector<string> util;
  string line, val;
  std::ifstream stream(kProcDirectory + LinuxParser::kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> val) util.push_back(val);
  }
  stream.close();

  util.erase(util.begin());  // pop string "cpu"
  return util;
}

int LinuxParser::TotalProcesses() {
  string key;
  int value;
  string line;
  std::ifstream stream(kProcDirectory + LinuxParser::kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      }
    }
  }
  stream.close();
  return 0;
}

int LinuxParser::RunningProcesses() {
  string key;
  int value;
  string line;
  std::ifstream stream(kProcDirectory + LinuxParser::kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      }
    }
  }
  stream.close();

  return 0;
}

string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + LinuxParser::kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    return line;
  }
  return string("cmd not found");
}
string LinuxParser::Ram(int pid) {
  string key;
  string value;
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + LinuxParser::kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        return value;
      }
    }
  }
  stream.close();

  return std::string("0");
}

std::string LinuxParser::Uid(int pid) {
  string key;
  int value;
  int uid;
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + LinuxParser::kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid") {
        uid = value;
        break;
      }
    }
  }
  stream.close();

  return std::to_string(uid);
}

string LinuxParser::User(int pid) {
  string uname, line;
  int uid = stoi(LinuxParser::Uid(pid));
  int id;
  std::ifstream stream(LinuxParser::kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> uname >> id;
      if (id == uid) {
        return uname;
      }
    }
  }
  stream.close();

  return std::string("not found");
}

long LinuxParser::UpTime(int pid) {
  string value;
  std::vector<string> values;
  int uptime{0};
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + LinuxParser::kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) values.push_back(value);
    uptime = std::stol(values.at(kStartTime_));
    uptime /= sysconf(_SC_CLK_TCK);
    uptime = LinuxParser::UpTime() - uptime;
  }
  stream.close();
  return uptime;
}
