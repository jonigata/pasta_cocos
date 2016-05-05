// 2016/03/20 Naoyuki Hirayama

/*!
	@file	  player.hpp
	@brief	  <ŠT—v>

	<à–¾>
*/

#ifndef PLAYER_HPP_
#define PLAYER_HPP_

class Player {
public:
    Player(Board& board) : board_(board) {}

    void tap(const cocos2d::Vec2& v) {
        auto team = board_.team(TeamTag::Alpha);
        auto enemy = board_.team(TeamTag::Beta);
        if (team->in_teritory(v)) {
            auto target = enemy->find_nearest_station(v);
            // auto target = cocos2d::Vec2(256, -256);
            
            board_.settle_station(TeamTag::Alpha, v, target);
        }
    }

private:

    Board& board_;

};

#endif // PLAYER_HPP_
