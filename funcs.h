#ifndef FUNCS_H
#define FUNCS_H

#define DECK_SIZE 52
#define NUM_LINES 14 // 14 lines per card in ASCII art
#define CHARS_PER_LINE 19 // 18 characters plus an '\0'
#define NUM_SUITS 4
#define NUM_NUMBERS 13
#define MAX_HAND_SIZE 5 // pontoon 5 trick rule

/* structs */

// card struct
struct card {
    int number;
    int suit;
    char *line[NUM_LINES];
    // note: this is an array of addresses, each address will point to a lookup table element or assigned value in stack
    // note: this is okay because we don't need to edit later, use fixed values if need to change periodically
};

struct hand {
    int *index;
    int size;
    int value;
};

/* enumerations */
enum card_number {TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE};
enum card_suit {HEARTS, DIAMONDS, CLUBS, SPADES};

/* functions*/

// CLI
void menu_item_1(void);
void menu_item_2(void);
void menu_item_3(void);

// game functions
void deck_printout(void);
void random_card(void);
void pontoon(void);

// functions for all games
struct card *create_model_deck(void);
void init_deck(struct card given_model_deck[]);
void print_card(struct card given_model_deck[], int index);
int *create_shuffled_ideck(void);
void shuffle_ideck(int ideck[]);
struct card *create_hidden_card(void);
void print_hidden_cards(struct card *hidden_card);
void print_hand(struct hand *given_hand, struct card *given_model_deck);
void assign_hand_value(struct hand *given_hand, struct card *given_model_deck);
void print_user_hand(struct hand *given_user_hand, struct card *given_hidden_card, struct card *given_model_deck);
void print_both_hands(struct hand *given_user_hand, struct hand *given_banker_hand, struct card *given_model_deck);

#endif