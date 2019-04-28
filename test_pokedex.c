// Assignment 2 19T1 COMP1511: Pokedex
// test_pokedex.c
//
// This file allows you to automatically test the functions you
// implement in pokedex.c.
//
// This program was written by Avi Dargan (z5258082)
// on 28/04/19
//
// Version 1.0.1: Added pointer check for the provided tests.

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "pokedex.h"

////////////////////////////////////////////////////////////////////////////////
//                              MY DEFINITIONS                               //
//////////////////////////////////////////////////////////////////////////////

//Sample data on Placeholdermon, the Pokémon I've made up to use in tests.
#define PLACEHOLDER_ID 42
#define PLACEHOLDER_NAME "Placeholdermon"
#define PLACEHOLDER_HEIGHT 1.51
#define PLACEHOLDER_WEIGHT 809
#define PLACEHOLDER_FIRST_TYPE NORMAL_TYPE
#define PLACEHOLDER_SECOND_TYPE GHOST_TYPE

#define NUMTYPES 17

//Change this seed manually to shuffle all tests with random elements.
#define PLACEHOLDER_SEED 1511

// Sample data on Bulbasaur, the Pokemon with pokemon_id 1.
#define BULBASAUR_ID 1
#define BULBASAUR_NAME "Bulbasaur"
#define BULBASAUR_HEIGHT 0.7
#define BULBASAUR_WEIGHT 6.9
#define BULBASAUR_FIRST_TYPE GRASS_TYPE
#define BULBASAUR_SECOND_TYPE POISON_TYPE

// Sample data on Ivysaur, the Pokemon with pokemon_id 2.
#define IVYSAUR_ID 2
#define IVYSAUR_NAME "Ivysaur"
#define IVYSAUR_HEIGHT 1.0
#define IVYSAUR_WEIGHT 13.0
#define IVYSAUR_FIRST_TYPE GRASS_TYPE
#define IVYSAUR_SECOND_TYPE POISON_TYPE


////////////////////////////////////////////////////////////////////////////////
//                               MY PROTOTYPES                               //
//////////////////////////////////////////////////////////////////////////////

static void test_exploration_boundary(void);
static void test_nested_pokedex(void);
static void test_long_evo(void);
static void test_subdex_removal(void);
static void empty_pokedex_testing(void);
static void test_subdex_evolution(void);
static void single_pokemon_testing();

static Pokemon create_placeholdermon(void);
static void randomly_populate(Pokedex pokedex, int numPkmn);
static Pokemon random_pokemon(int id);
static char *random_name(void);

////////////////////////////////////////////////////////////////////////////////

// Tests for Pokedex functions from pokedex.c.
static void test_new_pokedex(void);
static void test_add_pokemon(void);
static void test_get_found_pokemon(void);
static void test_next_pokemon(void);

// Helper functions for creating/comparing Pokemon.
static Pokemon create_bulbasaur(void);
static Pokemon create_ivysaur(void);
static int is_same_pokemon(Pokemon first, Pokemon second);
static int is_copied_pokemon(Pokemon first, Pokemon second);

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {

    //Randomise all future tests with random elements.
    srand(PLACEHOLDER_SEED);

    printf("Welcome to the COMP1511 Pokedex Tests!\n");

    printf("\n==================== Pokedex Tests ====================\n");

    test_new_pokedex();
    test_add_pokemon();
    test_next_pokemon();
    test_get_found_pokemon();

    //MY TESTS:
    empty_pokedex_testing();
    single_pokemon_testing();
    test_exploration_boundary();
    test_long_evo();
    test_subdex_evolution();
    test_subdex_removal();
    test_nested_pokedex();

    printf("\nAll Pokedex tests passed, you are Awesome!\n");
}

////////////////////////////////////////////////////////////////////////
//                     Pokedex Test Functions                         //
////////////////////////////////////////////////////////////////////////

