/* Structs, Enumerations and Constant Declarations*/

#ifndef STRUCTS_H
#define STRUCTS_H

// constants
const int DECK_SIZE = 52;

// structs
struct card {
    int suit;
    int number;
};

// enumerations
enum card_number {ONE=1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};
enum card_suit {HEARTS, DIAMONDS, CLUBS, SPADES};

#endif