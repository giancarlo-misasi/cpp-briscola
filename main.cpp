#include "Phase.h"
#include "Game.h"
#include "mcts.h"
#include "graphviz.h"
#include <iostream>
#include <ranges>
#include <memory>
#include <execution>

class GameState : public Game, public State {
public:
    [[nodiscard]] GameState serialize_copy() const {
        GameState result{};
        result.load_state(serialize());
        return result;
    }

    void print(std::ostream &strm) const override {
        strm << to_string();
    }

    static GameState from_game(const Game &game) {
        GameState result{};
        result.load_state(game.serialize());
        return result;
    }
};

class GameAction : public Action<GameState> {
    Card _card{Face::ACE, Suit::SWORD};
public:
    GameAction() = default;

    explicit GameAction(Card card) : _card{card} {
    }

    auto operator<=>(const GameAction &other) const {
        return _card.operator<=>(other._card);
    }

    [[nodiscard]] Card card() const {
        return _card;
    }

    void set_card(Card card) {
        _card = card;
    }

    void execute(GameState &state) override {
        state.play(_card);
        state.score_and_take();
    }

    void print(std::ostream &strm) const override {
        strm << _card.to_string();
    }
};

class GameBackPropogation : public Backpropagation<GameState> {
    int _player;
public:
    explicit GameBackPropogation(int player) : _player{player} {
    }

    void set_player(int player) {
        _player = player;
    }

    float updateScore(const GameState &state, float backpropScore) override {
        return backpropScore;
    }
};

class GameTerminationCheck : public TerminationCheck<GameState> {
public:
    bool isTerminal(const GameState &state) override {
        return state.phase() == Phase::END;
    }
};

class GameScoring : public Scoring<GameState> {
    int _team{0};
public:
    explicit GameScoring(int player) {
        set_player(player);
    }

    void set_player(int player) {
        _team = player % 2;
    }

    float score(const GameState &state) override {
        int opposite_team = _team == 0 ? 1 : 0;
        int my_score = state.team_score(_team);
        int opponent_score = state.team_score(opposite_team);
        if (my_score > opponent_score) {
            return 1;
        } else if (my_score < opponent_score) {
            return 0;
        } else {
            return 0.5;
        }
    }
};

class GameExpansionStrategy : public ExpansionStrategy<GameState, GameAction> {
    int _card_index;
public:
    explicit GameExpansionStrategy(GameState *state)
            : ExpansionStrategy<GameState, GameAction>(state), _card_index{0} {
    }

    [[nodiscard]] bool canGenerateNext() const override {
        return _card_index < state->current_player_cards().size();
    }

    GameAction generateNext() override {
        int index = _card_index++;
        return GameAction{state->current_player_cards()[index]};
    }
};

class GamePlayoutStrategy : public PlayoutStrategy<GameState, GameAction> {
    std::random_device _device;
    std::mt19937 _generator = std::mt19937(_device());
    std::uniform_int_distribution<unsigned int> _distribution;
public:
    explicit GamePlayoutStrategy(GameState *state)
            : PlayoutStrategy<GameState, GameAction>(state),
              _distribution{std::uniform_int_distribution<unsigned int>(0, 2)} {
    }

    void generateRandom(GameAction &action) override {
        auto &cards = state->current_player_cards();
        int i = cards.size();
        while (i >= cards.size()) {
            i = _distribution(_generator);
        }
        action.set_card(cards[i]);
    }
};

using GameMcts = MCTS<GameState, GameAction, GameExpansionStrategy, GamePlayoutStrategy>;

auto backpropogation = std::make_shared<GameBackPropogation>(1);
auto termination = std::make_shared<GameTerminationCheck>();
auto scoring = std::make_shared<GameScoring>(1);

void set_ai_player_index(int index) {
    backpropogation->set_player(index);
    scoring->set_player(index);
}

GameMcts setup_mcts(const Game &game) {
    auto state = GameState::from_game(game);
    auto mcts = GameMcts{state.serialize_copy(), backpropogation, termination, scoring};
    mcts.setTime(100); // think on it hard!
    mcts.setC(0.4); // favor less exploration
//    mcts.setMinT(5);
//    mcts.setMinVisits(5);
    mcts.setMinIterations(20);
    return mcts;
}

