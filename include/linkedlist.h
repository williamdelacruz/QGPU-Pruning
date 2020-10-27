#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdlib.h>
#include <stdio.h>

/*
 *   Data structure of a node
 */
typedef struct nodeList_ {
    int index;
    float q;
    struct nodeList_ *sig;
} nodeList;


// Prototypes

nodeList *new_node(int, float, nodeList *);
int insertIni(int, float, nodeList **, nodeList **, nodeList *);
int insertIni(int, nodeList **, nodeList *);
int insertEnd(int, float, nodeList **, nodeList **, nodeList *);
int removeFirst(int *, nodeList **, nodeList **);
int insertSort(int, float, nodeList **, nodeList **, nodeList *);


#endif // LINKEDLIST_H
