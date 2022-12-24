//
// Created by Gianc on 2022-12-23.
//

#include "Game.h"
#include "Phase.h"
#include "Trick.h"
#include "Player.h"
#include "Deck.h"
#include "Card.h"
#include <vector>
#include <span>
#include <iostream>

namespace {
    void append_all(std::vector<int> &into, const std::vector<int> &from) {
        into.emplace_back(from.size());
        into.insert(into.end(), from.begin(), from.end());
    }
}

Game::Game(bool two_player, int seed) {
    setup(two_player, seed);
}

int Game::team_score(int team) const {
    return _teams[team % 2];
}

Phase Game::phase() const {
    return _phase;
}

Suit Game::trump() const {
    return _deck.bottom().suit;
}

int Game::current_player() const {
    return _player;
}

const std::vector<Card> &Game::current_player_cards() const {
    return _players[_player].cards();
}

int Game::current_trick_score() const {
    return _trick.score();
}

std::optional<Card> Game::current_trick_winner() const {
    return _trick.winner();
}

const std::vector<Card> &Game::current_trick_cards() const {
    return _trick.cards();
}

std::string Game::to_string() const {
    std::ostringstream ss;
//    ss << _deck.to_string();
//    ss << "Phase: " << ::to_string(_phase) << std::endl;
    ss << _trick.to_string();
    for (int i = 0; auto &player: _players) {
        ss << "{" << i << "}" << player.to_string();
        i++;
    }
    ss << "D: " << _dealer << ", P: " << _player << ", W: " << _winner << std::endl;
    ss << "[A]: " << _teams[0] << ", [B]: " << _teams[1] << std::endl;
    return ss.str();
}

std::vector<int> Game::serialize() const {
    std::vector<int> data{};
    append_all(data, _deck.serialize());
    data.emplace_back(_teams[0]);
    data.emplace_back(_teams[1]);
    data.emplace_back(_players.size());
    for (auto &player: _players) {
        append_all(data, player.serialize());
    }
    data.emplace_back(_dealer);
    data.emplace_back(_player);
    data.emplace_back(_winner);
    append_all(data, _trick.serialize());
    data.emplace_back((int) _phase);
    return data;
}

void Game::load_state(std::vector<int> data) {
    std::span<int> span{data};
    std::span<int> s;

    int offset = 0;
    int size = data[offset++];
    s = span.subspan(offset, size);
    offset += s.size();

    _deck = deserialize_deck(s);
    _teams[0] = data[offset++];
    _teams[1] = data[offset++];

    int num_players = data[offset++];
    _players.clear();
    for (int i = 0; i < num_players; i++) {
        size = data[offset++];
        s = span.subspan(offset, size);
        offset += s.size();
        _players.emplace_back(deserialize_player(s));
    }

    _dealer = data[offset++];
    _player = data[offset++];
    _winner = data[offset++];

    size = data[offset++];
    s = span.subspan(offset, size);
    offset += s.size();

    _trick = deserialize_trick(s);
    _phase = (Phase) data[offset++];
}

void Game::setup(bool two_player, int seed) {
    _deck.setup(seed);
    for (int &score: _teams) {
        score = 0;
    }

    _players.clear();
    int number_of_players = two_player ? 2 : 4;
    for (int i = 0; i < number_of_players; i++) {
        _players.emplace_back();
    }

    increment_dealer();
    _player = _dealer;
    increment_player();
    _trick.clear();
    _trick.set_trump(_deck.bottom().suit);

    int remaining = 40 - 3 * (int) _players.size();
    while (_deck.remaining_cards() > remaining) {
        _players.at(_player).take_card(_deck.deal());
        increment_player();
    }

    _phase = Phase::PLAY;
}

void Game::play(Card card) {
    _players[_player].play_card(card);
    if (_trick.add(card)) {
        _winner = _player;
    }

    increment_player();
    if (_trick.full(_players.size())) {
        _phase = Phase::SCORE_TRICK;
    }
}

void Game::score_trick() {
    _players.at(_winner).take_points(_trick.score());
    _player = _winner;
    _trick.clear();
    _phase = Phase::TAKE;

    bool cards_remain = false;
    for (auto &player: _players) {
        if (player.has_cards()) {
            cards_remain = true;
            break;
        }
    }

    if (!cards_remain) {
        _phase = Phase::SCORE_GAME;
    }
}

void Game::take() {
    if (!_deck.is_empty()) {
        _players[_player].take_card(_deck.deal());
        increment_player();
    }

    bool ready_to_play = true;
    if (!_deck.is_empty()) {
        for (auto &player: _players) {
            if (player.cards_remaining() != 3) {
                ready_to_play = false;
            }
        }
    }

    if (ready_to_play) {
        _phase = Phase::PLAY;
    }
}

void Game::score_game() {
    for (int i = 0; i < _players.size(); i++) {
        _teams[i % 2] += _players.at(i).score();
    }

    _phase = Phase::END;
}

void Game::score_and_take() {
    if (_phase == Phase::SCORE_TRICK) {
        score_trick();
    }

    while (_phase == Phase::TAKE) {
        take();
    }

    if (_phase == Phase::SCORE_GAME) {
        score_game();
    }
}

void Game::increment_dealer() {
    _dealer++;
    _dealer %= _players.size();
}

void Game::increment_player() {
    _player++;
    _player %= _players.size();
}
