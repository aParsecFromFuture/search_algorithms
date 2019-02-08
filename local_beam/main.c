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
	int capacity;
	node *root;
};

struct node{
	int vertice;
	int h_cost;
	node *next;
};

graph* create_graph(int);
void set_edge(graph*,int,int,int);
p_queue* create_list(int);
void push(p_queue*,int,int);
node* shift(p_queue*);
void local_beam(graph*,p_queue*);
int h(int);

int main(){
	graph *g = create_graph(7);
	set_edge(g,0,1,3);
	set_edge(g,0,2,6);
	set_edge(g,1,3,2);
	set_edge(g,1,4,5);
	set_edge(g,2,5,1);
	set_edge(g,2,6,4);
	
	int start_node = 0;
	p_queue *list = create_list(2);
	push(list,start_node,h(start_node));
	local_beam(g,list);
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

p_queue* create_list(int c){
	p_queue *list = malloc(sizeof(p_queue));
	list->capacity = c;
	list->root = NULL;
}

void push(p_queue *list,int v,int h_c){
	if(list->root == NULL){
		list->root = malloc(sizeof(node));
		list->root->vertice = v;
		list->root->h_cost = h_c;
		list->root->next = NULL;
	}else{
		int size = 2;
		node *iter = list->root;
		while(iter->next != NULL){
			size++;
			iter = iter->next;
		}
		iter->next = malloc(sizeof(node));
		iter->next->vertice = v;
		iter->next->h_cost = h_c;
	
		if(size > list->capacity){
			node *max = malloc(sizeof(node));
			max->vertice = list->root->vertice;
			max->h_cost = list->root->h_cost;
			max->next = NULL;

			iter = list->root;
			while(iter != NULL){
				if(max->h_cost < iter->h_cost){
					max->vertice = iter->vertice;
					max->h_cost = iter->h_cost;
				}
				iter = iter->next;
			}

			if(max->vertice == list->root->vertice){
				list->root = list->root->next;
			}else{
				iter = list->root;
				while(iter->next->vertice != max->vertice){
					iter = iter->next;
				}
				iter->next = iter->next->next;
			}
		}
	}
}

node* shift(p_queue *list){
	if(list->root == NULL){
		return NULL;
	}
	node *min = malloc(sizeof(node));
	min->vertice = list->root->vertice;
	min->h_cost = list->root->h_cost;
	min->next = NULL;
	node *iter = list->root;
	while(iter != NULL){
		if(min->h_cost > iter->h_cost){
			min->vertice = iter->vertice;
			min->h_cost = iter->h_cost;
		}
		iter = iter->next;
	}
	if(min->vertice == list->root->vertice){
		list->root = list->root->next;
	}else{
		iter = list->root;
		while(min->vertice != iter->next->vertice){
			iter = iter->next;
		}
		iter->next = iter->next->next;
	}
	return min;
}

void local_beam(graph *g,p_queue *list){
	node *n;
	if((n = shift(list)) == NULL){
		return;
	}
	int at = n->vertice;
	int h_cost = n->h_cost;
	for(int to = 0;to < g->v_count;to++){
		if(g->adj_matrix[at][to] && !g->isvisited[to] && h(at) > h(to)){
			g->prev_list[to] = at;
			push(list,to,h(to));
			g->isvisited[to] = 1;
		}
	}
	local_beam(g,list);
}

int h(int v){
	return 10 - v;
}
