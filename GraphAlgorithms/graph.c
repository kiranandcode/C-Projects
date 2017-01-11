#include "graph.h"

global_elements elementStore = {0, NULL};

graphPtr graphConstructor(int vertices)
{
    graphPtr output = malloc(sizeof(graph));
    output->vertices = vertices;
    output->graph = malloc(sizeof(int*)*vertices);
    int i;
    for(i = 0; i<vertices; i++)
    {
        output->graph[i] = calloc(sizeof(int), vertices);
    }
    return output;
}

void graphPrint(graphPtr Adj_Mat)
{
    int i, j;
    printf("  ");
    for(i = 0; i< Adj_Mat->vertices; i++)
    {
        printf(" %2d ", i);
    }
    printf("\n");
    for(i = 0; i< Adj_Mat->vertices; i++)
    {
        printf("%d ", i);
        printf("[");
        for(j = 0; j< Adj_Mat->vertices; j++)
        {
            printf("%2d, ", Adj_Mat->graph[i][j]);
        }
        printf("\b\b]\n");
    }
    printf("\n");
    return;
}

graphPtr graphDestructor(graphPtr graph)
{
    int i;
    for(i = 0; i< graph->vertices; i++) free(graph->graph[i]);
    free(graph->graph);

    free(graph);
    return NULL;
}

int getEdge(graphPtr graph, tuple edge)
{
    return graph->graph[edge.a][edge.b];
}

void setEdge(graphPtr graph, tuple edge, int cost)
{
    graph->graph[edge.a][edge.b] = cost;
    return;
}

tuple graphMinEdge(graphPtr graph)
{
    tuple out;
    int i, j, min, flag = 0;
    for(i = 0; i<graph->vertices; i++)
    {
        for(j = 0; j<graph->vertices; j++)
        {
            if(!flag && graph->graph[i][j] != 0) min = graph->graph[i][j], out.a = i, out.b = j, flag = 1;
            if(graph->graph[i][j] != 0 && graph->graph[i][j] < min) min = graph->graph[i][j], out.a = i, out.b = j;
        }
    }
    return out;
}


t_QueuePtr t_QueueConstructor(tuple value, int weight)
{
    t_QueuePtr newNode = malloc(sizeof(t_Queue));
    newNode->nextNode = NULL;
    newNode->weight = weight;
    newNode->value = value;
    return newNode;
}

void t_QueueDestructorAux(t_QueuePtr *start)
{
    if(*start == NULL) return;
    t_QueueDestructorAux(&((*start)->nextNode));
    free(*start);
    *start = NULL;
    return;
}


tuple getQueueTuple(t_QueuePtr start, int index)
{
    while(start->nextNode != NULL && index > 0) index--, start = start->nextNode;
    return start->value;
}

void insertQueueTupleAux(t_QueuePtr *strPtr, tuple value, int weight)
{
    t_QueuePtr start = *strPtr;
    if(start == NULL)
    {
        *strPtr = malloc(sizeof(t_Queue));
        (*strPtr)->nextNode = NULL;
        (*strPtr)->weight = weight;
        (*strPtr)->value = value;
        return;
    }
    if(weight < start->weight)
    {
        t_QueuePtr newNode = malloc(sizeof(t_Queue));
        newNode->nextNode = start->nextNode;
        newNode->weight = start->weight;
        newNode->value = start->value;
        start->nextNode = newNode;
        start->value = value;
        start->weight = weight;
        return;
    }
    while( start->nextNode != NULL && weight > start->nextNode->weight) start = start->nextNode;
    t_QueuePtr endNode = malloc(sizeof(t_Queue));
    endNode->nextNode = start->nextNode;
    endNode->value = value;
    endNode->weight = weight;
    start->nextNode = endNode;
    return;
}

tuple popQueueTupleAux(t_QueuePtr *startptr)
{
    t_QueuePtr start = *startptr;
    assert(start != NULL);
    tuple output = start->value;
    *startptr = start->nextNode;
    free(start);
    return output;
}

void printTuple(tuple value)
{
    printf("(%d, %d)", value.a, value.b);
}

void printT_Node(t_QueuePtr node)
{
    printf("Value: ");
    printTuple(node->value);
    printf("\nWeight: %d\n", node->weight);
    printf("Current Address: %p -> Next Address: %p\n", node, node->nextNode);
}

void printT_Queue(t_QueuePtr start)
{
    if(start == NULL) return;
    int index = 0;
    while(1)
    {
        printf("Index: %d\n", index);
        printT_Node(start);
        if(start->nextNode != NULL) start = start->nextNode;
        else break;
        index++;
        printf("\n");
    }
    printf("---------------------------------------------\n");
    return;
}



