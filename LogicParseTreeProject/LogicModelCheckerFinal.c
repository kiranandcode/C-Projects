#include <stdio.h>
#include <string.h>   /* for all the new-fangled string functions */
#include <stdlib.h>     /* malloc, free, rand */


const int Fsize=50;
int no_edges;
int no_nodes;
int i;
const int cases=10;


int parse(char *ref)
{
    /* return 1 if atomic, 2 if  neg, 3 if binary, 4 if exists, 5 if for all, ow 0*/
    char *right;
    int scope = 0;
    int flag = 0;
    // upScope tells the function how it was called - 0 for at the top level, 1 for at the first expression of a BC and 2 for at the second expression of a BC
    // before any recursion occurs, upScope is changed, and then after the value is returned, upScope is set back to it's initial value.
    static int upScope = 0;
    int scopeContainer = upScope;
    switch(*ref)
    {
        case 'X':
            // X case is relatively simple, check the next 4 characters fit the scheme.
            ref++;
            if (*ref != '[') return 0;
            ref++;
            if (!(*ref == 'x' || *ref == 'y' || *ref == 'z')) return 0;
            ref++;
            if (!(*ref == 'x' || *ref == 'y' || *ref == 'z')) return 0;
            ref++;
            if (*ref != ']') return 0;

            // Thanks to upScope, the function knows what to expect following the atomic formula and can check
            if(upScope == 0) if(*(ref+1) != 0) return 0;
            if(upScope == 1) if(!(*(ref+1) == '^' || *(ref+1) == '>' || *(ref+1) == 'v')) return 0;
            if(upScope == 2) if(*(ref+1) != ')') return 0;
            return 1;
            break;
        case 'A':
            // Existential formula check the next character, then recurse on the rest
            ref++;
            if (!(*ref == 'x' || *ref == 'y' || *ref == 'z')) return 0;
            ref++;

            // As existential formula never end up at the end of a formula, they don't change the upScope, just pass it on
            if(parse(ref) == 0)
            {
                upScope = scopeContainer;
                return 0;
            }
            // upScope is executed on both halves of the if statement, so that irrespective of where the function returns control, upScope is maintained.
            upScope = scopeContainer;
            return 5;
            break;
        case 'E':
            // Same as above
            ref++;
            if (!(*ref == 'x' || *ref == 'y' || *ref == 'z')) return 0;
            ref++;
            if(parse(ref) == 0)
            {
                upScope = scopeContainer;
                return 0;
            }
            upScope = scopeContainer;
            return 4;
            break;
        case '(':
            // Checks the first half of the BC by recursing and changing upScope accordingly
            upScope = 1;
            if(parse(ref+1) == 0)
            {
                upScope = scopeContainer;
                return 0;
            }
            // Once checked, returns upScope back to it's initial value, and continues
            upScope = scopeContainer;

            // Checks BC by iterating through until equal no of open and close brackets have been met from the start.
            while(*ref)
            {
                switch(*ref)
                {
                    case '(':
                        scope++;
                        break;
                    case ')':
                        if(flag != 1) return 0;
                        flag = 0;
                        scope--;
                        break;
                    case '^':
                    case '>':
                    case 'v':
                        flag++;
                        upScope = 2;
                        if(scope == 1) if(parse(ref+1) == 0)
                            {
                                upScope = scopeContainer;
                                return 0;
                            }
                        upScope = scopeContainer;
                        break;
                }
                ref++;
                if (scope == 0) break;
            }
            if (scope != 0) return 0;
            if(flag != 0) return 0;
            return 3;
            break;
        case '-':
            ref++;
            if(parse(ref) == 0)
            {
                upScope = scopeContainer;
                return 0;
            }
            upScope = scopeContainer;
            return 2;
            break;
        default:
            return 0;
            break;
    }

}