// `test_new_pokedex` checks whether the new_pokedex and destroy_pokedex
// functions work correctly, to the extent that it can.
//
// It does this by creating a new Pokedex, checking that it's not NULL,
// then calling destroy_pokedex.
//
// Note that it isn't possible to check whether destroy_pokedex has
// successfully destroyed/freed the Pokedex, so the best we can do is to
// call the function and make sure that it doesn't crash..
static void test_new_pokedex(void) {
    printf("\n>> Testing new_pokedex\n");

    printf("    ... Creating a new Pokedex\n");
    Pokedex pokedex = new_pokedex();

    printf("       --> Checking that the returned Pokedex is not NULL\n");
    assert(pokedex != NULL);

    printf("    ... Destroying the Pokedex\n");
    destroy_pokedex(pokedex);

    printf(">> Passed new_pokedex tests!\n");
}

// `test_add_pokemon` checks whether the add_pokemon function works
// correctly.
//
// It does this by creating the Pokemon Bulbasaur (using the helper
// functions in this file and the provided code in pokemon.c), and
// calling add_pokemon to add it to the Pokedex.
//
// Some of the ways that you could extend these test would include:
//   - adding additional Pokemon other than just Bulbasaur,
//   - checking whether the currently selected Pokemon is correctly set,
//   - checking that functions such as `count_total_pokemon` return the
//     correct result after more Pokemon are added,
//   - ... and more!
static void test_add_pokemon(void) {
    printf("\n>> Testing add_pokemon\n");

    printf("    ... Creating a new Pokedex\n");
    Pokedex pokedex = new_pokedex();

    printf("    ... Creating Bulbasaur\n");
    Pokemon bulbasaur = create_bulbasaur();

    printf("    ... Adding Bulbasaur to the Pokedex\n");
    add_pokemon(pokedex, bulbasaur);

    printf("    ... Destroying the Pokedex\n");
    destroy_pokedex(pokedex);

    printf(">> Passed add_pokemon tests!\n");
}

// `test_next_pokemon` checks whether the next_pokemon function works
// correctly.
//
// It does this by creating two Pokemon: Bulbasaur and Ivysaur (using
// the helper functions in this file and the provided code in pokemon.c).
//
// It then adds these to the Pokedex, then checks that calling the
// next_pokemon function changes the currently selected Pokemon from
// Bulbasaur to Ivysaur.
//
// Some of the ways that you could extend these tests would include:
//   - adding even more Pokemon to the Pokedex,
//   - calling the next_pokemon function when there is no "next" Pokemon,
//   - calling the next_pokemon function when there are no Pokemon in
//     the Pokedex,
//   - ... and more!
static void test_next_pokemon(void) {
    printf("\n>> Testing next_pokemon\n");

    printf("    ... Creating a new Pokedex\n");
    Pokedex pokedex = new_pokedex();

    printf("    ... Creating Bulbasaur and Ivysaur\n");
    Pokemon bulbasaur = create_bulbasaur();
    Pokemon ivysaur = create_ivysaur();

    printf("    ... Adding Bulbasaur and Ivysaur to the Pokedex\n");
    add_pokemon(pokedex, bulbasaur);
    add_pokemon(pokedex, ivysaur);

    printf("       --> Checking that the current Pokemon is Bulbasaur\n");
    assert(is_same_pokemon(get_current_pokemon(pokedex), bulbasaur));

    printf("    ... Moving to the next pokemon\n");
    next_pokemon(pokedex);

    printf("       --> Checking that the current Pokemon is Ivysaur\n");
    assert(is_same_pokemon(get_current_pokemon(pokedex), ivysaur));

    printf("    ... Destroying the Pokedex\n");
    destroy_pokedex(pokedex);

    printf(">> Passed next_pokemon tests!\n");
}

