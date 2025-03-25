#include <cmath>
#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

// NOTE: I KNOW IT WOULD BE BETTER TO NO USE GLOBALS
// it would be better to read them from config file

int base_delay = 3; //step delay at full speed, lowest possible delay.
const double ramp_amount = 15; //amount of acceleration variation ???
const double ramp_percent = 0.20; //defines at what point of the command we finish
int round_step_delay = 0; // integer that will hold the extra delay needed

void ramp(double progress){
	//cout<<progress<<endl;

	//if at beginning of the movement, start at high delay and
	//gradually decrease delay
	if(progress < ramp_percent){
		round_step_delay = floorf(base_delay+ramp_amount - ((progress)/ramp_percent)*ramp_amount);
		if (round_step_delay < base_delay) round_step_delay = base_delay;
		sleep_for(milliseconds(round_step_delay));
		/*cout << "speed up: " << round_step_delay << endl;*/
		/*cout << "speed up prog: " << progress << endl;*/

		// if in the middle keep the delay at step_delay, full speed ahead!
	} else if(progress >= ramp_percent && progress <= 1-ramp_percent){
		sleep_for(milliseconds(base_delay));
		/*cout << "full speed: " << base_delay << endl;*/
	}

	//if at the end of the movement, gradually add delay until stop
	else{
		round_step_delay = floorf(base_delay+ramp_amount - ((1-progress)/ramp_percent)*ramp_amount);
		sleep_for(milliseconds(round_step_delay));
		/*cout <<"slow down: " << round_step_delay << endl;*/
	}
}


