#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define ESCAPE_CHAR '\\'
#define isLowerAlpha(x)   ((x) >= 'a' && (x) <= 'z')
#define isUpperAlpha(x)   ((x) >= 'A' && (x) <= 'Z')
#define isNumeric(x)      ((x) >= '0' && (x) <= '9')
#define isAlphaNum(x)     (isLowerAlpha(x) || isUpperAlpha(x) || isNumeric(x))
#define isValidLexemme(x)  (isAlphaNum(x) || (x) == ' ' || (x) == '=' || (x) == ';' || (x) == ',' || (x) == '{' || (x) == '}' || (x) == '[' || (x) == ']' || (x) == '!' || (x) == '^' || (x) == '.')

//#define  DEBUG 0

typedef struct NDA_state {
    int edge_count;
    int isFinal;
    char *symbol_guard;
    struct NFA_state **edges;
} NFA_state;

NFA_state **global_state_table = NULL;
unsigned long long int global_state_count = 0;


void deleteState(NFA_state *state) {
    if(state->edge_count != 0) {
        free(state->edges);
        free(state->symbol_guard);
    }
    free(state);
}


void deleteAllStates() {
    int i;
    for(i = 0; i<global_state_count; i++) {
        deleteState(global_state_table[i]);
        global_state_table[i] = NULL;
    }
    if(global_state_table != NULL)
        free(global_state_table), global_state_table = NULL;
    global_state_count = 0;
}


NFA_state *getNDA_state() {
    NFA_state *result = malloc(sizeof(NFA_state));

    // keep a reference to the new state in the state table.
    global_state_table = realloc(global_state_table, ++global_state_count * sizeof(NFA_state));
    global_state_table[global_state_count-1] = result;


    result->edge_count = 0;
    result->isFinal = 0;
    result->edges = NULL;
    result->symbol_guard = NULL;
    return result;
}

void addEdge(NFA_state *state, char symbol, NFA_state *to) {
    state->edge_count++;
    state->edges = realloc(state->edges, sizeof(NFA_state *)*state->edge_count);
    state->symbol_guard = realloc(state->symbol_guard, sizeof(char)*state->edge_count);
    state->edges[state->edge_count-1] = to;
    state->symbol_guard[state->edge_count-1] = symbol;
    return;
}

// Parse an individual symbol
void symbol(char *string, NFA_state **start, NFA_state **end) {
    NFA_state *ed = *end;
    NFA_state *new = getNDA_state();
    addEdge(ed, *string, new);
    *end = new;
    *start = ed;
    return;
}


