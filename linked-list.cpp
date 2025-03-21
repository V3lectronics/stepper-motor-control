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
