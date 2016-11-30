#include <stdio.h>
#include <string.h>   /* for all the new-fangled string functions */
#include <stdlib.h>     /* malloc, free, rand */


const int Fsize=50;
int no_edges;
int no_nodes;
int i;
const int cases=6;


int parse(char *g)
{/* return 1 if atomic, 2 if  neg, 3 if binary, 4 if exists, 5 if for all, ow 0*/
    char *ref = g;
    int scope = 0;
    int flag = 0;
    int negation = 0;
    int output = 0;
    while(*ref) {
                            //fprintf(stdout, "%d\n", ref-g);
        switch(*ref) {
        case 'X':
            if(ref-g == 0) output = 1;
            if(negation == 1) negation = 0;
            ref++;
                            //fprintf(stdout, "%c", *ref);
            if (*ref != '[') return 0;
            ref++;
                            //fprintf(stdout, "%c", *ref);
            if (!(*ref == 'x' || *ref == 'y' || *ref == 'z')) return 0;
            ref++;
                            //fprintf(stdout, "%c", *ref);
            if (!(*ref == 'x' || *ref == 'y' || *ref == 'z')) return 0;
            ref++;
                            //fprintf(stdout, "%c", *ref);
            if (*ref != ']') return 0;
            break;
        case 'A':
            if(ref-g == 0) output = 5;
            if(negation == 1) negation = 0;
            ref++;
                            //fprintf(stdout, "%c", *ref);
            if (!(*ref == 'x' || *ref == 'y' || *ref == 'z')) return 0;
            break;
        case 'E':
            if(ref-g == 0) output = 4;
            if(negation == 1) negation = 0;
            ref++;
                            //fprintf(stdout, "%c", *ref);
            if (!(*ref == 'x' || *ref == 'y' || *ref == 'z')) return 0;
            break;
        case '(':
               if(ref-g == 0) output = 3;
               if(negation == 1) negation = 0;
               scope++;
            break;
        case ')':
            if(negation == 1) return 0;
            scope--;
            if(scope < 0) return 0;
            if (flag != 1) return 0;
            flag = 0;
            break;
        case '>':
            if(negation == 1) return 0;
            flag++;
            break;
        case 'v':
            if(negation == 1) return 0;
            flag++;
            break;
        case '^':
            if(negation == 1) return 0;
            flag++;
            break;
        case '-':
            if(ref-g == 0) output = 2;
            negation = 1;
            break;
        default:
            //fprintf(stdout, "%c", *ref);
            return 0;
            break;
        }
        ref++;
    }
    fprintf(stdout, "Upon reaching the end scope is %d\n", scope);
    if (scope != 0) return 0;
    if(flag != 0) return 0;
    return output;
}



int eval(char *nm, int edges[no_edges][2], int size, int V[3])
{/* returns 1 if formla nm evaluates to true in graph with 'size' nodes, no_edges edges, edges stored in 'edges', variable assignment V.  Otherwise returns 0.*/





}



int main()
{
  char *name=malloc(Fsize); /*create space for the formula*/
  FILE *fp, *fpout;

  /* reads from input.txt, writes to output.txt*/
 if ((  fp=fopen("input.txt","r"))==NULL){printf("Error opening file");exit(1);}
  if ((  fpout=fopen("output.txt","w"))==NULL){printf("Error opening file");exit(1);}

  int j;
  for(j=0;j<cases;j++)
    {
      fscanf(fp, "%s %d %d",name,&no_nodes,&no_edges);/*read number of nodes, number of edges*/
      int edges[no_edges][2];  /*Store edges in 2D array*/
      for(i=0;i<no_edges;i++)	 fscanf(fp, "%d%d", &edges[i][0], &edges[i][1]);/*read all the edges*/

      /*Assign variables x, y, z to nodes */
      int W[3];
      /*Store variable values in array
	value of x in V[0]
	value of y in V[1]
	value of z in V[2] */
      for(i=0;i<3;i++)  fscanf(fp, "%d", &W[i]);
      int p=parse(name);
      switch(p)
	{case 0:fprintf(fpout,"%s is not a formula.  ", name);break;
	case 1: fprintf(fpout,"%s is an atomic formula.  ",name);break;
	case 2: fprintf(fpout,"%s is a negated formula.  ",name);break;
	case 3: fprintf(fpout,"%s is a binary connective formula.  ", name);break;
	case 4: fprintf(fpout,"%s is an existential formula.  ",name);break;
	case 5: fprintf(fpout,"%s is a universal formula.  ",name);break;
	default: fprintf(fpout,"%s is not a formula.  ",name);break;
	}

      /*Now check if formula is true in the graph with given variable assignment. */
      if (eval(name, edges, no_nodes,  W)==1)	fprintf(fpout,"The formula %s is true in G under W\n", name);
      else fprintf(fpout,"The formula %s is false in G under W\n", name);
    }

  return(0);
}



