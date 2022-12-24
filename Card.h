//
// Created by Gianc on 2022-12-22.
//

#ifndef BRISCOLA_CARD_H
#define BRISCOLA_CARD_H

#include "Face.h"
#include "Suit.h"
#include <map>

class Card {

public:
    Face face;
    Suit suit;

    Card(Face f, Suit s);

    auto operator<=>(const Card &) const = default;

    [[nodiscard]] std::string to_string() const;

    [[nodiscard]] bool is_worse_than(Card other, Suit trump) const;

    [[nodiscard]] int serialize() const;
};

Card deserialize_card(int data);

bool sort_lowest_face(const Card &left, const Card &right);

bool sort_highest_face(const Card &left, const Card &right);

#endif //BRISCOLA_CARD_H
