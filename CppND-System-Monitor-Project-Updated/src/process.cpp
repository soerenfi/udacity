#include <unistd.h>
#include <cctype>
#include <exception>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return pid_; }

void Process::CalcCpuUtilization() {
  auto active = LinuxParser::ActiveJiffies(pid_);
  auto uptime = LinuxParser::UpTime();              // uptime of the system (seconds)
  auto uptime_process = LinuxParser::UpTime(pid_);  // uptime of process in seconds

  long d_active = active - active_last_;
  long d_uptime_process = uptime_process - uptime_process_last_;

  active_last_ = active;
  uptime_last_ = uptime;
  uptime_process_last_ = uptime_process;

  cpu_utilization_ = (double)d_active / sysconf(_SC_CLK_TCK) / d_uptime_process;
}
double Process::CpuUtilization() const { return cpu_utilization_; }

string Process::Command() { return LinuxParser::Command(pid_); }

string Process::Ram() {
  auto ram = stol(LinuxParser::Ram(pid_));
  return std::to_string(ram / 1000);
}

string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process& other) { return this->CpuUtilization() > other.CpuUtilization(); }