/*************************************************************************
	> File Name: Segment.c
	> Author: 
	> Mail: 
	> Created Time: 2015年04月01日 星期三 20时59分50秒
 ************************************************************************/

#include<stdio.h>
#include "Segment.h"

Segment* segHead;
Seglet* letHead;
SegmentManager Manager, *Iterator;

/*
Segment init_Segment(void) {
    Segment seg;
    memset(&seg, 0, sizeof(seg));
    return seg;
}
*/

void init_SegmentManager(void) {
    memset(&Manager, 0, sizeof(Manager));
    return;
}

Segment *createSegment(void) {
    Segment *seg = (Segment*)malloc(sizeof(Segment));
    return seg;
}

Seglet *createSeglet(char *command) {
    Seglet *let = (Seglet*)malloc(sizeof(Seglet));
    Object *obj = (Object*)malloc(sizeof(Object));

    obj->command = command;
    obj->timestamp =
    obj->avaliable = true;
    let->length = strlen(command);
    let->objector = obj;
    let->next = NULL;
    return let;
}

void setHead(Segment* seg, char *ip, int port) {
    //! TODO
    //! need to set others
    seg->header.capacity = 1024;
    seg->header.used = true;
    seg->header.sin_addr = ip;
    seg->header.sin_port = port;
}

char *parseIpPort(char *cont) {
    int len = strlen(cont) -1;
    char str[64];// remove the last '\r\n'
    memcpy(str, cont, len);
    str[len - 1] = '\0';// carefully using memcpy
    char *IpPort = strrchr(str, '\n');
    IpPort += 1; // skip '\n'
    return IpPort;
}

char *parseCommand(char *cont, char *Iport) {
    //remove the tail, change the cont background
    int len = strlen(cont) - strlen(Iport) - 2;
    static char temp[1024];
    memcpy(temp, cont, len);
    temp[len] = '\0';
    return temp;
}



char *getIp(char *cont) {
    static char ip[20];
    int i = 0;
    while (cont[i]!= ':') {
        i++;
    }
    memcpy(ip, cont, i);
    ip[i] = '\0';
    return ip;
}

int getPort(char *cont) {
    int port = 0;
    int i = 0;
    while (cont[i] != '\0') {
        if (cont[i] == ':') {
            i++;
            while (cont[i] != '\0') {
                port = port * 10 + (cont[i] - '0');
                i++;
            }
            break;
        }
        i++;
    }
    return port;
}

Segment *getSegment(SegmentManager *manager, char *ip, int port) {
    SegmentManager *iterator = manager;
    while (iterator->segment->next != NULL) {
        if (strcmp(iterator->segment->next->header.sin_addr, ip) == 0 && iterator->segment->next->header.sin_port == port) {
            break;
        } else {
            iterator->segment->next = iterator->segment->next->next;
        }
    }
    return iterator->segment->next;
}

void appendToSegment(char *cont) {//, struct in_addr addr, unsigned short port) {
    Iterator = &Manager;
    char *IpPort = parseIpPort(cont);
    char *rip = getIp(IpPort);
    int rport = getPort(IpPort);
    char *command = parseCommand(cont, IpPort);


    //! if no Segment avaliable, try to create
    if (Iterator->segment == NULL) {
        Iterator->segment = createSegment();
        Iterator->segment->next = NULL;
        setHead(Iterator->segment, rip, rport);
        //Iterator->used = true; //! set current segment is using
        Iterator->segment->segleter = createSeglet(command);
        Iterator->segment->p = &Iterator->segment->segleter;
        Iterator->segment->segleter->next = NULL;
    } else {
        Segment *currSeg = getSegment(Iterator, rip, rport);
        if (currSeg == NULL) {
            currSeg = createSegment();
            currSeg->next = NULL;
            setHead(currSeg, rip, rport);
            //Iterator->used = true; //! set current segment is using
            currSeg->segleter = createSeglet(command);
            currSeg->p = &currSeg->segleter;
            currSeg->segleter->next = NULL;
        } else {
            Seglet *seglet = currSeg->p;
            seglet->next = createSeglet(command);
            seglet->next->next = NULL;
            currSeg->p = seglet->next;
        }

    }

}
