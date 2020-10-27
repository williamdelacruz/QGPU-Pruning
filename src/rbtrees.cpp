#include "rbtrees.h"

RBTrees::RBTrees(int dim1, int dim2)
{
    Tnill  = node_Tnill();
    RBT    = Tnill;
    higher = Tnill;
    dim    = dim1;
    dimy   = dim2;
    num_nodes = 0;
    c  = 0;
    cc = 0;
    
    List_nodes	= (node *)malloc(sizeof(node)*dim1*dim2);
    Trees	    = (tree *)malloc (sizeof(tree)*dim1*dim2);
    inicialize_Trees();
}


RBTrees::~RBTrees()
{
    free(List_nodes);
    free(Trees);
}




/********************************************************
* Funciones que apoyan al Unwrapping a través de la poda
  que usan las funciones generales de inserción y
  eliminación de los arboles RBT
********************************************************/

void RBTrees::inicialize_Trees()
{
    int i,dimension;
    dimension=dim*dimy;
    
    for (i=0;i<dimension;i++)
    {
        Trees[i].h   = Tnill;
        Trees[i].t   = Tnill;
    }
}


void RBTrees::count_nodes(node *z, int *x)
{   node *y;
 
    if (z!=Tnill)
    {
        count_nodes(z->left,x);
        *x=*x+1;
        count_nodes(z->right,x);
    }
}


void RBTrees::RB_insert_node(float k, int index)
{
    node *z;

    z = new_node(k,index);
    insert_node(&RBT,z);

    if (k >= higher->key)
        higher=z;
    

    //si el numero de nodos alcanza el limite, poda el arbol
    num_nodes++;
    
    if (num_nodes == dim*2)
        dividedTree();
}



void RBTrees::dividedTree()
{
    node *x, *y;

    y=RBT;
    x=RBT->right;
    x->p=Tnill;

    //construir el arbol temporal
    Trees[c].t=y->left;
    Trees[c].t->p=Tnill;
    Trees[c].h=y;

    y->left=Tnill;
    y->right=Tnill;

    RBT=x;

    RBT->color=0;
    Trees[c].t->color=0;
    y->color=1;

    insert_node(&Trees[c].t,y);
    c++;
    num_nodes=0;
    count_nodes(RBT,&num_nodes);
}




void RBTrees::RB_delete_higher_node(node **T, node **x)
{
    node *z;
    z = (*x);

    if ((*x)->left != Tnill)
        (*x) = (*x)->left;
    else
        (*x) = (*x)->p;

    RB_delete_node(T,z);
}


void RBTrees::get_higher(float *h, int *index)
{
    *h  = higher->key;
    *index = higher->index;
}


void RBTrees::print_trees()
{
    int i,j=0;
  
    printf("\nExisten %d subarboles",c);

  
  for (i=j;i<c;i++)
       {printf("\n subarbol %d \n", i);
        inorden(Trees[i].t);
        getchar();
       } 
}

/*
void RBTrees::dividedTreeRight(int i)
{
    node *y, *x;
    
    y = Trees[i].t;

    if (y->right != Tnill)
        { RBT        = y->right;
          y->right   = Tnill;
          RBT->p     = Tnill;
          RBT->color = 0;
	  higher     = Trees[i].h;
    	  num_nodes  = 0;
	  count_nodes(RBT,&num_nodes);  
        }
    else
       if (y->left != Tnill)
          { RBT        = y->left;
            y->left    = Tnill;
            RBT->p     = Tnill;
            RBT->color = 0;
            y->color   = 1;
	    higher     = y;
            
            insert_node(&RBT,higher);
    	    num_nodes=0;
	    count_nodes(RBT,&num_nodes); 
            cc++;             
          } 
}    

*/

void RBTrees::dividedTreeRight(int i)
{
    node *y, *x;
    
    y= Trees[i].t;
    if (y->right != Tnill)
        { RBT        = y->right;
          y->right   = Tnill;
          RBT->p     = Tnill;
          RBT->color = 0;
	  higher     = Trees[i].h;
    	  num_nodes  = 0;
	  count_nodes(RBT,&num_nodes);  
        }
    else
       if (y->left != Tnill)
          { RBT        = y->left;
            y->left    = Tnill;
            RBT->p     = Tnill;
            RBT->color = 0;
            x = RBT;
            while (x->right != Tnill)
                   x = x->right;
	    higher = x;
    	    num_nodes=0;
	    count_nodes(RBT,&num_nodes);              
          }
       else 
           {
             RBT = y;
             higher = RBT; 
             num_nodes=1;
             cc++;
           }
}    


void RBTrees::check_pruning()
{ 
  if (cc<c) 
      dividedTreeRight(cc);         
}


/***********************************************
 *  Funciones de inicialización de los arboles
 *  y del nodo centinela
 ***********************************************/


node* RBTrees::node_Tnill()
{
    node * x;
    x=(node *) malloc(sizeof(node));
    x->key= -1;
    x->index= -1;
    x->left=NULL;
    x->right=NULL;
    x->p=NULL;
    x->color=0;
    return x;
}



node* RBTrees::new_node(float k, int index)
{
    List_nodes[index].key=k;
    List_nodes[index].index= index;
    List_nodes[index].left=Tnill;
    List_nodes[index].right=Tnill;
    List_nodes[index].p=Tnill;
    List_nodes[index].color=1;
    return &List_nodes[index];
}



/**************************************************
 *     Funciones para insertar un nodo en el arbol
 **************************************************/