t_QueuePtr graphToEdges(graphPtr graph)
{
    tuple temp;
    t_QueuePtr output;
    int i, j, flag = 0;
    for(i = 0; i< graph->vertices; i++)
    {
        for(j=0; j<graph->vertices; j++)
        {
            if(graph->graph[i][j] != 0)
            {
                temp.a = i;
                temp.b = j;
                if(!flag)
                {
                    output = t_QueueConstructor(temp, graph->graph[i][j]);
                    flag = 1;
                }
                else
                {
                    insertQueueTuple(output, temp, graph->graph[i][j]);
                }
            }
        }
    }
    return output;
}

graphPtr edgesToGraph(t_QueuePtr edges, int vertices)
{
    graphPtr output = graphConstructor(vertices);
    while(edges != NULL)
    {
        setEdge(output, edges->value, 1);
        edges = edges->nextNode;
    }
    return output;
}





void increaseStoreSize()
{
    if(elementStore.size == 0)
    {
        elementStore.size += 1;
        elementStore.elements = malloc(sizeof(element));
        return;
    }
    elementStore.size += 1;
    elementStore.elements = realloc(elementStore.elements, elementStore.size*sizeof(elementPtr));
    return;
}


void registerElement(elementPtr S)
{
    increaseStoreSize();
    elementStore.elements[elementStore.size-1] = S;
}

void clearStore()
{

    int i,j, max = 0, flag = 0;
    setPtr store[elementStore.size];
    for(i = 0; i<elementStore.size; i++)
    {
        for(j = 0; j<max; j++) {
                if(elementStore.elements[i]->container == store[j]) {flag = 1; break;}
        }
        if(flag == 0) store[max] = elementStore.elements[i]->container, max++;
        flag = 0;
    }
    for(j = 0; j<max; j++) {
        free(store[j]);
    }





    for(i = 0; i<elementStore.size; i++)
    {

        if(elementStore.elements != NULL)
        {

            free(elementStore.elements[i]);
        }
    }
    free(elementStore.elements);
    elementStore.size = 0;
}

void printElement(elementPtr elem)
{
    printf("Element[Value=%d,parentSet=%p]", elem->value, elem->container);
}

elementPtr elementFromValue(int value)
{
    int i;
    for(i = 0; i<elementStore.size; i++)
    {
        if(elementStore.elements == NULL)
        {
            printf("Really odd, null value in storage\n");
            continue;
        }
        if(elementStore.elements[i] == NULL)
        {
            printf("Pointer in contiguous arr empty\n");
            continue;
        }
        if(elementStore.elements[i]->value == value)
        {
            return elementStore.elements[i];
        }
    }
    return NULL;
}

setPtr makeSet(int val)
{
    if(elementFromValue(val) != NULL) return NULL;
    setPtr newSet = malloc(sizeof(set));
    elementPtr newElem = malloc(sizeof(element));
    newSet->head = newElem;
    newSet->tail = newElem;
    newElem->container = newSet;
    newElem->next = NULL;
    newElem->value = val;
    assert(newSet == newElem->container);
    registerElement(newElem);
    return newSet;
}

void unionSetAux(setPtr *a, setPtr *b)
{
    if(a == NULL || b == NULL) return;
    setPtr setA = *a;
    setPtr setB = *b;
    if(setA == NULL || setB == NULL) return;

    setA->tail->next = (setB->head);
    elementPtr current = setB->head;
    while(current)
    {
        current->container = setA;
        current = current->next;
    }

    setB->head = NULL;
    setB->tail = NULL;
    free(setB);
    *b = NULL;
    return;
}

void unionSet(setPtr setA, setPtr setB)
{
    if(setA == NULL || setB == NULL)
    {
        printf("called on empty parameters A: %p, B: %p\n", setA, setB);
        return;
    }
    setA->tail->next = (setB->head);
    elementPtr current = setB->head;
    while(current != NULL)
    {
        current->container = setA;
        current = current->next;
    }

    setB->head = NULL;
    setB->tail = NULL;
    free(setB);
    return;
}

setPtr findSet(int val)
{
    if(elementFromValue(val) == NULL)
    {
        printf("Called on nonExisting value\n");
        return NULL;
    }
    return elementFromValue(val)->container;
}

int lengthSet(setPtr set) {
    int counter = 0;
    elementPtr elem = set->head;
    while(elem) {
        counter++;
        elem = elem->next;
    }
    return counter;
}

void printSet(setPtr set)
{
    if(set == NULL) return;
    if(set->head == NULL) return;
    elementPtr current = set->head;
    printf("Set: (");
    while(current)
    {
        printf("%d, ", current->value);
        current = current->next;
    }
    printf("\b\b)");
}


void startScreen()
{
    printf("\n");
    printf("------------------------------------\n");
    printf("      Graph Processing Program      \n");
    printf("                  BY                \n");
    printf("           Kiran Gopinathan         \n");
    printf("------------------------------------\n");
    printf("                Enjoy!              \n\n");
    return;
}


