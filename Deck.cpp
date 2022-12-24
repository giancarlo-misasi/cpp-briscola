//
// Created by Gianc on 2022-12-22.
//

#include "Deck.h"
#include "Card.h"
#include <random>
#include <vector>
#include <span>

Deck::Deck() {
    setup(0);
}

int Deck::remaining_cards() const {
    return _cards.size() - _index;
}

bool Deck::is_empty() const {
    return remaining_cards() <= 0;
}

Card Deck::bottom() const {
    return _cards.back();
}

std::vector<int> Deck::serialize() const {
    return {_seed, _index};
}

std::string Deck::to_string() const {
    std::ostringstream ss;
    for (int i = 0; i < _cards.size(); i++) {
        if (_index == i) {
            ss << ">";
        }
        ss << _cards[i].to_string();
        if (i + 1 < _cards.size()) {
            ss << ",";
        }
    }
    ss << std::endl;
    return ss.str();
}

void Deck::setup(unsigned int seed) {
    _seed = seed;
    _index = 0;
    _cards.clear();
    for (Face f: FACES) {
        for (Suit s: SUITS) {
            _cards.emplace_back(f, s);
        }
    }
    auto random = std::default_random_engine{seed};
    std::shuffle(_cards.begin(), _cards.end(), random);
}

void Deck::skip_to(int index) {
    _index = index;
}

Card Deck::deal() {
    if (is_empty()) {
        return _cards.back();
    }
    return _cards[_index++];
}

Deck deserialize_deck(std::span<int> data) {
    Deck deck{};
    deck.setup(data.front());
    deck.skip_to(data.back());
    return deck;
}
