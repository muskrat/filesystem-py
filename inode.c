/* Filesystem ideas from Python program rewritten in C
 *
 * I K Stead, 23-09-2012
 *
 * Something I can't decide is what values/data types are best to pass
 * around the program. It seems like I could proceed in two ways: 
 * 
 * - Have all interactions with inodes through the table, with functions 
 *   that take a Table * arg and an index. 
 * 
 * - Have a function to look up an inode in the table, returning an 
 *   Inode *, and functions which accept an Inode * and perform 
 *   manipulations on it. 
 * 
 * I might try and code up both and then set out the pros and cons.
 *
 * !!! I have to look inodes up anyway, before I can use the index. So
 * might as well do it the second way.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int FSIZE = 1000;
const int MAXFILES = 1000;
const int START_INDEX = 0;

typedef struct inode_struct {  /* Contains all file data except for name */
    int id;
    time_t last_mod;
    char data[FSIZE];
} Inode;

typedef struct inode_table {   /* Indexes all inodes. */
    int next_free_index;
    Inode *table[MAXFILES];
} Table;

/* Return pointer to a new inode table. */
Table *table_init()    
{
    Table *new_t = malloc(sizeof(Table));
    new_t-> next_free_index = START_INDEX;

    return new_t;
}

/* Allocate memory for a new inode and add its pointer to the table. */
void table_alloc_inode(Table *table)
{
    int i = table->next_free_index;
    table-> next_free_index++;
    Inode *node = malloc(sizeof(Inode));
    table-> table[i] = node;
}

/* Return inode pointer at given index in table. 
 * TODO: better error checking 
 */
Inode *table_get_inode(Table *table, int index)
{
    if (index < table-> next_free_index) 
        return table-> table[index];
    else 
        return NULL;
}

/* Print table to stdout for debugging purposes. */
void table_print(Table *table)
{
    int i;
    for (i = 0; i < table-> next_free_index; i++) {
        Inode *inode = table-> table[i];
        printf("Inode at index %i:\n  Data: %s\n", i, inode-> data);
    }
}

/* Deallocate inode at given address. TODO: Find out how to fill the 
 * "holes" this leaves in the table.
 */
void table_free_inode(Table *table, int index)
{
    Inode *inode = table-> table[index];
    free(inode);
}

/* Destroy all inodes, then destroy table */
void table_rinse(Table *table)
{
    int i;
    for (i = 0; i < table-> next_free_index; i++) {
        free(table-> table[i]); /* What if there is no inode at table[i]? */
    }
    free(table);
}

/* Copy a string to the data field of given inode. This will overwrite
 * any data that was in there already...
 */
void inode_set_data(Table *table, int index, char string[])
{
    if (strlen(string) < FSIZE) {
        Inode *inode = table-> table[index];
        strcpy(inode-> data, string);
    }
}


int main()
{
    Table *table = table_init();
    
    char input;
    while((input = getchar()) != EOF) {
        if (input == 'm') {
            table_alloc_inode(table);
        } else if (input == 'd') {
            table_print(table);
        }
    }

    /* Clean up */
    table_rinse(table);

    return EXIT_SUCCESS;
}
