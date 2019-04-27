// Assignment 2 19T1 COMP1511: Pokedex
// pokedex.c
//
// This program was written by Avi Dargan (z5258082)
// on 23/04/19-02/05-19
// Version 1.1.1: Renamed "pokemon_id" to "id" in change_current_pokemon.

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "pokedex.h"

////////////////////////////////////////////////////////////////////////////////
//                             AVI's DEFINITIONS                             //
//////////////////////////////////////////////////////////////////////////////

#define TRUE 1
#define FALSE 0

#define NULL_ID -1

struct pokedex {
    struct pokenode *head;
    struct pokenode *last;
    struct pokenode *selectedNode;
    int numNodes;
    int lowestID;
};

struct pokenode {
    struct pokenode *next;
    struct pokenode *previous;
    Pokemon         pokemon;
    int found;
    struct pokenode *evoNode;
};


////////////////////////////////////////////////////////////////////////////////
//                             AVI's PROTOTYPES                              //
//////////////////////////////////////////////////////////////////////////////

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

static struct pokenode *search_for_id(Pokedex pokedex, int id);

static void print_evo_format(struct pokenode *pokenode);

static void clean_sub_dex(Pokedex pokedex);

static void populate_subDex_types(Pokedex pokedex, Pokedex subDex, pokemon_type type);
static int common_type(struct pokenode *node, pokemon_type type);

static void populate_subDex_found(Pokedex pokedex, Pokedex subDex);
static void add_pokemon_in_order(Pokedex pokedex, struct pokenode *node);

static void populate_subDex_search(Pokedex pokedex, Pokedex subDex, char *query);
static int contains_search(char *text, char *query);
static int length_of_string(char *string);

static void printerr(char *message);

////////////////////////////////////////////////////////////////////////////////

Pokedex new_pokedex(void) {
    Pokedex new_pokedex = malloc(sizeof (struct pokedex));
    assert(new_pokedex != NULL);

    new_pokedex->head = NULL;
    new_pokedex->selectedNode = NULL;
    new_pokedex->selectedNode = NULL;
    new_pokedex->numNodes = 0;
    new_pokedex->lowestID = NULL_ID;

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

            destroy_pokemon(pokedex->head->pokemon);
            free(pokedex->head);
            reset_pokedex(pokedex);

        } else if (pokedex->selectedNode == pokedex->last) {

            pokedex->selectedNode = pokedex->last->previous;
            destroy_pokemon(pokedex->last->pokemon);
            free(pokedex->last);
            pokedex->selectedNode->next = NULL;
            pokedex->last = pokedex->selectedNode;
            pokedex->numNodes -= 1;

        } else if (pokedex->selectedNode == pokedex->head && pokedex->numNodes > 1){

            pokedex->selectedNode = pokedex->head->next;
            destroy_pokemon(pokedex->head->pokemon);
            free(pokedex->head);
            pokedex->selectedNode->previous = NULL;
            pokedex->head = pokedex->selectedNode;
            pokedex->numNodes -= 1;

        } else {

            struct pokenode *memToFree = pokedex->selectedNode;
            pokedex->selectedNode = pokedex->selectedNode->next;
            pokedex->selectedNode->previous = memToFree->previous;
            pokedex->selectedNode->previous->next = pokedex->selectedNode;
            pokedex->numNodes -= 1;

            destroy_pokemon(memToFree->pokemon);
            free(memToFree);
        }
    }

}

//Does not check for empty Pokédex as that is done by remove_pokemon().
void destroy_pokedex(Pokedex pokedex) {

    int i = 0;
    int maxIterations = pokedex->numNodes;

    while (i < maxIterations) {
        remove_pokemon(pokedex);
        i++;
    }

    free(pokedex);

}

////////////////////////////////////////////////////////////////////////
//                         Stage 3 Functions                          //
////////////////////////////////////////////////////////////////////////

void go_exploring(Pokedex pokedex, int seed, int factor, int how_many) {

    if (factor < pokedex->lowestID || pokedex->head == NULL) {
        printerr("ERROR: There are no Pokémon to be found in the given range!");
    } else {
        srand(seed);
        int i = 0;
        while (i < how_many) {

            int idQuery = rand()%factor + 1;
            struct pokenode *result = search_for_id(pokedex, idQuery);

            if (result != NULL) {
                result->found = TRUE;
                i++;
            }
        }
    }
}

