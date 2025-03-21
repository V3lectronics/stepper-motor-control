#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include <string>
using namespace std;

struct Node{
	string timestamp;
	string command;
	Node* next;
	Node(string new_timestamp, string new_command){
		timestamp = new_timestamp;
		command = new_command;
	}
};

void display_list(Node* head);

void append(Node*& head, string timestamp, string command);

#endif
