#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
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


/* CLI functions */

// calls deck_printout
void menu_item_1(void) {
    printf("\n>> Deck Printout\n");
    deck_printout();
}

// calls random_card
void menu_item_2(void) {
    printf("\n>> Random Card\n");
    random_card();
}

// calls pontoon
void menu_item_3(void) {
    printf("\n>> Pontoon\n");
    pontoon();
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
        printf("\nEnter 'c' or 'C' for a random card.\nEnter 'r or 'R' to reset the deck.\nEnter 'b' or 'B' to stop playing.\n");
        if (!fgets(buff, sizeof(buff), stdin)) { // assign input to buff and check assessible
            puts("\nInput error. Exiting.\n");
            exit(1);
        }
        buff[0] = tolower(buff[0]);
        switch(buff[0]) {
          case 'c': // play new card
            if (shuffled_index < DECK_SIZE) { // still in deck, play
                print_card(model_deck, shuffled_ideck[shuffled_index++]);
            } else { // end of deck, reset and play
                printf("You've reached the end of the deck, resetting deck...\n");
                // reset
                shuffle_ideck(shuffled_ideck);
                shuffled_index = 0;
                // play
                print_card(model_deck, shuffled_ideck[shuffled_index++]);
            }
            break;
          case 'r':  // reset deck
            printf("Shuffling Deck...");
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

void pontoon(void) {
    // create variables / allocate memory
    printf("Creating Deck...\n");
    struct card *model_deck = create_model_deck(); // reference deck
    struct card *hidden_card = create_hidden_card(); // purely for display purposes
    printf("Shuffling Deck...\n");
    // shuffled decl and hands are just index values for model_deck
    int *shuffled_ideck = create_shuffled_ideck();
    struct hand *user_hand = calloc(1, sizeof(struct hand));
    struct hand *banker_hand = calloc(1, sizeof(struct hand));
    user_hand->index = calloc(MAX_HAND_SIZE, sizeof(int));
    banker_hand->index = calloc(MAX_HAND_SIZE, sizeof(int));
    // note: need to allocate memory for arrays not just struct pointers, didn't have to for model_deck.line[i] because that was a pointer to a string stored in stack.
    // note: i thought about realloc but its really not worth it for arrays this small
    char buff[64]; // stores user input
    // game
    int game_ongoing = 1;
    while (game_ongoing) {
        // define game-ending senarios variables now because they need to persist between while loops for double break;
        int bust = 0;
        int five_card_trick = 0;
        // clear hands
        for (int i = 0; i < MAX_HAND_SIZE; i++) {user_hand->index[i] = 0;}
        user_hand->size = 0;
        user_hand->value = 0;
        for (int i = 0; i < MAX_HAND_SIZE; i++) {banker_hand->index[i] = 0;}
        banker_hand->size = 0;
        banker_hand->value = 0;
        // note: technically only need to adjust size and value and rest will write over during the game, but this feels like good practice
        // reshuffle
        int shuffled_index = 0;
        shuffle_ideck(shuffled_ideck);
        int while_condition = 1;
        // check to proceed        
        while (while_condition) {
            // safely take input
            printf("\nLet's play Pontoon! Do you wish to proceed?\nEnter 's' to start or 'e' to exit.\n");
            if (!fgets(buff, sizeof(buff), stdin)) {
                puts("\nInput error. Exiting.\n");
                exit(1);
            }
            // determine next action
            buff[0] = tolower(buff[0]); // allow for lowercase
            switch (buff[0]) {
              case 's':
                while_condition = 0; // breaks while loop
                break;
              case 'e':
                game_ongoing = 0;
                while_condition = 0;
                break;
              default:
                printf("\nInvalid input, please try again.\n");
                break;
            }
        }
        if (game_ongoing == 0) break; // if user entered e, this will activate
        // note: this is my blueprint for user input.

        // initial deal
        for (int i = 0; i < 2; i++) {
            user_hand->index[user_hand->size++] = shuffled_ideck[shuffled_index++];
            banker_hand->index[banker_hand->size++] = shuffled_ideck[shuffled_index++];
        }
        assign_hand_value(user_hand, model_deck);
        assign_hand_value(banker_hand, model_deck);
        
        // check for a pontoon - outside turn so doesn't need own variable to break out of while loops
        if (banker_hand->value == 21) { // banker wins on draw so evaluate their cards first
            printf("\nBanker's Hand:\n");
            print_hidden_cards(hidden_card);
            printf("\nUser's Hand:\n");
            print_hand(user_hand, model_deck);
            printf("\nBanker wins with a pontoon!");
            continue;
        }
        if (user_hand->value == 21) {
            printf("\nBanker's Hand:\n");
            print_hidden_cards(hidden_card);
            printf("\nUser's Hand:\n");
            print_hand(user_hand, model_deck);
            printf("\nBanker wins with a pontoon!");
            continue;
        }

        // note: increment size after calling it so size means size not index
        // user's turn
        while_condition = 1;
        while (while_condition) {
            // checks for game-ending senarios now so can print actual bankers cards 
            // check for bust
            if (user_hand->value > 21) {
                bust = 1;
                // print cards
                printf("\nBanker's Hand:\n");
                print_hidden_cards(hidden_card);
                printf("\nUser's Hand:\n");
                print_hand(user_hand, model_deck);
                printf("Hand value: %d\n", user_hand->value);
                printf("BUST! Banker wins!\n");
                break;
            }
            // check for a 5 card trick
            if (user_hand->size == 5) {
                five_card_trick = 1;
                printf("\nBanker's Hand:\n");
                print_hidden_cards(hidden_card);
                printf("\nUser's Hand:\n");
                print_hand(user_hand, model_deck);
                printf("Hand value: %d\n", user_hand->value);
                printf("User wins with a 5 card trick!");
                break;
            }

            // print cards
            printf("\nBanker's Hand:\n");
            print_hidden_cards(hidden_card);
            printf("\nUser's Hand:\n");
            print_hand(user_hand, model_deck);
            printf("Hand value: %d\n", user_hand->value);
            
            // safely take input
            printf("\nEnter 't' to twist.\nEnter 's' to stick.\nEnter 'e' to exit game.\n");
            if (!fgets(buff, sizeof(buff), stdin)) { // assign input to buff and check assessible
                puts("\nInput error. Exiting.\n");
                exit(1);
            }
            // determine next action
            buff[0] = tolower(buff[0]); // allow for lowercase
            switch (buff[0]) {
              case 't':
                user_hand->index[user_hand->size++] = shuffled_ideck[shuffled_index++];
                assign_hand_value(user_hand, model_deck);
                break;
              case 's':
                while_condition = 0; // breaks while loop
                break;
              case 'e':
                game_ongoing = 0;
                while_condition = 0;
                break;
              default:
                printf("\nInvalid input, please try again.\n");
                break;
            }
        }
        if (!game_ongoing) break; // if user entered e, this will activate
        if (bust) continue;
        if (five_card_trick) continue;
    }

    // free memory
    free(hidden_card);
    hidden_card = NULL;
    free(model_deck);
    model_deck = NULL;
    free(shuffled_ideck);
    shuffled_ideck = NULL;
    free(user_hand);
    user_hand = NULL;
    free(banker_hand);
    banker_hand = NULL;
}

/* functions for all games */

struct card *create_model_deck(void) { // calls on init_deck - used in all games
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

void init_deck(struct card given_model_deck[]) { // used in create_model_deck
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

void print_card(struct card given_model_deck[], int index) { // used in random_card
    /* prints a single card from model deck depending on index */
    for (int l = 0; l < NUM_LINES; l++) {
        printf("\n%s", given_model_deck[index].line[l]);
    }
    printf("\n");
}

int *create_shuffled_ideck(void) { // calls on shuffle_ideck - used in random_card
    /* allocates memory and shuffles ideck */
    int *ideck = malloc(sizeof(int) * DECK_SIZE);
    if (!ideck) {
        printf("create_model_deck memory allocation failure");
        return NULL;
    }
    shuffle_ideck(ideck);
    return ideck;
}

void shuffle_ideck(int ideck[]) { // used in create_shuffled_ideck and random_deck
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

struct card *create_hidden_card(void) { // used in pontoon
    /* creates back of card so dealers hand print hidden */
    struct card *hid_card = calloc(1, sizeof(struct card)); // calloc initialises which is good because we don't assign to all numbers
    if (!hid_card) {
        printf("create_hid_card memory allocation failure");
        return NULL;
    }
    // note: need -> not . because hid_card is a pointer
    // first two lines
    hid_card->line[0] =               "==================";
    hid_card->line[1] =               "|                |";
    for (int i = 2; i <= NUM_LINES - 4; i = i + 2) { // middle lines (i<=NUM_LINES-4 not -3 because line[i+1] term)
        hid_card->line[i] =           "| * * * * * * *  |";
        hid_card->line[i + 1] =       "|  * * * * * * * |";
    }
    // last two lines
    hid_card->line[NUM_LINES - 2] =   "|                |";
    hid_card->line[NUM_LINES - 1] =   "==================";
} // note: card struct but number and suit aren't allocated, this won't be an issue unless called but I'm unsure if its good practice

void print_hidden_cards(struct card *hid_card) { // used in pontoon
    /* prints the house's cards */
    for (int i = 0; i < NUM_LINES; i++) {
        printf("%s%s%s\n", hid_card->line[i], CARD_SPACING, hid_card->line[i]);
    }
    printf("\n");
}
// note: hidden_card is for display only - this is overcomplicated, just reduce to one print_hidden_cards() which is the same every time

void print_hand(struct hand *given_hand, struct card *given_model_deck) {
    /* prints all cards in a given hand side-by-side with spacing */
    for (int i = 0; i < NUM_LINES; i++) {
        for (int j = 0; j < given_hand->size; j++) { // each line seperately considered
            printf("%s%s", given_model_deck[given_hand->index[j]].line[i], CARD_SPACING);
        }
        printf("\n");
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