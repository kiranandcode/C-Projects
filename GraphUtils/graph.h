//
// Created by gopia on 01/05/2017.
//

#ifndef GRAPHUTILS_GRAPH_H
#define GRAPHUTILS_GRAPH_H

#define DJIKSTRA_LIMIT 1000

#include <stdio.h>
#include <stdlib.h>

typedef struct graph {
    int **graph;
    int nodes;
    int validated;
} graph;



typedef struct stack {
    int node;
    struct stack *next;
} stack;

typedef struct path {
    int from;
    int to;
    struct path *prev;
    struct path *next;
} path;

typedef struct djikstra_path_tree {
    int root;
    int nodes;
    int *nearest;
    int *costs;
} path_matrix;


int **matrix(int rows, int columns);

void delete_path_matrix(path_matrix *path_tree);

void delete_path(path *p);

void delete_graph(graph *grp);

path_matrix *dfs(graph *grp, int start);

path_matrix *bfs(graph *grp, int start);

graph *generateFlowTo(graph *original, path_matrix *mat, int end);

graph *generateSearchGraph(graph *original, path_matrix *mat);

path_matrix *djikstra(graph *grp, int start);

path *shortest_path_to_node(path_matrix *path_tree, int to);

graph *getData(FILE *fp);

graph *augmentGraph(graph *capacity_graph, graph *f, graph *fstar);

graph *residualFlowGraph(graph *capacity_graph, graph *flow_graph);

void printGraph(const char *spec, graph *grp, FILE *fp);

void printPath(const char *spec, path *pt, FILE *fp);

graph *openGraph(const char *fileName);

graph *ford_fulkerson(graph *capacity_graph, int source, int sink);

#endif //GRAPHUTILS_GRAPH_H
