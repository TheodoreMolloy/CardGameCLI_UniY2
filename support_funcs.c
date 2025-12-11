#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include "funcs.h"

static const char CARD_SPACING[] = "        "; // standard spacing between cards

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

/* creation and assignment functions for all games */

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
            // note: the manually assigned lines are stored in the stack (wherever they fit)
            // increment manually
            i++;
        }
    }
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
    srand(time(NULL)); // seeds rand() with no seconds since 1/1/1970
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

void assign_hand_value(struct hand *given_hand, struct card *given_model_deck) {
    /* finds the value of a given hand */
    int hand_value = 0;
    int num_aces = 0;
    for (int i = 0; i < given_hand->size; i++) {
        int card_num = given_model_deck[given_hand->index[i]].number;
        if (card_num < JACK) {
            hand_value += card_num + 2; // based on enumeration
        } else if (card_num < ACE) {
            hand_value += 10;
        } else {
            num_aces++; // need to consider aces individually
        }
    }
    while (num_aces > 0) {
        if (num_aces == 1 & hand_value < 11) { // because only one ace can be 11 (will be bust otherwise)
            hand_value += 11;
            num_aces--;
        } else {
            hand_value += 1;
            num_aces--;
        }
    }
    // note: might be a better method than if statements, used them because there's not too many options
    given_hand->value = hand_value;
}

/* printout functions */

void print_card(struct card given_model_deck[], int index) {
    /* prints a single card from model deck depending on index */
    for (int l = 0; l < NUM_LINES; l++) {
        printf("\n%s", given_model_deck[index].line[l]);
    }
    printf("\n");
}

void print_hidden_cards() {
    /* prints the back's of the house's cards (hence hidden) */
    char card_line[NUM_LINES][CHARS_PER_LINE];
    strcpy(card_line[0], "==================");
    strcpy(card_line[1], "|                |");
    for (int i = 2; i <= NUM_LINES - 4; i = i + 2) { // middle lines (i<=NUM_LINES-4 not -3 because line[i+1] term)
        strcpy(card_line[i],   "| * * * * * * *  |");
        strcpy(card_line[i+1], "|  * * * * * * * |");
    }
    // last two lines
    strcpy(card_line[NUM_LINES - 2], "|                |");
    strcpy(card_line[NUM_LINES - 1], "==================");
    for (int i = 0; i < NUM_LINES; i++) {
        printf("%s%s%s\n", card_line[i], CARD_SPACING, card_line[i]);
    }
    printf("\n");
}

void print_hand(struct hand *given_hand, struct card *given_model_deck) {
    /* prints all cards in a given hand side-by-side with spacing */
    for (int i = 0; i < NUM_LINES; i++) {
        for (int j = 0; j < given_hand->size; j++) { // each line seperately considered
            printf("%s%s", given_model_deck[given_hand->index[j]].line[i], CARD_SPACING);
        }
        printf("\n");
    }
}

void print_user_hand(struct hand *given_user_hand, struct card *given_model_deck) { // calls on print_hidden_cards and print_hand
    printf("\nBanker's Hand:\n");
    print_hidden_cards();
    printf("\nUser's Hand:\n");
    print_hand(given_user_hand, given_model_deck);
    printf("Hand value: %d\n", given_user_hand->value);
}

void print_both_hands(struct hand *given_user_hand, struct hand *given_banker_hand, struct card *given_model_deck) { // calls on print_hand
    printf("\nBanker's Hand:\n");
    print_hand(given_banker_hand, given_model_deck);
    printf("Banker's hand value: %d\n", given_banker_hand->value);
    printf("\nUser's Hand:\n");
    print_hand(given_user_hand, given_model_deck);
    printf("Hand value: %d\n", given_user_hand->value);
}
