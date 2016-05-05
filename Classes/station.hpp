// 2016/03/20 Naoyuki Hirayama

/*!
	@file	  station.hpp
	@brief	  <ŠT—v>

	<à–¾>
*/

#ifndef STATION_HPP_
#define STATION_HPP_

#include "castle.hpp"
#include "team_tag.hpp"

const float STATION_LIFETIME = 50.0f;

class Station : public ImmovablePartawn {
public:
    Station(
        TeamTag team_tag,
        Castle& castle, 
        const cocos2d::Vec2& origin, 
        std::shared_ptr<IPartawn> target,
        float speed)
        : ImmovablePartawn(team_tag, origin),
          castle_(castle), 
          target_(target)  {
        speed_ = speed;
        timer_ = 1.0f;
    }

    PartawnAppearance appearance() {
        return PartawnAppearance::Station;
    }

    std::weak_ptr<IPartawn> target() {
        return target_;
    }

    void target(std::weak_ptr<IPartawn> target) {
        target_ = target;
    }

    void update(float elapsed) {
        timer_ -= elapsed;
        if (timer_ <= 0) {
            timer_ = 1.0f;
            auto target = target_.lock();
            if (target) {
                castle_.emit(
                    Castle::EmitEntry {
                        team_tag_, location(), target->location() });
            }
        }
        life_ -= (1.0f / STATION_LIFETIME) * elapsed;
    }

private:
    Castle& castle_;
    std::weak_ptr<IPartawn> target_;
    float   speed_;
    float   timer_;

};

#endif // STATION_HPP_
