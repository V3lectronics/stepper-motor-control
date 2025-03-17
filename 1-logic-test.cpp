#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <cmath>
#include <wiringPi.h>

using namespace std::chrono;
using namespace std::this_thread;
using namespace std;
const string config_path = "motor.conf";

//PIN DEFINITIONS
// !!! CAUTION !!!
// If the program is interrupted or suspended and the
// pins are in HIGH state the engine coils may overheat.
//
//naming convention:
//hm → horizontal motor
//vm → vertical motor
//
//	    wiringPi physical
//	        ↓    ↓
const int hm1 = 29; //40
const int hm2 = 28; //38
const int hm3 = 27; //36
const int hm4 = 26; //32
//--------------- ;
const int vm1 = 25; //37
const int vm2 = 24; //35
const int vm3 = 23; //33
const int vm4 = 22; //31
//(remember to connect GND);

const int step_delay = 10; //step delay at full speed
const double ramp_amount = 20; //acceleration
const double ramp_percent = 0.20; //defines at what point of the command we finish
							 //and start acceleration.

void ramp(double progress){
		//cout<<progress<<endl;
		int round_step_delay;

		if(progress < ramp_percent){
			round_step_delay = floorf(step_delay - progress * ramp_amount);
			sleep_for(milliseconds(round_step_delay));
			cout << "0-20%" << round_step_delay << endl;

		} else if(progress > 1-ramp_percent){
			round_step_delay = floorf(step_delay + progress * ramp_amount);
			sleep_for(milliseconds(round_step_delay));
			cout << "80-100%" << round_step_delay << endl;
		}

		else{
			round_step_delay = floorf(step_delay - ramp_amount);
			sleep_for(milliseconds(step_delay));
			cout <<"20%-80%" << round_step_delay << endl;
		}
}

//takes a command and moves the engines
//returns 0 if executed succefully  
int run_command(string command, string arg){

	//initialize the pins
	pinMode(hm1, OUTPUT);
	pinMode(hm2, OUTPUT);
	pinMode(hm3, OUTPUT);
	pinMode(hm4, OUTPUT);
	pinMode(vm1, OUTPUT);
	pinMode(vm2, OUTPUT);
	pinMode(vm3, OUTPUT);
	pinMode(vm4, OUTPUT);
	digitalWrite(hm1, LOW);
	digitalWrite(hm2, LOW);
	digitalWrite(hm3, LOW);
	digitalWrite(hm4, LOW);
	digitalWrite(vm1, LOW);
	digitalWrite(vm2, LOW);
	digitalWrite(vm3, LOW);
	digitalWrite(vm4, LOW);


	int intarg = stoi(arg);

	cout<<"executing: "<<command<<" "<<intarg<<endl;

	if(command == "sleep"){
		sleep_for(milliseconds(intarg));
	}

	int clock_sequence[][4] = {
    {1, 0, 0, 1},
    {0, 0, 1, 1},
    {0, 1, 1, 0},
    {1, 1, 0, 0},
};

int counter_clock_sequence[][4] = {
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 1},
    {1, 0, 0, 1},
};


if (command == "up") {
	int phase = 0;

	double progress = 0; // 0-1 value representing command execution progress
	for (int i = 0; i < intarg; i++) {
		progress = double(i)/double(intarg);
		digitalWrite(hm1, clock_sequence[phase][0]);
		digitalWrite(hm2, clock_sequence[phase][1]);
		digitalWrite(hm3, clock_sequence[phase][2]);
		digitalWrite(hm4, clock_sequence[phase][3]);
		phase = (phase + 1) % 4; 

		ramp(progress);

	}
}

if (command == "down") {
    int phase = 0;
    for (int i = 0; i < intarg; i++) {
        digitalWrite(hm1, counter_clock_sequence[phase][0]);
        digitalWrite(hm2, counter_clock_sequence[phase][1]);
        digitalWrite(hm3, counter_clock_sequence[phase][2]);
        digitalWrite(hm4, counter_clock_sequence[phase][3]);
        phase = (phase + 1) % 4; 
        sleep_for(milliseconds(step_delay));
    }
}

if(command == "right"){
	int phase = 0;
	for (int i = 0; i < intarg; i++) {
		digitalWrite(vm1, clock_sequence[phase][0]);
		digitalWrite(vm2, clock_sequence[phase][1]);
		digitalWrite(vm3, clock_sequence[phase][2]);
		digitalWrite(vm4, clock_sequence[phase][3]);
		phase = (phase + 1) % 4; 
		sleep_for(milliseconds(step_delay));
	}
}

if(command == "left"){
	int phase = 0;
	for (int i = 0; i < intarg; i++) {
		digitalWrite(vm1, counter_clock_sequence[phase][0]);
		digitalWrite(vm2, counter_clock_sequence[phase][1]);
		digitalWrite(vm3, counter_clock_sequence[phase][2]);
		digitalWrite(vm4, counter_clock_sequence[phase][3]);
		phase = (phase + 1) % 4;
		sleep_for(milliseconds(step_delay));
	}
}

	//cleanup to stop heating up coils
	digitalWrite(hm1, LOW);
	digitalWrite(hm2, LOW);
	digitalWrite(hm3, LOW);
	digitalWrite(hm4, LOW);
	digitalWrite(vm1, LOW);
	digitalWrite(vm2, LOW);
	digitalWrite(vm3, LOW);
	digitalWrite(vm4, LOW);
	
	return 0;
}

int execute_command_list(string commands_file_dir){
	fstream commands_file(commands_file_dir);
	if (not commands_file.is_open()) {
        cerr << "ERROR couldn't open "<<config_path<<endl;
        return 1;
    }

	if(step_delay < 0){
		cerr<<"ERROR step delay has to be a positive integer"<<endl;
		return 1;
	}
	if(step_delay < 5){
		cout<<"WARNING low step delay, this may cause unexpected behavior"<<endl;
	}

	string command, arg;
	while((commands_file>>command)and(commands_file>>arg)){
		run_command(command, arg);
	}

	commands_file.close();
	return 0;
}

int main(){
	cout<<"using config from: "<<config_path<<endl;	
	fstream config_file(config_path);

	if (not config_file.is_open()) {
        cerr << "ERROR couldn't open "<<config_path<<endl;
        return 1;
    	}

	if (wiringPiSetup() == -1){
		cerr << "ERROR wiringPi setup failed" <<endl;
		return 1;
	}

	string commands_file_dir, engine_1_name, engine_2_name;
	string engine_1_dpfs, engine_2_dpfs;	
	config_file >> commands_file_dir;
	config_file >> commands_file_dir;

	config_file >> engine_1_name;
	config_file >> engine_1_name;

	config_file >> engine_1_dpfs;
	config_file >> engine_1_dpfs;

	config_file >> engine_2_name;
	config_file >> engine_2_name;

	config_file >> engine_2_dpfs;
	config_file >> engine_2_dpfs;



	cout<<"----------------"<<endl;	
	cout<<"running commands from: "<<commands_file_dir<<endl;	
	execute_command_list(commands_file_dir);

	

		

	config_file.close();
	return 0;
}
