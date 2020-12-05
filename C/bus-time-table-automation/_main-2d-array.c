#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <io.h>
//#include <locale.h>

typedef struct routeId_head* routeId_pointer;
typedef struct folderpath_node* folderpath_pointer;
typedef struct _head_h* headh_pointer;
typedef struct _head_r* headr_pointer;
typedef struct _head_p* headp_pointer;
typedef struct _head_c* headc_pointer;
typedef struct bus_node* node_pointer;

// routeId struct
typedef struct routeId_head {
	routeId_pointer next;
	folderpath_pointer down;
	int routeId;
	char rootpathStr[32];
}routeId_head;

// folderpath struct
typedef struct folderpath_node {
	char filepathStr[47];
	folderpath_pointer next;
}folderpath_node;

//headheadheadhead
typedef struct _head_h {
	headh_pointer next_h;
	headr_pointer down_h;
}_head_h;

//headheadhead
typedef struct _head_r {
	headr_pointer next_r;
	headp_pointer down_r;
	int maxStaSeq;
	int maxBusCnt;
	int routeId;
} _head_r;

//head head
typedef struct _head_p {
	headp_pointer next_p;
	headc_pointer down_p;
	node_pointer** dailyTimeTable;
	struct tm qdate;
	int totalBus;
	//int routeId;
} _head_p;

//head
typedef struct _head_c {
	headc_pointer next_c;
	node_pointer down_c;
	char plateNo[14];
	int busNoExp;
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
	char plateNo[14];
	int plateType;
	int remainSeat;
	//link
	node_pointer next;
} bus_node;

/* routeId function*/
routeId_pointer initRouteIdHeadptr(int routeId);
void pushRouteId(routeId_pointer, int);
routeId_pointer getRouteIdList();
void readRouteIdList(routeId_pointer routeIdList);
void readAllRouteIdList(routeId_pointer routeIdList);

/*folderpath function*/
folderpath_pointer initFolderpathPtr();
void pushFolderpath(routeId_pointer, char*);
void readFolderpathList(folderpath_pointer filepathListPointer);
void getFolderpathList(routeId_pointer routeIdList);
folderpath_pointer listdir(routeId_pointer);
void getFilepathList(routeId_pointer routeIdList);

/* */
headh_pointer initHeadH();
headr_pointer initHeadR(routeId_pointer tempRouteId);
headp_pointer initHeadP(headr_pointer headR);
headc_pointer initHeadC();
node_pointer initNode();


void pushHeadR(headh_pointer headH, headr_pointer headR);
void pushHeadP(headr_pointer headR, headp_pointer headP);
void pushHeadC(headp_pointer headP, headc_pointer headC);
void pushNode(headc_pointer headC, node_pointer node);

node_pointer read_lines(char* buffer);
headc_pointer newHeadC(headp_pointer headP, node_pointer node);
void readHeadH(headh_pointer headH);

/* node_pointer를 담는 2차원 배열 dailyTimeTable에 node_pointer형 node를 추가하는 함수입니다.
	행의 번호: 정류소번호(headC->stationExp)
	열의 번호: 호차 번호(headC->busNoExp)*/
void addNode(node_pointer node, node_pointer** dailyTimeTable, headc_pointer headC) {
	dailyTimeTable[headC->stationExp][headC->busNoExp] = node;
}

/* stationExp보다 "작은" stationSeq가 들어왔을-새로운 버스가 운행을 시작할때-headC를 업데이트 하고 노드를 추가한다.*/
void updateHeadC(headc_pointer headC, headp_pointer headP, node_pointer node) {
	// 해당 plateNo를 관리하는 headC의 stationExp를 업데이트 한다: headC->stationExp = node->stationSeq;
	headC->stationExp = node->stationSeq;
	// 해당 plateNo를 관리하는 headC의 busNoExp를 업데이트 한다: headC->busNoExp = ++headP->totalBus;
	headC->busNoExp = ++headP->totalBus;
}

