#include <iostream>
#include <string>
#include "linux_parser.h"
#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
  // PrevIdle = previdle + previowait
  // Idle = idle + iowait

  // PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq +
  // prevsteal
  // NonIdle = user + nice + system + irq + softirq + steal

  // PrevTotal = PrevIdle + PrevNonIdle
  // Total = Idle + NonIdle

  // # differentiate: actual value minus the previous one
  // totald = Total - PrevTotal
  // idled = Idle - PrevIdle

  // // CPU_Percentage = (totald - idled)/totald

  long idle = LinuxParser::IdleJiffies();
  long active = LinuxParser::ActiveJiffies();
  long total = idle + active;

  long d_idle = idle - idle_last_;
  long d_active = active - active_last_;
  long d_total = total - total_last_;

  // store current state as last
  total_last_ = total;
  idle_last_ = idle;
  active_last_ = active;

  return (float)(d_total - d_idle) / d_total;
}