void RBTrees::insert_node(node **T, node *z)
{
    node *x, *y;
    
    if (*T==Tnill)
    {
        (*T)=z;
        (*T)->color=0;
    }
    else
    {
        x=(*T);
        while (x!=Tnill)
        {
            y=x;
            if (z->key < x->key)
                x=x->left;
            else
                x=x->right;
        }
        if (z->key < y->key)
            y->left=z; //si es hijo izquierdo de y
        else
            y->right=z; //si es hijo derecho de y

        z->p=y;  //se ininicialize_Trees()dica el padre del nodo insertado
    }
    
    RB_tree_fixup(T,z);
 }



void RBTrees::RB_tree_fixup(node ** T, node *z)
{
    node *y;
  
    while (z->p->color == 1)
    {
        if (z->p == z->p->p->left)
        {
            y=z->p->p->right;
            if (y->color==1)
            {
                z->p->color = 0;
                y->color=0;
                z->p->p->color=1;
                z=z->p->p;
            }
            else
            {
                if (z==z->p->right)
                {
                    z=z->p;
                    left_rotate(T,z);
                }
                z->p->color=0;
                z->p->p->color=1;
                right_rotate(T,z->p->p);
            }
        }
        else
        {
            y=z->p->p->left;
            if (y->color==1)
            {
                z->p->color = 0;
                y->color=0;
                z->p->p->color=1;
                z=z->p->p;
            }
            else
            {
                if (z==z->p->left)
                {
                    z=z->p;
                    right_rotate(T,z);
                }
                z->p->color=0;
                z->p->p->color=1;
                left_rotate(T,z->p->p);
            }
        }
    }
 
    (*T)->color=0;
}


void RBTrees::left_rotate(node **T, node *x)
{
    node *y;
    y=x->right;
    x->right = y->left;
    
    if (y->left != Tnill)
        (y->left)->p=x;

    y->p=x->p;

    if(x->p == Tnill)
        *T=y;
    else
        if (x==(x->p)->left)
            (x->p)->left=y;
        else
            (x->p)->right =y;
    y->left =x;
    x->p =y;
}


void RBTrees::right_rotate(node **T, node *x)
{
    node *y;
    y=x->left;
    x->left = y->right;

    if (y->right != Tnill)
        (y->right)->p=x;

    y->p=x->p;
    
    if(x->p == Tnill)
        *T=y;
    else
        if (x==(x->p)->right)
            (x->p)->right=y;
        else
            (x->p)->left =y;
    y->right =x;
    x->p =y;
}




/*
 *    Funciones para eliminar un nodo
 */
void RBTrees::RB_delete_node(node **T, node *z)
{
    node *y, *x;
    int color;
    y=z;
    color = y->color;
 
    if (z->left == Tnill)
    {
        x=z->right;
        RB_transplant(T,z,z->right);
    }
    else
    {
        if (z->right==Tnill)
        {
            x=z->left;
            RB_transplant(T,z,z->left);
        }
        else
        {
            y=tree_minimum(z->right);
            color = y->color;
            x=y->right;
         
            if (y->p==z)
                x->p=y;
            else
            {
                RB_transplant(T,y,y->right);
                y->right=z->right;
                y->right->p=y;
            }
         
            RB_transplant(T,z,y);
            y->left=z->left;
            y->left->p=y;
            y->color=z->color;
        }
    }
 
    if (color==0)
        RB_delete_fixup(T,x);
}


void RBTrees::RB_delete_fixup(node **T, node *x)
{
    node *w;

    while (x->p!=Tnill && x->color==0)
    {
        if (x==x->p->left)
        {
            w=x->p->right;
            if (w->color==1)
            {
                w->color=0;
                x->p->color=1;
                left_rotate(T,x->p);
                w=x->p->right;
            }
            if (w->left->color==0 && w->right->color==0)
            {
                w->color=1;
                x=x->p;
            }
            else
            {
                if (w->right->color==0)
                {
                    w->left->color=0;
                    w->color=1;
                    right_rotate(T,w);
                    w=x->p->right;
                }
                w->color=x->p->color;
                x->p->color=0;
                w->right->color=0;
                left_rotate(T,x->p);
                x=*T;
            }
        }
        else
        {
            w=x->p->left;
            if (w->color==1)
            {
                w->color=0;
                x->p->color=1;
                right_rotate(T,x->p);
                w=x->p->left;
            }
            if (w->right->color==0 && w->left->color==0)
            {
                w->color=1;
                x=x->p;
            }
            else
            {
                if (w->left->color==0)
                {
                    w->right->color=0;
                    w->color=1;
                    left_rotate(T,w);
                    w=x->p->left;
                }
                w->color=x->p->color;
                x->p->color=0;
                w->left->color=0;
                right_rotate(T,x->p);
                x=*T;
            }
        }
    }
    
    x->color=0;
}


node * RBTrees::tree_minimum(node *x)
{
    while (x->left!=Tnill)
        x=x->left;
    return x;
}

void RBTrees::RB_transplant(node **T, node * u, node *v)
{
    if (u->p == Tnill)
        *T=v;
    else
        if(u == u->p->left)
            u->p->left=v;
        else
            u->p->right=v;
    v->p = u->p;
}


/**************/
/** funciones de impresion*****/


void RBTrees::printInorden()
{
    inorden(RBT);
}

void RBTrees::inorden(node *z)
{
    if (z!=Tnill)
    {
        inorden(z->left);
        printf(" (%f) ",z->key);
        inorden(z->right);
    }
}

