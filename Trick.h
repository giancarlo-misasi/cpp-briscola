//
// Created by Gianc on 2022-12-22.
//

#ifndef BRISCOLA_TRICK_H
#define BRISCOLA_TRICK_H

#include "Card.h"
#include <optional>
#include <span>

class Trick {

    Suit _trump;
    int _score;
    std::optional<Card> _winner;
    std::vector<Card> _cards;
public:
    explicit Trick(Suit trump);

    [[nodiscard]] int score() const;

    [[nodiscard]] std::optional<Card> winner() const;

    [[nodiscard]] const std::vector<Card> &cards() const;

    [[nodiscard]] int full(int num_players) const;

    [[nodiscard]] std::string to_string() const;

    [[nodiscard]] std::vector<int> serialize() const;

    void set_trump(Suit trump);

    bool add(Card card);

    void clear();
};

Trick deserialize_trick(std::span<int> data);


#endif //BRISCOLA_TRICK_H
