#include "linked-list.h"

struct Node{
	string timestamp;
	string command;
	Node* next;
	Node(string new_timestamp, string new_command){
		timestamp = new_timestamp;
		command = new_command;
	}
};

Node* head;

void display_list(Node* head){
	cout<<"--------"<<endl;
	Node* current = head;
	while (current!=nullptr){
		cout <<'['<< current->timestamp<< " : " << current->command<<']' << " ";
		current = current->next;
	}
	cout<<endl;
	cout<<"--------"<<endl;
}
