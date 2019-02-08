#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct graph graph;

struct graph{
	size_t v_count;
	bool *isvisited;
	int **adj_matrix;
	int *next_list;
};

graph* create_graph(int);
void set_edge(graph*,int,int,int);
void hill_climbing(graph *g,int);
int h(int);

int main(){
	graph *g = create_graph(7);
	set_edge(g,0,1,3);
	set_edge(g,0,2,2);
	set_edge(g,1,3,6);
	set_edge(g,1,4,4);
	set_edge(g,2,5,7);
	set_edge(g,2,6,5);

	int start_node = 0;

	hill_climbing(g,start_node);
	while(start_node != -1){
		printf("%d, ",start_node);
		start_node = g->next_list[start_node];
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
	g->next_list = malloc(sizeof(int) * v);
	memset(g->next_list,-1,sizeof(int) * v);
	return g;
}

void set_edge(graph *g,int v1,int v2,int w){
	g->adj_matrix[v1][v2] = w;
	g->adj_matrix[v2][v1] = w;
}

void hill_climbing(graph *g,int at){
	g->isvisited[at] = 1;
	int vertice_min = -1;
	int h_cost = -1;
	for(int to = 0;to < g->v_count;to++){
		if(g->adj_matrix[at][to] && !g->isvisited[to]){
			if(h(at) > h(to)){ 
				if(vertice_min == -1){
					vertice_min = to;
					h_cost = h(to);
				}else if(h_cost > h(to)){
					vertice_min = to;
					h_cost = h(to);
				}
			}
		}
	}
	if(vertice_min == -1){
		return;
	}else{
		g->next_list[at] = vertice_min;
		hill_climbing(g,vertice_min);
	}
}

int h(int v){
	return 10 - v;
}
