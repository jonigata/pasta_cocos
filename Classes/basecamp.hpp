// 2016/03/20 Naoyuki Hirayama

/*!
	@file	  basecamp.hpp
	@brief	  <ŠT—v>

	<à–¾>
*/

#ifndef BASECAMP_HPP_
#define BASECAMP_HPP_

#include "castle.hpp"

class Basecamp : public ImmovablePartawn {
public:
    Basecamp(TeamTag team_tag, Castle& castle, const cocos2d::Vec2& origin)
        : ImmovablePartawn(team_tag, origin), castle_(castle) {
    }

    PartawnAppearance appearance() {
        return PartawnAppearance::Basecamp;
    }

    void update(float elapsed) {}

private:
    Castle& castle_;

};
#endif // BASECAMP_HPP_