// `test_get_found_pokemon` checks whether the get_found_pokemon
// function works correctly.
//
// It does this by creating two Pokemon: Bulbasaur and Ivysaur (using
// the helper functions in this file and the provided code in pokemon.c).
//
// It then adds these to the Pokedex, sets Bulbasaur to be found, and
// then calls the get_found_pokemon function to get all of the Pokemon
// which have been found (which should be just the one, Bulbasaur).
//
// Some of the ways that you could extend these tests would include:
//   - calling the get_found_pokemon function on an empty Pokedex,
//   - calling the get_found_pokemon function on a Pokedex where none of
//     the Pokemon have been found,
//   - checking that the Pokemon in the new Pokedex are in ascending
//     order of pokemon_id (regardless of the order that they appeared
//     in the original Pokedex),
//   - checking that the currently selected Pokemon in the returned
//     Pokedex has been set correctly,
//   - checking that the original Pokedex has not been modified,
//   - ... and more!
static void test_get_found_pokemon(void) {
    printf("\n>> Testing get_found_pokemon\n");

    printf("    ... Creating a new Pokedex\n");
    Pokedex pokedex = new_pokedex();

    printf("    ... Creating Bulbasaur and Ivysaur\n");
    Pokemon bulbasaur = create_bulbasaur();
    Pokemon ivysaur = create_ivysaur();

    printf("    ... Adding Bulbasaur and Ivysaur to the Pokedex\n");
    add_pokemon(pokedex, bulbasaur);
    add_pokemon(pokedex, ivysaur);

    printf("       --> Checking that the current Pokemon is Bulbasaur\n");
    assert(get_current_pokemon(pokedex) == bulbasaur);

    printf("    ... Setting Bulbasaur to be found\n");
    find_current_pokemon(pokedex);

    printf("    ... Getting all found Pokemon\n");
    Pokedex found_pokedex = get_found_pokemon(pokedex);

    printf("       --> Checking the correct Pokemon were copied and returned\n");
    assert(count_total_pokemon(found_pokedex) == 1);
    assert(count_found_pokemon(found_pokedex) == 1);
    assert(is_copied_pokemon(get_current_pokemon(found_pokedex), bulbasaur));

    printf("    ... Destroying both Pokedexes\n");
    destroy_pokedex(pokedex);
    destroy_pokedex(found_pokedex);

    printf(">> Passed get_found_pokemon tests!\n");
}


////////////////////////////////////////////////////////////////////////////////
//                             MY TEST FUNCTIONS                             //
//////////////////////////////////////////////////////////////////////////////

//empty_pokedex_testing ensures that all the functions that do not exit when
//used on an empty Pokédex function as behaved, i.e. do nothing, or return
//null values.
static void empty_pokedex_testing(void) {

    printf("\n>> Testing functions on an empty Pokédex.\n");

    printf("    ... Creating the empty Pokédex.\n");
    Pokedex pokedex = new_pokedex();

    printf("    ... Detailing a non-existent Pokémon.\n");
    detail_pokemon(pokedex);

    printf("    ... Printing a non-existent list of Pokémon.\n");
    print_pokemon(pokedex);

    printf("    ... Navigating forwards to a non-existent Pokémon.\n");
    next_pokemon(pokedex);

    printf("    ... Navigating backwards to a non-existent Pokémon.\n");
    prev_pokemon(pokedex);

    printf("    ... Navigating to the non-existent Pokémon with ID: 100.\n");
    change_current_pokemon(pokedex, 100);

    printf("    ... Remove current, non-existent Pokémon.\n");
    remove_pokemon(pokedex);

    printf("    ... Ensuring the number of found Pokémon is 0.\n");
    assert(count_found_pokemon(pokedex) == 0);

    printf("    ... Ensuring the total number of Pokémon is 0.\n");
    assert(count_total_pokemon(pokedex) == 0);

    printf("    ... Showing the evolutions of a non-existent Pokémon.\n");
    show_evolutions(pokedex);

    printf("    ... Ensuring the next evolution of a non-existent Pokémon is DOES_NOT_EVOLVE.\n");
    assert(get_next_evolution(pokedex) == DOES_NOT_EVOLVE);

    printf("    ... Creating a Pokédex of Ghost type Pokémon.\n");
    Pokedex ghostDex = get_pokemon_of_type(pokedex, GHOST_TYPE);

    printf("    ... Destroying the Pokédex of Ghost type Pokémon.\n");
    destroy_pokedex(ghostDex);

    printf("    ... Creating a Pokédex of found Pokémon.\n");
    Pokedex foundDex = get_found_pokemon(pokedex);

    printf("    ... Destroying the Pokédex of found Pokémon.\n");
    destroy_pokedex(foundDex);

    printf("    ... Creating a Pokédex of Pokémon with the letter 'A'.\n");
    Pokedex searchDex = search_pokemon(pokedex, "A");

    printf("    ... Destroying the Pokédex of found Pokémon.\n");
    destroy_pokedex(searchDex);

    printf("    ... Destroying the empty Pokédex.\n");
    destroy_pokedex(pokedex);

    printf(">> Passed empty Pokédex tests!\n");
}

