#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "funcs.h"


// ASCII art lookup tables
// note: could have used a table of structs for clarity but it felt overcomplicated for this task - might use later
static char number_art_table[NUM_NUMBERS][2][CHARS_PER_LINE] = { // two number-specific lines in each card
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
static char suit_art_table[NUM_SUITS][8][CHARS_PER_LINE] = { // eight suit-specific lines in each card
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


/* CLI functions */

// calls deck_printout
void menu_item_1(void) {
    printf("\n>> Deck Printout\n");
    deck_printout();
}

void menu_item_2(void) {
    printf("\n>> Random Card\n");
    random_card();
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


/* game functions */

void deck_printout(void) {
    // create deck
    struct card *model_deck = create_model_deck();
    // print cards
    printf("Printing Deck...\n");
    for (int i = 0; i < DECK_SIZE; i++) {
        print_card(model_deck, i);
    }
    printf("\n");
    // free memory
    free(model_deck);
    model_deck = NULL;
}

void random_card(void) {
    // create model deck and shuffled index deck
    printf("Creating Deck...\n");
    struct card *model_deck = create_model_deck();
    printf("Shuffling Deck...\n");
    int *shuffled_ideck = create_shuffled_ideck();
    // game
    int game_ongoing = 1;
    char buff[64]; // stores user input
    int shuffled_index = 0; // stores index of current card in shuffled ideck
    while (game_ongoing) {
        int model_deck_index = shuffled_ideck[shuffled_index]; // stores index of current card in model deck
        printf("\nEnter 'c' or 'C' for a random card.\nEnter 'r or 'R' to reset the deck.\nEnter 'b' or 'B' to stop playing.\n");
        if (!fgets(buff, sizeof(buff), stdin)) { // assign input to buff and check assessible
            puts("\nInput error. Exiting.\n");
            exit(1);
        }
        buff[0] = tolower(buff[0]);
        switch(buff[0]) {
          case 'c': // play new card
            if (shuffled_index < DECK_SIZE) {
                print_card(model_deck, model_deck_index);
                shuffled_index++;
            } else { // end of deck, reset and play
                printf("You've reached the end of the deck, resetting deck...\n");
                shuffle_ideck(shuffled_ideck);
                shuffled_index = 0;
                model_deck_index = shuffled_ideck[shuffled_index];
                print_card(model_deck, model_deck_index);
                shuffled_index++;
            }
            break;
          case 'r':  // reset deck
            shuffle_ideck(shuffled_ideck);
            shuffled_index = 0;
            break;
          case 'b': // stop playing
            game_ongoing = 0;
            break;
          default:
            printf("Invalid input, please try again.\n");
            break;
        }

    }
    // free memory
    free(model_deck);
    model_deck = NULL;
    free(shuffled_ideck);
    shuffled_ideck = NULL;
}


/* functions for all games */

struct card *create_model_deck(void) { // calls on init_deck
    // initialise deck
    printf("Creating deck...\n");
    // allocate memory for main deck
    struct card *deck = malloc(sizeof(struct card) * DECK_SIZE);
    if (!deck) {
        printf("create_model_deck memory allocation failure");
        return NULL;
    }
    // assign actual cards
    init_deck(deck);
    return deck;
}

void init_deck(struct card given_model_deck[]) {
    /* assigns an array of cards their suit, number, and properties (incl. ASCII representations) in standard deck order */
    // note: use an ASCII lookup table to avoid if or switch statement
    int i = 0;
    for (int s = 0; s < NUM_SUITS; s++) {
        for (int n = 0; n < NUM_NUMBERS; n++) {
            // note: use . not -> because deck[i] is a struct even though deck is a pointer
            given_model_deck[i].number = n;
            given_model_deck[i].suit = s; // 0 is TWO, 12 is ACE, use enumeration names during games not here
            // assign ASCII art
            given_model_deck[i].line[0] = "==================";
            given_model_deck[i].line[1] = number_art_table[n][0];
            given_model_deck[i].line[2] = "|                |";
            for (int l = 0; l < 8; l++) given_model_deck[i].line[l+3] = suit_art_table[s][l];
            given_model_deck[i].line[11] = "|                |";
            given_model_deck[i].line[12] = number_art_table[n][1];
            given_model_deck[i].line[13] = "==================";
            // increment manually
            i++;
        }
    }
}

void print_card(struct card given_model_deck[], int index) {
    for (int l = 0; l < NUM_LINES; l++) {
        printf("\n%s", given_model_deck[index].line[l]);
    }
    printf("\n");
}

int *create_shuffled_ideck(void) { // calls on shuffle_ideck
    /* allocates memory and shuffles ideck */
    int *ideck = malloc(sizeof(int) * DECK_SIZE);
    if (!ideck) {
        printf("create_model_deck memory allocation failure");
        return NULL;
    }
    shuffle_ideck(ideck);
    return ideck;
}

void shuffle_ideck(int ideck[]) {
    /* assigns 0 to 51 to array of an index deck (ideck) and uses a fisher-yates shuffle */
    for (int n = 0; n < DECK_SIZE; n++) ideck[n] = n;
    for (int i = 0; i < DECK_SIZE; i++) {
        // pick a random second index value from i to 51 to swap with
        int j = i + rand() % (DECK_SIZE - i);
        // swap indexes i and j
        int temp = ideck[i];
        ideck[i] = ideck[j];
        ideck[j] = temp;
    }
}