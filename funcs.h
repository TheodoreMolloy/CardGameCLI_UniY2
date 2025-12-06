#ifndef FUNCS_H
#define FUNCS_H

// structs
struct card {
    int number;
    int suit;
    int value;
    
};

// enumerations
enum card_number {ONE=1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};
enum card_suit {HEARTS, DIAMONDS, CLUBS, SPADES};

// CLI
void menu_item_1(void);
void menu_item_2(void);
void menu_item_3(void);
void menu_item_4(void);

// deck initialisation
void init_deck(struct card given_deck[], const int size);

// main functions
void card_reveal(void);

#endif