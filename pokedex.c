// Assignment 2 19T1 COMP1511: Pokedex
// pokedex.c
//
// This program was written by YOUR-NAME-HERE (z5555555)
// on INSERT-DATE-HERE
//
// Version 1.0.0: Assignment released.
// Version 1.0.1: Minor clarifications about `struct pokenode`.
// Version 1.1.0: Moved destroy_pokedex function to correct location.
// Version 1.1.1: Renamed "pokemon_id" to "id" in change_current_pokemon.

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Add any extra #includes your code needs here.
//
// But note you are not permitted to use functions from string.h
// so do not #include <string.h>

#include "pokedex.h"

// Add your own #defines here.

//Colour the output of the error messages to make them clear;
//Code from StackOverflow:
//https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c
#define COL_ERR_RED "\033[0;31m"
#define COL_ERR_RESET "\033[0m"

#define TRUE 1
#define FALSE 0

// Note you are not permitted to use arrays in struct pokedex,
// you must use a linked list.
//
// You are also not permitted to use arrays in any of the functions
// below.
//
// The only exception is that char arrays are permitted for
// search_pokemon and functions it may call, as well as the string
// returned by pokemon_name (from pokemon.h).
//
// You will need to add fields to struct pokedex.
// You may change or delete the head field.

struct pokedex {
    struct pokenode *head;
    struct pokenode *last;
    struct pokenode *selectedNode;
    int numNodes;
};


// You don't have to use the provided struct pokenode, you are free to
// make your own struct instead.
// If you use the provided struct pokenode, you will need to add fields
// to it to store other information.

struct pokenode {
    struct pokenode *next;
    struct pokenode *previous;
    Pokemon         pokemon;
    int found;
};

// Add any other structs you define here.


// Add prototypes for any extra functions you create here.
static struct pokenode *create_pokenode(Pokemon pokemon);
static void check_for_duplicate_id(Pokedex pokedex, struct pokenode *node);
static void add_node_to_dex(Pokedex pokedex, struct pokenode *node);

static void print_seen_pokemon(struct pokenode *pokenode);
static void print_caught_pokemon(struct pokenode *pokenode);
static void print_censored_name(const char *name);

static void print_first_pokemon(Pokedex pokedex);
static void print_pokemon_to_list(Pokedex pokedex, struct pokenode *pokenode);
static void print_selection_indicator(Pokedex pokedex, struct pokenode *pokenode);

static void reset_pokedex(Pokedex pokedex);

static void printerr(char *message);

// You need to implement the following 20 functions.
// In other words, replace the lines calling fprintf & exit with your code.
// You can find descriptions of what each function should do in pokedex.h


Pokedex new_pokedex(void) {
    Pokedex new_pokedex = malloc(sizeof (struct pokedex));
    assert(new_pokedex != NULL);
    // add your own code here
    new_pokedex->head = NULL;
    new_pokedex->numNodes = 0;

    return new_pokedex;
}

////////////////////////////////////////////////////////////////////////
//                         Stage 1 Functions                          //
////////////////////////////////////////////////////////////////////////

void add_pokemon(Pokedex pokedex, Pokemon pokemon) {

    struct pokenode *node = create_pokenode(pokemon);
    check_for_duplicate_id(pokedex, node);
    add_node_to_dex(pokedex, node);

    //DEBUG OUTPUT
    // printf("numNodes = %d\n", pokedex->numNodes);
    // printf("head= %p\n", pokedex->head);
    // printf("second= %p\n", pokedex->head->next);
    // if(pokedex->numNodes >=2) {
    //     printf("third= %p\n", pokedex->head->next->next);
    // }
    // printf("\nselected node: %p\n", pokedex->selectedNode);
    // printf("last node: %p\n", pokedex->last);
}

void detail_pokemon(Pokedex pokedex) {

    if (pokedex->head != NULL) {
        struct pokenode *selectedPokenode = pokedex->selectedNode;

        if (selectedPokenode->found == FALSE) {
            print_seen_pokemon(selectedPokenode);
        } else {
            print_caught_pokemon(selectedPokenode);
        }
    } else {
        printerr("ERROR: The Pokédex is empty!");
    }

}

Pokemon get_current_pokemon(Pokedex pokedex) {

    if (pokedex->head == NULL) {
        printerr("ERROR: The Pokédex is empty!");
        return NULL;
    } else {
        return pokedex->selectedNode->pokemon;
    }

}

void find_current_pokemon(Pokedex pokedex) {

    if (pokedex->head != NULL) {
        pokedex->selectedNode->found = TRUE;
    } else {
        printerr("ERROR: The Pokédex is empty!");
    }

}

