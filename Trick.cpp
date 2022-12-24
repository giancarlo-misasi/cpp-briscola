//
// Created by Gianc on 2022-12-22.
//

#include "Trick.h"
#include "Card.h"
#include <optional>

Trick::Trick(Suit trump) : _trump(trump), _score{0}, _cards{} {
}

int Trick::score() const {
    return _score;
}

std::optional<Card> Trick::winner() const {
    return _winner;
}

const std::vector<Card> &Trick::cards() const {
    return _cards;
}

int Trick::full(int num_players) const {
    return _cards.size() == num_players;
}

std::string Trick::to_string() const {
    std::ostringstream ss;
    for (auto & card : _cards) {
        if (_winner.has_value() && _winner.value() == card) {
            ss << ">";
        }
        ss << card.to_string() << ",";
    }
    ss << "T=" << ::to_string(_trump) << ", S=" << _score << "" << std::endl;
    return ss.str();
}

std::vector<int> Trick::serialize() const {
    std::vector<int> data{};
    data.emplace_back((int) _trump);
    for (auto &card : _cards) {
        data.emplace_back(card.serialize());
    }
    return data;
}

void Trick::set_trump(Suit trump) {
    _trump = trump;
}

bool Trick::add(Card card) {
    _score += to_points(card.face);
    _cards.emplace_back(card);

    if (!_winner.has_value() || _winner->is_worse_than(card, _trump)) {
        _winner = card;
        return true;
    }

    return false;
}

void Trick::clear() {
    _score = 0;
    _winner.reset();
    _cards.clear();
}

Trick deserialize_trick(std::span<int> data) {
    Trick trick{(Suit) data.front()};
    for (int i = 1; i < data.size(); i++) {
        trick.add(deserialize_card(data[i]));
    }
    return trick;
}
