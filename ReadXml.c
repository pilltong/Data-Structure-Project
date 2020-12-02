#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_TERMS 1000
#define TOTAL_BUS 100
typedef struct _head_c* headc_pointer;
typedef struct _head_c {
    //data
    int busNo;
    char* plateNo;
    int stationExp;
    //link
    headc_pointer next_c;
} _head_c;
typedef struct bus_node* node_pointer;
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

typedef struct data {
    struct tm qtime;
    int routeId;
    int stationId;
    int stationSeq;
    int endBus;
    int lowPlate;
    char* plateNo;
    int plateType;
    int remainSeat;
} data;
_head_c* head[TOTAL_BUS];
bus_node* tail[TOTAL_BUS];

struct tm read_first_line(FILE* f);
data data_read(FILE* f);
int main() {
    int i, j, k, a;
    int listNum = 0;
    int count = 0;
    int nth = 1;
    FILE* fp;
    data temp;
    struct tm x;


    for (i = 0; i < TOTAL_BUS; i++) {

        head[i] = malloc(sizeof(_head_c));

        head[i]->stationExp = 0; //expectation값 0으로 초기화
        tail[i] = malloc(sizeof(bus_node));
    }



    fp = fopen("A.txt", "r");
    if (fp == NULL) {
        printf("Could not open file");
    }

    x = read_first_line(fp);

    while (!feof(fp)) {
        temp=data_read(fp);

        if (listNum == 0) {/*첫 데이터*/
            //헤드 선언
            head[listNum]->next_c= tail[listNum];
            head[listNum]->plateNo = temp.plateNo;
            head[listNum]->stationExp = temp.stationSeq;//exp값 업데이트

            /*list에 node 추가*/
            bus_node* tempNode = (bus_node*)malloc(sizeof(bus_node));
            tempNode->stationSeq = temp.stationSeq;
            tempNode->qtime = temp.qtime;
            tempNode->plateNo = temp.plateNo;
            tail[listNum]->next = tempNode;
            tail[listNum] = tail[listNum]->next;

            listNum++;
        }
        else {
            j = 0;
            for (i = 0; i < listNum; i++) {
                if (head[i]->plateNo != temp.plateNo) {
                    j++;
                    continue;
                }

                else if (head[i]->plateNo == temp.plateNo) {/*버스 번호 같음*/
                    if (head[i]->stationExp < temp.stationSeq) {/*기존 리스트에 추가(exp보다 클 경우)*/
                        head[i]->stationExp = temp.stationSeq;//exp 업데이트
                        bus_node* tempNode = (bus_node*)malloc(sizeof(bus_node));
                        tempNode->stationSeq = temp.stationSeq;
                        tempNode->qtime = temp.qtime;
                        tempNode->plateNo = temp.plateNo;
                        tail[i]->next = tempNode;
                        tail[i] = tail[i]->next;

                    }
                    else if (head[j]->stationExp == temp.stationSeq) {
                        break;/*다음 data 넘어가야함*/
                    }

                    else {/*새로운 list 만들고 추가(exp보다 작을 경우)*/
                        j++;
                    }
                }
            }
            if (j == listNum) {
                tail[listNum] = head[listNum];
                head[listNum]->plateNo = temp.plateNo;
                head[listNum]->stationExp = temp.stationSeq;//exp값 업데이트

                /*list에 추가*/
                bus_node* tempNode = (bus_node*)malloc(sizeof(bus_node));
                tempNode->stationSeq = temp.stationSeq;
                tempNode->qtime = temp.qtime;
                tempNode->plateNo = temp.plateNo;
                tail[listNum]->next = tempNode;
                tail[listNum] = tail[listNum]->next;
                listNum++;
            }
        }
    }






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

    //first->routeId = routeId;
    t.tm_year = year;
    t.tm_mon = month;
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min = minute;
    t.tm_sec = second;

    free(buffer);

    return t;
}


data read_lines(FILE* f) {
    char* buffer;
    char* plateNo;
    data n;

  
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

    n.qtime.tm_year = year;
    n.qtime.tm_mon = month;
    n.qtime.tm_mday = day;
    n.qtime.tm_hour = hour;
    n.qtime.tm_min = minute;
    n.qtime.tm_sec = second;
    n.endBus = endbus;
    n.lowPlate = lowplate;
    n.plateNo = plateNo;
    n.plateType = platetype;
    n.remainSeat = remainseat;
    n.routeId = routeId;
    n.stationId = stationId;
    n.stationSeq = stationSeq;

    return n;
}
