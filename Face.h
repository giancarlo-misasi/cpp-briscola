//
// Created by Gianc on 2022-12-22.
//

#ifndef BRISCOLA_FACE_H
#define BRISCOLA_FACE_H

#include <string>
#include <vector>
#include <sstream>

enum class Face {
    TWO,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    PRINCE,
    HORSE,
    KING,
    THREE,
    ACE
};

const std::vector<Face> FACES{
        Face::TWO,
        Face::FOUR,
        Face::FIVE,
        Face::SIX,
        Face::SEVEN,
        Face::PRINCE,
        Face::HORSE,
        Face::KING,
        Face::THREE,
        Face::ACE,
};

[[nodiscard]] int to_points(Face face);

[[nodiscard]] std::string to_string(Face face);

#endif //BRISCOLA_FACE_H
