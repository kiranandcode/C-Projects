#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "graph.h"




int main() {
    graph *val = openGraph("graph.grp");


    path_matrix *p = djikstra(val,0);
    printPathMatrix(NULL, p, stdout);

    printPath(NULL, shortest_path_to_node(p, 4), stdout);
    delete_path_matrix(p);
    p = prims(val,0);

    printPathMatrix(NULL, p, stdout);

    printPath(NULL, shortest_path_to_node(p, 4), stdout);

   /* path_matrix *djik = djikstra(val, 0);
    path *p = shortest_path_to_node(djik,4);
    printPath(NULL, p,stdout);

    delete_path_matrix(djik);
    delete_path(p);

    printGraph(NULL, val, stdout);

    printGraph(NULL, generateFlowTo(val, dfs(val, 0), 4), stdout);

    delete_graph(val);*/

/*
    graph *val = openGraph("capacity.grp");
    graph *flow = openGraph("flow.grp");

    printGraph(NULL, val, stdout);
    printGraph(NULL, ford_fulkerson(val, 0, 3), stdout);*/

/*
    graph *residual = residualFlowGraph(val, flow);

    printGraph(NULL, residual, stdout);

    djik = djikstra(residual, 0);

    printPath(NULL, shortest_path_to_node(djik, 3),stdout);
*/



    return 0;


}