void print_pokemon(Pokedex pokedex) {

    if (pokedex->head != NULL) {
        print_first_pokemon(pokedex);
        struct pokenode *p = pokedex->head->next;
        while (p != NULL) {
            print_pokemon_to_list(pokedex, p);
            p = p->next;
        }
    } else {
        printerr("ERROR: The Pokédex is empty!");
    }

}

////////////////////////////////////////////////////////////////////////
//                         Stage 2 Functions                          //
////////////////////////////////////////////////////////////////////////

void next_pokemon(Pokedex pokedex) {

    if (pokedex->head != NULL && pokedex->selectedNode->next != NULL) {
        pokedex->selectedNode = pokedex->selectedNode->next;
    }

}

void prev_pokemon(Pokedex pokedex) {

    if (pokedex->head != NULL && pokedex->selectedNode->previous != NULL) {
        pokedex->selectedNode = pokedex->selectedNode->previous;
    }

}

void change_current_pokemon(Pokedex pokedex, int id) {

    struct pokenode *match = NULL;
    struct pokenode *p = pokedex->head;

    while (match == NULL && p != NULL) {
        if (pokemon_id(p->pokemon) == id) {
            match = p;
        }
        p = p->next;
    }

    if (match != NULL) {
        pokedex->selectedNode = match;
    }

}

void remove_pokemon(Pokedex pokedex) {

    if (pokedex->head != NULL) {
        if (pokedex->numNodes == 1) {
            free(pokedex->head);
            reset_pokedex(pokedex);
        } else if (pokedex->selectedNode == pokedex->last) {
            pokedex->selectedNode = pokedex->last->previous;
            free(pokedex->last);
            pokedex->selectedNode->next = NULL;
            pokedex->last = pokedex->selectedNode;

            pokedex->numNodes -= 1;
        } else {

        }
    }

}

void destroy_pokedex(Pokedex pokedex) {
    fprintf(stderr, "exiting because you have not implemented the destroy_pokedex function in pokedex.c\n");
    exit(1);
}

////////////////////////////////////////////////////////////////////////
//                         Stage 3 Functions                          //
////////////////////////////////////////////////////////////////////////

void go_exploring(Pokedex pokedex, int seed, int factor, int how_many) {
    fprintf(stderr, "exiting because you have not implemented the go_exploring function in pokedex.c\n");
    exit(1);
}

int count_found_pokemon(Pokedex pokedex) {
    fprintf(stderr, "exiting because you have not implemented the count_found_pokemon function in pokedex.c\n");
    exit(1);
}

int count_total_pokemon(Pokedex pokedex) {
    fprintf(stderr, "exiting because you have not implemented the count_total_pokemon function in pokedex.c\n");
    exit(1);
}

////////////////////////////////////////////////////////////////////////
//                         Stage 4 Functions                          //
////////////////////////////////////////////////////////////////////////

void add_pokemon_evolution(Pokedex pokedex, int from_id, int to_id) {
    fprintf(stderr, "exiting because you have not implemented the add_pokemon_evolution function in pokedex.c\n");
    exit(1);
}

void show_evolutions(Pokedex pokedex) {
    fprintf(stderr, "exiting because you have not implemented the show_evolutions function in pokedex.c\n");
    exit(1);
}

int get_next_evolution(Pokedex pokedex) {
    fprintf(stderr, "exiting because you have not implemented the get_next_evolution function in pokedex.c\n");
    exit(1);
}

////////////////////////////////////////////////////////////////////////
//                         Stage 5 Functions                          //
////////////////////////////////////////////////////////////////////////

Pokedex get_pokemon_of_type(Pokedex pokedex, pokemon_type type) {
    fprintf(stderr, "exiting because you have not implemented the get_pokemon_of_type function in pokedex.c\n");
    exit(1);
}

Pokedex get_found_pokemon(Pokedex pokedex) {
    fprintf(stderr, "exiting because you have not implemented the get_found_pokemon function in pokedex.c\n");
    exit(1);
}

Pokedex search_pokemon(Pokedex pokedex, char *text) {
    fprintf(stderr, "exiting because you have not implemented the search_pokemon function in pokedex.c\n");
    exit(1);
}

// Add definitions for your own functions below.
// Make them static to limit their scope to this file.

//create_pokenode utilised malloc to assign a chunk of memory to a pokenode,
//then populate it with the Pokémon it recieves, and returns a pointer to a
//pokenode. It also checks to see if there is enough memory to assign.
static struct pokenode *create_pokenode(Pokemon pokemon) {

    struct pokenode *node = malloc(sizeof(struct pokenode));

    //malloc returns a null pointer if there is no available memory
    if (node == NULL) {
        //Coloured error mesasge, for clarity's sake.
        printerr("ERROR: Out of memory!");
    }

