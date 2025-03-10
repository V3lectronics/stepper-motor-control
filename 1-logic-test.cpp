#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <wiringPi.h>

using namespace std::chrono;
using namespace std::this_thread;
using namespace std;
const string config_path = "motor.conf";

//PIN DEFINITIONS
// !!! CAUTION !!!
// If the program is interrupted or suspended and the
// pins are in HIGH state the engine coils may overheat.
//hm = horiz. motor, vm vert. motor
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


//takes a command and moves the engines
//returns 0 if executed succefully  
int run_command(string command, string arg){
	pinMode(hm1, OUTPUT);
	pinMode(hm2, OUTPUT);
	pinMode(hm3, OUTPUT);
	pinMode(hm4, OUTPUT);
	digitalWrite(hm1, LOW);
	digitalWrite(hm2, LOW);
	digitalWrite(hm3, LOW);
	digitalWrite(hm4, LOW);

	int intarg = stoi(arg);

	cout<<"executing: "<<command<<" "<<intarg<<endl;

	if(command == "sleep"){
		sleep_for(milliseconds(intarg));
	}

	if(command == "up"){ // this should not be up fix later !!!
		//digitalWrite(hm3, HIGH);
		//return 100;
		for (int i = 0; i < intarg*2; i++) {
			cout<<"a"<<endl;
			switch (i % 4) {
				case 0:
					digitalWrite(hm1, HIGH);
					digitalWrite(hm2, LOW);
					digitalWrite(hm3, HIGH);
					digitalWrite(hm4, LOW);
					break;
				case 1:
					digitalWrite(hm1, LOW);
					digitalWrite(hm2, HIGH);
					digitalWrite(hm3, HIGH);
					digitalWrite(hm4, LOW);
					break;
				case 2:
					digitalWrite(hm1, LOW);
					digitalWrite(hm2, HIGH);
					digitalWrite(hm3, LOW);
					digitalWrite(hm4, HIGH);
					break;
				case 3:
					digitalWrite(hm1, HIGH);
					digitalWrite(hm2, LOW);
					digitalWrite(hm3, LOW);
					digitalWrite(hm4, HIGH);
					break;
			}
			sleep_for(milliseconds(5));
		}	
	}

	if(command == "right"){
	
	}

	if(command == "down"){
	
	}

	if(command == "left"){
	
	}

	digitalWrite(hm1, LOW);
	digitalWrite(hm2, LOW);
	digitalWrite(hm3, LOW);
	digitalWrite(hm4, LOW);
	
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
