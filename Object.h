/*************************************************************************
	> File Name: Object.h
	> Author: Aaron
	> Mail: chengfeizh@gmail.com
	> Created Time: 2015年03月31日 星期二 11时04分01秒
 ************************************************************************/
//here
//what
#ifndef _OBJECT_H
#define _OBJECT_H

#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
/**
 * the object structure as follows:
 *
 * +-----------+-----------+---------+----------------+--------+
 * | avaliable | timestamp | command | starDataOffset | length |
 * +-----------+-----------+---------+----------------+--------+
 *
 * avaliable(bool) : object whether avaliable now, 1 is yes , 0 is no 
 * timestamp(time_t) : time of object was created
 * command(string) : record full command, example : set key1 0 0 3 
 *                                                  fei
 * length(int) : the length of an object structure 
 */


typedef struct object {
    bool avaliable;
    time_t timestamp;
    char command[1024];
} Object;

void init_object(void);

bool getAvaliable(const Object *obj);
time_t getTimestamp(const Object *obj);
void setAvaliable(bool avaliable);
void setTimestamp(time_t timestamp);
Object setCommandServer(char *key, char *data, int nbyte);
int  setLength(int ntotal, Object *obj);
void setCommandClient(char *com, Object *obj);
#endif
