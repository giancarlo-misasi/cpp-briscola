//
// Created by Gianc on 2022-12-23.
//

#ifndef BRISCOLA_GAME_H
#define BRISCOLA_GAME_H


#include <iostream>
#include "Deck.h"
#include "Trick.h"
#include "Phase.h"
#include "Player.h"

class Game {

    Deck _deck{};
    int _teams[2]{0, 0};
    std::vector<Player> _players{};
    int _dealer{0};
    int _player{0};
    int _winner{0};
    Trick _trick{_deck.bottom().suit};
    Phase _phase{Phase::SETUP};

public:
    Game() = default;
    Game(bool two_player, int seed);

    [[nodiscard]] int team_score(int team) const;

    [[nodiscard]] Phase phase() const;

    [[nodiscard]] Suit trump() const;

    [[nodiscard]] int current_player() const;

    [[nodiscard]] const std::vector<Card> &current_player_cards() const;

    [[nodiscard]] int current_trick_score() const;

    [[nodiscard]] std::optional<Card> current_trick_winner() const;

    [[nodiscard]] const std::vector<Card> &current_trick_cards() const;

    [[nodiscard]] std::string to_string() const;

    [[nodiscard]] std::vector<int> serialize() const;

    void print() const {
        std::cout << to_string() << std::endl;
    }

    void load_state(std::vector<int> data);

    void setup(bool two_player, int seed);

    void play(Card card);

    void score_trick();

    void take();

    void score_game();

    void score_and_take();

private:
    void increment_dealer();

    void increment_player();
};

#endif //BRISCOLA_GAME_H
