#include "globals.h"
#include "commander.h"

#define RECV_Q_MAX 4

typedef struct command_tag {
	instruction inst;
	unsigned char glbl_id;
	unsigned char buf[512];
} command;

typedef struct commander_tag {
	unsigned char glbl_id;
	command *cmd; // current command in queue
} commander;

static const command NO_COMMAND = { 0 };
static command recv_queue[RECV_Q_MAX] = { 0 };
static unsigned char recv_queue_idx = 0;

// current player index will always be 0, however current player glbl id
// will change as necessary
static commander commanders[CLIENTS_MAX] = { 0 };

/**
 * The buffer will be loaded with the following,
 * and can be casted to the correct type given
 * the correct instruction.
 */

void commander_update(void)
{
	// receive commands
	if (recv_queue[recv_queue_idx].inst == INSTRUCTION_NONE) {
		commander_recv(&recv_queue[recv_queue_idx]);
	}
}

void commander_init(void)
{
	// If the client is connecting to a server,
	// ask for an id, else auto assign one

}

void commander_cleanup(void)
{
	// close anything
}

void commander_recv(command *cmd)
{
	// get player input
	// get network input
}

void commander_send(instruction inst)
{

}
