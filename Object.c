/*************************************************************************
	> File Name: Object.c
    > Author: Aaron
    > Mail: chengfeizh@gmail.com
	> Created Time: 2015年03月31日 星期二 11时04分09秒
 ************************************************************************/

#include <stdio.h>
#include <Object.h>
#include <string.h>
#include <stdlib.h>
Object Obj;


void init_object(void) {
    memset(&Obj, 0, sizeof(Obj));
    return;
}

int  setLength(int ntotal, Object *obj) {
    return 0;
}

void setCommandClient(char *com, Object *obj) {
    strcpy(obj->command, com);
}

Object setCommandServer(char *key, char *data, int nbyte) {
    char buf[1024] =  "";
    char str[10];
    sprintf(str, "%d", nbyte);
    strcat(buf, "set ");
    strcat(buf, key);
    strcat(buf, " 0 0 ");
    strcat(buf, str);
    strcat(buf, "\r\n");
    strcat(buf, data);
    strcpy(Obj.command, buf);
    //memcpy(Obj.command, buf, strlen(buf));
    //free(buf);
    return Obj;
}
