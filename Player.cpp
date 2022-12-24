//
// Created by Gianc on 2022-12-22.
//

#include "Player.h"
#include "Card.h"
#include <vector>

int Player::score() const {
    return _score;
}

const std::vector<Card> &Player::cards() const {
    return _cards;
}

bool Player::has_cards() const {
    return !_cards.empty();
}

int Player::cards_remaining() const {
    return _cards.size();
}

std::string Player::to_string() const {
    std::ostringstream ss;
    for (auto &card : _cards) {
        ss << card.to_string() << ",";
    }
    ss << " S=" << _score << std::endl;
    return ss.str();
}

std::vector<int> Player::serialize() const {
    std::vector<int> data{};
    data.emplace_back(_score);
    for (Card c: _cards) {
        data.emplace_back(c.serialize());
    }
    return data;
}

void Player::take_card(Card card) {
    _cards.emplace_back(card);
}

void Player::take_points(int points) {
    _score += points;
}

void Player::play_card(Card card) {
    std::erase(_cards, card);
}

Player deserialize_player(std::span<int> data) {
    Player p{};
    p.take_points(data.front());
    for (int i = 1; i < data.size(); i++) {
        p.take_card(deserialize_card(data[i]));
    }
    return p;
}
