#ifndef RAMP_H
#define RAMP_H

#include <cmath>
#include <chrono>

//extern meaning defined in another file (ramp.cpp)
extern const int base_delay; //step delay at full speed, lowest possible delay.
extern const double ramp_amount; //amount of acceleration variation ???
extern const double ramp_percent; //defines at what point of the command we finish
						 //and start acceleration.
extern int round_step_delay; // integer that will hold the extra delay needed

void ramp(double progress);

#endif

