//this is just a test
#include <wiringPi.h>
#include <iostream>
#include <chrono>
#include <thread>

const int LED_PIN = 29;

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

int main(int argc, char * argv[])
{
	if (wiringPiSetup() == -1){
		cerr << "wiring pi failed" <<endl;
		return 1;
		}

	pinMode(LED_PIN, OUTPUT);

	while(true){
		digitalWrite(LED_PIN, HIGH);
		sleep_for(milliseconds(500));
		digitalWrite(LED_PIN, LOW);
		sleep_for(milliseconds(500));
	}
	return 0;
}

