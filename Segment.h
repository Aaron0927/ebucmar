/*************************************************************************
	> File Name: Segment.h
	> Author: 
	> Mail: 
	> Created Time: 2015年04月01日 星期三 20时58分24秒
 ************************************************************************/

#ifndef _SEGMENT_H
#define _SEGMENT_H
#endif
#include "Object.h"
#include <arpa/inet.h>
#include <stdbool.h>
#define MAX_SEGMENT_LENGTH 1024 * 1024
/**
 * the Segment structure as follows:
 *
 * +--------------+---------+---------+---------+---------+---------
 * | segment head | length1 | object1 | length2 | object2 |  ... ...
 * +--------------+---------+---------+---------+---------+---------
 *
 * segment head(struct) :
 * length1 : object1 length
 * object1 :
 */

typedef struct head {
    bool used;
    int segletnum; //! record the total seglets number of a segment
    int capacity;
    char sin_addr[20];
    int sin_port;
} Head;

typedef struct seglet {
    int length;
    Object *objector;
    struct seglet *next;
} Seglet;

typedef struct segment {
    Head header;
    void *p; //! store a tail Seglet position
    Seglet *segleter;
    struct segment *next;
} Segment;

typedef struct segmentmanager {
    bool used;
    Segment *segment;
} SegmentManager;


int getCapacity(const Segment *seg);
char *parseIpPort(char *cont);

char *parseCommand(const char *cont, char *Iport);
char *getIp(char *cont);
int getPort(char *cont);
int getObjectLength(const Seglet *seg);
Object *getNextObject(const Seglet *seg);



Segment init_Segment(void);
void setHead(Segment *seg, char *ip, int port);
void setCapacity(Segment *seg, int bits);
void setIpPort(Segment *seg);
void appendToSegment(char *cont);//, struct in_addr addr, unsigned short port);//! append an object to segment
void appendToManager(Segment *seg);//! append an segment to manager
void saveToDisk(Segment *seg);

void init_SegmentManager(void);
Segment *createSegment(void);
Segment *getSegment(SegmentManager *manager, char *ip, int port);
Seglet *createSeglet(char *command);
Segment *getLastSegment(SegmentManager *manager);
void setSegletNum(Segment *seg);
int getSegletNum(Segment *seg);

//++++++++++++++++++++++++++++++++++ storage ++++++++++++++++++++++++++++++++++++++++++//
int persist(Segment *seg);
char  *mkStorage(char *dirname);

//++++++++++++++++++++++++++++++++++ recovery ++++++++++++++++++++++++++++++++++++++++++//

Segment *readFile(char *dirName, char *fileName);
int getSegmentLength(char *str);
Segment *loadToMem(char *ipPort);
