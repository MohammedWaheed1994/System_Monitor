#include "format.h"

#include <string>

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  long Hours{0};
  long Mins{0};
  long Seconds{0};

  string Hours_String;
  string Mins_String;
  string Seconds_String;

  Hours = seconds / 3600;
  Mins = (seconds % 3600) / 60;
  Seconds = seconds % 60;

  Hours_String = to_string(Hours);
  Mins_String = to_string(Mins);
  Seconds_String = to_string(Seconds);

  if(Hours_String.size() < 2) Hours_String = "0" + Hours_String;
  if(Mins_String.size() < 2) Mins_String = "0" + Mins_String;
  if(Seconds_String.size() < 2) Seconds_String = "0" + Seconds_String;


  return Hours_String+ ":" + Mins_String + ":" + Seconds_String;
}