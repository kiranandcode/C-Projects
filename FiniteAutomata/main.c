#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define isLowerAlpha(x)   ((x) >= 'a' && (x) <= 'z')
#define isUpperAlpha(x)   ((x) >= 'A' && (x) <= 'Z')
#define isNumeric(x)      ((x) >= '0' && (x) <= '9')
#define isAlphaNum(x)     (isLowerAlpha(x) || isUpperAlpha(x) || isNumeric(x))

typedef struct NDA_state {
    int edge_count;
    int isFinal;
    char *symbol_guard;
    struct NDA_state **edges;
} NDA_state;

NDA_state *getNDA_state() {
    NDA_state *result = malloc(sizeof(NDA_state));
    result->edge_count = 0;
    result->isFinal = 0;
    result->edges = NULL;
    result->symbol_guard = NULL;
    return result;
}

void addEdge(NDA_state *state, char symbol, NDA_state *to) {
    state->edge_count++;
    state->edges = realloc(state->edges, sizeof(NDA_state *)*state->edge_count);
    state->symbol_guard = realloc(state->symbol_guard, sizeof(char)*state->edge_count);
    state->edges[state->edge_count-1] = to;
    state->symbol_guard[state->edge_count-1] = symbol;
    return;
}

// Parse an individual symbol
void symbol(char *string, NDA_state **start, NDA_state **end) {
    NDA_state *ed = *end;
    NDA_state *new = getNDA_state();
    addEdge(ed, *string, new);
    *end = new;
    *start = ed;
    return;
}


NDA_state *parse(char *str_p, NDA_state **endst) {
    char c = *str_p;

    // Check for ending status
    if(c == '|' || c == ')' || c == '\0') return NULL;

    printf("Parse called on %c\n", *str_p);

    // Check for alphanum status
    if(isAlphaNum(c)){
        printf("Parsing alphachar\n");
        if(*(str_p+1) != '*') {
            printf("Parsing without Kleane\n");
            NDA_state *nd;
            NDA_state *next = parse(str_p+1, &nd);
            NDA_state *state = getNDA_state();
            if(next == NULL){
                next = getNDA_state();
                if(endst != NULL)
                    *endst = next;
            }
            else if(endst != NULL)
                *endst = nd;

            addEdge(state, c, next);
            return state;
        } else {
            printf("Parsing with Kleane\n");
            NDA_state *nding;
            NDA_state *st = getNDA_state();
            NDA_state *end = parse(str_p+2, &nding);
            if(end == NULL){
                end = getNDA_state();
                if(endst != NULL) {
                    *endst = end;
                }
            } else if (endst != NULL){
                 *endst = nding;
            }
            NDA_state *mid = getNDA_state();
            NDA_state *nd = getNDA_state();
            addEdge(mid, c, nd);
            addEdge(nd, '\0', mid);
            addEdge(nd, '\0', end);
            addEdge(st, '\0', mid);
            addEdge(st, '\0', end);
            return st;
        }
    }

    // Implement parsing for recursive () expressions
    if(c=='(') {
        printf("Parsing subformula\n");
        char *str = str_p;
        // Acumulate all sub regexes

        // Storing all subregexes into an array of states_st - to be combined at end
        NDA_state **states_st = NULL;
        NDA_state **states_nd = NULL;
        int count = 0;


        int scope = 0;
            while(1) {
                NDA_state *end_st;
                NDA_state *subRegex_st = parse(str+1, &end_st);


                // Store into array
                states_st = realloc(states_st, (size_t) ++count);
                states_st[count-1] = subRegex_st;

                states_nd = realloc(states_nd, (size_t)count);
                states_nd[count-1] = end_st;


                char q;
                //drop chars until | or ) in scope
                while(q = *str) {
                    printf("Considering %c - the scope is %d\n", q, scope);
                    int breaking = 0;

                    switch(q) {
                        case '(':
                            scope++;
                            break;
                        case ')':
                            scope--;
                            if(scope == 0)
                                breaking = 1;
                            break;
                        case '|':
                            {
                            // Drop first character, and parse
                            NDA_state *end_st;
                            NDA_state *subRegex_st = parse(str+1, &end_st);


                            // Store into array
                            states_st = realloc(states_st, (size_t) ++count);
                            states_st[count-1] = subRegex_st;

                            states_nd = realloc(states_nd, (size_t)count);
                            states_nd[count-1] = end_st;
                            }
                            break;
                        default:
                            break;
                    }

                    if(breaking) break;
                    str++;
                }
                if(*str == ')') break;
            }

        // Now to combine into a single state system
        // Check if Kleane's system required
        if(*(str+1) != '*') {
            NDA_state *con_strt = getNDA_state();
            NDA_state *ending;
            NDA_state *con_end = NULL;
            if(*(str+1) != '\0') {
                con_end = parse(str+2, &ending);
            }
            if(con_end == NULL){
                con_end = getNDA_state();
                if(endst != NULL)
                    *endst = con_end;
            } else if (endst != NULL) {
                *endst = ending;
            }
            int i;
            for(i = 0; i<count; i++) {
                addEdge(con_strt, '\0', states_st[i]);
                if(states_st[i] == NULL) fprintf(stderr, "Error: Fatal - Internal Parser error - See line 153.\n");
                if(states_nd[i] == NULL) fprintf(stderr, "Error: Fatal - Internal Parser Error, end or start node is null - See line 154.\n");
                addEdge(states_nd[i], '\0', con_end);
            }


            return con_strt;

        } else {
            printf("Parsing subformula with Kleane\n");
            NDA_state *beg = getNDA_state();
            NDA_state *con_strt = getNDA_state();
            NDA_state *ending;
            NDA_state *true_end = parse(str+2, &ending);
            NDA_state *con_end = getNDA_state();

            if(true_end == NULL){
                true_end = getNDA_state();
                if(endst != NULL)
                    *endst = true_end;
            } else if (endst != NULL) {
                *endst = ending;
            }
            int i;
            for(i = 0; i<count; i++) {
                addEdge(con_strt, '\0', states_st[i]);
                if(states_st[i] == NULL) fprintf(stderr, "Error: Fatal - Internal Parser error - See line 153.\n");
                if(states_nd[i] == NULL) fprintf(stderr, "Error: Fatal - Internal Parser Error, end or start node is null - See line 154.\n");
                addEdge(states_nd[i], '\0', con_end);
            }


            addEdge(beg, '\0', con_strt);
            addEdge(con_end, '\0', con_strt);
            addEdge(con_end, '\0', true_end);
            addEdge(beg, '\0', true_end);

            return beg;

        }


    }



}


int main() {
    NDA_state *result = parse("((a*|b)cd)*", NULL);
    printf("Hello, World!\n");
    return 0;
}