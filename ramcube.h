/* ramcube related codes */
#ifndef RAMCUBE_H
#define RAMCUBE_H

void ramcube_adjust_memcached_settings(struct settings *);
void ramcube_init(struct event_base *, const struct settings *);
ulong ramcube_process_commands(conn *, void *, const size_t, char *left_com);
int ramcube_server_proxy_new(conn *);
ulong ramcube_post_set_data(conn *);


extern char *ramcube_config_file; 
//++++++++++++++++++++++++++++++++++ recovery ++++++++++++++++++++++++++++++++++++++//
void recoveryBackupConnect(char *Ip, int port, struct event_base *pBase);
//客户端发送心跳包
void HeartBeat(char *Ip, int port, struct event_base *pBase);
void HB_cb(int fd, short ev, void *arg);
#endif // #ifndef RAMCUBE_H
