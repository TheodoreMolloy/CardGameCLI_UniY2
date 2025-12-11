#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include "funcs.h"


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
    printf("Shuffling Deck...\n");
    // shuffled ideck and hands are just index values for model_deck
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
            print_both_hands(user_hand, banker_hand, model_deck);
            printf("\nBanker wins with a pontoon!\n");
            continue;
        }
        if (user_hand->value == 21) {
            print_both_hands(user_hand, banker_hand, model_deck);
            printf("\nUser wins with a pontoon!\n");
            continue;
        }
        // note: continue sends back to start of while loop

        // note: increment size after calling it so size means size not index
        // user's turn
        printf("\nUser's turn\n");
        // define game-ending senarios variables now because they need to persist between while loops for double break;
        int early_end = 0;
        while_condition = 1;
        while (while_condition) {
            // checks for game-ending senarios now so can print actual bankers cards 
            // check for bust
            if (user_hand->value > 21) {
                early_end = 1;
                print_both_hands(user_hand, banker_hand, model_deck);
                printf("BUST! Banker wins!\n");
                break;
            }
            // check for a 5 card trick
            if (user_hand->size == 5) {
                early_end = 1;
                print_both_hands(user_hand, banker_hand, model_deck);
                printf("User wins with a 5 card trick!\n");
                break;
            }

            // print cards
            print_user_hand(user_hand, model_deck);
            
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
        if (early_end) continue;

        // bankers turn
        printf("\nBanker's turn\n");
        while_condition = 1;
        while (while_condition) {
            // print cards
            print_both_hands(user_hand, banker_hand, model_deck);
            // checks for game-ending senarios
            if (banker_hand->value > 21) {
                printf("\nBanker is bust! User wins!\n");
                early_end = 1;
                break;
            }
            // check for a 5 card trick
            if (banker_hand->size == 5) {
                printf("\nBanker wins with a 5 card trick!\n");
                early_end = 1;
                break;
            }
            // banker twists until 17 or higher
            if (banker_hand->value < 17) {
                banker_hand->index[banker_hand->size++] = shuffled_ideck[shuffled_index++];
                assign_hand_value(banker_hand, model_deck);
            } else while_condition = 0;
            sleep(1); // pause for 1 second between banker turns
        }
        if (early_end) continue;
        if (user_hand->value > banker_hand->value) printf("\nUser Wins!\n");
        else if (user_hand->value == banker_hand->value) printf("Banker wins by default!");
        else printf("\nBanker wins!\n");
    }

    // free memory
    free(model_deck);
    model_deck = NULL;
    free(shuffled_ideck);
    shuffled_ideck = NULL;
    free(user_hand);
    user_hand = NULL;
    free(banker_hand);
    banker_hand = NULL;
}

