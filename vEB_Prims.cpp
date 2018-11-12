#include<bits/stdc++.h>
using namespace std;
typedef unsigned long long llu;

typedef struct node{

	long int v;
	long int p;
	long int key;
	long int temp;
	node(){

	}
	node(long int x, long int parent, long int m){
		v = x;
		p = parent;
		key = m;
        temp=m;
	}
} node;

class vEB
{
    int u;    
    node *min, *max;
    vEB *summary;
    vEB **cluster;
public:
vEB(long int x)
{
    this->u=x;
    this->min=NULL;
    this->max=NULL;
    if(u==2)
    {
        this->summary = NULL;
        this->cluster = NULL;
    }
    else
    {
        int subSize=(int)sqrt(u);
        this->summary = new vEB(subSize);
        this->cluster = new vEB*[subSize];
    }
}
void insert(long int, node);
void Delete(long int);
node* member(int);
int findmin()
{ 	if(min == NULL)
		return -1;
	else
    	return ((*min).temp);
}

int findmax()
{	if(max == NULL)
		return -1;
	else
    	return ((*max).temp);
}

int findminkey(){
	return ((*min).key);
}

int findminkeyvertex(){
	return ((*min).v);
}

int findminkeyparent(){
	return ((*min).p);
}

node* find(long int x){
	if(u == 2){
		if((*min).v == x){
			return min;
		}
		else if((*max).v == x)
			return max;
		else return NULL;
	}
	if((*min).v == x)
		return min;
	else if ((*max).v == x)
		return max;
	long int subSize=(int)sqrt(u);
	node* temp;
	for(long int i = 0; i < subSize; i++){
		temp = cluster[i]->find(x);
		if(temp != NULL)
			break;
	}
	return temp;
}
};

void vEB::insert(long int x, node element)
{
    if(u==2)
    {
        if(x==0)
        {
            if(min==NULL)
            {
                min=new node;
                max=new node;
                element.temp = x;
                *min=element;
                *max=element;
            }
            else{
            	element.temp = x;
                *min=element;
            }
        }
        else if(x==1)
        {
            if(min==NULL)
            {
                min=new node;
                max=new node;
                element.temp = x;
                *min=element;
                *max=element;
            }
            else{
            	element.temp = x;
                *max=element;
            }
        }
    }
    else
    {
        if(min==NULL)
        {
            min=new node;
            max=new node;
            element.temp = x;
            *min=element;
            *max=element;
        }
        else
        {
            if (x<(*min).temp)
            {
                node currMin = (*min);
                element.temp = x;
                (*min) = element;
                this->insert(currMin.temp, currMin);
            }
            else
            {
                int subSize=(int)sqrt(u);
                int hi = x / subSize, lo = x % subSize;
                //int h=hi;
                //int l=lo;
                if(cluster[hi] == NULL)
                {
                    cluster[hi] = new vEB(subSize);
                    cluster[hi] -> insert(lo, element);
                    summary -> insert(hi, element);
                }
                else 
                    cluster[hi] -> insert(lo, element);
                
                if (x>(*max).temp)
                    (*max)=element;
            }
        }
    }
}

// void vEB::Delete(long int x){
// 	cout<<x<<endl;
// 	if(min->temp == max->temp){
// 		min = NULL;
// 		max = NULL;
// 	}
// 	else if(u == 2){
// 		if(x == 0){
// 			min = max;
// 		}
// 		else max = min;
// 	}
// 	else if(x == (*min).temp){
// 		llu subSize = (llu)sqrt(u);
// 		llu nextMinHi = summary -> findmin();
// 		llu nextMinLo = cluster[nextMinHi] -> findmin();
// 		llu nextMin = nextMinHi * subSize + nextMinLo;
//         node* temp = member(nextMin);
//         min = temp;
//         this -> Delete(nextMin);

        
//         llu hi = x / subSize, lo = x % subSize;
//         if(cluster[hi]->findmin() == -1){
//         	(this->summary)->Delete(hi);
//         	if(x == (*max).temp){
//         		llu nextMaxHi = summary -> findmax();
//         		if(nextMaxHi == -1){
//         			max = min;
//         		}
//         		else{
//         			llu nextMaxLo = cluster[nextMaxHi] -> findmax();
// 	                llu nextMax = nextMaxHi * subSize + nextMaxLo;
// 	                node* temp = member(nextMax);
// 	                max = temp;
//         		}
//         	}
//         }
//         else if(x == (*max).temp){
//         	llu subSize = (llu)sqrt(u);
//         	llu hi = x / subSize, lo = x % subSize;
//         	long int t = cluster[hi]->findmax();
//         	t = hi * subSize + t;
//         	node* temp = member(t);
//         	max = temp;
//         }
// 	}
// }

