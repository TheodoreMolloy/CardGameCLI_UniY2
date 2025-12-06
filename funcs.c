#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "funcs.h"

const int DECK_SIZE = 52;

// card_reveal
void menu_item_1(void) {
    printf("\n>> Random Cards Reveal\n");
    card_reveal();
}

void menu_item_2(void) {
    printf("\n>> Menu 2\n");
    printf("\nSome code here does something useful\n");
    /* you can call a function from here that handles menu 2 */
}

void menu_item_3(void) {
    printf("\n>> Menu 3\n");
    printf("\nSome code here does something useful\n");
    /* you can call a function from here that handles menu 3 */
}

void menu_item_4(void) {
    printf("\n>> Menu 4\n");
    printf("\nSome code here does something useful\n");
    /* you can call a function from here that handles menu 4 */
}

// deck initialisations

void init_deck(struct card given_deck[], const int size) {
    /* assigns an array of cards their suit, number, and properties in order */
    size_t i = 0;
    for (size_t s = 0; s < 4; s++) {
        for (size_t n = 1; n < 14; n++) {
            given_deck[i].number = n;
            given_deck[i].suit = s;
            i++;
        }
    }
}



// card reveal

void card_reveal(void) {
    // initialise deck
    printf("Creating deck...\n");
    // allocate memory for main deck
    struct card *deck = malloc(sizeof(struct card) * DECK_SIZE);
    if (!deck) return;
    // assign actual cards
    init_deck(deck, DECK_SIZE);

    // reveal cards
    for (size_t i = 0; i < DECK_SIZE; i++) {
        printf("%zu, %d, %d\n", i, deck[i].suit, deck[i].number);
        // note: use . not -> because deck[i] is a struct even though deck is a pointer
    }
    
    // free memory
    free(deck);
    deck = NULL;
}
