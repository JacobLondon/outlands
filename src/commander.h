#ifndef OUTLANDS_COMMANDER_H
#define OUTLANDS_COMMANDER_H

#if 0

typedef struct commander_tag commander;
typedef struct command_tag command;

void commander_update(void);

/**
 * Each module which needs to send/receive thru a commander
 * needs to have a get_commmanded / set_commanded
 * where get_commanded would fill a commanded buf and give to commander
 * and set_commanded would received a buf of bytes and put it into its state
 */

// TODO: Each of these needs to correspond with a public 'context.h' function
typedef enum instruction_tag {
	INSTRUCTION_NONE,
	INSTRUCTION_RELOAD,
	INSTRUCTION_CONNECT,
	INSTRUCTION_DISCONNECT,
	INSTRUCTION_DUDES_INIT,
	INSTRUCTION_DUDES_LOAD,
	INSTRUCTION_DUDES_ASSIGN,
	INSTRUCTION_SHIP_LOAD,
	INSTRUCTION_SCENE_SELECT,
	INSTRUCTION_KEY_LOAD,
	INSTRUCTION_COUNT
} instruction;

/**
 * A client sending an instruction is different than a client receiving one.
 * Client Send:
 * - CONNECT
 * - RELOAD
 * 
 * Client Receive:
 * - CONNECT 1=commander::id 
 * - RELOAD
 */

void commander_init(void);
void commander_cleanup(void);

/**
 * When a receive happens, it is allowed to modify
 * ANYTHING in the local state depending on the
 * instruction
 */
void commander_recv(command *cmd);
void commander_recv_process(command *cmd);
/**
 * When a send happens, it may ONLY contain
 * local state information, regarding the
 * global player id
 */
void commander_send(instruction inst, ...);

#endif // 0

#endif // OUTLANDS_COMMANDER_H