//single_pokemon_testing ensures that all the functions perform correctly
//when used on a Pokédex with one Pokémon.
static void single_pokemon_testing(void) {

    printf("\n>> Testing functions on a Pokédex with one Pokémon.\n");

    printf("    ... Creating the empty Pokédex.\n");
    Pokedex pokedex = new_pokedex();

    printf("    ... Creating a Placeholdermon.\n");
    Pokemon placeholdermon = create_placeholdermon();

    printf("    ... Adding Placeholdermon to the Pokédex.\n");
    add_pokemon(pokedex, placeholdermon);

    printf("    ... Detailing Placeholdermon (CENSORED).\n\n");
    detail_pokemon(pokedex);

    printf("\n    ... Getting current Pokémon, i.e., Placeholdermon.\n");
    get_current_pokemon(pokedex);

    printf("    ... Finding current Pokémon.\n");
    find_current_pokemon(pokedex);
    assert(count_found_pokemon(pokedex) == 1);

    printf("    ... Printing a list with only Placeholdermon.\n\n");
    print_pokemon(pokedex);

    printf("\n    ... Navigating forwards to a non-existent Pokémon.\n");
    next_pokemon(pokedex);
    assert(get_current_pokemon(pokedex) == placeholdermon);

    printf("    ... Navigating backwards to a non-existent Pokémon.\n");
    prev_pokemon(pokedex);
    assert(get_current_pokemon(pokedex) == placeholdermon);

    printf("    ... Navigating to the non-existent Pokémon with ID: 100.\n");
    change_current_pokemon(pokedex, 100);
    assert(get_current_pokemon(pokedex) == placeholdermon);

    printf("    ... Remove Placeholdermon.\n");
    remove_pokemon(pokedex);
    assert(count_total_pokemon(pokedex) == 0);

    printf("    ... Adding Placeholdermon back, and finding it again.\n");
    Pokemon newPlaceholdermon = create_placeholdermon();
    add_pokemon(pokedex, newPlaceholdermon);
    find_current_pokemon(pokedex);

    printf("    ... Ensuring the number of found Pokémon is 1.\n");
    assert(count_found_pokemon(pokedex) == 1);

    printf("    ... Ensuring the total number of Pokémon is 1.\n");
    assert(count_total_pokemon(pokedex) == 1);

    printf("    ... Adding a random Pokémon and finding it.\n");
    Pokemon randomEvolution = random_pokemon(PLACEHOLDER_ID+1);
    add_pokemon(pokedex, randomEvolution);
    next_pokemon(pokedex);
    find_current_pokemon(pokedex);
    prev_pokemon(pokedex);
    assert(count_total_pokemon(pokedex) == 2);

    printf("    ... Setting the random Pokémon as the evolution.\n");
    add_pokemon_evolution(pokedex, PLACEHOLDER_ID, PLACEHOLDER_ID+1);
    assert(get_next_evolution(pokedex) != DOES_NOT_EVOLVE);

    printf("    ... Showing the evolutions of Placeholdermon.\n\n");
    show_evolutions(pokedex);

    printf("\n    ... Ensuring the next evolution of a non-existent Pokémon is not DOES_NOT_EVOLVE.\n");
    assert(get_next_evolution(pokedex) != DOES_NOT_EVOLVE);

    printf("    ... Destroying the Pokedex.\n");
    destroy_pokedex(pokedex);

    printf(">> Passed single Pokémon tests!\n");
}

