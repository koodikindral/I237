#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "../lib/helius_microrl/microrl.h"
#include "hmi_msg.h"
#include "cli_microrl.h"
#include "print_helper.h"
#include "../lib/andy_brown_memdebug/memdebug.h"
#include "../lib/matejx_avr_lib/mfrc522.h"

typedef struct card {
    char *uid;
    char *user;
    short size;
    struct card * next;
} card_t;

card_t * linkedlist = NULL;

void rfid_read(void)
{
    Uid uid;
    Uid *uid_ptr = &uid;

    if (PICC_IsNewCardPresent()) {
        uart0_puts("Card selected!\n\r");
        PICC_ReadCardSerial(uid_ptr);
        char ssize[2];
        itoa(uid.size, ssize, 10);
        uart0_puts("\n\r");
        uart0_puts("Card UID:");
        char uid_string[100] = "\0";

        for (byte i = 0; i < uid.size; i++) {
            char suid[20];
            itoa(uid.uidByte[i], suid, 10);
            strcat(uid_string, suid);
        }

        uart0_puts(uid_string);
        uart0_puts(" (");
        uart0_puts(ssize);
        uart0_puts(" Bytes)\r\n");
        uart0_puts("Card Type: ");
        uart0_puts(PICC_GetTypeName(PICC_GetType(uid.sak)));
        uart0_puts("\n\r\n\r");
    } else {
        uart0_puts("Error for selecting card.\n\r\n\r");
    }
}
void rfid_remove(const char *const *argv)
{
    card_t * current = linkedlist;
    card_t * temp;

    if (strcmp(linkedlist->uid, argv[1]) == 0) {
        temp = linkedlist->next;
        free(linkedlist);
        linkedlist = temp;
    }

    while (current != NULL) {
        if (strcmp(current->next->uid, argv[1]) == 0) {
            temp = current->next->next;
            free(current->next->uid);
            free(current->next->user);
            free(current->next);
            current->next = temp;
        }

        current = current->next;
    }
}

void rfid_add(const char *const *argv)
{
    if (linkedlist == NULL) {
        linkedlist = malloc(sizeof(card_t));

        if (linkedlist == NULL) {
            uart0_puts("Insufficent memory in linkedlist \r\n");
            return;
        }

        linkedlist->uid = malloc(strlen(argv[1]) + 1);

        if (linkedlist->uid == NULL) {
            uart0_puts("Insufficent memory in cards linkedlist \r\n");
            free(linkedlist);
            return;
        }

        strcpy(linkedlist->uid, argv[1]);
        linkedlist->user = malloc(strlen(argv[2]) + 1);

        if (linkedlist->user == NULL) {
            uart0_puts("Insufficent memory in users linkedlist \r\n");
            free(linkedlist->uid);
            free(linkedlist);
            return;
        }

        strcpy(linkedlist->user, argv[2]);
        linkedlist->size = strlen(argv[1]);
        linkedlist->next = NULL;
        uart0_puts("\r\nUID added: ");
        uart0_puts(linkedlist->uid);
        uart0_puts(", user: ");
        uart0_puts(linkedlist->user);
        uart0_puts("\r\n\r\n");
        return;
    }

    card_t * current = linkedlist;

    while (current != NULL) {
        if (strcmp(current->uid, argv[1]) == 0) {
            uart0_puts("\r\nDuplicate card: ");
            uart0_puts(current->user);
            uart0_puts("\r\n\r\n");
            return;
        }

        current = current->next;
    }

    current = linkedlist;

    while (current ->next != NULL) {
        current = current->next;
    }

    current->next = malloc(sizeof(card_t));
    current = current->next;
    current->uid = malloc(strlen(argv[1]) + 1);
    strcpy(current->uid, argv[1]);
    current->user = malloc(strlen(argv[2]) + 1);
    strcpy(current->user, argv[2]);
    current->size = strlen(argv[1]);
    current->next = NULL;
    uart0_puts("\r\nAdded UID ");
    uart0_puts(current->uid);
    uart0_puts(" for user ");
    uart0_puts(current->user);
    uart0_puts("\r\n\r\n");
}

void rfid_print_list()
{
    if (linkedlist == NULL) {
        uart0_puts("\r\nCard list is empy\r\n\r\n");
        return;
    }

    card_t * current = linkedlist;
    int count = 0;
    char counts[10];
    uart0_puts("\r\n");

    while (current != NULL) {
        count++;
        itoa(count, counts, 10);
        uart0_puts(counts);
        uart0_puts(". User: ");
        uart0_puts(current->user);
        uart0_puts(" UID: ");
        uart0_puts(current->uid);
        uart0_puts(" (");
        char sizes[10];
        itoa(current->size, sizes, 10);
        uart0_puts(sizes);
        uart0_puts(" Bytes)\r\n");
        current = current->next;
    }

    uart0_puts("\r\n");
}

char *find(char* search)
{
    card_t * current = linkedlist;

    while (current != NULL) {
        if (strcmp(current->uid, search) == 0) {
            return current->user;
        }

        current = current->next;
    }

    return NULL;
}