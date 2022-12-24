//
// Created by Gianc on 2022-12-22.
//

#ifndef BRISCOLA_PLAYER_H
#define BRISCOLA_PLAYER_H


#include "Card.h"
#include <vector>
#include <span>

class Player {

    int _score{0};
    std::vector<Card> _cards{};
public:
    [[nodiscard]] int score() const;

    [[nodiscard]] const std::vector<Card> &cards() const;

    [[nodiscard]] bool has_cards() const;

    [[nodiscard]] int cards_remaining() const;

    [[nodiscard]] std::string to_string() const;

    [[nodiscard]] std::vector<int> serialize() const;

    void take_card(Card card);

    void take_points(int points);

    void play_card(Card card);
};

Player deserialize_player(std::span<int> data);

#endif //BRISCOLA_PLAYER_H