void readDailyTimeTable(node_pointer** dtt, headp_pointer headP) {
	int row = 54;
	int col = headP->totalBus;
	for (int i = 1; i < row; i++) {
		for (int j = 1; j < col; j++) {
			if (dtt[i][j] == NULL) {
				printf("88:88  ");
			}
			else
				printf("%2d:%2d ", dtt[i][j]->qtime.tm_hour, dtt[i][j]->qtime.tm_min);
		}
		printf("\n");
	}
}


int main(void) 
{
	//setlocale(LC_ALL, "ko_KR.UTF-8");

	routeId_pointer routeIdList = getRouteIdList();
	getFolderpathList(routeIdList);
	getFilepathList(routeIdList);
	readAllRouteIdList(routeIdList);

	headh_pointer headH;
	headH = initHeadH();

	routeId_pointer tempRouteId = routeIdList->next;
	while (tempRouteId != NULL)
	{
		int routeId = tempRouteId->routeId;
		//printf("%d", routeId);
		headr_pointer headR;
		headR = initHeadR(tempRouteId);
		pushHeadR(headH, headR);
		//printf("%d %d", headR->maxBusCnt, headR->maxStaSeq);

		/*하나의 폴더 경로에 대해 탐색합니다.
			: 새로운 routeId에 대해 프로세스를 시작합니다*/
		folderpath_pointer tempFilePath = tempRouteId->down;
		while (tempFilePath != NULL) 
		{
			/*하나의 파일 경로에 대해 탐색합니다.
				: 새로운 datetime에 대해 프로세스를 시작합니다.*/
			char* filepath = tempFilePath->filepathStr;
			headp_pointer headP = initHeadP(headR);

			/* node_pointer** initDailyTimeTable(){ */
			node_pointer** dailyTimeTable;
			if (1) {
				int row = 100;//headR->maxStaSeq;
				int col = 100;//headR->maxBusCnt;
				dailyTimeTable = malloc(row * sizeof(node_pointer*));
				for (int i = 0; i < row; i++)
					dailyTimeTable[i] = calloc(col, sizeof(node_pointer));
			}
			//readDailyTimeTable(dailyTimeTable, headP);

			pushHeadP(headR, headP);
			FILE* inputFile = fopen(filepath, "r");
			// 첫번째 줄 stationId 와 querytime
			char firstLine[44];
			fgets(firstLine, sizeof(firstLine), inputFile);
			//printf("%s", firstLine);
			// 두번째 줄 부터 마지막 줄
			char buffer[85];
			while (fgets(buffer, sizeof(buffer), inputFile) != NULL) 
			{
				//printf("%s", buffer);
				node_pointer node = read_lines(buffer);
				char* curPlateNo = node->plateNo;
				//printf("%s\n", curPlateNo);
				// plateNo 가 존재하는지 확인한다.
				headc_pointer headC = headP->down_p;
				int plateNoFlag = 0;
				while (headC != NULL)
				{
					if (!strcmp(headC->plateNo, curPlateNo))
					{
						/* headC 연결리스트에 같은 plateNo가 "존재 할때" */
						int stationExp = headC->stationExp;									
						int stationCur = node->stationSeq;									// ROW
						if (stationCur > stationExp){
							/* stationExp보다 "큰" stationSeq가 들어왔을때-다음 정류소를 지났을때-headC를 업데이트 하고 노드를 추가한다.*/
							// state #2
							//pushNode(headC, node);
							headC->stationExp = stationCur; // 해당 plateNo를 관리하는 headC의 stationExp를 업데이트 한다.
							addNode(node, dailyTimeTable, headC);
						}
						else if (stationCur == stationExp) {
							/* stationExp보다 "같은" stationSeq가 들어왔을때-다음 정류소를 지나지 못했을때-node의 메모리를 해제한다.*/
							free(node);
						}
						else {
							updateHeadC(headC, headP, node);
							// dailyTimeTable에 Node를 추가한다: dailyTimeTable[stationExp][busNoExp] = node;
							addNode(node, dailyTimeTable, headC);
						}
						plateNoFlag = 1;
						break;
					}
					else 
						headC = headC->next_c;
				}
				if (plateNoFlag) continue;

				/* headC 연결리스트에 같은 plateNo가 "존재 하지 않을때" */
				//printf("headC를 새로 생성합니다.\n");
				headc_pointer _headC = newHeadC(headP, node); // newHeadC
				// dailyTimeTable에 Node를 추가한다: dailyTimeTable[stationExp][busNoExp] = node;
				addNode(node, dailyTimeTable, _headC);
				//char buffer[85];
			}
			fclose(inputFile);
			readDailyTimeTable(dailyTimeTable, headP);
			tempFilePath = tempFilePath->next;
		}
		tempRouteId = tempRouteId->next;
	}
	//readHeadH(headH);
	
	return 0;
}