//test_exploration_boundary specifically checks if in an exploration with the
//lowest Pokémon ID, x, if a factor of x, which should ideally represent the
//ID's from 1 to x, returns an error.
static void test_exploration_boundary(void) {

    printf("\n>> Testing exploration boundaries.\n");

    printf("    ... Creating a new Pokédex.\n");
    Pokedex pokedex = new_pokedex();

    printf("    ... Creating a Placeholdermon.\n");
    Pokemon placeholdermon = create_placeholdermon();

    printf("    ... Adding Placeholdermon to the Pokédex.\n");
    add_pokemon(pokedex, placeholdermon);

    printf("    ... Exploring from ID: 1-42 for Placeholdermon.\n");
    go_exploring(pokedex, PLACEHOLDER_SEED, 42, 1);

    printf("    ... Ensuring that Placeholdermon was found.\n");
    assert(count_found_pokemon(pokedex) == 1);

    printf("    ... Destroying the Pokedex.\n");
    destroy_pokedex(pokedex);

    printf(">> Passed exploration boundary tests!\n");
}

//test_long_evo simply just tests an evolution chain of 100 random Pokémon.
//This is just simple volume testing.
static void test_long_evo(void) {

    printf("\n>> Testing long evolution chains.\n");

    printf("    ... Creating a new original Pokédex.\n");
    Pokedex pokedex = new_pokedex();

    printf("    ... Populating it with 100 random Pokémon.\n");
    randomly_populate(pokedex, 100);

    printf("    ... Finding every Pokémon.\n");
    for (int i = 0; i < 100; i++) {
        find_current_pokemon(pokedex);
        next_pokemon(pokedex);
    }

    printf("    ... Chaining together evolutions.\n");
    for (int i = 1; i < 100; i++) {
        add_pokemon_evolution(pokedex, i, i+1);
    }

    printf("    ... Here's the result!\n\n");
    change_current_pokemon(pokedex, 1);
    show_evolutions(pokedex);

    printf("\n   ... Destroying the Pokedex.\n");
    destroy_pokedex(pokedex);

    printf("\n>> Passed long evolution chain testing!\n");
}

//Ensures that there are NO evolutions in a SubDex, by creating a Pokédex with
//a 100-length chain of evolutions, and then creating a SubDex from that.
static void test_subdex_evolution(void) {

    printf("\n>> Testing SubDex evolution behavious.\n");

    printf("    ... Creating a new original Pokédex.\n");
    Pokedex pokedex = new_pokedex();

    printf("    ... Populating it with 100 random Pokémon.\n");
    randomly_populate(pokedex, 100);

    printf("    ... Finding every Pokémon.\n");
    for (int i = 0; i < 100; i++) {
        find_current_pokemon(pokedex);
        next_pokemon(pokedex);
    }

    printf("    ... Chaining together evolutions.\n");
    for (int i = 1; i < 100; i++) {
        add_pokemon_evolution(pokedex, i, i+1);
    }

    printf("    ... Creating a Pokédex of found Pokémon.\n");
    Pokedex foundDex = get_found_pokemon(pokedex);

    printf("    ... Checking that all 100 Pokémon do not have evolutions.\n");
    for (int i = 0; i < 100; i++) {
        assert(get_next_evolution(foundDex) == DOES_NOT_EVOLVE);
        next_pokemon(foundDex);
    }

    printf("    ... Destroying the Pokédex of found Pokémon.\n");
    destroy_pokedex(foundDex);

    printf("    ... Destroying the original Pokedex.\n");
    destroy_pokedex(pokedex);

    printf("\n>> Passed SubDex evolution testing!\n");

}

