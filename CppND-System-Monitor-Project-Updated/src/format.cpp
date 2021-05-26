#include "format.h"
#include <sstream>
#include <string>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int hh = seconds / 3600;
  int mm = seconds / 60 - (hh * 60);
  int ss = seconds - (hh * 3600) - (mm * 60);
  std::stringstream stream;
  stream << hh << ":" << mm << ":" << ss;
  return stream.str();
}