routeId_pointer initRouteIdHeadptr(int routeId) {
	routeId_pointer routeIdHeadPtr = malloc(sizeof(routeId_head));
	routeIdHeadPtr->next = NULL;
	routeIdHeadPtr->down = NULL;
	routeIdHeadPtr->routeId = routeId;
	return routeIdHeadPtr;
}

void pushRouteId(routeId_pointer routeIdList, int routeId) {
	// Stack 구조로 routeIdList를 구현합니다.
	// routeId_head를 초기화 합니다.
	routeId_pointer routeIdPtr = initRouteIdHeadptr(routeId);

	routeIdPtr->next = routeIdList->next;
	routeIdList->next = routeIdPtr;
}

routeId_pointer getRouteIdList() {
	char folderPath[] = { "DATA/route-id-list/routeIdList.txt" };
	FILE* routeIdListTXT;
	routeIdListTXT = fopen(folderPath, "r");
	// routeId_pointer 형식의 변수 routeIdList를 초기화 합니다.
	routeId_pointer routeIdList = initRouteIdHeadptr(0);

	int routeId;
	while (fscanf(routeIdListTXT, "%d", &routeId) != EOF) {
		//printf("%d\n", routeId);
		pushRouteId(routeIdList, routeId);
	}
	return routeIdList;
}

void readRouteIdList(routeId_pointer routeIdList) {
	routeId_pointer tempRouteId = routeIdList->next;
	if (tempRouteId == NULL) {
		printf("Stack UnderFlow");
		return;
	}
	while (tempRouteId != NULL) {
		int routeId = tempRouteId->routeId;
		printf("%d\n", routeId);
		tempRouteId = tempRouteId->next;
	}
}

void readAllRouteIdList(routeId_pointer routeIdList) {
	routeId_pointer tempRouteId = routeIdList->next;
	if (tempRouteId == NULL) {
		printf("Stack UnderFlow");
		return;
	}
	while (tempRouteId != NULL) {
		int routeId = tempRouteId->routeId;
		printf("%d\n", routeId);
		folderpath_pointer tempRouteIdFolder = tempRouteId->down;
		while (tempRouteIdFolder != NULL) {
			char* filePath = tempRouteIdFolder->filepathStr;
			printf("%s\n", filePath);
			tempRouteIdFolder = tempRouteIdFolder->next;
		}
		tempRouteId = tempRouteId->next;
	}
}

folderpath_pointer initFolderpathPtr() {
	folderpath_pointer folderpathPtr = malloc(sizeof(folderpath_node));
	folderpathPtr->next = NULL;
	return folderpathPtr;
}

void pushFolderpath(routeId_pointer tempRouteId, char* strRouteId) {
	folderpath_pointer tempStringPtr = initFolderpathPtr();
	strcpy(tempStringPtr->filepathStr, strRouteId);
	tempStringPtr->next = tempRouteId->down;
	tempRouteId->down = tempStringPtr;
}

void readFolderpathList(folderpath_pointer filepathListPointer)
{
	folderpath_pointer tempString = filepathListPointer->next;
	if (tempString == NULL) {
		printf("Stack UnderFlow");
		return;
	}
	while (tempString != NULL) {
		char* filepath = tempString->filepathStr;
		printf("%s\n", filepath);
		tempString = tempString->next;
	}
}