//test_subdex_removal observes the behaviour of removal of a Pokémon inside a
//SubDex. After creating a SubDex of 50 found Pokémon from a Pokédex of 100
//random Pokémon, we can remove 25 Pokémon from this SubDex, and check that:
//a) The SubDex has 25 Pokémon.
//b) The original Pokédex still has the original 100 Pokémon.
static void test_subdex_removal(void) {

    printf("\n>> Testing SubDex removal.\n");

    printf("    ... Creating a new original Pokédex.\n");
    Pokedex pokedex = new_pokedex();

    printf("    ... Populating it with 100 random Pokémon.\n");
    randomly_populate(pokedex, 100);

    printf("    ... Finding every other Pokémon.\n");
    for (int i = 0; i < 100; i++) {
        if (i%2 != 0) {
            find_current_pokemon(pokedex);
        }
        next_pokemon(pokedex);
    }

    printf("    ... Creating a new Pokédex from found Pokémon.\n");
    Pokedex foundDex = get_found_pokemon(pokedex);

    printf("    ... Removing every other Pokémon in this SubDex.\n");
    for (int i = 0; i < 50; i++) {
        if (i%2 != 0) {
            remove_pokemon(foundDex);
        }
        next_pokemon(foundDex);
    }

    printf("    ... Ensuring the No. of Pokémon in the SubDex is 25.\n");
    assert(count_total_pokemon(foundDex) == 25);

    printf("    ... and that the No. of Pokémon in original Pokédex is 100.\n");
    assert(count_total_pokemon(pokedex) == 100);

    printf("    ... Destroying the SubDex.\n");
    destroy_pokedex(foundDex);

    printf("    ... Destroying the Pokedex.\n");
    destroy_pokedex(pokedex);

    printf(">> Passed SubDex removal testing!\n");

}

//test_nested_pokedex ensures that nested SubDexes can be created, and explored.
//For example, the below creates a Pokédex filled with 100 randomly named
//Pokémon with varied secondary types. It then finds every second one, and
//creates a Pokédex with all the found Pokémon. It then looks to find every
//single one that is a Ghost type, and then finds the Pokémon with the letter
//'A' in their name. It then exits out of each Pokedex.
static void test_nested_pokedex(void) {

    printf("\n>> Testing nested Pokédexes.\n");

    printf("    ... Creating a new original Pokédex.\n");
    Pokedex pokedex = new_pokedex();

    printf("    ... Populating it with 100 random Pokémon.\n");
    randomly_populate(pokedex, 100);

    printf("    ... Finding every other Pokémon.\n");
    for (int i = 0; i < 100; i++) {
        if (i%2 != 0) {
            find_current_pokemon(pokedex);
        }
        next_pokemon(pokedex);
    }

    printf("    ... Creating a new Pokédex from found Pokémon.\n");
    Pokedex foundDex = get_found_pokemon(pokedex);

    printf("    ... Creating a new Pokédex of Ghost type Pokémon.\n");
    Pokedex ghostDex = get_pokemon_of_type(foundDex, GHOST_TYPE);

    printf("    ... Creating a new Pokédex of Pokémon with the letter 'A'.\n");
    Pokedex searchDex = search_pokemon(ghostDex, "a");

    printf("    ... Here's the results! (usually empty)\n\n");
    print_pokemon(searchDex);

    printf("\n    ... Destroying the searched Pokédex.\n");
    destroy_pokedex(searchDex);

    printf("    ... Destroying the Ghost types Pokédex.\n");
    destroy_pokedex(ghostDex);

    printf("    ... Destroying the found Pokédex.\n");
    destroy_pokedex(foundDex);

    printf("    ... Destroying the original Pokédex.\n");
    destroy_pokedex(pokedex);

    printf(">> Passed nested Pokedex testing!\n");
}

////////////////////////////////////////////////////////////////////////
//                     Helper Functions                               //
////////////////////////////////////////////////////////////////////////

// Helper function to create Bulbasaur for testing purposes.
static Pokemon create_bulbasaur(void) {
    Pokemon pokemon = new_pokemon(
            BULBASAUR_ID, BULBASAUR_NAME,
            BULBASAUR_HEIGHT, BULBASAUR_WEIGHT,
            BULBASAUR_FIRST_TYPE,
            BULBASAUR_SECOND_TYPE
    );
    return pokemon;
}

