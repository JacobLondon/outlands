#ifndef OUTLANDS_KEY_MANAGER_H_
#define OUTLANDS_KEY_MANAGER_H_

/* Manage groupings of key objects. Call
 * update on many key objects at once
 * for convenience.
 * Key objects can still be manually
 * managed however.
 * 
 * The key manager has its own internal
 * state as a file module. This should
 * be the highest level of key object
 * hierarchy.
 */

void key_man_init(void);
void key_man_cleanup(void);

/* load static key object initializations by name, end with NULL term */
void key_man_load(char **names);

/* call update on all key objects */
void key_man_update(void);

#endif // OUTLANDS_KEY_MANAGER_H_