void vEB::Delete(long int x)
{ cout<<x<<" "<<u<<endl;
    if (u == 2)
    {
        if (x == 0)
        {
            if ((*max).temp == 0)
            {
                min = max = NULL;
            }
            else min = max;
        }
        else if (x == 1)
        {
            if ((*min).temp == 1)
            {
                min = max = NULL;
            }
            else max = min;
        }
    }
    else
    {
        llu subSize = (llu)sqrt(u);
        llu hi = x / subSize, lo = x % subSize;
        cout<<(*min).temp<<" "<<(*max).temp<<endl;
        if (x == (*min).temp)
        {
            if (x == (*max).temp)
            {
                min = max = NULL;
            }
            else
            {	
                llu nextMinHi = summary -> findmin();
                //cout<<cluster[summary -> findmin()] -> findmin()<<endl;
                llu nextMinLo = cluster[summary -> findmin()] -> findmin();
                llu nextMin = nextMinHi * subSize + nextMinLo;
                node* temp = member(nextMin);
                this -> Delete(nextMin);
                min = temp;
            }
        }
        else
        {
            cluster[hi] -> Delete(lo);
            if (cluster[hi] -> min == NULL)
            {
                summary -> Delete(hi);
                delete cluster[hi];
                cluster[hi] = NULL;
            }
            if (x == (*max).temp)
            {
                if (summary -> min == NULL) max = min;
                else
                {
                    llu nextMaxHi = summary -> findmax();
                    llu nextMaxLo = cluster[summary -> findmax()] -> findmax();
                    llu nextMax = nextMaxHi * subSize + nextMaxLo;
                    node* temp = member(nextMax);
                    max = temp;
                }
            }
        }
    }
}

node* vEB::member(int x)
{
    if (u == 2)
    {
        if (min == NULL) return NULL;
        if (x == 0 && (*min).temp == 0) return (min);
        else if (x == 1 && (*max).temp == 1) return (max);
        return NULL;
    }
    else
    {
        if (min == NULL) return NULL;
        if (x < (*min).temp || x > (*max).temp) return NULL;
        else if (x == (*min).temp) return min;
        else if(x == (*max).temp) return max;
        else
        {
            int subSize = (int)sqrt(u);
            int hi = x / subSize, lo = x % subSize;
            if (cluster[hi] == NULL) return NULL;
            else return cluster[hi] -> member(lo);
        }

    }
}

// llu* vEB::succ(llu x)
// {
//     if (u == 2)
//     {
//         if (x == 1) return NULL;
//         else if (x == 0)
//         {
//             if (M == NULL) return NULL;
//             if ((*M) == 0) return NULL;
//             return M;
//         }
//         else return NULL;
//     }
//     else
//     {
//         if (m == NULL) return NULL;
//         if (x >= (*M)) return NULL;
//         if (x < (*m)) return m;
//         llu subSize = (llu)sqrt(u);
//         llu hi = x / subSize;
//         llu lo = x % subSize;
//         if (cluster[hi] == NULL)
//         {
//             llu* next = summary -> succ(hi);
//             llu* ret = new llu;
//             (*ret) = (*next) * subSize + cluster[(*next)] -> min();
//             return ret;
//         }
//         else
//         {
//             llu *newLo, *newHi;
//             newHi = new llu;
//             newLo = new llu;
//             (*newHi) = hi;
//             llu maxInCluster = cluster[hi] -> max();
//             if (lo < maxInCluster) newLo = cluster[hi] -> succ(lo);
//             else
//             {
//                 newHi = summary -> succ(hi);
//                 (*newLo) = cluster[(*newHi)] -> min();
//             }
//             llu *ret = new llu;
//             (*ret) = (*newHi) * subSize + (*newLo);
//             return ret;
//         }
//     }
// }

