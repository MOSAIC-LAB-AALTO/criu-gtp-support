#ifndef __CR_PROTOBUF_H__
#define __CR_PROTOBUF_H__

#include "asm/types.h"
#include "compiler.h"
#include "util.h"

enum {
	PB_INVENTORY,
	PB_FDINFO,
	PB_CORE,
	PB_MM,
	PB_VMAS,
	PB_SIGACT,
	PB_ITIMERS,
	PB_CREDS,
	PB_FS,
	PB_UTSNS,
	PB_IPCNS_VAR,
	PB_IPCNS_SHM,
	PB_IPCNS_MSG,
	PB_IPCNS_MSG_ENT,
	PB_IPCNS_SEM,
	PB_MOUNTPOINTS,
	PB_NETDEV,
	PB_PSTREE,
	PB_GHOST_FILE,
	PB_TCP_STREAM,
	PB_SK_QUEUES,
	PB_REG_FILES,
	PB_INETSK,
	PB_UNIXSK,
	PB_PACKETSK,
	PB_PIPES,
	PB_FIFO,
	PB_PIPES_DATA,
	PB_REMAP_FPATH,
	PB_EVENTFD,
	PB_EVENTPOLL,
	PB_EVENTPOLL_TFD,
	PB_SIGNALFD,
	PB_INOTIFY,
	PB_INOTIFY_WD,
	PB_FANOTIFY,
	PB_FANOTIFY_MARK,
	PB_TTY,
	PB_TTY_INFO,
	PB_FILE_LOCK,
	PB_RLIMIT,
	PB_IDS,

	PB_MAX
};
/*
 * ATTENTION
 *
 * This typdefs represent "generic" prototypes for
 * pack/unpack/getsize functions generated by PB
 * engine, thus (!!!) if PB engine change arguments
 * order or their number we may meet serious problems.
 *
 * FIXME
 *
 * Find a way to verify PB generated functions statemens
 * to match this typedefs.
 */

void cr_pb_init(void);

extern int do_pb_read_one(int fd, void **objp, int type, bool eof);

#define pb_read_one(fd, objp, type) do_pb_read_one(fd, (void **)objp, type, false)
#define pb_read_one_eof(fd, objp, type) do_pb_read_one(fd, (void **)objp, type, true)

extern int pb_write_one(int fd, void *obj, int type);

#define pb_pksize(__obj, __proto_message_name)						\
	(__proto_message_name ##__get_packed_size(__obj) + sizeof(u32))

#define pb_repeated_size(__obj, __member)						\
	(sizeof(*(__obj)->__member) * (__obj)->n_ ##__member)

#define pb_msg(__base, __type)			\
	container_of(__base, __type, base)

#include <google/protobuf-c/protobuf-c.h>

extern void do_pb_show_plain(int fd, int type, int single_entry,
		void (*payload_hadler)(int fd, void *obj, int flags),
		int flags, const char *pretty_fmt);

/* Don't have objects at hands to also do typechecking here */
#define pb_show_plain_payload_pretty(__fd, __type, payload_hadler, flags, pretty)	\
	do_pb_show_plain(__fd, __type, 0, payload_hadler, flags, pretty)

#define pb_show_plain_payload(__fd, __proto_message_name, payload_hadler, flags)	\
	pb_show_plain_payload_pretty(__fd, __proto_message_name, payload_hadler, flags, NULL)

#define pb_show_plain_pretty(__fd, __proto_message_name, __pretty)		\
	pb_show_plain_payload_pretty(__fd, __proto_message_name, NULL, 0, __pretty)

#define pb_show_plain(__fd, __type)							\
	pb_show_plain_payload(__fd, __type, NULL, 0)

#define pb_show_vertical(__fd, __type)							\
	do_pb_show_plain(__fd, __type, 1, NULL, 0, NULL)

int collect_image(int fd_t, int obj_t, unsigned size,
		int (*collect)(void *obj, ProtobufCMessage *msg));
int collect_image_sh(int fd_t, int obj_t, unsigned size,
		int (*collect)(void *obj, ProtobufCMessage *msg));

#endif /* __CR_PROTOBUF_H__ */
