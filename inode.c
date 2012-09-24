/* Filesystem ideas from Python program rewritten in C
 *
 * I K Stead, 23-09-2012
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FSIZE       1000
#define MAXFILES    1000

struct inode {
    char data[FSIZE];
};

static struct inode *TABLE[MAXFILES]; /* Table of references to inodes */
static int NEXT_FREE_INDEX = 0;

/* Allocate memory for a new inode and return pointer to it */
struct inode *inode_alloc()
{
    struct inode* i = malloc(sizeof(struct inode));
    return i;
}
/* Free the inode at given address */
void inode_dealloc(struct inode *node)
{
    free(node);
}
/* Write a string to given inode's data field */
void inode_write(struct inode *node, char string[])
{
    if (strlen(string) < FSIZE) {
    strcpy(node->data, string);
    }
}
/* Print inode contents to stdout */
void inode_print(struct inode *node)
{
    char *data = node->data;
    printf("\nData : %s \n\n", data);
}
/* Add inode pointer to table */
void inodetable_add(struct inode *node)
{
    if (NEXT_FREE_INDEX < MAXFILES) {
        TABLE[NEXT_FREE_INDEX] = node;
        NEXT_FREE_INDEX++;
    }
}
/* Print inode table contents to stdout */
void inodetable_print()
{
    int i;
    for (i = 0; i < NEXT_FREE_INDEX; i++) {
        struct inode *node = TABLE[i];
        printf("Inode at index %i: \n", i);
        printf("\tData : %s \n", node->data);
    }
}

int main()
{
    struct inode* node = inode_alloc();

    inode_write(node, "gangbang");
    inodetable_add(node);
    inodetable_print();
    inode_dealloc(node);
    return EXIT_SUCCESS;
}
