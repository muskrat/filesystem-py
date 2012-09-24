/* Filesystem ideas from Python program rewritten in C
 *
 * I K Stead, 23-09-2012
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int FSIZE = 1000;
const int MAXFILES = 1000;
const int START_INDEX = 0;
const int MAX_CMD_LEN = 128;

const int MYDIR = 1;
const int MYFILE = 2;

typedef struct inode_struct {  /* Contains all file data except for name */
    int id;
    int type;
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
        if (inode != NULL) { /* i.e. node at index hasn't been deleted */
            printf("Inode at index %i:\n  Data: %s\n", i, inode-> data);
        }
    }
}

/* Deallocate inode at given address. */
void table_del_inode(Table *table, int index)
{
    Inode *inode = table-> table[index];
    /* If we freed this pointer, table_rinse would give double free error. */
    inode = NULL;
}

/* Destroy all inodes, then destroy table */
void table_rinse(Table *table)
{
    int i;
    for (i = 0; i < table-> next_free_index; i++) {
        free(table-> table[i]);
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
    
    /* Command interpreter */
    char input[MAX_CMD_LEN];
    while (strcmp(input, "quit") != 0) {
        gets(input);
        if (strcmp(input, "make") == 0) {
            table_alloc_inode(table);
        } else if (strcmp(input, "show") == 0) {
            table_print(table);
        } else if (strcmp(input, "test") == 0) {
            table_del_inode(table, 0);
        } else if (strcmp(input, "set") == 0) {
            inode_set_data(table, 1, "gangbang fetish");
        }
    }

    /* Clean up */
    table_rinse(table);

    return EXIT_SUCCESS;
}
