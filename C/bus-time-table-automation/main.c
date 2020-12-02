#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <io.h>


//head head
typedef struct _head_p* headp_pointer;
typedef struct _head_p {
	headp_pointer down_p;
	headp_pointer next_p;
	struct tm qdate;
	int totalBus;
	int routeId;
} _head_p;

//head
typedef struct _head_c* headc_pointer;
typedef struct _head_c {
	headc_pointer next_c;
	headc_pointer down_c;
	int busNo;
	char* plateNo;
	int stationExp;
} _head_c;

//main struct
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

headc_pointer data_read(FILE* f);
struct tm read_first_line(FILE* f);
node_pointer read_lines(FILE* f);
headp_pointer first;

// routeId struct
typedef struct routeId_node* routeId_pointer;
typedef struct routeId_node {
	int routeId;
	routeId_pointer next;
}routeId_node;
// routeId function
void pushRouteId(routeId_pointer, int);
void readRouteIdList(routeId_pointer);
routeId_pointer getRouteIdList(char*);


// folderpath struct
typedef struct folderpath_node* folderpath_pointer;
typedef struct folderpath_node {
	char filepathStr[47];
	folderpath_pointer next;
	folderpath_pointer down;
}folderpath_node;

// folderpath function
folderpath_pointer initFolderpathPtr();
void pushFolderpath(folderpath_pointer, char*);
void readFolderpathList(folderpath_pointer);
folderpath_pointer getFolderpathList(routeId_pointer);



// 파일 경로 제작 함수
char* makeFileName(int, int, int);
char* makeFileName(int year, int month, int day) {
	char tempString[16];
	sprintf(tempString, "/%d-%d-%d.txt", year, month, day);
	return tempString;
}

/*
코드출처: [C] 파일 목록 가져오기, 꾸준희, 2018.08.16, 	https://eehoeskrap.tistory.com/256
*/
folderpath_pointer initFolderpathPtr() {
	folderpath_pointer folderpathPtr = malloc(sizeof(folderpath_node));
	folderpathPtr->down = NULL;
	folderpathPtr->next = NULL;
	return folderpathPtr;
}

folderpath_pointer listdir(char* filepathStr) {
	folderpath_pointer routeIdPathHead = initFolderpathPtr();

	char* extension = "/*.txt*";
	char inputFolderPath[33];
	char* slash = "/";
	strcpy(inputFolderPath, filepathStr);
	strcat(inputFolderPath, slash);
	char pathFinder[39];
	strcpy(pathFinder, filepathStr);
	strcat(pathFinder, extension);
	//printf("%s", pathFinder);

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
		pushFolderpath(routeIdPathHead, filePath);
		fdResult = _findnext(handle, &findData);
	}
	_findclose(handle);
	return routeIdPathHead;
}

void pushPathPtr(folderpath_pointer, folderpath_pointer);
void pushPathPtr(folderpath_pointer headPointer, folderpath_pointer pointer) {
	pointer->down = headPointer->down;
	headPointer->down = pointer;
}

void readPathPtr(folderpath_pointer headPointer);
void readPathPtr(folderpath_pointer headPointer) {
	folderpath_pointer tempPointer = headPointer->down;
	while (tempPointer != NULL) {
		folderpath_pointer routeIdPointer = tempPointer->down;
		while (routeIdPointer != NULL) {
			printf("%s\n", routeIdPointer->filepathStr);
			routeIdPointer = routeIdPointer->next;
		}
		tempPointer = tempPointer->down;
	}
}


int main(void) {
	
	char routeIdListPath[] = { "DATA/route-id-list/routeIdList.txt" };
	routeId_pointer routeIdList = getRouteIdList(routeIdListPath);
	//readRouteIdList(routeIdList);

	folderpath_pointer inputFolderPathList = getFolderpathList(routeIdList);
	//readFolderpathList(inputFolderPathList);

	
	folderpath_pointer tempFolderPathPtr = inputFolderPathList->next;
	folderpath_pointer folderPath = initFolderpathPtr();
	while (tempFolderPathPtr != NULL) {
		char* filepathStr = tempFolderPathPtr->filepathStr;
		folderpath_pointer routeIdFolderpath = listdir(filepathStr);
		pushPathPtr(folderPath, routeIdFolderpath);
		tempFolderPathPtr = tempFolderPathPtr->next;
	}

	readPathPtr(folderPath);

	
	return 0;
}


/*
routeId와 관련된 함수 목록입니다.
- pushRouteId:		routeId를 routeId_node형식에 담아서 routeIdList에 추가하는 함수입니다.
- readRouteIdList:	연결리스트로 된 routeIdList의 헤드노드에 접근하여 마지막 노드까지 routeId를
								출력하는 함수입니다.
- getRouteIdList:	텍스트 파일에 저장된 routeId를 함수[pushRouteId]를 호출하여 연결리스트에
								저장하고 연결리스트를 반환합니다. 
*/
void pushRouteId(routeId_pointer routeIdList, int routeId) {
	// Stack 구조로 routeIdList를 구현합니다.
	// routeId_node를 초기화 합니다.
	routeId_pointer routeIdPtr = malloc(sizeof(routeId_node));
	routeIdPtr->routeId = routeId;
	routeIdPtr->next = NULL;

	// routeId_node를 
	routeIdPtr->next = routeIdList->next;
	routeIdList->next = routeIdPtr;
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
routeId_pointer getRouteIdList(char* folderPath) {
	FILE* routeIdListTXT;
	routeIdListTXT = fopen(folderPath, "r");
	// routeId_pointer 형식의 변수 routeIdList를 초기화 합니다.
	routeId_pointer routeIdList = malloc(sizeof(routeId_node));
	routeIdList->next = NULL;

	int routeId;
	while (fscanf(routeIdListTXT, "%d", &routeId) != EOF) {
		//printf("%d\n", routeId);
		pushRouteId(routeIdList, routeId);
	}
	return routeIdList;
}

/*
folderPath와 관련된 함수 목록입니다.
*/
void pushFolderpath(folderpath_pointer stringPtr, char* strRouteId) {
	folderpath_pointer tempStringPtr = malloc(sizeof(folderpath_node));
	strcpy(tempStringPtr->filepathStr, strRouteId);
	tempStringPtr->next = NULL;
	tempStringPtr->next = stringPtr->next;
	stringPtr->next = tempStringPtr;
}
void readFolderpathList(folderpath_pointer StringList)
{
	folderpath_pointer tempString = StringList->next;
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
folderpath_pointer getFolderpathList(routeId_pointer routeIdList) {

	folderpath_pointer inputFolderPathList = malloc(sizeof(folderpath_node));
	inputFolderPathList->next = NULL;

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
		pushFolderpath(inputFolderPathList, inputRootPath);
		tempRouteId = tempRouteId->next;
	}
	return inputFolderPathList;
}