// Helper function to create Ivysaur for testing purposes.
static Pokemon create_ivysaur(void) {
    Pokemon pokemon = new_pokemon(
            IVYSAUR_ID, IVYSAUR_NAME,
            IVYSAUR_HEIGHT, IVYSAUR_WEIGHT,
            IVYSAUR_FIRST_TYPE,
            IVYSAUR_SECOND_TYPE
    );
    return pokemon;
}

// Helper function to compare whether two Pokemon are the same.
// This checks that the two pointers contain the same address, i.e.
// they are both pointing to the same pokemon struct in memory.
//
// Pokemon ivysaur = new_pokemon(0, 'ivysaur', 1.0, 13.0, GRASS_TYPE, POISON_TYPE)
// Pokemon also_ivysaur = ivysaur
// is_same_pokemon(ivysaur, also_ivysaur) == TRUE
static int is_same_pokemon(Pokemon first, Pokemon second) {
    return first == second;
}

// Helper function to compare whether one Pokemon is a *copy* of
// another, based on whether their attributes match (e.g. pokemon_id,
// height, weight, etc).
//
// It also checks that the pointers do *not* match -- i.e. that the
// pointers aren't both pointing to the same pokemon struct in memory.
// If the pointers both contain the same address, then the second
// Pokemon is not a *copy* of the first Pokemon.
//
// This function doesn't (yet) check that the Pokemon's names match
// (but perhaps you could add that check yourself...).
static int is_copied_pokemon(Pokemon first, Pokemon second) {
    return (pokemon_id(first) == pokemon_id(second))
    &&  (first != second)
    &&  (pokemon_height(first) == pokemon_height(second))
    &&  (pokemon_weight(first) == pokemon_weight(second))
    &&  (pokemon_first_type(first) == pokemon_first_type(second))
    &&  (pokemon_second_type(first) == pokemon_second_type(second));
}

// Write your own helper functions here!


//create_placeholdermon is a helper function to create Placeholdermon for
//testing purposes.
static Pokemon create_placeholdermon(void) {
    Pokemon pokemon = new_pokemon(
            PLACEHOLDER_ID, PLACEHOLDER_NAME,
            PLACEHOLDER_HEIGHT, PLACEHOLDER_WEIGHT,
            PLACEHOLDER_FIRST_TYPE,
            PLACEHOLDER_SECOND_TYPE
    );
    return pokemon;
}

//randomly_populate adds numPkmn randomly generated Pokémon to the given
//Pokédex.
static void randomly_populate(Pokedex pokedex, int numPkmn) {

    int i = 1;

    while (i <= numPkmn) {
        Pokemon pokemon = random_pokemon(i);
        add_pokemon(pokedex, pokemon);
        i++;
    }

}

//random_pokemon returns a randomly generated Pokémon, with a given id, random
//name, and a primary Normal typing, and a random secondary type.
static Pokemon random_pokemon(int id) {

    //Create a random name with varying case.
    char name[6];
    int i = 0;
    while (i < 5) {
        int lwrCase = rand()%2;
        if (lwrCase == 0) {
            name[i] = 'A'+(rand()%25);
        } else {
            name[i] = 'a'+(rand()%24);
        }
        i++;
    }
    name[i] = '\0';

    pokemon_type validTypes[NUMTYPES] = {
        FIRE_TYPE, FIGHTING_TYPE, WATER_TYPE, FLYING_TYPE, GRASS_TYPE,
        POISON_TYPE, ELECTRIC_TYPE, GROUND_TYPE, PSYCHIC_TYPE, ROCK_TYPE,
        ICE_TYPE, BUG_TYPE, DRAGON_TYPE, GHOST_TYPE, DARK_TYPE,STEEL_TYPE,
        FAIRY_TYPE
    };

    Pokemon pokemon = new_pokemon(
        id, name, PLACEHOLDER_HEIGHT, PLACEHOLDER_WEIGHT,
        NORMAL_TYPE, validTypes[rand()%NUMTYPES]
    );

    return pokemon;

}
