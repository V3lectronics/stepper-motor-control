#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>

using namespace std;
const string config_path = "motor.conf";

//takes a command and alligns the engines
//returns 0 if executed succefully 
int run_command(string command, string argument){
	cout<<"executing: "<<command<<" "<<argument<<endl;
	
	return 0;
}

int main(){
	cout<<"using config from: "<<config_path<<endl;	
	fstream config_file(config_path);

	if (not config_file.is_open()) {
        cerr << "Error opening "<<config_path<<endl;
        return 1;
    }

	string commands_file_dir, engine_1_name, engine_2_name;
	double engine_1_dpfs, engine_2_dpfs;	
	config_file >> commands_file_dir;
	config_file >> commands_file_dir;

	config_file >> engine_1_name;
	config_file >> engine_1_name;
	config_file >> engine_1_name;

	config_file >> engine_1_dpfs;
	config_file >> engine_1_dpfs;

	config_file >> engine_2_name;
	config_file >> engine_2_name;
	config_file >> engine_2_name;

	config_file >> engine_2_dpfs;
	config_file >> engine_2_dpfs;

	cout<<engine_1_name<<endl;
	cout<<engine_2_name<<endl;




	cout<<"running commands from: "<<commands_file_dir<<endl;	


	

		

	config_file.close();
	return 0;
}
