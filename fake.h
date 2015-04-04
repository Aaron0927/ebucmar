typedef unsigned long size_t;
typedef short uint16_t;
//#define FAKE 0

struct hostent
{
  char *h_name;			/* Official name of host.  */
  char **h_aliases;		/* Alias list.  */
  int h_addrtype;		/* Host address type.  */
  int h_length;			/* Length of address.  */
  char **h_addr_list;		/* List of addresses from name server.  */
#define	h_addr	h_addr_list[0] /* Address, for backward compatibility.*/
};


char *strtok (char *__s, char *__delim);
int strcmp (char *__s1, char *__s2);
char *strcpy (char *__dest, char *__src);

int atoi (char *__nptr);

void *memset (void *__s, int __c, size_t __n);

void *memcpy (void *__dest, void *__src, size_t __n);

#define isalpha(c)	__isctype((c), _ISalpha);

uint16_t htons (uint16_t __hostshort);
uint16_t ntohs (uint16_t __netshort);

#define	PF_INET		2	/* IP protocol family.  */
#define	AF_INET		PF_INET

char *inet_ntoa(struct in_addr __in);
in_addr_t inet_addr (char *__cp);

#define evutil_socket_t int

/**
  Shared implementation of a bufferevent.

  This type is exposed only because it was exposed in previous versions,
  and some people's code may rely on manipulating it.  Otherwise, you
  should really not rely on the layout, size, or contents of this structure:
  it is fairly volatile, and WILL change in future versions of the code.
**/
struct bufferevent {
	/** Event base for which this bufferevent was created. */
	struct event_base *ev_base;
	/** Pointer to a table of function pointers to set up how this
	    bufferevent behaves. */
	const struct bufferevent_ops *be_ops;

	/** A read event that triggers when a timeout has happened or a socket
	    is ready to read data.  Only used by some subtypes of
	    bufferevent. */
	struct event ev_read;
	/** A write event that triggers when a timeout has happened or a socket
	    is ready to write data.  Only used by some subtypes of
	    bufferevent. */
	struct event ev_write;

	/** An input buffer. Only the bufferevent is allowed to add data to
	    this buffer, though the user is allowed to drain it. */
	struct evbuffer *input;

	/** An input buffer. Only the bufferevent is allowed to drain data
	    from this buffer, though the user is allowed to add it. */
	struct evbuffer *output;

	struct event_watermark wm_read;
	struct event_watermark wm_write;

	bufferevent_data_cb readcb;
	bufferevent_data_cb writecb;
	/* This should be called 'eventcb', but renaming it would break
	 * backward compatibility */
	bufferevent_event_cb errorcb;
	void *cbarg;

	struct timeval timeout_read;
	struct timeval timeout_write;

	/** Events that are currently enabled: currently EV_READ and EV_WRITE
	    are supported. */
	short enabled;
};

typedef struct conn {
    int    sfd;
    sasl_conn_t *sasl_conn;
    enum conn_states  state;
    enum bin_substates substate;
    struct event event;
    short  ev_flags;
    short  which;   /** which events were just triggered */

    char   *rbuf;   /** buffer to read commands into */
    char   *rcurr;  /** but if we parsed some already, this is where we stopped */
    int    rsize;   /** total allocated size of rbuf */
    int    rbytes;  /** how much data, starting from rcur, do we have unparsed */

    char   *wbuf;
    char   *wcurr;
    int    wsize;
    int    wbytes;
    /** which state to go into after finishing current write */
    enum conn_states  write_and_go;
    void   *write_and_free; /** free this memory after finishing writing */

    char   *ritem;  /** when we read in an item's value, it goes here */
    int    rlbytes;

    /* data for the nread state */

    /**
     * item is used to hold an item structure created after reading the command
     * line of set/add/replace commands, but before we finished reading the actual
     * data. The data is read into ITEM_data(item) to avoid extra copying.
     */

    void   *item;     /* for commands set/add/replace  */

    /* data for the swallow state */
    int    sbytes;    /* how many bytes to swallow */

    /* data for the mwrite state */
    struct iovec *iov;
    int    iovsize;   /* number of elements allocated in iov[] */
    int    iovused;   /* number of elements used in iov[] */

    struct msghdr *msglist;
    int    msgsize;   /* number of elements allocated in msglist[] */
    int    msgused;   /* number of elements used in msglist[] */
    int    msgcurr;   /* element in msglist[] being transmitted now */
    int    msgbytes;  /* number of bytes in current msg */

    item   **ilist;   /* list of items to write out */
    int    isize;
    item   **icurr;
    int    ileft;

    char   **suffixlist;
    int    suffixsize;
    char   **suffixcurr;
    int    suffixleft;

    enum protocol protocol;   /* which protocol this connection speaks */
    enum network_transport transport; /* what transport is used by this connection */

    /* data for UDP clients */
    int    request_id; /* Incoming UDP request ID, if this is a UDP "connection" */
    struct sockaddr request_addr; /* Who sent the most recent request */
    socklen_t request_addr_size;
    unsigned char *hdrbuf; /* udp packet headers */
    int    hdrsize;   /* number of headers' worth of space is allocated */

    bool   noreply;   /* True if the reply should not be sent. */
    /* current stats command */
    struct {
        char *buffer;
        size_t size;
        size_t offset;
    } stats;

    /* Binary protocol stuff */
    /* This is where the binary header goes */
    protocol_binary_request_header binary_header;
    uint64_t cas; /* the cas to return */
    short cmd; /* current command being processed */
    int opaque;
    int keylen;
    conn   *next;     /* Used for generating a list of conn structures */
    LIBEVENT_THREAD *thread; /* Pointer to the thread object serving this connection */
	void *ramcube_proxy; /* for ramcube use ONLY */
}conn_t;