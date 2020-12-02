#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct _head_p *headp_pointer;
typedef struct _head_c *headc_pointer;
typedef struct bus_node *node_pointer;
//head head
typedef struct _head_p {
	headp_pointer down_p;
	headp_pointer next_p;
	struct tm qdate;
	int totalBus;
	int routeId;
} _head_p;
//head
typedef struct _head_c {
	headc_pointer next_c;
	headc_pointer down_c;
	int busNo;
	char* plateNo;
	int stationExp;
} _head_c;
//main struct
typedef struct bus_node {
	//data
	struct tm qtime;
	int routeId;
	int stationId;
	int stationSeq;
	int endBus;
	int lowPlate;
	char* plateNo;
	int plateType;
	int remainSeat;
	//link
	node_pointer next;
} bus_node;

headc_pointer data_read(FILE *f);
struct tm read_first_line(FILE *f);
node_pointer read_lines(FILE *f);
headp_pointer first;

int main(void) {
	FILE *f;
	
	f = fopen("A.txt", "r");
	first = (headp_pointer)malloc(sizeof(_head_p));
	first -> qdate = read_first_line(f);
	first -> down_p = data_read(f);
	fclose(f);
	return 0;
}
headp_pointer data_read(FILE *f){
	headc_pointer head_c;
	node_pointer temp, list, last;
	
	head_c = (headc_pointer)malloc(sizeof(_head_c));

	temp = data_read(f);
	head_c->next_c = temp;
	head_c->busNo = 1;
	head_c->plateNo = temp->plateNo;
	head_c->stationExp = temp->stationSeq + 1;
	






	/*
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
	
	last -> link_option.next = head;
	for(temp = head -> down; temp != head; temp = temp -> link_option.next){
		printf("%d %d %d\n", temp -> t, temp -> plateNum, temp -> station_seq);
	}
	*/
	return head_c;
}

struct tm read_first_line(FILE* f) {
	char* buffer;
	struct tm t;

	buffer = (char)malloc(sizeof(char) * 33);

	fgets(buffer, sizeof(buffer), f);

	char* routeid = strtok(buffer, " ");
	int routeId = atoi(routeid);

	char* strYear = strtok(buffer, "-");
	int year = atoi(strYear);

	char* strMonth = strtok(NULL, "-");
	int month = atoi(strMonth);

	char* strDay = strtok(NULL, "T");
	int day = atoi(strDay);

	char* strHour = strtok(NULL, ":");
	int hour = atoi(strHour);

	char* strMinute = strtok(NULL, ":");
	int minute = atoi(strMinute);

	char* strSecond = strtok(NULL, ".");
	int second = atoi(strSecond);

	first->routeId = routeId;
	t.tm_year = year;
	t.tm_mon = month;
	t.tm_mday = day;
	t.tm_hour = hour;
	t.tm_min = minute;
	t.tm_sec = second;

	free(buffer);

	return t;
}

node_pointer read_lines(FILE* f) {
	char* buffer;
	char* plateNo;
	struct tm t;
	node_pointer n;

	n = (node_pointer)malloc(sizeof(bus_node));
	buffer = (char)malloc(sizeof(char) * 74);

	fgets(buffer, sizeof(buffer), f);

	char* strYear = strtok(buffer, "-");
	int year = atoi(strYear);

	char* strMonth = strtok(NULL, "-");
	int month = atoi(strMonth);

	char* strDay = strtok(NULL, "T");
	int day = atoi(strDay);

	char* strHour = strtok(NULL, ":");
	int hour = atoi(strHour);

	char* strMinute = strtok(NULL, ":");
	int minute = atoi(strMinute);

	char* strSecond = strtok(NULL, ".");
	int second = atoi(strSecond);

	char* e_b = strtok(NULL, " ");
	int endbus = atoi(e_b);

	char* l_p = strtok(NULL, " ");
	int lowplate = atoi(l_p);

	plateNo = strtok(NULL, " ");

	char* p_t = strtok(NULL, " ");
	int platetype = atoi(p_t);

	char* r_s = strtok(NULL, " ");
	int remainseat = atoi(r_s);

	char* r_i = strtok(NULL, " ");
	int routeId = atoi(r_i);

	char* s_i = strtok(NULL, " ");
	int stationId = atoi(s_i);

	char* s_s = strtok(NULL, " ");
	int stationSeq = atoi(s_s);

	n->qtime.tm_year = year;
	n->qtime.tm_mon = month;
	n->qtime.tm_mday = day;
	n->qtime.tm_hour = hour;
	n->qtime.tm_min = minute;
	n->qtime.tm_sec = second;
	n->endBus = endbus;
	n->lowPlate = lowplate;
	n->plateNo = plateNo;
	n->plateType = platetype;
	n->remainSeat = remainseat;
	n->routeId = routeId;
	n->stationId = stationId;
	n->stationSeq = stationSeq;

	return n;
}


















	


	













