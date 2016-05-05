// 2016/03/20 Naoyuki Hirayama

/*!
	@file	  Partawn.hpp
	@brief	  <ŠT—v>

	<à–¾>
*/

#ifndef STANDARD_PARTAWN_HPP_
#define STANDARD_PARTAWN_HPP_

#include "partawn.hpp"

class StandardPartawn : public TrivialPartawn {
public:
    StandardPartawn(TeamTag team_tag, const cocos2d::Vec2& target, float speed)
        : TrivialPartawn(team_tag){
        target_ = target;
        speed_ = speed;
        attack_count_ = 0;
        
    }

    cocos2d::Vec2 constraint_velocity(const cocos2d::Vec2& av) {
        cocos2d::Vec2 v = av;

        float maxSpeed = 100.0f;
        float l = v.lengthSquared();
        if (maxSpeed * maxSpeed < l) {
            v *= (1.0f / sqrt(l)); 
        }

        return v;
    }

    cocos2d::Vec2 move(const cocos2d::Vec2& p) {
        if (0 < attack_count_) { attack_count_ = 0; return cocos2d::Vec2(0,0); }
        cocos2d::Vec2 d = target_ - p;
        return d * (speed_ / d.length());
    }

    void update(float elapsed) {
        life_ -= 0.1f * elapsed;
    }

    void preattack() {
        attack_count_++;
    }

    void attack(float elapsed, IPartawn* target) {
        target->suffer_damage(elapsed * 1.0f / attack_count_);
        // cocos2d::log("attack: %f, %d", elapsed, attack_count_);
    }

    PartawnAppearance appearance() {
        return PartawnAppearance::Standard;
    }

private:
    cocos2d::Vec2  target_;
    float   speed_;
    int     attack_count_;

};

#endif // STANDARD_PARTAWN_HPP_
