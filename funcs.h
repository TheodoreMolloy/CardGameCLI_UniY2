#ifndef FUNCS_H
#define FUNCS_H

#define DECK_SIZE 52
#define NUM_LINES 14 // 14 lines per card in ASCII art
#define CHARS_PER_LINE 19 // 18 characters plus an '\0'
#define NUM_SUITS 4
#define NUM_NUMBERS 13

/* structs */

// card struct
struct card {
    int number;
    int suit;
    char *line[NUM_LINES];
    // note: this is an array of addresses, each address will point to a lookup table element or assigned value in stack
    // note: this is okay because we don't need to edit later, use fixed values if need to change periodically
};

/* enumerations */
enum card_number {TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE};
enum card_suit {HEARTS, DIAMONDS, CLUBS, SPADES};

/* functions*/

// CLI
void menu_item_1(void);
void menu_item_2(void);
void menu_item_3(void);
void menu_item_4(void);

// game functions
void deck_printout(void);
void random_card(void);

// functions for all games
struct card *create_model_deck(void);
void init_deck(struct card given_model_deck[]);
void print_card(struct card given_model_deck[], int index);
int *create_shuffled_ideck(void);
void shuffle_ideck(int ideck[]);

#endif