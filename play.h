/**
 *  @file play.h
 *  @author Vincent Li
 *  The executed file for a person-v-person game.
 */

#ifndef PLAY
#define PLAY

#include <utility>

#include "player.h"
#include "playerhuman.h"
#include "playerminimax.h"

class Play {
    public:
        /**
         *  Play a game between any two kinds of players.
         *  Return PLAYER_X_WON, PLAYER_O_WON, or DRAW.
         */
        int play(Player& playerX, Player& playerO);
};

#endif  // PLAY