#include <ctime>
#include <iomanip>

#include <sstream>
#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) {
  std::tm timestamp{};
  timestamp.tm_hour = seconds / 3600;
  timestamp.tm_min = seconds / 60 - (timestamp.tm_hour * 60);
  timestamp.tm_sec = seconds - (timestamp.tm_hour * 3600) - (timestamp.tm_min * 60);
  std::stringstream ss;
  ss << std::put_time(&timestamp, "%H:%M:%S");
  return ss.str();
}