void getFolderpathList(routeId_pointer routeIdList) {
	routeId_pointer tempRouteId = routeIdList->next;
	if (tempRouteId == NULL) {
		printf("RouteId가 비어있습니다. 파일을 다시 확인해주세요.");
		exit(0);
	}
	while (tempRouteId != NULL) {
		char inputRootPath[32] = { "DATA/daily-time-table/" };
		char routeIdStirng[10];
		_itoa(tempRouteId->routeId, routeIdStirng, 10);
		//printf("%s\n", routeIdStirng);
		strcat(inputRootPath, routeIdStirng);
		//printf("%s\n", inputRootPath);
		strcpy(tempRouteId->rootpathStr, inputRootPath);
		tempRouteId = tempRouteId->next;
	}
}

folderpath_pointer listdir(routeId_pointer tempRouteId) {
	char* filepathStr = tempRouteId->rootpathStr;
	folderpath_pointer routeIdPathHead = initFolderpathPtr();

	char* extension = "/*.txt*";
	char* slash = "/";
	char inputFolderPath[33];
	strcpy(inputFolderPath, filepathStr);
	strcat(inputFolderPath, slash);
	char pathFinder[39];
	strcpy(pathFinder, filepathStr);
	strcat(pathFinder, extension);
	//printf("%s", pathFinder);

	/* 	코드출처: [C] 파일 목록 가져오기, 작성자: 꾸준희, 2018.08.16, 	https://eehoeskrap.tistory.com/256 	*/
	struct _finddata_t findData;
	intptr_t handle;
	int fdResult = 1;

	handle = _findfirst(pathFinder, &findData);

	if (handle == -1) {
		return routeIdPathHead;
	}
	while (fdResult != -1) {
		//printf("파일명 : %s, 크기:%d\n", findData.name, findData.size);
		char filePath[47];
		strcpy(filePath, inputFolderPath);
		strcat(filePath, findData.name);
		//printf("%s\n", filePath);
		pushFolderpath(tempRouteId, filePath);
		fdResult = _findnext(handle, &findData);
	}
	_findclose(handle);
	return routeIdPathHead;
}

void getFilepathList(routeId_pointer routeIdList) {
	routeId_pointer tempRouteId = routeIdList->next;
	while (tempRouteId != NULL) {
		listdir(tempRouteId);
		tempRouteId = tempRouteId->next;
	}
}

/*상원님함수*/
node_pointer read_lines(char* buffer) {
	node_pointer node = initNode();

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

	char* _ = strtok(NULL, " ");						// 출력양식 수정으로 12월 4일을 포함한 이후 데이터부터는 필요 없습니다. (microSeconds, timezone을 포함하지 않습니다. [.00000+09:00])
	//printf("%s\n", _);

	char* e_b = strtok(NULL, " ");
	int endbus = atoi(e_b);

	char* l_p = strtok(NULL, " ");
	int lowplate = atoi(l_p);

	char* plateNo = malloc(sizeof(char) * 10);
	plateNo = strtok(NULL, " ");

	char* p_t = strtok(NULL, " ");
	int platetype = atoi(p_t);

	char* r_s = strtok(NULL, " ");
	int remainseat = atoi(r_s);

	char* r_i = strtok(NULL, " ");
	int routeId = atoi(r_i);

	char* s_i = strtok(NULL, " ");
	int stationId = atoi(s_i);
	//printf("%d\n", stationId);

	char* s_s = strtok(NULL, " ");
	int stationSeq = atoi(s_s);

	node->qtime.tm_year = year;
	node->qtime.tm_mon = month;
	node->qtime.tm_mday = day;
	node->qtime.tm_hour = hour;
	node->qtime.tm_min = minute;
	node->qtime.tm_sec = second;
	node->endBus = endbus;
	node->lowPlate = lowplate;
	strcpy(node->plateNo, plateNo);
	node->plateType = platetype;
	node->remainSeat = remainseat;
	node->routeId = routeId;
	node->stationId = stationId;
	node->stationSeq = stationSeq;

	return node;
}

headh_pointer initHeadH() {
	headh_pointer tempHeadH_pointer = malloc(sizeof(_head_h));
	tempHeadH_pointer->down_h = NULL;
	tempHeadH_pointer->next_h = NULL;
	return tempHeadH_pointer;
}