int count_found_pokemon(Pokedex pokedex) {

    int foundNum = 0;
    struct pokenode *p = pokedex->head;

    while (p != NULL) {
        if (p->found == TRUE) {
            foundNum++;
        }
        p = p->next;
    }

    return foundNum;
}

int count_total_pokemon(Pokedex pokedex) {
    return pokedex->numNodes;
}

////////////////////////////////////////////////////////////////////////
//                         Stage 4 Functions                          //
////////////////////////////////////////////////////////////////////////

void add_pokemon_evolution(Pokedex pokedex, int from_id, int to_id) {

    if (from_id == to_id) {
        printerr("ERROR: Given IDs are identical!");
    }

    if(pokedex->head != NULL) {
        struct pokenode *fromNode = search_for_id(pokedex, from_id);
        struct pokenode *toNode = search_for_id(pokedex, to_id);

        if (fromNode == NULL || toNode == NULL) {
            printerr("No Pokémon with given ID found in Pokédex!");
        } else {
            fromNode->evoNode = toNode;
        }

    } else {
        printerr("ERROR: The Pokédex is empty!");
    }

}

void show_evolutions(Pokedex pokedex) {

    if (pokedex->head != NULL) {
        struct pokenode *p = pokedex->selectedNode;

        while(p->evoNode != NULL) {
            print_evo_format(p);
            printf(" --> ");
            p = p->evoNode;
        }
        print_evo_format(p);
        printf("\n");
    }
}

int get_next_evolution(Pokedex pokedex) {

    if (pokedex->head != NULL) {
        struct pokenode *node = pokedex->selectedNode;
        if (node->evoNode != NULL) {
            return pokemon_id(node->evoNode->pokemon);
        }

        return DOES_NOT_EVOLVE;
    }

    return DOES_NOT_EVOLVE;
}

////////////////////////////////////////////////////////////////////////
//                         Stage 5 Functions                          //
///////////////////////////////////////////////////////////////////////

Pokedex get_pokemon_of_type(Pokedex pokedex, pokemon_type type) {

    if (type == NONE_TYPE || type == INVALID_TYPE || type == MAX_TYPE) {
        printerr("ERROR: Please enter a valid type!");
    }

    Pokedex subDex = new_pokedex();
    populate_subDex_types(pokedex, subDex, type);
    clean_sub_dex(subDex);

    return subDex;

}

Pokedex get_found_pokemon(Pokedex pokedex) {

    Pokedex subDex = new_pokedex();
    populate_subDex_found(pokedex, subDex);
    clean_sub_dex(subDex);

    return subDex;

}

Pokedex search_pokemon(Pokedex pokedex, char *text) {

    Pokedex subDex = new_pokedex();
    populate_subDex_search(pokedex, subDex, text);
    clean_sub_dex(subDex);

    return subDex;

}

////////////////////////////////////////////////////////////////////////////////
//                              AVI's FUNCTIONS                              //
//////////////////////////////////////////////////////////////////////////////

//create_pokenode utilised malloc to assign a chunk of memory to a pokenode,
//then populate it with the Pokémon it recieves, and returns a pointer to a
//pokenode. It also checks to see if there is enough memory to assign.
static struct pokenode *create_pokenode(Pokemon pokemon) {

    struct pokenode *node = malloc(sizeof(struct pokenode));

    //malloc returns a null pointer if there is no available memory
    if (node == NULL) {
        printerr("ERROR: Out of memory!");
    }

    node->next = NULL;
    node->pokemon = pokemon;
    node->found = FALSE;
    node->evoNode = NULL;

    return node;
}

