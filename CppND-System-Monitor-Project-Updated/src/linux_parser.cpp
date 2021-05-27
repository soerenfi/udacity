#include <curses.h>
#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization

// TODO: improve?
float LinuxParser::MemoryUtilization() {
  string key, unit;
  int value;
  string line;
  int mem_total, mem_free, mem_available, buffers;
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
        if (key == "MemAvailable") {
          mem_available = value;
        }
        if (key == "Buffers") {
          buffers = value;
        }
      }
    }
  }
  float mem_used = mem_total - mem_free;
  return mem_used / mem_total;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  float uptime, idle;
  string line;
  std::ifstream stream(kProcDirectory + LinuxParser::kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idle;
  }
  return (long)uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies(); }

// TODO: Read and return the number of active jiffies for a PID
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
    return u_time + s_time;  // + cu_time + cs_time;
  }
  return 0;
}

// TODO: Read and return the number of active jiffies for the system
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

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  auto util = LinuxParser::CpuUtilization();
  auto idle = stol(util.at(LinuxParser::CPUStates::kIdle_));
  auto iowait = stol(util.at(LinuxParser::CPUStates::kIOwait_));
  return idle + iowait;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  std::vector<string> util;
  string line, val;
  std::ifstream stream(kProcDirectory + LinuxParser::kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> val) util.push_back(val);
  }
  util.erase(util.begin());  // pop string "cpu"
  return util;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string key;
  int value;
  string line;
  int processes, procs_running;
  std::ifstream stream(kProcDirectory + LinuxParser::kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          processes = value;
        }
        if (key == "procs_running") {
          procs_running = value;
        }
      }
    }
  }
  return processes;
}

// TODO: do not repeat yourself
// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string key;
  int value;
  string line;
  int processes, procs_running;
  std::ifstream stream(kProcDirectory + LinuxParser::kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          processes = value;
        }
        if (key == "procs_running") {
          procs_running = value;
        }
      }
    }
  }
  return procs_running;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + LinuxParser::kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    return line;
  }
  return string("cmd not found");
}
// TODO: Read and return the memory used by a process
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
  return std::string("0");
}

// TODO: Read and return the user ID associated with a process
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
  return std::to_string(uid);
}

// TODO: Read and return the user associated with a process
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
  return std::string("not found");
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
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
  return uptime;
}
