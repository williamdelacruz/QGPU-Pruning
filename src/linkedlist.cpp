#include "linkedlist.h"


/*
 *   Create a new node
 */
nodeList *new_node(int index, float q, nodeList *list_nodes)
{
    list_nodes[index].index = index;
    list_nodes[index].q     = q;
    list_nodes[index].sig   = NULL;
    return &list_nodes[index];
}



/*
 *   Insert at the beginning of the list
 */
int insertIni(int index, float q, nodeList **list, nodeList **last, nodeList *list_nodes)
{
    nodeList *nuevo = new_node(index, q, list_nodes);
    
    if (*list==NULL) {
        *list = nuevo;
        *last = *list;
    }
    else {
        nuevo->sig = *list;
        *list = nuevo;
    }
    
    return 0;
}



/*
 *   Insert at the beginning of the list
 */
int insertIni(int index, nodeList **list, nodeList *list_nodes)
{
    nodeList *nuevo = new_node(index, 0, list_nodes);
    
    if (*list==NULL)
        *list = nuevo;
    else {
        nuevo->sig = *list;
        *list = nuevo;
    }
    
    return 0;
}



/*
 *   Insert at the end of the list
 */
int insertEnd(int index, float q, nodeList **list, nodeList **last, nodeList *list_nodes)
{
    nodeList *nuevo = new_node(index, q, list_nodes);
    
    if (*list==NULL) {
        *list = nuevo;
        *last = *list;
    }
    else {
        (*last)->sig = nuevo;
        *last = nuevo;
    }
    
    return 0;
}



/*
 *    Remove the first element of the list
 */
int removeFirst(int *index, nodeList **list, nodeList **last)
{
    if (*list==NULL)
        return -1;
    else {
        *index = (*list)->index;
        *list = (*list)->sig;
        if (*list==NULL)
            *last = NULL;
    }
    
    return 0;
}


/*
 *    Insert items and sort
 */
int insertSort(int index, float q, nodeList **list, nodeList **last, nodeList *list_nodes)
{
    nodeList *nuevo;
    nodeList *ptr1, *ptr2;
    int flag;
    
    // list is empty
    if (*list==NULL)
    {
        nuevo = new_node(index, q, list_nodes);
        *list = nuevo;
        *last = *list;
    }
    else {
        // insert at the beginning
        if (q >= (*list)->q)
        {
            insertIni(index, q, list, last, list_nodes);
        }
        else
        {
            // insert at the end
            if (q <= (*last)->q)
            {
                insertEnd(index, q, list, last, list_nodes);
            }
            else
            {   // insert in the meedle
                ptr1 = *list;
                ptr2 = *list;
                
                while (ptr1->sig!=NULL && q <= ptr1->q) {
                    ptr2 = ptr1;
                    ptr1 = ptr1->sig;
                }
                
                nuevo = new_node(index, q, list_nodes);
                ptr2->sig = nuevo;
                nuevo->sig = ptr1;
            }
        }
    }
    
    return 0;
}
