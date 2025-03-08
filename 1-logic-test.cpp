#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <wiringPi.h>

using namespace std::chrono;
using namespace std::this_thread;
using namespace std;
const string config_path = "motor.conf";

//pin definitions
//hm = horiz. motor, vm vert. motor
const int hm1 = 29
const int hm2 = 29
const int hm3 = 29
const int hm4 = 29
const int vm1 = 29
const int vm2 = 29
const int vm3 = 29
const int vm4 = 29
//remember GND;


//takes a command and moves the engines
//returns 0 if executed succefully  
int run_command(string command, string arg){

	cout<<"executing: "<<command<<" "<<arg<<endl;

	if(command == "sleep"){
		sleep_for(milliseconds(stoi(arg)));
	}

	if(command == "up"){
	
	}

	if(command == "right"){
	
	}

	if(command == "down"){
	
	}

	if(command == "left"){
	
	}
	
	return 0;
}

int execute_command_list(string commands_file_dir){
	fstream commands_file(commands_file_dir);
	if (not commands_file.is_open()) {
        cerr << "Error opening "<<config_path<<endl;
        return 1;
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
        cerr << "Error opening "<<config_path<<endl;
        return 1;
    	}

	if (wiringPiSetup() == -1){
		cerr << "wiringPi setup failed" <<endl;
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