    node->next = NULL;
    node->pokemon = pokemon;
    node->found = FALSE;

    return node;

}

//add_node_to_dex has two branches of code. First, if this node is to be the
//initial node, it initialises the head pointer of the Pokedex struct, as well
//as the selected node, which does not change when adding a Pokémon.
//Otherwise, it simply points the last node's next pointer to the newly created
//node. Finally, it increments the number of nodes in the list, and sets the
//last node in the list to the new node.
static void add_node_to_dex(Pokedex pokedex, struct pokenode *node) {
    if (pokedex->numNodes == 0) {
        pokedex->head = node;
        node->previous = NULL;
        pokedex->selectedNode = pokedex->head;
    } else {
        node->previous = pokedex->last;
        pokedex->last->next = node;
    }

    pokedex->last = node;
    pokedex->numNodes++;

}

//check_for_duplicate_id checks if there is already a Pokémon with that ID
//stored in the Pokédex. For example, if Glameow (#431) is added using
//add_pokemon() twice, the program would print out an error message, and exit.
static void check_for_duplicate_id(Pokedex pokedex, struct pokenode *node) {

    struct pokenode *p = pokedex->head;

    while(p != NULL) {
        if (pokemon_id(p->pokemon) == pokemon_id(node->pokemon)) {
            //Coloured error mesasge, for clarity's sake.
            printerr("ERROR: Duplicate Pokémon ID!");
        }

        p = p->next;
    }

}

//print_seen_pokemon only prints the ID of the given Pokémon, and censors then
//remaining information.
static void print_seen_pokemon(struct pokenode *pokenode) {

    int id = pokemon_id(pokenode->pokemon);

    printf("Id: %03d\n", id);
    printf("Name: ");
    print_censored_name(pokemon_name(pokenode->pokemon));
    printf("Height: --\n");
    printf("Weight: --\n");
    printf("Type: --\n");

}

//print_caught_pokemon prints the information of the given Pokémon. It ensures
//to check that a second type is present before printing one.
static void print_caught_pokemon(struct pokenode *pokenode) {

    int id = pokemon_id(pokenode->pokemon);
    char *name = pokemon_name(pokenode->pokemon);
    double height = pokemon_height(pokenode->pokemon);
    double weight = pokemon_weight(pokenode->pokemon);
    pokemon_type firstType = pokemon_first_type(pokenode->pokemon);
    pokemon_type secondType = pokemon_second_type(pokenode->pokemon);

    printf("Id: %03d\n", id);
    printf("Name: %s\n", name);
    printf("Height: %.2lfm\n", height);
    printf("Weight: %.2lfkg\n", weight);

    printf("Type: %s", pokemon_type_to_string(firstType));
    if (secondType != NONE_TYPE) {
        printf(" %s", pokemon_type_to_string(secondType));

    }
    printf("\n");

}

//print_censored_name prints out the correct number of "*", one for each
//character in the Pokémon's name.
static void print_censored_name(const char *name) {

    int i = 0;
    while (name[i] != '\0') {
        printf("*");
        i++;
    }
    printf("\n");

}

//print_first_pokemon begins the list when the 'p' command is called by printing
//the first, or header, Pokémon.
static void print_first_pokemon(Pokedex pokedex) {

    struct pokenode *headerNode = pokedex->head;
    int id = pokemon_id(headerNode->pokemon);
    const char *name = pokemon_name(headerNode->pokemon);

    print_selection_indicator(pokedex, headerNode);

    printf("    #%03d: ", id);
    if (headerNode->found == FALSE) {
        print_censored_name(name);
    } else {
        printf("%s\n", name);
    }

}

static void print_pokemon_to_list(Pokedex pokedex, struct pokenode *pokenode) {

    int id = pokemon_id(pokenode->pokemon);
    const char *name = pokemon_name(pokenode->pokemon);

    print_selection_indicator(pokedex, pokenode);

    printf("    #%03d: ", id);
    if (pokenode->found == FALSE) {
        print_censored_name(name);
    } else {
        printf("%s\n", name);
    }

}

static void print_selection_indicator(Pokedex pokedex, struct pokenode *pokenode) {

    if (pokedex->selectedNode == pokenode) {
        printf("-->");
    } else {
        printf("   ");
    }

}

static void reset_pokedex(Pokedex pokedex) {

    pokedex->head = NULL;
    pokedex->last = NULL;
    pokedex->selectedNode = NULL;
    pokedex->numNodes = 0;

}


//printerr is a quick helper function to make outputting errors a bit easier.
//Colours an input string red and prints it to the error output stream.
static void printerr(char *message) {
        fprintf(stderr, COL_ERR_RED"%s\n"COL_ERR_RESET, message);
        exit(1);
}
