#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  auto active = LinuxParser::ActiveJiffies(pid_);
  auto uptime = LinuxParser::UpTime();              // uptime of the system (seconds)
  auto uptime_process = LinuxParser::UpTime(pid_);  // uptime of process in seconds
  // auto uptime_process = uptime - starttime_process;

  long d_active = active - active_last_;
  long d_uptime = uptime - uptime_last_;
  long d_uptime_process = uptime_process - uptime_process_last_;

  active_last_ = active;
  uptime_last_ = uptime;
  uptime_process_last_ = uptime_process;

  return (float)active / sysconf(_SC_CLK_TCK) / uptime_process;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::Uid(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process& other) { return this->CpuUtilization() > other.CpuUtilization(); }