#include "Day22.h"
#include <unordered_set>

Game Day22::parseInput(std::string &input) {
    const std::string p1("Player 1:");
    const std::string p2("Player 2:");

    size_t p1Idx = input.find(p1) + p1.size();
    size_t p2Idx = input.find(p2) + p2.size();

    std::stringstream streamP1(input.substr(p1Idx));
    std::stringstream streamP2(input.substr(p2Idx));

    Game game;
    int card;
    while (streamP1 && streamP1 >> card)
        game.player1.push_back(card);

    while (streamP2 && streamP2 >> card)
        game.player2.push_back(card);

    return game;
}

void playGame(Game &game) {
    deck_t &p1 = game.player1;
    deck_t &p2 = game.player2;

    while (!p1.empty() && !p2.empty()) {
        card_t c1 = p1.front(); p1.pop_front();
        card_t c2 = p2.front(); p2.pop_front();

        if (c1 < c2) {
            // p2 win
            p2.push_back(c2);
            p2.push_back(c1);
        } else {
            // p1 win
            p1.push_back(c1);
            p1.push_back(c2);
        }
    }
}

unsigned long long scoreDeck(deck_t &deck) {
    unsigned long long score = 0;
    for (size_t i = 0; i < deck.size(); i++) {
        score += (deck.size() - i) * deck[i];
    }

    return score;
}

std::string Day22::runPart1(Game &input) {
    std::stringstream output;

    playGame(input);

    output << std::max(scoreDeck(input.player1), scoreDeck(input.player2));
    return output.str();
}

Winner playRecursiveGame(Game& game) {
    // check if game was played before
    std::unordered_set<Game> knownGames;

    deck_t &p1 = game.player1;
    deck_t &p2 = game.player2;

    while (!p1.empty() && !p2.empty()) {
        // check if round is auto win
        if (knownGames.find(game) != knownGames.end())
            return Winner::PLAYER_1;

        // insert game as known state
        knownGames.insert(game);

        // draw cards
        card_t c1 = p1.front(); p1.pop_front();
        card_t c2 = p2.front(); p2.pop_front();

        // determine winner
        Winner roundWinner;
        if (p1.size() >= c1 && p2.size() >= c2) {
            Game subGame { deck_t(p1.begin(), p1.begin() + c1), deck_t(p2.begin(), p2.begin() + c2)};
            roundWinner = playRecursiveGame(subGame);
        } else {
            roundWinner = (c1 < c2) ? Winner::PLAYER_2 : Winner::PLAYER_1;
        }

        // modify deck
        switch (roundWinner) {
            case PLAYER_1:
                p1.push_back(c1);
                p1.push_back(c2);
                break;
            case PLAYER_2:
                p2.push_back(c2);
                p2.push_back(c1);
                break;
        }
    }

    return p1.empty() ? Winner::PLAYER_2 : Winner::PLAYER_1;
}

std::string Day22::runPart2(Game &input) {
    std::stringstream output;
    playRecursiveGame(input);

    output << std::max(scoreDeck(input.player1), scoreDeck(input.player2));
    return output.str();
}