//add_node_to_dex has two branches of code. First, if this node is to be the
//initial node, it initialises the head pointer of the Pokedex struct, as well
//as the selected node, which does not change when adding a Pokémon.
//Otherwise, it simply points the last node's next pointer to the newly created
//node. Finally, it increments the number of nodes in the list, and sets the
//last node in the list to the new node, and updates the lowest Pokémon ID in
//the Pokédex (for use in exploration).
static void add_node_to_dex(Pokedex pokedex, struct pokenode *node) {
    if (pokedex->numNodes == 0) {
        pokedex->head = node;
        node->previous = NULL;
        pokedex->selectedNode = pokedex->head;
        pokedex->lowestID = pokemon_id(node->pokemon);
    } else {
        node->previous = pokedex->last;
        pokedex->last->next = node;
        if (pokedex->lowestID > pokemon_id(node->pokemon)) {
            pokedex->lowestID = pokemon_id(node->pokemon);
        }
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
    printf("Height: %.1lfm\n", height);
    printf("Weight: %.1lfkg\n", weight);

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

////////////////////////////////////////////////////////////////////////////////
// MAYBE GET RID OF THIS FUNCTION                                             //
////////////////////////////////////////////////////////////////////////////////

//print_first_pokemon begins the list when the 'p' command is called by printing
//the first, or header, Pokémon.
static void print_first_pokemon(Pokedex pokedex) {

    struct pokenode *headerNode = pokedex->head;
    int id = pokemon_id(headerNode->pokemon);
    const char *name = pokemon_name(headerNode->pokemon);

    print_selection_indicator(pokedex, headerNode);

    printf("#%03d: ", id);
    if (headerNode->found == FALSE) {
        print_censored_name(name);
    } else {
        printf("%s\n", name);
    }

}

//print_pokemon_to_list formats the Pokémon's ID, and prints the Pokémon's name,
//either censored or uncensored depending on it's found status.
static void print_pokemon_to_list(Pokedex pokedex, struct pokenode *pokenode) {

    int id = pokemon_id(pokenode->pokemon);
    const char *name = pokemon_name(pokenode->pokemon);

    print_selection_indicator(pokedex, pokenode);

    printf("#%03d: ", id);
    if (pokenode->found == FALSE) {
        print_censored_name(name);
    } else {
        printf("%s\n", name);
    }

}

//print_selection_indicator prints either an arrow (-->) or blank space
//depending on whether the given node is currently selected or not.
static void print_selection_indicator(Pokedex pokedex, struct pokenode *pokenode) {

    if (pokedex->selectedNode == pokenode) {
        printf("--> ");
    } else {
        printf("    ");
    }

}

//reset_pokedex simply reverts the Pokédex to it's initial values.
static void reset_pokedex(Pokedex pokedex) {

    pokedex->head = NULL;
    pokedex->last = NULL;
    pokedex->selectedNode = NULL;
    pokedex->numNodes = 0;

}

//search_for_id searches through the entire Pokédex for the given ID, and if
//found, returns a pointer to the node containing that Pokémon. Otherwise, it
//simply returns a null pointer.
static struct pokenode *search_for_id(Pokedex pokedex, int id) {

    struct pokenode *p = pokedex->head;
    while (p != NULL) {

        if (pokemon_id(p->pokemon) == id) {
            return p;
        }

        p = p->next;
    }

    return NULL;
}

//print_evo_format prints out a given pokenode's Pokémon's details. For example,
//prints Porygon as "#137 Porygon [Normal]" if found, and "#137 ???? [????]", if
//not.
static void print_evo_format(struct pokenode *pokenode) {

    int id = pokemon_id(pokenode->pokemon);
    char *name = pokemon_name(pokenode->pokemon);
    pokemon_type firstType = pokemon_first_type(pokenode->pokemon);
    pokemon_type secondType = pokemon_second_type(pokenode->pokemon);

    printf("#%03d ", id);
    if (pokenode->found) {
        printf("%s [%s", name, pokemon_type_to_string(firstType));
        if (secondType != NONE_TYPE) {
            printf(" %s", pokemon_type_to_string(secondType));
        }
        printf("]");
    } else {
        printf("???? [????]");
    }

}

//clean_sub_dex cleans new SubDex's of their evolution data and marks every
//Pokémon as found.
static void clean_sub_dex(Pokedex pokedex) {

    struct pokenode *p = pokedex->head;
    while (p != NULL) {
        p->evoNode = NULL;
        p->found = TRUE;
        p = p->next;
    }

}

//populate_subDex_types finds every Pokémon with the given type in the original
//Pokédex, and copies them to the subDex.
static void populate_subDex_types(Pokedex pokedex, Pokedex subDex, pokemon_type type) {

    struct pokenode *p = pokedex->head;
    while (p != NULL) {

        if (common_type(p, type) && p->found) {
            add_pokemon(subDex, clone_pokemon(p->pokemon));
        }

        p = p->next;
    }

}

//Checks if the passed Pokémon (inside the pokenode) is of the given type, be it
//primary or secondary.
static int common_type(struct pokenode *node, pokemon_type type) {

    if (pokemon_first_type(node->pokemon) == type) {
        return TRUE;
    } else if (pokemon_second_type(node->pokemon) == type) {
        return TRUE;
    } else {
        return FALSE;
    }

}

//Finds every "found" Pokémon in the original Pokédex, and copies
//them to the subDex.
static void populate_subDex_found(Pokedex pokedex, Pokedex subDex) {

    struct pokenode *p = pokedex->head;
    while (p != NULL) {

        if (p->found) {
            struct pokenode *node = create_pokenode(clone_pokemon(p->pokemon));
            add_pokemon_in_order(subDex, node);
        }

        p = p->next;
    }

}

//add_pokemon_in_order is a special version of add_node_to_dex().
//The logic is the same, simply with amendments to account for when a node is
//added to the middle of the list.
static void add_pokemon_in_order(Pokedex pokedex, struct pokenode *node) {

    if (pokedex->numNodes == 0) {
        pokedex->head = node;
        node->previous = NULL;
        pokedex->selectedNode = pokedex->head;
        pokedex->lowestID = pokemon_id(node->pokemon);
    } else {
        if (pokemon_id(node->pokemon) < pokemon_id(pokedex->head->pokemon)) {
            node->next = pokedex->head;
            pokedex->head = node;
            node->next->previous = node;
            pokedex->selectedNode = node;
            pokedex->lowestID = pokemon_id(node->pokemon);
        } else {
            struct pokenode *p = pokedex->head;
            while(p->next != NULL && (pokemon_id(node->pokemon) > pokemon_id(p->next->pokemon))) {
                p = p->next;
            }
            node->next = p->next;
            p->next = node;
            node->previous = p;
            if (node->next == NULL) {
                pokedex->last = node;
            } else {
                node->next->previous = node;
            }
        }
    }

    pokedex->numNodes++;
}

//Look through the Pokédex for Pokémon with names that contain the search query
//then add them to the subDex.
static void populate_subDex_search(Pokedex pokedex, Pokedex subDex, char *query) {

    struct pokenode *p = pokedex->head;
    while (p != NULL) {

        if (contains_search(pokemon_name(p->pokemon), query) && p->found) {
            add_pokemon(subDex, clone_pokemon(p->pokemon));
        }

        p = p->next;
    }

}

static int contains_search(char *text, char *query) {

    int textLength = length_of_string(text) + 1;
    int queryLength = length_of_string(query) + 1;
    char lwrText[textLength];
    char lwrQuery[queryLength];

    if (queryLength > textLength) {
        return FALSE;
    }

    for (int i = 0; i < textLength; i++) {
        if (text[i] >= 'A' && text[i] <= 'Z') {
            lwrText[i] = text[i] + 32;
        } else {
            lwrText[i] = text[i];
        }
    }

    for (int i = 0; i < queryLength; i++) {
        if (query[i] >= 'A' && query[i] <= 'Z') {
            lwrQuery[i] = query[i] + 32;
        } else {
            lwrQuery[i] = query[i];
        }
    }

    int i = 0;
    int j = 0;

    //Following 10 (exactly) lines of general purpose C code are based on the
    //following StackOverflow answer by user MikeCAT (25/02/16).
    //https://stackoverflow.com/a/35631538
    while ((lwrText[j] != '\0') && (lwrQuery[i] != '\0')) {
        if (lwrQuery[i] == lwrText[j]) {
            i++;
        }
        j++;
    }

    if (lwrQuery[i] == '\0') {
        return TRUE;
    }

    return FALSE;
}

//length_of_string returns the length of the given string.
static int length_of_string(char *string) {

    int i = 0;
    while (string[i] != '\0') {
        i++;
    }

    return i;
}

//printerr is a quick helper function to make outputting errors a bit easier.
//Prints the given string to the error output stream.
static void printerr(char *message) {
        fprintf(stderr, "%s\n", message);
        exit(1);
}
