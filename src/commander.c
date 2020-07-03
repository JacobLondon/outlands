#include <stdarg.h>
#include <memory.h>
#include "globals.h"
#include "commander.h"
#include "context.h"
#include "ship_man.h"
#include "dude.h"
#include "key_man.h"
#include "scene_man.h"
#include "util.h"

#if 0

#define RECV_Q_MAX 32
#define SEND_Q_MAX 32

typedef struct command_tag {
	instruction inst;
	unsigned char glbl_id;
	unsigned char buf[64];
} command;

typedef struct commander_tag {
	unsigned char glbl_id;
	command *cmd; // current command in queue
} commander;

// TODO: Why send strings? just send id's or something

static struct {
	ship_type type;
	unsigned char count;
	char *species;
} args_dudes_load;

static struct {
	unsigned char id;
	int x;
	int y;
} args_dudes_assign;

static struct {
	char *name;
	unsigned type;
} args_ship_load;

static struct {
	unsigned idx;
} args_scene_select;

static struct {
	char **keys;
} args_key_load;


// the recv queue will need to receive from another thread, the only
// way to receive is from the network, all player commands into commander
// are through the miscellaneous code, so player (current client) actions
// don't need to flow from this interface, just to it as each shared state changes
static command recv_q[RECV_Q_MAX] = { 0 };
static unsigned char recv_q_idx = 0;
static command send_q[SEND_Q_MAX] = { 0 };
static unsigned char send_q_idx;

// current player index will always be 0, however current player glbl id
// will change as necessary
static commander commanders[CLIENTS_MAX] = { 0 };

static void package(instruction inst, void *buf, size_t size);

/**
 * The buffer will be loaded with the following,
 * and can be casted to the correct type given
 * the correct instruction.
 */

void commander_update(void)
{
	// process received commands
	if (recv_q[recv_q_idx].inst == INSTRUCTION_NONE) {
		commander_recv_process(&recv_q[recv_q_idx]);
	}

}

void commander_init(void)
{
	// If the client is connecting to a server,
	// ask for an id, else auto assign one
	memset(&args_dudes_load, 0, sizeof(args_dudes_load));
	memset(&args_dudes_assign, 0, sizeof(args_dudes_assign));
	memset(&args_ship_load, 0, sizeof(args_ship_load));
	memset(&args_scene_select, 0, sizeof(args_scene_select));
	memset(&args_key_load, 0, sizeof(args_key_load));

	commander_send(INSTRUCTION_CONNECT);
}

void commander_cleanup(void)
{
	// close anything
}

void commander_recv(command *cmd)
{

}

void commander_recv_process(command *cmd)
{
	// get network input
}

void commander_send(instruction inst, ...)
{
	va_list ap;

	va_start(ap, inst);
	switch (inst) {
		// actually put jobs in, only operate on updates
		case INSTRUCTION_RELOAD:
			context_reload();
			package(inst, NULL, 0);
			break;
		case INSTRUCTION_CONNECT:
			if (global_connect_to_network) {
				(void)0;
			}
			else {
				global_id = 0;
			}
			break;
		case INSTRUCTION_DISCONNECT:
			break;
		case INSTRUCTION_DUDES_INIT:
			break;
		case INSTRUCTION_DUDES_LOAD:
			args_dudes_load.type = va_arg(ap, unsigned);
			args_dudes_load.species = va_arg(ap, char *);
			args_dudes_load.count = va_arg(ap, unsigned);
			dude_load(args_dudes_load.count, args_dudes_load.species, ship_man_get(args_dudes_load.type));
			package(inst, &args_dudes_load, sizeof(args_dudes_load));
			break;
		case INSTRUCTION_DUDES_ASSIGN:
			args_dudes_assign.id = va_arg(ap, unsigned char);
			args_dudes_assign.x = va_arg(ap, int);
			args_dudes_assign.y = va_arg(ap, int);
			dude_job_assign(args_dudes_assign.id, args_dudes_assign.x, args_dudes_assign.y);
			package(inst, &args_dudes_assign, sizeof(args_dudes_assign));
			break;
		case INSTRUCTION_SHIP_LOAD:
			args_ship_load.name = va_arg(ap, char *);
			args_ship_load.type = va_arg(ap, unsigned);
			(void)ship_man_load(args_ship_load.name, args_ship_load.type);
			package(inst, &args_ship_load, sizeof(args_ship_load));
			break;
		case INSTRUCTION_SCENE_SELECT:
			args_scene_select.idx = va_arg(ap, unsigned);
			context_set_scene(args_scene_select.idx);
			package(inst, &args_scene_select, sizeof(args_scene_select));
			break;
		case INSTRUCTION_KEY_LOAD:
			args_key_load.keys = va_arg(ap, char **);
			key_man_load(args_key_load.keys);
			package(inst, &args_key_load, sizeof(args_key_load));
			break;
		default:
			msg_assert(0, "Invalid instruction");
	}
	va_end(ap);
}

static void package(instruction inst, void *buf, size_t size)
{
	send_q[send_q_idx].inst = inst;
	send_q[send_q_idx].glbl_id = global_id;
	if (buf) {
		assert(sizeof(send_q[send_q_idx].buf) >= size);
		memcpy(send_q[send_q_idx].buf, buf, size);
	}

	send_q_idx = (send_q_idx + 1) % SEND_Q_MAX;
}

#endif // 0