NFA_state *parse(char *str_p, NFA_state **endst) {
    char c = *str_p;

    // Check for ending status
    if(c == '|' || c == ')' || c == '\0') return NULL;

#ifdef DEBUG
    printf("Parse called on %c\n", *str_p);
#endif

    // Check for alphanum status
    if(isValidLexemme(c) ||
            // Or if Escape char move one forward and parse
            (c == ESCAPE_CHAR)){
        if(c == ESCAPE_CHAR) {
            str_p++;
            // Handle escape codes
            c = *str_p;
            switch(c) {
                case 'e':
                    c = '\0';
                    break;
                default:
                    break;
            }

        }
#ifdef DEBUG
        printf("Parsing alphachar\n");
#endif
        if(*(str_p+1) != '*') {
#ifdef DEBUG
            printf("Parsing without Kleane\n");
#endif



            NFA_state *nd;
            NFA_state *next = parse(str_p+1, &nd);
            NFA_state *state = getNDA_state();
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
#ifdef DEBUG
            printf("Parsing with Kleane\n");
#endif
            NFA_state *nding;
            NFA_state *st = getNDA_state();
            NFA_state *end = parse(str_p+2, &nding);
            if(end == NULL){
                end = getNDA_state();
                if(endst != NULL) {
                    *endst = end;
                }
            } else if (endst != NULL){
                 *endst = nding;
            }
            NFA_state *mid = getNDA_state();
            NFA_state *nd = getNDA_state();
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

#ifdef DEBUG
        printf("Parsing subformula\n");
#endif
        char *str = str_p;
        // Acumulate all sub regexes

        // Storing all subregexes into an array of states_st - to be combined at end
        NFA_state **states_st = NULL;
        NFA_state **states_nd = NULL;
        int count = 0;


        int scope = 0;
            while(1) {
                NFA_state *end_st;
                NFA_state *subRegex_st = parse(str+1, &end_st);


                // Store into array
                states_st = realloc(states_st, (size_t) ++count*sizeof(struct NDA_state*));
                states_st[count-1] = subRegex_st;

                states_nd = realloc(states_nd, (size_t)count*sizeof(struct NDA_state*));
                states_nd[count-1] = end_st;


                char q;
                //drop chars until | or ) in scope
                while((q = *str)) {

#ifdef DEBUG
                    printf("Considering %c - the scope is %d\n", q, scope);
#endif
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
                            NFA_state *end_state;
                            NFA_state *subRegex_state = parse(str+1, &end_state);


                            // Store into array
                            states_st = realloc(states_st, (size_t) ++count*sizeof(struct NDA_state*));
                            states_st[count-1] = subRegex_state;

                            states_nd = realloc(states_nd, (size_t)count*sizeof(struct NDA_state*));
                            states_nd[count-1] = end_state;
                            }
                            break;
                        case ESCAPE_CHAR:
                            // If escape char, skip next char
                            str++;
                            break;
                        default:
                            break;
                    }

                    if(breaking) break;
                    str++;
                }
#ifdef DEBUG
                printf("I'm breaking out on a %c\n", *str);
#endif
                if(*str == ')') break;
                else{
                    fprintf(stderr, "Error: Fatal - Could not parse regex formula - is it well defined?");
                    return NULL;
                }

            }
#ifdef DEBUG
        printf("1Char lookahead says %c\n", *(str+1));
        printf("Does '\\0' == *(str+1)? let's find out: %d\n", ('\0' == *(str+1)));
#endif

        // Now to combine into a single state system
        // Check if Kleane's system required
        if(*(str+1) != '*') {

            NFA_state *con_strt = getNDA_state();
            NFA_state *ending;
            NFA_state *con_end = NULL;
            if(*(str+1) != '\0') {
                con_end = parse(str+1, &ending);
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
#ifdef DEBUG
            printf("Parsing subformula with Kleane\n");
#endif
            NFA_state *beg = getNDA_state();
            NFA_state *con_strt = getNDA_state();
            NFA_state *ending;
            NFA_state *true_end = parse(str+2, &ending);
            NFA_state *con_end = getNDA_state();

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

bool checkString(char *string, NFA_state *state) {
    int i;
    // Vacuously true
    if(state->isFinal == 1){
        if(*string == '\0')
            return 1;

    }

    for(i =0; i<state->edge_count;i++) {

        char c = state->symbol_guard[i];

        switch(c) {
            case '\0':
                if(checkString(string, (NFA_state *) state->edges[i]))
                    return 1;
                break;
            default:
                if(c == *string)
                    if(checkString(string+1, (NFA_state *) state->edges[i]) == 1)
                        return 1;
        }
    }


    return 0;

}


void test_NFA_parsing(){

    deleteAllStates();

    NFA_state *end;
    NFA_state *regex;


    regex = parse("a", &end);
    end->isFinal = 1;


    assert(checkString("a", regex) == 1);
    assert(checkString("b", regex) == 0);
    assert(checkString("ab", regex) == 0);
    assert(checkString("abc", regex) == 0);

    deleteAllStates();


    regex = parse("(a|b)", &end);
    end->isFinal = 1;

    assert(checkString("a", regex) == 1);
    assert(checkString("b", regex) == 1);
    assert(checkString("ab", regex) == 0);
    assert(checkString("abc", regex) == 0);


    deleteAllStates();

    regex = parse("a*", &end);
    end->isFinal = 1;

    assert(checkString("a", regex) == 1);
    assert(checkString("aa", regex) == 1);
    assert(checkString("aaaaaaaaa", regex) == 1);
    assert(checkString("aaab", regex) == 0);
    assert(checkString("aaaba", regex) == 0);
    assert(checkString("baaaaa", regex) == 0);
    assert(checkString("bbbbb", regex) == 0);


    deleteAllStates();

    regex = parse("(a|b)*", &end);
    end->isFinal = 1;


    assert(checkString("a", regex) == 1);
    assert(checkString("aa", regex) == 1);
    assert(checkString("aaaaaaaaa", regex) == 1);
    assert(checkString("aaab", regex) == 1);
    assert(checkString("aaaba", regex) == 1);
    assert(checkString("baaaaa", regex) == 1);
    assert(checkString("bbbbb", regex) == 1);
    assert(checkString("as", regex) == 0);
    assert(checkString("aas", regex) == 0);
    assert(checkString("aaaaaaaasa", regex) == 0);
    assert(checkString("aaabs", regex) == 0);
    assert(checkString("aaabas", regex) == 0);
    assert(checkString("baaaaas", regex) == 0);
    assert(checkString("bbbsbb", regex) == 0);


    deleteAllStates();

    regex = parse("(a|b)*s", &end);
    end->isFinal = 1;

    assert(checkString("as", regex) == 1);
    assert(checkString("aas", regex) == 1);
    assert(checkString("aaaaaaaaas", regex) == 1);
    assert(checkString("aaabs", regex) == 1);
    assert(checkString("aaabas", regex) == 1);
    assert(checkString("baaaaas", regex) == 1);
    assert(checkString("bbbbbs", regex) == 1);
    assert(checkString("as", regex) == 1);
    assert(checkString("aas", regex) == 1);
    assert(checkString("aaaaaaaasa", regex) == 0);
    assert(checkString("aaabs", regex) == 1);
    assert(checkString("aaabas", regex) == 1);
    assert(checkString("baaaaas", regex) == 1);
    assert(checkString("bbbsbb", regex) == 0);

    deleteAllStates();


    printf("Testing complete - regex works as expected.\n");

}
void append(char **string, char c) {
    if(*string != NULL) {
    int i = 0;
    char *ref;
    for(ref = *string; *ref != '\0'; ref++) i++;
        (*string) = realloc(*string, (i+2)*sizeof(char));
        (*string)[i] = c;
        (*string)[i+1] = '\0';
    } else {
        *string = malloc(2*sizeof(char));
        (*string)[0] = c;
        (*string)[1] = '\0';
    }
    return;
}
// Provides support for standard regex codes
char *pre_parse(char *string){
    char *ref= string;
    char *factored = NULL;
    for(ref = string; *ref != '\0'; ref++) {
        switch(*ref) {
            case '[':
            {   // scan forward until end
                char *temp;
                // Check valid before parse
                for(temp = ref; *temp != '\0' && *temp != ']'; temp++){if(*temp == '\\') temp++;};
                if(*temp != ']') {
                    fprintf(stderr, "Error: Fatal - Preparsing error, are you missing a ']'?\n");
                    if(factored != NULL) free(factored);
                    return NULL;
                } else {
                    append(&factored, '(');
                    for(temp = ref+1; *temp != '\0' && *temp != ']'; temp++){
                        if(temp != ref+1)
                            append(&factored, '|');
                        if(*temp == '\\') {
                            temp++;
                            switch(*(temp)){
                                case 'w':
                                    append(&factored, '(');
                                    append(&factored, 'a');
                                    append(&factored, '|');
                                    append(&factored, 'b');
                                    append(&factored, '|');
                                    append(&factored, 'c');
                                    append(&factored, '|');
                                    append(&factored, 'd');
                                    append(&factored, '|');
                                    append(&factored, 'e');
                                    append(&factored, '|');
                                    append(&factored, 'f');
                                    append(&factored, '|');
                                    append(&factored, 'g');
                                    append(&factored, '|');
                                    append(&factored, 'h');
                                    append(&factored, '|');
                                    append(&factored, 'i');
                                    append(&factored, '|');
                                    append(&factored, 'j');
                                    append(&factored, '|');
                                    append(&factored, 'k');
                                    append(&factored, '|');
                                    append(&factored, 'l');
                                    append(&factored, '|');
                                    append(&factored, 'm');
                                    append(&factored, '|');
                                    append(&factored, 'n');
                                    append(&factored, '|');
                                    append(&factored, 'o');
                                    append(&factored, '|');
                                    append(&factored, 'p');
                                    append(&factored, '|');
                                    append(&factored, 'q');
                                    append(&factored, '|');
                                    append(&factored, 'r');
                                    append(&factored, '|');
                                    append(&factored, 's');
                                    append(&factored, '|');
                                    append(&factored, 't');
                                    append(&factored, '|');
                                    append(&factored, 'u');
                                    append(&factored, '|');
                                    append(&factored, 'v');
                                    append(&factored, '|');
                                    append(&factored, 'w');
                                    append(&factored, '|');
                                    append(&factored, 'x');
                                    append(&factored, '|');
                                    append(&factored, 'y');
                                    append(&factored, '|');
                                    append(&factored, 'z');
                                    append(&factored, ')');
                                    break;
                                case 'd':
                                    append(&factored, '(');
                                    append(&factored, '0');
                                    append(&factored, '|');
                                    append(&factored, '1');
                                    append(&factored, '|');
                                    append(&factored, '2');
                                    append(&factored, '|');
                                    append(&factored, '3');
                                    append(&factored, '|');
                                    append(&factored, '4');
                                    append(&factored, '|');
                                    append(&factored, '5');
                                    append(&factored, '|');
                                    append(&factored, '6');
                                    append(&factored, '|');
                                    append(&factored, '7');
                                    append(&factored, '|');
                                    append(&factored, '8');
                                    append(&factored, '|');
                                    append(&factored, '9');
                                    append(&factored, ')');
                                    break;
                                default:


                                    append(&factored, '\\');
                                // Skip the escaped character, but append it to result
                                append(&factored, *temp);
                                break;
                            }
                        } else {
                            append(&factored, *temp);
                        }
                    }
                    append(&factored, ')');
                }
                ref = temp+1;
            } break;
            case '\\':
                    ref++;
                    switch(*(ref)){

                        case 'w':
                            append(&factored, '(');
                            append(&factored, 'a');
                            append(&factored, '|');
                            append(&factored, 'b');
                            append(&factored, '|');
                            append(&factored, 'c');
                            append(&factored, '|');
                            append(&factored, 'd');
                            append(&factored, '|');
                            append(&factored, 'e');
                            append(&factored, '|');
                            append(&factored, 'f');
                            append(&factored, '|');
                            append(&factored, 'g');
                            append(&factored, '|');
                            append(&factored, 'h');
                            append(&factored, '|');
                            append(&factored, 'i');
                            append(&factored, '|');
                            append(&factored, 'j');
                            append(&factored, '|');
                            append(&factored, 'k');
                            append(&factored, '|');
                            append(&factored, 'l');
                            append(&factored, '|');
                            append(&factored, 'm');
                            append(&factored, '|');
                            append(&factored, 'n');
                            append(&factored, '|');
                            append(&factored, 'o');
                            append(&factored, '|');
                            append(&factored, 'p');
                            append(&factored, '|');
                            append(&factored, 'q');
                            append(&factored, '|');
                            append(&factored, 'r');
                            append(&factored, '|');
                            append(&factored, 's');
                            append(&factored, '|');
                            append(&factored, 't');
                            append(&factored, '|');
                            append(&factored, 'u');
                            append(&factored, '|');
                            append(&factored, 'v');
                            append(&factored, '|');
                            append(&factored, 'w');
                            append(&factored, '|');
                            append(&factored, 'x');
                            append(&factored, '|');
                            append(&factored, 'y');
                            append(&factored, '|');
                            append(&factored, 'z');
                            append(&factored, ')');
                            break;
                        case 'd':
                            append(&factored, '(');
                            append(&factored, '0');
                            append(&factored, '|');
                            append(&factored, '1');
                            append(&factored, '|');
                            append(&factored, '2');
                            append(&factored, '|');
                            append(&factored, '3');
                            append(&factored, '|');
                            append(&factored, '4');
                            append(&factored, '|');
                            append(&factored, '5');
                            append(&factored, '|');
                            append(&factored, '6');
                            append(&factored, '|');
                            append(&factored, '7');
                            append(&factored, '|');
                            append(&factored, '8');
                            append(&factored, '|');
                            append(&factored, '9');
                            append(&factored, ')');
                            break;
                        default:

                            append(&factored, '\\');
                            // Skip the escaped character, but append it to result
                            append(&factored, *ref);
                            break;
                }

                break;
            default:
                append(&factored, *ref);
        }
    }

    return factored;
}

int main() {
    //test_NFA_parsing();
    NFA_state *end;

    char *parsed = pre_parse("\\w*\\[\\d*\\]");
    printf("%s\n", parsed);
    NFA_state *result = parse(parsed, &end);
    end->isFinal = 1;
    if(checkString("a[2]", result)){
        printf("Your string matches the spec my friend!\n");
    } else {
        printf("Er-awww. Wrong string buddy.\n");
    }
    return 0;
}