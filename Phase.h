//
// Created by Gianc on 2022-12-22.
//

#ifndef BRISCOLA_PHASE_H
#define BRISCOLA_PHASE_H

#include <vector>
#include <string>

enum class Phase {
    SETUP,
    PLAY,
    SCORE_TRICK,
    TAKE,
    SCORE_GAME,
    END
};

const std::vector<Phase> PHASES{
        Phase::SETUP,
        Phase::PLAY,
        Phase::SCORE_TRICK,
        Phase::TAKE,
        Phase::SCORE_GAME,
        Phase::END,
};

[[nodiscard]] std::string to_string(Phase phase);

#endif //BRISCOLA_PHASE_H
