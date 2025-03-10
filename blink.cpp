#include <wiringPi.h>
#include <iostream>
#include <chrono>
#include <thread>

const int LED_PIN = 26;

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

int main()
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