std::random_device rd;
std::mt19937 generator = std::mt19937(rd());
std::uniform_int_distribution<unsigned int> distribution{0, 2};

void random_card_strategy(Game &game) {
    auto &cards = game.current_player_cards();
    int i = cards.size();
    while (i >= cards.size()) {
        i = distribution(generator);
    }
    game.play(cards[i]);
}

void first_card_strategy(Game &game) {
    game.play(game.current_player_cards().front());
}

void play_non_trump_before_trump(std::ranges::input_range auto &&cards, Game &game) {
    if (cards.empty()) {
        return;
    }

    const auto trump = game.trump();
    auto not_trump = cards | std::ranges::views::filter([trump](auto c) {
        return c.suit != trump;
    });

    if (!not_trump.empty()) {
        game.play(not_trump.front());
    } else {
        game.play(cards.back());
    }
}

void rule_based_strategy(Game &game) {
    // no winner yet, play first highest value card
    if (!game.current_trick_winner()) {
        auto highest = game.current_player_cards();
        std::sort(highest.begin(), highest.end(), ::sort_highest_face);
        game.play(highest.front());
        return;
    }

    const auto trump = game.trump();
    auto lowest = game.current_player_cards();
    std::sort(lowest.begin(), lowest.end(), ::sort_lowest_face);

    // otherwise, let's see if its worth winning, if not play our worst
    if (game.current_trick_score() == 0) {
        play_non_trump_before_trump(lowest, game);
        return;
    }

    // otherwise, find cards that can beat the winner
    const auto winner = game.current_trick_winner().value();
    auto winning_lowest = lowest | std::ranges::views::filter([winner, trump](auto c) {
        return winner.is_worse_than(c, trump);
    });

    // if we have them, and it earns points, do it
    if (!winning_lowest.empty()) {
        play_non_trump_before_trump(winning_lowest, game);
        return;
    }

    // otherwise, just play the lowest card we have
    play_non_trump_before_trump(lowest, game);
}

auto mcts_strategy(Game &game) {
    auto mcts = setup_mcts(game);
    auto action = mcts.calculateAction();
    game.play(action.card());
    return mcts;
}

void human(Game &game) {
    game.print();

    std::cout << "Pick a card index (0,1,2): [";
    const auto &cards = game.current_player_cards();
    for (auto c: cards) {
        std::cout << c.to_string() << ", ";
    }
    std::cout << "]";

    int index = cards.size();
    while (index < 0 || index >= cards.size()) {
        std::cin >> index;
    }
    game.play(cards[index]);
}

double run_simulation(int seed) {
//    std::cout << "Start game: " << seed << std::endl;
    Game game{true, seed};
    while (game.phase() == Phase::PLAY) {
        if (game.current_player() == 0) {
//            random_card_strategy(game);
            rule_based_strategy(game);
//            human(game);
        } else {
//            random_card_strategy(game);
//            rule_based_strategy(game);
            mcts_strategy(game);

//            // debugging
//            auto mcts = mcts_strategy(game);
//            writeDotFile(mcts.getRoot(), "the_graph.dot");
        }
        game.score_and_take();
    }

//    std::cout << "Finished " << seed << ": " << game.to_string() << std::endl;
    int t0 = game.team_score(0);
    int t1 = game.team_score(1);
    if (t1 > t0) {
        return 1.0;
    } else if (t1 == t0) {
        return 0.5;
    } else {
        return 0.0;
    }
}

int main() {
    set_ai_player_index(1);
    std::vector<int> n(100);
    std::iota(n.begin(), n.end(), 1);
    auto wins = std::transform_reduce(
            std::execution::par_unseq,
            n.cbegin(),
            n.cend(),
            0L,
            std::plus{},
            [](auto i) {
                return run_simulation(i);
            });
    std::cout << "Wins: " << (wins / (double) n.size()) << std::endl;
}
