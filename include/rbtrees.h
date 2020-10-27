#ifndef RBTREES_H
#define RBTREES_H

#include <stdlib.h>
#include <stdio.h>

typedef struct Node
{
    float key;
    int color;
    int index;
    struct Node *right, *left, *p;
}node;


typedef struct Tree
{
  node *t, *h;
}tree;


class RBTrees
{
    public:

        node *RBT;        // creaci0n del arbol general
        node *Tnill;      // nodo centineal
        node *higher;     
        int num_nodes;
        int c,cc,ban;

   
        RBTrees(int dim1, int dim2);
        virtual ~RBTrees();

  
        void RB_insert_node(float, int);
        void printInorden();
        void RB_delete_higher_node(node **, node **);
        void print_trees();
        void check_pruning();
        void get_higher(float *, int *);
    
       
    private:


        node *List_nodes; // asignacion de memoria para los elementos del arbol
	tree *Trees;

        int dim, dimy;

        
        node *node_Tnill();
        node *new_node(float,int);
        void left_rotate(node **, node *);
        void right_rotate(node **, node *);
        void RB_tree_fixup(node**, node *);
        void inorden(node *);
        void insert_node(node **, node *);
        void RB_transplant(node **, node *, node *);
        void RB_delete_node(node **, node *);
        void RB_delete_fixup(node **, node *);
        node *tree_minimum(node *);
        void inicialize_Trees();
        void dividedTree();
        void count_nodes(node *, int *);
        void dividedTreeRight(int);
};


#endif // RBTREES_H
