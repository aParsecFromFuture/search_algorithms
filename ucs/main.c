#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct graph graph;
typedef struct p_queue p_queue;
typedef struct node node;

struct graph{
	size_t v_count;
	bool *isvisited;
	int **adj_matrix;
	int *prev_list;
};

struct p_queue{
	node *root;
};

struct node{
	int vertice;
	int cost;
	node *next;
};

graph* create_graph(int);
void set_edge(graph*,int,int,int);
p_queue* create_list();
void push(p_queue*,int,int);
node* shift(p_queue*);
bool ucs(graph*,p_queue*,int);

int main(){
	graph *g = create_graph(7);
	set_edge(g,0,1,3);
	set_edge(g,0,2,2);
	set_edge(g,1,3,4);
	set_edge(g,1,4,5);
	set_edge(g,2,5,1);
	set_edge(g,2,6,2);

	int start_node = 0,goal_node = 5;

	g->isvisited[start_node] = 1;
	p_queue *list = create_list();
	push(list,start_node,0);
	if(ucs(g,list,goal_node)){
		while(goal_node != -1){
			printf("%d, ",goal_node);
			goal_node = g->prev_list[goal_node];
		}
	}
}

graph* create_graph(int v){
	graph *g = malloc(sizeof(graph));
	g->v_count = v;
	g->isvisited = calloc(v,sizeof(bool));
	g->adj_matrix = malloc(sizeof(int*) * v);
	for(int i = 0;i < v;i++){
		g->adj_matrix[i] = calloc(v,sizeof(int));
	}
	g->prev_list = malloc(sizeof(int) * v);
	memset(g->prev_list,-1,sizeof(int) * v);
	return g;
}

void set_edge(graph *g,int v1,int v2,int w){
	g->adj_matrix[v1][v2] = w;
	g->adj_matrix[v2][v1] = w;
}

p_queue* create_list(){
	p_queue *list = malloc(sizeof(p_queue));
	list->root == NULL;
	return list;
}

void push(p_queue *list,int v,int c){
	if(list->root == NULL){
		list->root = malloc(sizeof(node));
		list->root->vertice = v;
		list->root->cost = c;
		list->root->next = NULL;
	}else{
		node *iter = list->root;
		while(iter->next != NULL){
			iter = iter->next;
		}
		iter->next = malloc(sizeof(node));
		iter->next->vertice = v;
		iter->next->cost = c;
		iter->next->next = NULL;
	}
}

node* shift(p_queue *list){
	if(list->root == NULL){
		return NULL;
	}else{
		node *min = malloc(sizeof(node));
		min->vertice = list->root->vertice;
		min->cost = list->root->cost;
		min->next = NULL;

		node *iter = list->root;
		while(iter != NULL){
			if(min->cost > iter->cost){
				min->vertice = iter->vertice;
				min->cost = iter->cost;
			}
			iter = iter->next;
		}

		if(min->vertice == list->root->vertice){
			list->root = list->root->next;
		}else{
			node *iter = list->root;
			while(iter->next->vertice != min->vertice){
				iter = iter->next;
			}
			iter->next = iter->next->next;
		}
		return min;
	}
}

bool ucs(graph *g,p_queue *list,int goal){
	int at,cost;
	node *n;
	if((n = shift(list)) == NULL){
		return 0;
	}
	if(n->vertice == goal){
		return 1;
	}
	at = n->vertice;
	cost = n->cost;

	for(int to = 0;to < g->v_count;to++){
		if(g->adj_matrix[at][to] && !g->isvisited[to]){
			g->prev_list[to] = at;
			push(list,to,cost + g->adj_matrix[at][to]);
			g->isvisited[to] = 1;
		}
	}

	if(ucs(g,list,goal)){
		return 1;
	}
	return 0;
}
