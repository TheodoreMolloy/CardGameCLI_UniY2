#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "funcs.h"

const int DECK_SIZE = 52;
const int NUM_LINES = 14;

// ASCII art lookup tables
// note: could have used a table of structs for clarity but it felt overcomplicated for this task - might use later
static char number_art_table[13][2][19] = { // 13 numbers, two number-specific lines, 19 characters per line (18 and a \0)
    /*  2nd line         ,    13th line       */
    {"| 2              |", "|              2 |"},
    {"| 3              |", "|              3 |"},
    {"| 4              |", "|              4 |"},
    {"| 5              |", "|              5 |"},
    {"| 6              |", "|              6 |"},
    {"| 7              |", "|              7 |"},
    {"| 8              |", "|              8 |"},
    {"| 9              |", "|              9 |"},
    {"| 10             |", "|             10 |"},
    {"| J              |", "|              J |"},
    {"| Q              |", "|              Q |"},
    {"| K              |", "|              K |"},
    {"| A              |", "|              A |"},
};
static char suit_art_table[4][8][19] = { // 4 suits, two suit-specific lines, 19 characters per line (18 and a \0)
    /* 4th to 11th lines */
    { // note: \\ will read as just \ rather than an escape sequence
        "|       /\\       |",
        "|      /  \\      |",
        "|     /    \\     |",
        "|    /      \\    |",
        "|    \\      /    |",
        "|     \\    /     |",
        "|      \\  /      |",
        "|       \\/       |",
    },
        {
        "|     /\\  /\\     |",
        "|    /  \\/  \\    |",
        "|   /        \\   |",
        "|   \\        /   |",
        "|    \\      /    |",
        "|     \\    /     |",
        "|      \\  /      |",
        "|       \\/       |",
    },
        {
        "|       @@@      |",
        "|    @@@@@@@@    |",
        "|     @@@@@@     |",
        "|  @ @@@@@@@@ @  |",
        "| @@@@@@@@@@@@@@ |",
        "| @@@@@@@@@@@@@@ |",
        "|  @@   ##   @@  |",
        "|      ####      |",
    },
        {
        "|       @@       |",
        "|      @@@@      |",
        "|    @@@@@@@@    |",
        "|   @@@@@@@@@@   |",
        "|   @@@@@@@@@@   |",
        "|    @@@@@@@@    |",
        "|       ##       |",
        "|      ####      |",
    }
};

// deck_printout
void menu_item_1(void) {
    printf("\n>> Deck Printout\n");
    deck_printout();
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

void init_deck(struct card given_deck[]) {
    /* assigns an array of cards their suit, number, and properties (incl. ASCII representations) in standard deck order */
    // note: use an ASCII lookup table to avoid if or switch statement
    int i = 0;
    for (int s = 0; s < 4; s++) {
        for (int n = 0; n < 13; n++) {
            // note: use . not -> because deck[i] is a struct even though deck is a pointer
            given_deck[i].number = n;
            given_deck[i].suit = s; // 0 is TWO, 12 is ACE, use enumeration names during games not here
            // assign ASCII art
            given_deck[i].line[0] = "==================";
            given_deck[i].line[1] = number_art_table[n][0];
            given_deck[i].line[2] = "|                |";
            for (int l = 0; l < 8; l++) {
                given_deck[i].line[l+3] = suit_art_table[s][l];
            }
            given_deck[i].line[11] = "|                |";
            given_deck[i].line[12] = number_art_table[n][1];
            given_deck[i].line[13] = "==================";
            // increment manually
            i++;
        }
    }
    // don't assign value as it can change mid game for an ACE
}




// deck printout
void deck_printout(void) {
    // initialise deck
    printf("Creating deck...\n");
    // allocate memory for main deck
    struct card *deck = malloc(sizeof(struct card) * DECK_SIZE);
    if (!deck) return;
    // assign actual cards
    init_deck(deck);

    // print cards
    printf("Revealing cards...\n");
    for (int i = 0; i < DECK_SIZE; i++) {
        //printf("%d, %d, %d\n", i, deck[i].suit, deck[i].number);
        printf("\n");
        for (int l = 0; l < NUM_LINES; l++) {
            printf("\n%s", deck[i].line[l]);
        }    
    }
    printf("\n");
    // free memory

    free(deck);
    deck = NULL;
}