int main(){

	long int v, e, i, a, b, c, j,max=0;

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
	vEB *tree = new class vEB(65536);
    int *mst=new int[v+1];
    mst[1]=0;
	tree->insert(0, node(1, -1, 0));
    for(i=2;i<=v;i++)
    {
        tree->insert(max+i,node(i,-1,max+i));
        mst[i]=0;
    }
    long int ans = 0, x, ver, par;
    tree->Delete(0);
    cout<<endl;
    tree->Delete(9);

    for(i = 0; i < v; i++){
    	//find minimum node
    	x = tree->findminkey();
    	par = tree->findminkeyparent();
    	ver = tree->findminkeyvertex();
    	cout<<x<<" "<<par<<" -> "<<ver<<endl;
    	ans += x;
    	//delete this node
    	tree->Delete(x);
    	//traverse its adjacent nodes
    	for(j = 0; j < g[ver].size(); j++){
    		node* temp = tree->find(g[ver][j].first);
    		cout<<temp->v<<" with key "<<temp->key<<" found\n";
    		if((*temp).key > g[ver][j].second){
    			cout<<"yes smaller\n";
    			tree->Delete(temp->key);
    			cout<<"deleted\n";
    			tree->insert(g[ver][j].second, node(g[ver][j].first, ver, g[ver][j].second));
    		}
    	}
    }
  }


// // A C / C++ program for Prim's Minimum  
// // Spanning Tree (MST) algorithm. The program is  
// // for adjacency matrix representation of the graph 
// #include <stdio.h> 
// #include <limits.h> 
// #include<stdbool.h> 
// // Number of vertices in the graph 
// long int V;
  
// // A utility function to find the vertex with  
// // minimum key value, from the set of vertices  
// // not yet included in MST 
// int minKey(int key[], bool mstSet[]) 
// { 
// // Initialize min value 
// int min = INT_MAX, min_index; 
  
// for (int v = 0; v < V; v++) 
//     if (mstSet[v] == false && key[v] < min) 
//         min = key[v], min_index = v; 
  
// return min_index; 
// } 
  
// // A utility function to print the  
// // constructed MST stored in parent[] 
// int printMST(int parent[], int n, int graph[V][V]) 
// { 
// printf("Edge \tWeight\n"); 
// for (int i = 1; i < V; i++) 
//     printf("%d - %d \t%d \n", parent[i], i, graph[i][parent[i]]); 
// } 
  
// // Function to construct and print MST for  
// // a graph represented using adjacency  
// // matrix representation 
// void primMST(int graph[V][V]) 
// { 
//     // Array to store constructed MST 
//     int parent[V+1];  
//     // Key values used to pick minimum weight edge in cut 
//     int key[V+1];  
//     // To represent set of vertices not yet included in MST 
//     bool mstSet[V+1];  
  
//     // Initialize all keys as INFINITE 
//     for (int i = 0; i <= V; i++) 
//         key[i] = INT_MAX, mstSet[i] = false; 
  
//     // Always include first 1st vertex in MST. 
//     // Make key 0 so that this vertex is picked as first vertex. 
//     key[0] = 0;      
//     parent[0] = -1; // First node is always root of MST  
  
//     // The MST will have V vertices 
//     for (int count = 0; count < V-1; count++) 
//     { 
//         // Pick the minimum key vertex from the  
//         // set of vertices not yet included in MST 
//         int u = minKey(key, mstSet); 
  
//         // Add the picked vertex to the MST Set 
//         mstSet[u] = true; 
  
//         // Update key value and parent index of  
//         // the adjacent vertices of the picked vertex.  
//         // Consider only those vertices which are not  
//         // yet included in MST 
//         for (int v = 1; v <= V; v++) 
  
//         // graph[u][v] is non zero only for adjacent vertices of m 
//         // mstSet[v] is false for vertices not yet included in MST 
//         // Update the key only if graph[u][v] is smaller than key[v] 
//         if (graph[u][v] && mstSet[v] == false && graph[u][v] < key[v]) 
//             parent[v] = u, key[v] = graph[u][v]; 
//     } 
  
//     // print the constructed MST 
//     printMST(parent, V, graph); 
// }

// int main() 
// { 	
// 	long int v, e, i, a, b, c, j;

// 	cout<<"Enter the number of vertices: ";
// 	cin>>V;
// 	cout<<"Enter the number of edges: ";
// 	cin>>e;
// 	int graph[V+1][V+1];
// 	for(i = 0; i <= V; i ++){
// 		for(j = 0; i <= V; i ++){
// 			graph[i][j] = 0;
// 		}
// 	}
// 	cout<<"Enter the edges as: vertex1, vertex2, edge length"
// 	for(i = 0; i < e; i ++){
// 		cin>>a>>b>>c;
// 		graph[a][b] = c;
// 		graph[b][a] = c;
// 	}
  
//     // Print the solution 
//     primMST(graph); 
  
//     return 0; 
// } 