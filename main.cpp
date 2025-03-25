#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <cmath>
#include <wiringPi.h>
#include <ctime>

//custom module
#include "linked-list.h"

using namespace std::chrono;
using namespace std::this_thread;
using namespace std;

bool history_list_initialized_flag = 0;
string history_file_dir;

Node* head;

int save_history(string history_file_dir, string command, string arg){
	if (history_list_initialized_flag == 0){
		time_t timestamp = time(NULL);
		string ts = to_string(timestamp);
		Node* head = new Node(ts, "START");
		history_list_initialized_flag = 1;
	}

	time_t timestamp = time(NULL);
	string ts = to_string(timestamp);

	string command_arg = command+" "+arg;

	append(head, ts, command_arg);
	/*display_list(head);*/

	if (command == "end"){
		cout<<"Writing history to: "<<history_file_dir<<endl;	
		fstream history_file(history_file_dir);

		if (not history_file.is_open()) {
			cerr << "ERROR couldn't open "<<history_file_dir<<endl;
			return 1;
		}


		Node* current = head;
		while (current != nullptr) {
			history_file << current->timestamp << " " << current->command<< endl;
			current = current->next;
		}

		cout<<"History saved succesfully"<<endl;
		history_file.close();
	}
	return 0;
}

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

/*const int min_delay = 2; //minimal delay needed for engine to act predictably*/
const int base_delay = 3; //step delay at full speed, lowest possible delay.
const double ramp_amount = 15; //amount of acceleration variation ???
const double ramp_percent = 0.20; //defines at what point of the command we finish
						 //and start acceleration.
int round_step_delay; // integer that will hold the extra delay needed
				// for accelerration  & deceleration			

void ramp(double progress){
		//cout<<progress<<endl;

		//if at beginning of the movement, start at high delay and
		//gradually decrease delay
		if(progress < ramp_percent){
			round_step_delay = floorf(base_delay+ramp_amount - ((progress)/ramp_percent)*ramp_amount);
			if (round_step_delay < base_delay) round_step_delay = base_delay;
			sleep_for(milliseconds(round_step_delay));
			cout << "speed up: " << round_step_delay << endl;
			/*cout << "speed up prog: " << progress << endl;*/

		// if in the middle keep the delay at step_delay, full speed ahead!
		} else if(progress >= ramp_percent && progress <= 1-ramp_percent){
			sleep_for(milliseconds(base_delay));
			cout << "full speed: " << base_delay << endl;
		}

		//if at the end of the movement, gradually add delay until stop
		else{
			round_step_delay = floorf(base_delay+ramp_amount - ((1-progress)/ramp_percent)*ramp_amount);
			sleep_for(milliseconds(round_step_delay));
			cout <<"slow down: " << round_step_delay << endl;
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


	double progress = 0; // 0-1 value representing command execution progress
	if (command == "up") {
		int phase = 0;

		for (int i = 0; i < intarg; i++) {
			digitalWrite(hm1, clock_sequence[phase][0]);
			digitalWrite(hm2, clock_sequence[phase][1]);
			digitalWrite(hm3, clock_sequence[phase][2]);
			digitalWrite(hm4, clock_sequence[phase][3]);
			phase = (phase + 1) % 4; 

			progress = double(i)/double(intarg);
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
			/*sleep_for(milliseconds(base_delay));*/

			progress = double(i)/double(intarg);
			ramp(progress);
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
			/*sleep_for(milliseconds(base_delay));*/

			progress = double(i)/double(intarg);
			ramp(progress);
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
			/*sleep_for(milliseconds(base_delay));*/

			progress = double(i)/double(intarg);
			ramp(progress);
		}
	}

	//cleanup to stop coils from overheating / energy waste
	digitalWrite(hm1, LOW);
	digitalWrite(hm2, LOW);
	digitalWrite(hm3, LOW);
	digitalWrite(hm4, LOW);
	digitalWrite(vm1, LOW);
	digitalWrite(vm2, LOW);
	digitalWrite(vm3, LOW);
	digitalWrite(vm4, LOW);


	save_history(history_file_dir, command, to_string(intarg));
	
	return 0;
}

int execute_command_list(string commands_file_dir){
	fstream commands_file(commands_file_dir);
	if (not commands_file.is_open()) {
        cerr << "ERROR couldn't open "<<config_path<<endl;
        return 1;
    }

	if(base_delay < 0){
		cerr<<"ERROR step delay has to be a positive integer"<<endl;
		return 1;
	}
	if(base_delay < 5){
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

	config_file >> history_file_dir;
	config_file >> history_file_dir;

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