headr_pointer initHeadR(routeId_pointer tempRouteId) {
	headr_pointer tempHeadR_pointer = malloc(sizeof(_head_r));
	tempHeadR_pointer->down_r = NULL;
	tempHeadR_pointer->next_r = NULL;
	int routeId = tempRouteId->routeId;
	tempHeadR_pointer->routeId = routeId;
	tempHeadR_pointer->maxBusCnt = 10; // 운행시간 / 최소배차간격 tempRouteId->maxBusCnt
	tempHeadR_pointer->maxStaSeq = 10; // 정류장정보 tempRouteId->maxStaSeq
	return tempHeadR_pointer;
}

headp_pointer initHeadP(headr_pointer headR) {
	headp_pointer tempHeadP_pointer = malloc(sizeof(_head_p));
	tempHeadP_pointer->down_p = NULL;
	tempHeadP_pointer->next_p = NULL;
	node_pointer** dailyTimeTable;
	int row = headR->maxStaSeq;
	int col = headR->maxBusCnt;
	dailyTimeTable = malloc(row * sizeof(node_pointer*));
	for (int i = 0; i < row; i++)
		dailyTimeTable[i] = malloc(col * sizeof(node_pointer));
	tempHeadP_pointer->totalBus = 0;
	//tempHeadP_pointer->qdate
	return tempHeadP_pointer;
}

headc_pointer initHeadC() {
	headc_pointer tempHeadC_pointer = malloc(sizeof(_head_c));
	tempHeadC_pointer->down_c = NULL;
	tempHeadC_pointer->next_c = NULL;
	strcpy(tempHeadC_pointer->plateNo, "\0");
	tempHeadC_pointer->busNoExp = 0;
	tempHeadC_pointer->stationExp = 0;
	return tempHeadC_pointer;
}

node_pointer initNode() {
	node_pointer node = malloc(sizeof(bus_node));
	node->next = NULL;
	strcpy(node->plateNo, "\0");
	return node;
};

void pushHeadR(headh_pointer headH, headr_pointer headR) {
	headR->next_r = headH->down_h;
	headH->down_h = headR;
}

void pushHeadP(headr_pointer headR, headp_pointer headP) {
	headP->next_p = headR->down_r;
	headR->down_r = headP;
}

void pushHeadC(headp_pointer headP, headc_pointer headC) {
	headC->next_c = headP->down_p;
	headP->down_p = headC;
}

void pushNode(headc_pointer headC, node_pointer node) {
	node->next = headC->down_c;
	headC->down_c = node;
}

void readHeadH(headh_pointer headH)
{
	headr_pointer tempHeadR = headH->down_h;
	while (tempHeadR != NULL)
	{
		headr_pointer tempHeadP = tempHeadR->down_r;
		while (tempHeadP != NULL)
		{
			headc_pointer tempHeadC = tempHeadP->down_r;
			while (tempHeadC != NULL)
			{
				node_pointer tempNode = tempHeadC->down_c;
				printf("#%2d\t", tempHeadC->busNoExp);
				printf("RouteId: %10d\t", tempNode->routeId);
				printf("PlateNo: %s\n", tempNode->plateNo);
				while (tempNode != NULL)
				{
					printf("%7d-", tempNode->qtime.tm_year);
					printf("%2d-%2d", tempNode->qtime.tm_mon, tempNode->qtime.tm_mday);
					printf("%3d:%2d\t", tempNode->qtime.tm_hour, tempNode->qtime.tm_min);
					printf("%s", tempNode->plateNo);
					printf("%4d\n", tempNode->stationSeq);
					tempNode = tempNode->next;
				}
				tempHeadC = tempHeadC->next_c;
			}
			tempHeadP = tempHeadP->next_r;
		}
		tempHeadR = tempHeadR->next_r;
	}
}

headc_pointer newHeadC(headp_pointer headP, node_pointer node) {
	headc_pointer newHeadC = initHeadC();
	newHeadC->busNoExp = ++(headP->totalBus);
	newHeadC->stationExp = node->stationSeq;
	strcpy(newHeadC->plateNo, node->plateNo);
	pushHeadC(headP, newHeadC);
	//pushNode(newHeadC, node);

	return newHeadC;
}