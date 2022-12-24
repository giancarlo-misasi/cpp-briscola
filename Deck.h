//
// Created by Gianc on 2022-12-22.
//

#ifndef BRISCOLA_DECK_H
#define BRISCOLA_DECK_H


#include "Face.h"
#include "Suit.h"
#include "Card.h"
#include <vector>
#include <span>

class Deck {

private:
    int _seed;
    int _index;
    std::vector<Card> _cards;
public:
    Deck();

    [[nodiscard]] int remaining_cards() const;

    [[nodiscard]] bool is_empty() const;

    [[nodiscard]] Card bottom() const;

    [[nodiscard]] std::vector<int> serialize() const;

    [[nodiscard]] std::string to_string() const;

    void setup(unsigned int seed);

    void skip_to(int index);

    Card deal();
};

Deck deserialize_deck(std::span<int> data);


#endif //BRISCOLA_DECK_H
