//
// Created by Gianc on 2022-12-22.
//

#include "Card.h"
#include <iostream>

Card::Card(Face f, Suit s) : face(f), suit(s) {
}

[[nodiscard]] std::string Card::to_string() const {
    std::ostringstream ss;
    ss << ::to_string(face) << ::to_string(suit);
    return ss.str();
}

[[nodiscard]] bool Card::is_worse_than(Card other, Suit trump) const {
    if (suit == other.suit) {
        return other.face > face;
    }
    return other.suit == trump;
}

int Card::serialize() const {
    return (int)suit * 100 + (int)face;
}

Card deserialize_card(int data) {
    return {(Face) (data % 100), (Suit) (data / 100)};
}

bool sort_lowest_face(const Card &left, const Card &right) {
    return left.face < right.face;
}

bool sort_highest_face(const Card &left, const Card &right) {
    return left.face > right.face;
}
