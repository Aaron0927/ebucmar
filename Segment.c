/*************************************************************************
	> File Name: Segment.c
	> Author: 
	> Mail: 
	> Created Time: 2015年04月01日 星期三 20时59分50秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>       //for cmkdir
#include <fcntl.h>          //for access
#include <sys/types.h>
#include <dirent.h>         // dir options
#include "Segment.h"

Segment* segHead;
Seglet* letHead;
SegmentManager *Manager;
//! max segment is 8MB
#define MAX_SEGMENT_CAPACITY 1024 * 1024 * 8
/*
Segment init_Segment(void) {
    Segment seg;
    memset(&seg, 0, sizeof(seg));
    return seg;
}
*/

void init_SegmentManager(void) {
    //memset(&Manager, 0, sizeof(Manager));
    Manager = (SegmentManager *)malloc(sizeof(SegmentManager));
    Manager->used = false;
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
    time(&obj->timestamp);
    obj->avaliable = true;
    let->length = strlen(command);
    let->objector = obj;
    let->next = NULL;
    return let;
}

void setHead(Segment* seg, char *ip, int port) {
    //! TODO
    //! need to set others
    seg->header.capacity = MAX_SEGMENT_CAPACITY;
    seg->header.segletnum = 0;
    seg->header.used = true;
    seg->header.sin_addr = ip;
    seg->header.sin_port = port;
}

char *parseIpPort(char *cont) {
    int len = strlen(cont);
    //revome the last '\r\n'
    while(cont[len - 1] == '\r' || cont[len -1] == '\n') {
        --len;
    }
    char *str = (char *)malloc(len + 1);
    memcpy(str, cont, len);
    str[len] = '\0';
    char *temp = strrchr(str, '\n');
    temp += 1; //skip '\n'
    strcpy(str, temp);
    str[strlen(str)] = '\0';// carefully using memcpy, start from 0
    return str;
}

char *parseCommand(const char *cont, char *Iport) {
    int len = strlen(cont);
    //revome the last '\r\n'
    while(cont[len - 1] == '\r' || cont[len -1] == '\n') {
        --len;
    }
    //remove the tail, change the cont background
    len = len - strlen(Iport);
    //! need to free
    char *str = (char *)malloc(len + 1);
    memcpy(str, cont, len);
    str[len] = '\0';
    return str;
}



char *getIp(char *cont) {
    char *ip = (char *)malloc(sizeof(char) * 20);
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
    Segment *iterator = manager->segment;

    while (iterator != NULL) {
        if (strcmp(iterator->header.sin_addr, ip) == 0 && iterator->header.sin_port == port) {
            return iterator;
        } else {
            iterator = iterator->next;
        }
    }
    return iterator;
}


void setCapacity(Segment *seg ,int bits) {
    seg->header.capacity = seg->header.capacity - bits;
}

void setSegletNum(Segment *seg) {
    ++seg->header.segletnum;
}

int getSegletNum(Segment *seg) {
    return seg->header.segletnum;
}

Segment *getLastSegment(SegmentManager *manager) {
    Segment *iterator = manager->segment;

    while (iterator->next != NULL) {
        iterator = iterator->next;
    }
    return iterator;
}

void appendToSegment(char *cont) {//, struct in_addr addr, unsigned short port) {
    SegmentManager *Iterator = Manager;
    char *IpPort = parseIpPort(cont);
    char *rip = getIp(IpPort);
    int rport = getPort(IpPort);
    char *command = parseCommand(cont, IpPort);
    free(IpPort);

    //! if no Segment avaliable, try to create
    if (Iterator->used == false) {
        Iterator->segment = createSegment();
        Iterator->segment->next = NULL;
        setHead(Iterator->segment, rip, rport);
        Iterator->used = true; //! set current segment is using
        Iterator->segment->segleter = createSeglet(command);
        //! we only replace select length with command length roughly
        setCapacity(Iterator->segment, Iterator->segment->segleter->length);
        setSegletNum(Iterator->segment);
        Iterator->segment->p = Iterator->segment->segleter;
        Iterator->segment->segleter->next = NULL;
    } else {
        Segment *currSeg = getSegment(Iterator, rip, rport);
        if (currSeg == NULL) {
            currSeg = getLastSegment(Iterator);
            currSeg->next = createSegment();
            currSeg->next->next = NULL;
            setHead(currSeg->next, rip, rport);
            //Iterator->used = true; //! set current segment is using
            currSeg->next->segleter = createSeglet(command);
            //! we only replace select length with command length roughly
            setCapacity(currSeg->next, currSeg->next->segleter->length);
            setSegletNum(Iterator->segment);
            currSeg->next->p = currSeg->next->segleter;
            currSeg->next->segleter->next = NULL;
        } else {
            //! Ip existed so free it
            free(rip);
            Seglet *seglet = currSeg->p;
            seglet->next = createSeglet(command);
            //! we only replace select length with command length roughly
            setCapacity(currSeg, seglet->next->length);
            setSegletNum(Iterator->segment);
            seglet->next->next = NULL;
            currSeg->p = seglet->next;
        }

    }

    //++++++++ storage strategy
    //我们persist的策略是：我们先设定一个segment长度为8MB，直到存到某一次数据超错8MB，
    //即最后segment的capacity数值<=64，64是经验值，已经存不下一个seglet。
    //这个时候我们启动persist
    Segment *segIterator = Manager->segment;
    while(segIterator != NULL) {
        if (segIterator->header.capacity <= 8388585) { //8388585 just for test
            persist(Iterator->segment);

            //test!!
            if(segIterator->header.capacity <= 8388584) {
                Segment *tempSeg = loadToMem();
                fprintf(stderr, "%d\n", tempSeg->header.capacity);
            }
        }
        segIterator = segIterator->next;
    }

}

