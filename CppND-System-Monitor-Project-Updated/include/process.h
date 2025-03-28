#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid) : pid_(pid) { CalcCpuUtilization(); };
  int Pid();                      // TODO: See src/process.cpp
  std::string User();             // TODO: See src/process.cpp
  std::string Command();          // TODO: See src/process.cpp
  double CpuUtilization() const;  // TODO: See src/process.cpp
  void CalcCpuUtilization();      // TODO: See src/process.cpp
  std::string Ram();              // TODO: See src/process.cpp
  long int UpTime();              // TODO: See src/process.cpp
  bool operator<(Process& a);     // TODO: See src/process.cpp

  // TODO: Declare any necessary private members

 private:
  int pid_{-1};
  long active_last_{0};
  long uptime_last_{0};
  long uptime_process_last_{0};
  double cpu_utilization_{0};
};

#endif