graphPtr inputGraph()
{
    int vertices, edges;
    char selection;
    int weight;
    tuple edge;
    graphPtr new_graph;
    do
    {
        printf("Please enter the number of vertices in the graph:\n");
        fscanf(stdin, "%d", &vertices);
        fflush(stdin);
        printf("Please enter the number of edges in the graph:\n");
        fscanf(stdin, "%d", &edges);
        fflush(stdin);
        if(edges > vertices * vertices)
        {
            fprintf(stdout, "Edges must be less than V^2.\n"), selection = 'n';
            continue;
        }
        if(edges < 0 || vertices < 0)
        {
            fprintf(stdout, "Values can not be negative.\n"), selection = 'n';
            continue;
        }
        printf("The graph will have %d vertices and %d edges, correct? (Y\\n)\n", vertices, edges);
        fscanf(stdin, "%c", &selection);
        fflush(stdin);
    }
    while(selection != 'Y' && selection != 'y');
    new_graph = graphConstructor(vertices);
    while(edges > 0)
    {
        printf("Enter edge[%d] in the form (from, to):\n", edges);
        fscanf(stdin, "(%d,%d)", &edge.a, &edge.b);
        fflush(stdin);
        if(edge.a >= vertices || edge.b >= vertices)
        {
            printf("Invalid Vertex - remember, vertexes start from 0\n");
            continue;
        }
        printf("Enter the weight of the edge:\n");
        fscanf(stdin, "%d", &weight);
        fflush(stdin);
        setEdge(new_graph, edge, weight);
        edges--;
    }
    printf("Input Completed.\nThe adjacency matrix for the graph you entered is:\n");
    graphPrint(new_graph);
    return new_graph;
}



void testGraphs()
{
    printf("Testing Graph Code - please wait a second.\n");
    graph test = *(graphConstructor(2));
    assert(test.vertices = 2);
    tuple testTuple = {1, 0};
    setEdge(&test, testTuple, 10);
    assert(getEdge(&test, testTuple) == 10);
    assert(getEdge(&test, graphMinEdge(&test)) == 10);
    testTuple.a = 0;
    setEdge(&test, testTuple, 2);
    assert(getEdge(&test, graphMinEdge(&test)) == 2);
    graphDestructor(&test);
    printf("Testing Completed Successfully\n");
}

void testTupleLists()
{
    printf("Testing Tuple_List Code - please wait a second.\n");

    tuple a = {1,2}, b = {2,3}, c = {4,5};
    t_QueuePtr test = t_QueueConstructor(a, 1);
    tuple d = popQueueTuple(test);
    assert(d.a == a.a && d.b == a.b && test == NULL);

    insertQueueTuple(test, b, 0);

    insertQueueTuple(test, c, 2);
    d = popQueueTuple(test);
    assert(d.a == b.a && d.b == b.b && test != NULL);
    d = popQueueTuple(test);
    assert(d.a == c.a && d.b == c.b && test == NULL);
    t_QueueDestructor(test);
    printf("Testing Completed Successfully\n");
}

void testTupleGraphConv()
{
    printf("Testing Graph-TupleList Conversion Code - please wait a second.\n");
    tuple a = {0,1}, b = {2,3}, c = {4,2};
    t_QueuePtr list = t_QueueConstructor(a, 1);
    insertQueueTuple(list, b, 2);
    insertQueueTuple(list, c, 3);
    graphPtr Constructed = edgesToGraph(list, 5);
    assert(getEdge(Constructed, getQueueTuple(list, 0)) == 1);
    assert(getEdge(Constructed, getQueueTuple(list, 1)) == 1);
    assert(getEdge(Constructed, getQueueTuple(list, 2)) == 1);
    graphDestructor(Constructed);
    t_QueueDestructor(list);
    printf("Testing Completed Successfully\n");
}
void testSet()
{
    printf("Testing Set Code - please wait a second.\n");
    makeSet(1);
    makeSet(2);
    makeSet(3);
    makeSet(4);
    assert(findSet(1) != findSet(2));
    unionSet(findSet(1), findSet(2));
    assert(findSet(1) == findSet(2));
    assert(findSet(3) != findSet(4));
    unionSet(findSet(3), findSet(4));
    assert(findSet(3) == findSet(4));
    assert(findSet(4) != findSet(1));
    unionSet(findSet(3), findSet(1));
    assert(findSet(3) == findSet(2));
    clearStore();
    int i;
    for(i=1; i<5; i++)
    {
        assert(findSet(i) == NULL);
    }
    printf("Testing Completed Successfully\n");
}

void testSuite()
{
    if(DEBUG)
    {
        testGraphs();
        testTupleLists();
        testTupleGraphConv();
        testSet();
    }
}
