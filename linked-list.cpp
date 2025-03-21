#include "linked-list.h"
#include <iostream>

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

void append(Node*& head, string timestamp, string command){
	Node* current = head;
	Node* new_node = new Node(timestamp, command);

	if (head == nullptr){
		head = new_node;
		return;
	}	

	while (current->next!=nullptr){
		current=current->next;
	}

	current->next = new_node;
}