//++++++++++++++++++++++++++++++++++ storage ++++++++++++++++++++++++++++++++++++++++++//
/*
 * make a directory to storage data file
 */
char *mkStorage(char *dirname) {
    char *buf = (char *)malloc(128);
    if (getcwd(buf, 128) != 0) { //get current directory
        fprintf(stderr, "error to get cwd\n");
    }
    if (access("backup", F_OK) == -1) {
        fprintf(stderr, "directory not exist!\n");
        strcat(buf, "/backup/");
        mkdir(buf ,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        printf("%s has created!\n",buf);
    } else {
        strcat(buf, "/backup/");  //for change dir
    }
    if (chdir(buf) == 0) {
        fprintf(stderr, "successful chdir to %s\n", buf);
    }

    // chech segment directory whether exist
    if (access(dirname, F_OK) == -1) {
        fprintf(stderr, "%s is not exist!\n", dirname);
        strcat(buf, dirname);
        mkdir(buf ,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    } else {
        strcat(buf, dirname); //for return
    }

    return buf;
}

/*
 * move memory data to disk
 * directory name : 10.107.19.8.11211
 * filename format just like : 1024.1294201532.ram
 * segletNum . time . suffix
 */

int persist(Segment *seg) {
    FILE *fp;
    time_t t;
    time(&t); //! add time after filename

    char *maindir = (char *)malloc(128);
    if (getcwd(maindir, 128) != 0) { //get current directory
        fprintf(stderr, "error to ge cwd\n");
    }

    char filename[32] = "";
    char dirname[64] = "";
    char temp[40] = "";
    int segletNum = getSegletNum(seg);
    strcat(dirname, seg->header.sin_addr);
    sprintf(temp, ".%d", seg->header.sin_port);  //int to char
    strcat(dirname, temp);
    char *currdir = mkStorage(dirname);  //make a storage directory

    sprintf(filename, "%d.%ld.ram", segletNum, t);

    //go to target directory
    if (chdir(currdir) == 0) {
        fprintf(stderr, "successful chdir to %s\n", currdir);
    }
    free(currdir);
    if((fp = fopen(filename, "wb"))==NULL)
    {
        printf("Error to open!\n");
        return -1;
    }
    int totalSize = sizeof(Segment) + (sizeof(Seglet) + sizeof(Object)) * segletNum;
    fwrite(seg, totalSize,1,fp); /* 成块写入文件*/
    if (fflush(fp) == 0) {
        printf("successful!\n");
    }
    //sync();
    fclose(fp);

    //return main directory
    if (chdir(maindir) != 0) {
        fprintf(stderr, "error to change dir\n");
    }
    return 0;
}

//++++++++++++++++++++++++++++++++++ recovery ++++++++++++++++++++++++++++++++++++++++++//
Segment *loadToMem(void) {
    Segment *currSeg, *head;
    head = (Segment *)malloc(sizeof(Segment));
    currSeg = head;
    char *dirName = (char *)malloc(128);
    if (getcwd(dirName, 128) != 0) { //get current directory
        fprintf(stderr, "error to get cwd\n");
    }
    strcat(dirName, "/backup/");

    DIR *dir;
    struct dirent *ent;
    char *ptr;

    dir = opendir(dirName);
    if(dir == NULL)
    {
        fprintf(stderr,"Error : open directory %s \n",dirName);
        return head;
    }
    while( (ent = readdir(dir)) != NULL)
    {
        ptr = strrchr(ent->d_name,'.');
        if(ptr && (strcasecmp(ptr, ".ram") == 0)) {
            if(head->next == NULL) { //the first time should to set head
                currSeg = readFile(ent->d_name);   //read disk file to segment struct
                free(head);
                head = currSeg;
            } else {
                currSeg->next = readFile(ent->d_name);
                currSeg = currSeg->next;
                currSeg->next = NULL;
            }
        }
    }
    closedir(dir);
    return head;
}

Segment *readFile(char *fileName) {
    FILE *fp;
    Segment *seg = createSegment();
    if ((fp = fopen(fileName, "rb")) == NULL) {
        fprintf(stderr, "error to open %s\n", fileName);
        return seg;
    }
    int len = getSegmentLength(fileName);
    if (fread(seg, len, 1, fp) == 0) {
        fprintf(stderr, "error to read\n");
    }
    fclose(fp);
    return seg;
}

int getSegmentLength(char *str) {
    int i = 0;
    int length = 0;
    while(str[i] != '.') {
        length = length * 10 + (str[i] - '0');
    }
    return length;
}



























