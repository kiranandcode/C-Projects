#include "graph.h"





graphPtr djikstra(graphPtr graph) {
    clearStore();
    tuple edge;
    t_QueuePtr edges = graphToEdges(graph);
    t_QueuePtr MST = NULL;
    int i;
    for(i=0; i< graph->vertices; i++) makeSet(i);
    while(edges != NULL) {
        edge = popQueueTuple(edges);
        if(findSet(edge.a) != findSet(edge.b)){
                if(MST == NULL) MST = t_QueueConstructor(edge, 1);
                else insertQueueTuple(MST, edge, 1);
                unionSet(findSet(edge.a), findSet(edge.b));
        }
    }

    graphPtr MST_Graph = edgesToGraph(MST, graph->vertices);
    t_QueueDestructor(MST);
    t_QueueDestructor(edges);
    clearStore();
    return MST_Graph;
}
graphPtr Prims(graphPtr graph) {
    graphPtr MST = graphConstructor(graph->vertices);
    tuple edge;
    int i, j;
    int root = 0, seenCount = 1;
    int Key[graph->vertices], Parent[graph->vertices];
    for(i=0; i<graph->vertices; i++) Key[i] = Parent[i] = -1;
    Key[root] = 0;

    while(seenCount != graph->vertices) {
        i = minValIndex(Key);
        if(Parent[i] != -1) {
                edge.b = i;
                edge.a = Parent[i];
                Key[i] = -1;
                setEdge(MST,edge, 1);
                seenCount++;
                }
        for(j = 0; j<graph->vertices; j++) {
            if(graph->graph[i][j] != 0) {
                Parent[j] = i;
                Key[j] = graph->graph[i][j];
            }
        }
        Key[root] = -1;
    }
    return MST;
}

int minValIndexAux(int* array, int length) {
    int minimum = array[0];
    int i, index = 0;
    for(i = 0; i< length; i++) {
        if(minimum == -1) minimum = array[i], index = i;
        if(array[i] != -1 && array[i]<minimum) minimum = array[i], index = i;
    }
    return index;
}

int main()
{
    testSuite();
    startScreen();
    graphPtr graph = inputGraph();
    printf("The MST for the graph input is(Prims):\n");
    //graphPrint(Prims(graph));
    printf("The MST for the graph input is(Djikstra):\n");
    graphPrint(djikstra(graph));
    return 0;
}
