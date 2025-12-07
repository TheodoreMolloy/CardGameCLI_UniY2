#ifndef FUNCS_H
#define FUNCS_H


/* structs */

// card struc
struct card {
    int number;
    int suit;
    char *line[14]; // 14 lines, each will have 19 characters (18 characters plus an '\0')
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

// deck initialisation
void init_deck(struct card given_deck[]);

// main functions
void deck_printout(void);

#endif