#include <iostream>
#include <vector>
#include <utility>
using namespace std;

typedef struct vertex{
    long int v;
    long int p;
    long int key;
    vertex(long int x, long int m, long int parent){
        v = x;
        p = parent;
        key = m;
    }
} vertex;

typedef struct AVLNode{
    vertex key;
    struct AVLNode* left;
    struct AVLNode* right;
    long long int height;
} AVLNode;

AVLNode* ksmall;

AVLNode* newAVLNode(vertex element){
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->key = element;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

long long int h(AVLNode* node){
    if(node == NULL)
        return 0;
    else return(node->height);
}

AVLNode* leftrotate(AVLNode* node){
    AVLNode* temp = node->right;
    node->right = temp->left;
    temp->left = node;
    
    node->height = 1+max(h(node->left), h(node->right));
    temp->height = 1+max(h(temp->left), h(temp->right));
    
    return temp;
}

AVLNode* rightrotate(AVLNode* node){
    AVLNode* temp = node->left;
    node->left = temp->right;
    temp->right = node;
    
    node->height = 1+max(h(node->left), h(node->right));
    temp->height = 1+max(h(temp->left), h(temp->right));
    return temp;
}

AVLNode* leftrightrotate(AVLNode* node){
    node->left = leftrotate(node->left);
    node = rightrotate(node);
    
    return node;
}

AVLNode* rightleftrotate(AVLNode* node){
    node->right = rightrotate(node->right);
    node = leftrotate(node);
    
    return node;
}

AVLNode* reorder(AVLNode* node, vertex element){    //node is the node where the disbalance is detected
    long long int left_h = h(node->left);
    long long int right_h = h(node->right);
    
    if(left_h > right_h && element.key <= ((node->left)->key).key){
        return(rightrotate(node));
    }
    else if(left_h > right_h && ((node->left)->key).key < element.key){
        return(leftrightrotate(node));
    }
    else if(left_h < right_h && ((node->right)->key).key < element.key){
        return(leftrotate(node));
    }
    else{
        return(rightleftrotate(node));
    }
}

AVLNode* insert(AVLNode* node, vertex element){
    
    if(node == NULL){
        return(newAVLNode(element));
    }
    else if(element.key <= (node->key).key){
        node->left = insert(node->left, element);
    }
    else if(element.key > (node->key).key){
        node->right = insert(node->right, element);
    }
    
    long long int hl = h(node->left);
    long long int hr = h(node->right);
    node->height = 1 + max(hl, hr);
    
    if(abs(hl - hr) > 1)
        node = reorder(node, element);
    return node;
}

void print(AVLNode* node){
    if(node == NULL)
        return;
    print(node->left);
    cout<<node->key.key<<" "<<node->key.v<<" "<<node->key.p<<" at height: "<<node->height<<endl;
    print(node->right);
}

AVLNode* findSmallest(AVLNode* node){
    if(node->left == NULL)
        return node;
    return findSmallest(node->left);
}

int flag = 0;

AVLNode* delete_k(AVLNode* node){
    //cout<<"in delete\n";
    if(node == NULL)
       return NULL;
    else if((node->key).v == (ksmall->key).v){
        if(node->left == NULL && node->right == NULL){
            free(node);
            node = NULL;
        }
        else if(node->left && node->right == NULL){
            AVLNode* temp = node->left;
            *node = *temp;
            free(temp);
            temp = NULL;
        }
        else if(node->right && node->left == NULL){
            AVLNode* temp = node->right;
            *node = *temp;
            free(temp);
            temp = NULL;
        }
        else{
            AVLNode* temp = node->right;
            while(temp->left){
                temp = temp->left;
            }
            node->key = temp->key;
            ksmall = temp;
            flag = 0;
            node->right = delete_k(node->right);
        }
    }
    if(node){
        node->left = delete_k(node->left);
        node->right = delete_k(node->right);
    }
    
    if(node == NULL)
        return node;
    
    long long int hl = h(node->left);
    long long int hr = h(node->right);
    
    node->height = 1 + max(hl, hr);
    
    long long int diff = hl- hr;
    if(diff > 1){   //left tree longer
        int d = h((node->left)->left) - h((node->left)->right);
        if(d>=0)
            return(rightrotate(node));
        else
            return(leftrightrotate(node));
    }
    if(diff < -1){  //right tree longer
        int d = h((node->right)->right) - h((node->right)->left);
        if(d>=0){
            return(leftrotate(node));
        }
        else{
            return(rightleftrotate(node));
        }
    }
    return node;
}

void find_k(AVLNode* node, long int v){
    if(node == NULL)
        return;
    if((node->key).v == v)
        ksmall = node;
    find_k(node->left, v);
    if(ksmall == NULL)
        find_k(node->right, v);
}

int main(){

    long int v, e, i, a, b, c, max = 0, j;

    //TAKE GRAPH INPUT
    cout<<"Enter number of vertices: ";
    cin>>v;
    vector<vector<pair<long int, long int>>> g(v+1); //vertices numberes from 0 to v-1
    cout<<"Enter number of edges: ";
    cin>>e;
    cout<<"Enter the edges as-> start vertex end vertex edge length\n";
    for(i = 0; i < e; i ++){
        cin>>a>>b>>c;
        g[a].push_back(make_pair(b, c));
        g[b].push_back(make_pair(a, c));
        if(max < c)
            max = c;
    }
    //PRINT GRAPH
//  for(i = 0; i < v; i ++){
//      if(g[i].size() > 0){
//          cout<<i<<": ";
//          for(a = 0; a < g[i].size(); a++){
//              cout<<g[i][a].first<<" "<<g[i][a].second<<endl;;
//          }
//          cout<<endl;
//      }
//  }

    //INITIALISE DATA STRUCTURES
    AVLNode* root = NULL;
    int mst[v+1];
    mst[1] = 0;
    root = insert(root, vertex(1, 0, -1));
    for(i = 2; i <= v; i ++){
        mst[i] = 0;
        root = insert(root, vertex(i, max+1, -1));
    }

    //DELETE v VERTICES FROM TREE
    long int ans = 0;
    for(i = 0; i < v; i ++){
        ksmall = findSmallest(root);
        long int t = (ksmall->key).v;
        cout<<ksmall->key.p<<" -> "<<ksmall->key.v<<" through distance: "<<ksmall->key.key<<endl;
        ans += ksmall->key.key;
        mst[t] = 1;
        root = delete_k(root);
        for(j = 0; j < g[t].size(); j++){
            ksmall = NULL;
            if(mst[g[t][j].first] == 0){
                find_k(root, g[t][j].first);
                if(ksmall->key.key > g[t][j].second){
                    root = delete_k(root);
                    root = insert(root, vertex(g[t][j].first, g[t][j].second, t));
                }
            }
        }
    }
    cout<<"Minimum spanning tree weight: "<<ans<<endl;
    return 0;
}