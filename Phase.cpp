//
// Created by Gianc on 2022-12-22.
//

#include "Phase.h"
#include <string>
#include <sstream>

namespace {
    const std::vector<std::string> PHASE_SYMBOLS{
            "Setup",
            "Play",
            "Score Trick",
            "Take",
            "Score Game",
            "End",
    };
}

[[nodiscard]] std::string to_string(Phase phase) {
    std::ostringstream ss;
    ss << PHASE_SYMBOLS[(int) phase];
    return ss.str();
}
