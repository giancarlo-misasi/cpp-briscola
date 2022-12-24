//
// Created by Gianc on 2022-12-22.
//

#ifndef BRISCOLA_SUIT_H
#define BRISCOLA_SUIT_H

#include <string>
#include <vector>
#include <optional>
#include <list>

enum class Suit {
    COIN,
    CLUB,
    CUP,
    SWORD
};

const std::vector<Suit> SUITS{
        Suit::COIN,
        Suit::CLUB,
        Suit::CUP,
        Suit::SWORD,
};

[[nodiscard]] std::string to_string(Suit suit);

#endif //BRISCOLA_SUIT_H
