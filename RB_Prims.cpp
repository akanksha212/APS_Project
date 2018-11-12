#include <iostream> 
#include <queue> 
using namespace std; 

enum COLOR { RED, BLACK }; 

typedef struct vertex
{
    long long v;
    long long p;
    long long key;
    vertex()
    {

    }
    vertex(long long x, long long m, long long parent){
        v = x;
        p = parent;
        key = m;
    }
} vertex;

class Node { 
public: 
vertex val; 
COLOR color; 
Node *left, *right, *parent; 
Node()
{}

Node(vertex val) : val(val) { 
    parent = left = right = NULL; 

    color = RED; 
} 

Node *uncle() { 
    if (parent == NULL or parent->parent == NULL) 
    return NULL;

    if (parent->isOnLeft()) 
    return parent->parent->right; 
    else
    return parent->parent->left; 
} 

bool isOnLeft() { return this == parent->left; } 

 
Node *sibling() { 

    if (parent == NULL) 
    return NULL; 

    if (isOnLeft()) 
    return parent->right; 

    return parent->left; 
} 

void moveDown(Node *nParent) { 
    if (parent != NULL) { 
    if (isOnLeft()) { 
        parent->left = nParent; 
    } else { 
        parent->right = nParent; 
    } 
    } 
    nParent->parent = parent; 
    parent = nParent; 
} 

bool hasRedChild() { 
    return (left != NULL and left->color == RED) or 
        (right != NULL and right->color == RED); 
} 
}*ksmall; 

class RBTree { 
    public: 
Node *root; 
 
void leftRotate(Node *x) { 
    Node *nParent = x->right; 

    if (x == root) 
    root = nParent; 

    x->moveDown(nParent); 

    x->right = nParent->left; 

    if (nParent->left != NULL) 
    nParent->left->parent = x; 
    nParent->left = x; 
} 

void rightRotate(Node *x) { 
    Node *nParent = x->left; 

    if (x == root) 
    root = nParent; 

    x->moveDown(nParent); 

    x->left = nParent->right; 

    if (nParent->right != NULL) 
    nParent->right->parent = x; 

    nParent->right = x; 
} 

void swapColors(Node *x1, Node *x2) { 
    COLOR temp; 
    temp = x1->color; 
    x1->color = x2->color; 
    x2->color = temp; 
} 

void swapValues(Node *u, Node *v) { 
    vertex temp; 
    temp = u->val; 
    u->val = v->val; 
    v->val = temp; 
} 

void fixRedRed(Node *x) { 
    if (x == root) { 
    x->color = BLACK; 
    return; 
    } 

    Node *parent = x->parent, *grandparent = parent->parent, 
        *uncle = x->uncle(); 

    if (parent->color != BLACK) { 
    if (uncle != NULL && uncle->color == RED) { 
        parent->color = BLACK; 
        uncle->color = BLACK; 
        grandparent->color = RED; 
        fixRedRed(grandparent); 
    } else { 
        if (parent->isOnLeft()) { 
        if (x->isOnLeft()) { 
            swapColors(parent, grandparent); 
        } else { 
            leftRotate(parent); 
            swapColors(x, grandparent); 
        } 

        rightRotate(grandparent); 
        } else { 
        if (x->isOnLeft()) { 

            rightRotate(parent); 
            swapColors(x, grandparent); 
        } else { 
            swapColors(parent, grandparent); 
        } 

        leftRotate(grandparent); 
        } 
    } 
    } 
} 

Node *successor(Node *x) { 
    Node *temp = x; 

    while (temp->left != NULL) 
    temp = temp->left; 

    return temp; 
} 

Node *BSTreplace(Node *x) { 
    if (x->left != NULL and x->right != NULL) 
    return successor(x->right); 

    if (x->left == NULL and x->right == NULL) 
    return NULL; 
 
    if (x->left != NULL) 
    return x->left; 
    else
    return x->right; 
} 



void fixDoubleBlack(Node *x) { 
    if (x == root) 
    return; 

    Node *sibling = x->sibling(), *parent = x->parent; 
    if (sibling == NULL) { 
    fixDoubleBlack(parent); 
    } else { 
    if (sibling->color == RED) { 
        parent->color = RED; 
        sibling->color = BLACK; 
        if (sibling->isOnLeft()) { 
        rightRotate(parent); 
        } else { 
        leftRotate(parent); 
        } 
        fixDoubleBlack(x); 
    } else { 
        if (sibling->hasRedChild()) { 
        if (sibling->left != NULL and sibling->left->color == RED) { 
            if (sibling->isOnLeft()) { 
            sibling->left->color = sibling->color; 
            sibling->color = parent->color; 
            rightRotate(parent); 
            } else { 
            sibling->left->color = parent->color; 
            rightRotate(sibling); 
            leftRotate(parent); 
            } 
        } else { 
            if (sibling->isOnLeft()) { 
            sibling->right->color = parent->color; 
            leftRotate(sibling); 
            rightRotate(parent); 
            } else { 
            sibling->right->color = sibling->color; 
            sibling->color = parent->color; 
            leftRotate(parent); 
            } 
        } 
        parent->color = BLACK; 
        } else { 
        sibling->color = RED; 
        if (parent->color == BLACK) 
            fixDoubleBlack(parent); 
        else
            parent->color = BLACK; 
        } 
    } 
    } 
} 


Node* findSmallest1(Node *p)
{
    if(p->left == NULL)
        return p;
    return findSmallest1(p->left);
}

void levelOrder(Node *x) { 
    if (x == NULL) 
    return; 

    queue<Node *> q; 
    Node *curr; 
    q.push(x); 

    while (!q.empty()) { 

    curr = q.front(); 
    q.pop(); 

    cout << curr->val.key << " "; 

    if (curr->left != NULL) 
        q.push(curr->left); 
    if (curr->right != NULL) 
        q.push(curr->right); 
    } 
} 

void inorder(Node *x) { 
    if (x == NULL) 
    return; 
    inorder(x->left); 
    cout<<"node "<< x->val.v <<"parent  "<<x->val.p<<"distance "<<x->val.key<<endl; 
    inorder(x->right); 
}

RBTree() { root = NULL; } 

Node *getRoot() { return root; } 






void deleteNode(Node *v) { 
    Node *u = BSTreplace(v); 

    bool uvBlack = ((u == NULL or u->color == BLACK) and (v->color == BLACK)); 
    Node *parent = v->parent; 

    if (u == NULL) { 
    if (v == root) { 
        root = NULL; 
    } else { 
        if (uvBlack) { 

        fixDoubleBlack(v); 
        } else { 
        if (v->sibling() != NULL) 
            v->sibling()->color = RED; 
        } 

        if (v->isOnLeft()) { 
        parent->left = NULL; 
        } else { 
        parent->right = NULL; 
        } 
    } 
    delete v; 
    return; 
    } 

    if (v->left == NULL or v->right == NULL) { 
    if (v == root) { 
        v->val = u->val; 
        v->left = v->right = NULL; 
        delete u; 
    } else { 
        if (v->isOnLeft()) { 
        parent->left = u; 
        } else { 
        parent->right = u; 
        } 
        delete v; 
        u->parent = parent; 
        if (uvBlack) {  
        fixDoubleBlack(u); 
        } else { 
        u->color = BLACK; 
        } 
    } 
    return; 
    } 

    swapValues(u, v); 
    deleteNode(u); 
} 

Node *search(int n) { 
    Node *temp = root; 
    while (temp != NULL) { 
    if (n < temp->val.key) { 
        if (temp->left == NULL) 
        break; 
        else
        temp = temp->left; 
    } else if (n == temp->val.key) { 
        break; 
    } else { 
        if (temp->right == NULL) 
        break; 
        else
        temp = temp->right; 
    } 
    } 

    return temp; 
} 


void search_del(Node* node, long long v)
{
    if(node == NULL)
    {
        return;
    }
    if((node->val).v == v)
        ksmall = node;
    search_del(node->left, v);
    if(ksmall == NULL)
        search_del(node->right, v);
}

void insert(vertex n) { 
    Node *newNode = new Node(n); 
    if (root == NULL) { 
    newNode->color = BLACK; 
    root = newNode; 
    } else { 
    Node *temp = search(n.key); 

    if (temp->val.key == n.key) { 
        return; 
    } 

    newNode->parent = temp; 

    if (n.key < temp->val.key) 
        temp->left = newNode; 
    else
        temp->right = newNode; 

    fixRedRed(newNode); 
    } 
} 

Node* findSmallest()
{
   Node* p=root;
    if(p->left == NULL)
        return p;
    return findSmallest1(p->left);
}
}; 

