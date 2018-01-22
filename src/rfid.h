#ifndef _RFID_H_
#define _RFID_H_

char *find(char* search);
void rfid_add(const char *const *argv);
void rfid_print_list();
void rfid_read();
void rfid_remove(const char *const *argv);
#endif /* _RFID_H_ */
