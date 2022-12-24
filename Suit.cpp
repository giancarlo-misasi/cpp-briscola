//
// Created by Gianc on 2022-12-22.
//

#include "Suit.h"
#include <sstream>

namespace {
    const std::vector<std::string> SUIT_SYMBOLS{
            "@",    // Coin
            "$",    // Club
            "#",    // Cup
            "%",    // Sword
    };
}

[[nodiscard]] std::string to_string(Suit suit) {
    std::ostringstream ss;
    ss << SUIT_SYMBOLS[(int) suit];
    return ss.str();
}
