#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_SIZE 100

typedef struct _head *head_pointer;
typedef struct bus_node *node_pointer;

typedef struct _head {
	head_pointer down;
	int busNum;
} _head;

typedef struct bus_node {
	//data
	//struct tm qtime;
	///int routeId;
	//char plateNum[];
	int plateNum;
	int station_seq;
	int t;
	//link
	union {
		node_pointer next;
		node_pointer down;
	} link_option;

} bus_node;


head_pointer data_read(FILE *f);

int main(void) {
	FILE *f;
	head_pointer a;

	f = fopen("A.txt", "r");
	a = data_read(f);

	return 0;
}

head_pointer data_read(FILE *f){
	head_pointer head;
	node_pointer temp, list, last;
	int t, i_plateNum, seq;
	int i, j, busNum;
	
	head = (head_pointer)malloc(sizeof(_head));
	while(1){
		i = fscanf(f, "%d %d %d", &t, &plateNum, &seq);
		i_plateNum = plateNum;
		if(i == -1)
			break;
		else {
			if(head -> busNum == 0){
				list = (node_pointer)malloc(sizeof(bus_node));
				head -> down = list;
				list -> t = t;
				list -> plateNum = plateNum;
				list -> station_seq = seq;
				last = list;
				head -> busNum++;
			}
			else if {
				
			else {
				temp = (node_pointer)malloc(sizeof(bus_node));
				temp -> t = t;
				temp -> plateNum = plateNum;
				temp -> station_seq = seq;
				last -> link_option.next = temp;
				last = temp;
				head -> busNum++;
			}
		}
	}
	last -> link_option.next = head;
	for(temp = head -> down; temp != head; temp = temp -> link_option.next){
		printf("%d %d %d\n", temp -> t, temp -> plateNum, temp -> station_seq);
	}
	return head;
}





















	


	