int eval(char *nm, int edges[no_edges][2], int size, int V[3])
{
    /* returns 1 if formla nm evaluates to true in graph with 'size' nodes, no_edges edges, edges stored in 'edges', variable assignment V.  Otherwise returns 0.*/
    char* ref =  nm;
    int a,b, i;
    int flag = 0;
    int Vstar[3];
    int scope = 0;
    for(i = 0; i<3; i++) Vstar[i] = V[i];

    switch(*ref)
    {
        case 'X':
            ref += 2;
            switch(*ref)
            {
                case 'x':
                    a = 0;
                    break;
                case 'y':
                    a = 1;
                    break;
                case 'z':
                    a = 2;
                    break;
            }
            ref++;
            switch(*ref)
            {
                case 'x':
                    b = 0;
                    break;
                case 'y':
                    b = 1;
                    break;
                case 'z':
                    b = 2;
                    break;
            }
            for(i = 0; i<no_edges; i++)
            {
                if (edges[i][0] == V[a] && edges[i][1] == V[b]) flag = 1;
            }
            return flag;
            break;
        case '-':
            ref++;
            if(eval(ref, edges, size, V) == 0) return 1;
            else return 0;
        case 'E':
            ref++;
            switch(*ref)
            {
                case 'x':
                    ref++;
                    for(i=0; i<size; i++)
                    {
                        Vstar[0] = i;
                        if (eval(ref, edges, size, Vstar) == 1) return 1;
                    }
                    return 0;
                    break;
                case 'y':
                    ref++;
                    for(i=0; i<size; i++)
                    {
                        Vstar[1] = i;
                        if (eval(ref, edges, size, Vstar) == 1) return 1;
                    }
                    return 0;
                    break;
                case 'z':
                    ref++;
                    for(i=0; i<size; i++)
                    {
                        Vstar[2] = i;
                        if (eval(ref, edges, size, Vstar) == 1) return 1;
                    }
                    return 0;
                    break;
            }
            break;
        case 'A':
            ref++;
            switch(*ref)
            {
                case 'x':
                    ref++;
                    for(i=0; i<size; i++)
                    {
                        Vstar[0] = i;
                        if (eval(ref, edges, size, Vstar) == 0) return 0;
                    }
                    return 1;
                    break;
                case 'y':
                    ref++;
                    for(i=0; i<size; i++)
                    {
                        Vstar[1] = i;
                        if (eval(ref, edges, size, Vstar) == 0) return 0;
                    }
                    return 1;
                    break;
                case 'z':
                    ref++;
                    for(i=0; i<size; i++)
                    {
                        Vstar[2] = i;
                        if (eval(ref, edges, size, Vstar) == 0) return 0;
                    }
                    return 1;
                    break;
            }
            break;
        case '(':
            ref++;
            a = eval(ref, edges, size, V);
            do
            {
                if(*ref == '(') scope++;
                if(*ref == ')') scope--;
                if((*ref == '^' || *ref == '>' || *ref == 'v') && scope == 0) break;
                ref++;
            }
            while(1);
            b = eval(ref+1, edges, size, V);
            switch(*ref)
            {
                case '^':
                    if(a == 1 && b == 1) return 1;
                    else return 0;
                    break;
                case 'v':
                    if(a == 1 || b == 1) return 1;
                    else return 0;
                    break;
                case '>':
                    if (a == 1 && b == 0) return 0;
                    else return 1;
                    break;
            }
            break;
    }

    return 0;
}



int main()
{
    char *name=malloc(Fsize); /*create space for the formula*/
    FILE *fp, *fpout;

    /* reads from input.txt, writes to output.txt*/
    if ((  fp=fopen("input.txt","r"))==NULL)
    {
        printf("Error opening file");
        exit(1);
    }
    if ((  fpout=fopen("output.txt","w"))==NULL)
    {
        printf("Error opening file");
        exit(1);
    }

    int j;
    for(j=0; j<cases; j++)
    {
        fscanf(fp, "%s %d %d",name,&no_nodes,&no_edges);/*read number of nodes, number of edges*/
        int edges[no_edges][2];  /*Store edges in 2D array*/
        for(i=0; i<no_edges; i++)	 fscanf(fp, "%d%d", &edges[i][0], &edges[i][1]); /*read all the edges*/

        /*Assign variables x, y, z to nodes */
        int W[3];
        /*Store variable values in array
        value of x in V[0]
        value of y in V[1]
        value of z in V[2] */
        for(i=0; i<3; i++)  fscanf(fp, "%d", &W[i]);
        int p=parse(name);
        switch(p)
        {
            case 0:
                fprintf(fpout,"%s is not a formula.  ", name);
                break;
            case 1:
                fprintf(fpout,"%s is an atomic formula.  ",name);
                break;
            case 2:
                fprintf(fpout,"%s is a negated formula.  ",name);
                break;
            case 3:
                fprintf(fpout,"%s is a binary connective formula.  ", name);
                break;
            case 4:
                fprintf(fpout,"%s is an existential formula.  ",name);
                break;
            case 5:
                fprintf(fpout,"%s is a universal formula.  ",name);
                break;
            default:
                fprintf(fpout,"%s is not a formula.  ",name);
                break;
        }

        /*Now check if formula is true in the graph with given variable assignment. */
        if (p !=0)
        {
            if (eval(name, edges, no_nodes,  W)==1)	fprintf(fpout,"The formula %s is true in G under W\n", name);
            else fprintf(fpout,"The formula %s is false in G under W\n", name);
        }

    }

    return(0);
}