int main() 
{

clock_t t; 
t = clock(); 
RBTree tree;
long long v,a,b,c,e,i,max=0,j;
cout<<"Enter number of vertices"<<endl;
cin>>v;
tree.insert(vertex(0,0,-1));
int *mst=new int[v];
mst[0]=0;
vector<vector<pair<long long, long long>>> g(v);
cout<<"Enter number of edges: ";
cin>>e;
cout<<"Enter the edges as-> start vertex end vertex edge length\n";
for(i=0;i<e;i++)
{
    cin>>a>>b>>c;
    a=a-1;
    b=b-1;
    g[a].push_back(make_pair(b, c));
    g[b].push_back(make_pair(a, c));
    if(max < c)
        max = c;
}
for(i=1;i<v;i++)
{
    tree.insert(vertex(i,max+i,-1));
    mst[i]=0;
}

long long ans = 0;
for(i = 0; i < v; i ++)
{
     ksmall = tree.findSmallest();
         long long t=(ksmall->val).v;
        cout<<ksmall->val.p<<" -> "<<ksmall->val.v<<" through distance: "<<ksmall->val.key<<endl;
         ans += ksmall->val.key;
         mst[t] = 1;
        tree.deleteNode(ksmall);
         for(j = 0; j < g[t].size(); j++)
         {
            ksmall=NULL;
            if(mst[g[t][j].first] == 0)
            {

                tree.search_del(tree.root,g[t][j].first);
                if(ksmall->val.key > g[t][j].second)
                {
                    tree.deleteNode(ksmall);
                    tree.insert(vertex(g[t][j].first, g[t][j].second, t));

                }
            }
         }
}
cout<<"answer"<<ans<<endl;
t = clock() - t; 
double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
  
printf("fun() took %f seconds to execute \n", time_taken); 

return 0; 
} 
