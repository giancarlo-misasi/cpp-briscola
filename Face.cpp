//
// Created by Gianc on 2022-12-22.
//

#include "Face.h"

namespace {
    const std::vector<int> FACE_POINTS{0, 0, 0, 0, 0, 2, 3, 4, 10, 11};
    const std::vector<std::string> FACE_SYMBOLS{"2", "4", "5", "6", "7", "J", "Q", "K", "3", "A"};
}

int to_points(Face face) {
    return FACE_POINTS[(int) face];
}

std::string to_string(Face face) {
    std::ostringstream ss;
    ss << FACE_SYMBOLS[(int) face];
    return ss.str();
}
