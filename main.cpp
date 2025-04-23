#include <cstdlib>
#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <thread>
#include <cmath>
#include <wiringPi.h>
#include <ctime>

//custom module
#include "linked-list.h"
#include "ramp.h"

using namespace std::chrono;
using namespace std::this_thread;
using namespace std;

bool history_list_initialized_flag = 0;
string history_file_dir;
string engine_1_dpfs, engine_2_dpfs; //dpfs → Degrees Per Full Step

Node* head;

/**
 * @brief This function saves all executed commands into a history file.
 * 
 * @param history_file_dir the diretcory of the histor file.
 * @param command the command that needs to be saved.
 * @return 0 if success, 1 if error.
 */
int save_history(string history_file_dir, string command, string arg1, string arg2){
	if (history_list_initialized_flag == 0){
		time_t timestamp = time(NULL);
		string ts = to_string(timestamp);
		Node* head = new Node(ts, "START");
		history_list_initialized_flag = 1;
	}

	time_t timestamp = time(NULL);
	string ts = to_string(timestamp);

	string command_arg = command+" "+arg1+" "+arg2;

	append(head, ts, command_arg);
	/*display_list(head);*/

	if (command == "end"){
		cout<<"Writing history to: "<<history_file_dir<<endl;	
		fstream history_file(history_file_dir);

		if (not history_file.is_open()) {
			cerr<<"ERROR couldn't open "<<history_file_dir<<endl;
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

// NOTE: COMMANDS
void up(int intarg){
	double progress = 0; // 0-1 value representing command execution progress
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
void down(int intarg){
	double progress = 0; // 0-1 value representing command execution progress
	int phase = 0;
		for (int i = 0; i < intarg; i++) {
			digitalWrite(hm1, counter_clock_sequence[phase][0]);
			digitalWrite(hm2, counter_clock_sequence[phase][1]);
			digitalWrite(hm3, counter_clock_sequence[phase][2]);
			digitalWrite(hm4, counter_clock_sequence[phase][3]);
			phase = (phase + 1) % 4; 

			progress = double(i)/double(intarg);
			ramp(progress);
		}
}
void move_right(int intarg){
	double progress = 0; // 0-1 value representing command execution progress
	int phase = 0;
		for (int i = 0; i < intarg; i++) {
			digitalWrite(vm1, clock_sequence[phase][0]);
			digitalWrite(vm2, clock_sequence[phase][1]);
			digitalWrite(vm3, clock_sequence[phase][2]);
			digitalWrite(vm4, clock_sequence[phase][3]);
			phase = (phase + 1) % 4; 

			progress = double(i)/double(intarg);
			ramp(progress);
		}
}
void move_left(int intarg){
	double progress = 0; // 0-1 value representing command execution progress
	int phase = 0;
		for (int i = 0; i < intarg; i++) {
			digitalWrite(vm1, counter_clock_sequence[phase][0]);
			digitalWrite(vm2, counter_clock_sequence[phase][1]);
			digitalWrite(vm3, counter_clock_sequence[phase][2]);
			digitalWrite(vm4, counter_clock_sequence[phase][3]);
			phase = (phase + 1) % 4;

			progress = double(i)/double(intarg);
			ramp(progress);
		}
}

bool check_char(string word, char c){
	for (int i=0; i<size(word); i++) {
		if (word[i] == c){
			return true;
		}
	}
	return false;
}

int deg_to_steps(int deg){
	int steps;
	// I am assuming here that both engines have the same dpfs despite declaring 2 vars.
	steps = abs(deg / stof(engine_1_dpfs))/2;

	return steps;
}

/**
 * @brief Runs commands and takes arguments to modify how they are executed.
 * 
 * @param command the command to be run e.g. "up"
 * @param arg1 the first argument
 * @param arg2 the second argument
 * @return 0 if success, 1 if error.
 */
int run_command(string command, string arg1, string arg2){

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


	int intarg1 = stoi(arg1);
	int intarg2 = stoi(arg2);

	cout<<"executing: "<<command<<" "<<intarg1<<" "<<intarg2<<endl;

	if(command == "sleep"){
		sleep_for(milliseconds(intarg1));
	}
	else if (command == "up" || (command == "G01_A" && intarg1 > 0)) {
		//if the command is a gcode, change the argument to represent full steps 
		//instead of degrees.
		if (check_char(command, 'G')){
			intarg1 = deg_to_steps(intarg1);
			cout<<"Interpreted Gcode as up"<<endl;
		}
		up(intarg1);
	}
	else if (command == "down" || (command == "G01_A" && intarg1 < 0)) {
		if (check_char(command, 'G')){
			intarg1 = deg_to_steps(intarg1);
			cout<<"Interpreted Gcode as down"<<endl;
		}
		down(intarg1);
	}
	else if(command == "right" || (command == "G01_B" && intarg1 > 0)){
		if (check_char(command, 'G')){
			intarg1 = deg_to_steps(intarg1);
			cout<<"Interpreted Gcode as right"<<endl;
		}
		move_right(intarg1);
	}
	else if(command == "left" || (command == "G01_B" && intarg1 < 0)){
		if (check_char(command, 'G')){
			intarg1 = deg_to_steps(intarg1);
			cout<<"Interpreted Gcode as left"<<endl;
		}
		move_left(intarg1);
	}
	else if(command == "up-left"){
		// TODO: checkout timers (hardware) and priorities

		thread thread1(up, intarg1);
		thread thread2(move_left, intarg2);

		thread1.join();
		thread2.join();
	}
	else if(command == "up-right"){
		thread thread1(up, intarg1);
		thread thread2(move_right, intarg2);

		thread1.join();
		thread2.join();
	}
	else if(command == "down-left"){
		thread thread1(down, intarg1);
		thread thread2(move_left, intarg2);

		thread1.join();
		thread2.join();
	}
	else if(command == "down-right"){
		thread thread1(down, intarg1);
		thread thread2(move_right, intarg2);

		thread1.join();
		thread2.join();
	}
	else if(command == "end"){
		cout<<"done"<<endl;
	}
	else{
		cerr<<"ERROR unknown command: "<<command<<endl;
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


	save_history(history_file_dir, command, to_string(intarg1), to_string(intarg2));
	
	return 0;
}

int execute_command_list(string commands_file_dir){
	fstream commands_file(commands_file_dir);
	if (not commands_file.is_open()) {
        cerr << "ERROR couldn't open "<<commands_file_dir<<endl;
        return 1;
    }

	if(base_delay < 0){
		cerr<<"ERROR step delay has to be a positive integer"<<endl;
		return 1;
	}
	if(base_delay < 3){
		cout<<"WARNING low step delay, this may cause unexpected behavior"<<endl;
	}

	string command, arg1, arg2;
	while((commands_file>>command)and(commands_file>>arg1)and(commands_file>>arg2)){
		run_command(command, arg1, arg2);
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
	/*string engine_1_dpfs, engine_2_dpfs; */ //this has been changed to a global variable

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
