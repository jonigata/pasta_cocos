// 2016/04/18 Naoyuki Hirayama

/*!
	@file	  partawn.hpp
	@brief	  <ŠT—v>

	<à–¾>
*/

#ifndef PARTAWN_HPP_
#define PARTAWN_HPP_

enum class PartawnAppearance {
    Standard,
    Station,
    Basecamp,
};


class IConstraint {
public:
    virtual ~IConstraint() {}

    virtual cocos2d::Vec2 apply(const cocos2d::Vec2&) = 0;
};

class IPartawn {
public:
    virtual ~IPartawn() {}

    virtual cocos2d::Vec2 constraint_velocity(const cocos2d::Vec2&) = 0;
    virtual cocos2d::Vec2 move(const cocos2d::Vec2&) = 0;
    virtual cocos2d::Vec2 location() = 0;
    virtual void location(const cocos2d::Vec2&) = 0;
    virtual float life() = 0;
    virtual TeamTag team_tag() = 0;
    virtual void preattack() = 0;
    virtual void attack(float, IPartawn*) = 0;
    virtual void suffer_damage(float) = 0;
    virtual PartawnAppearance appearance() = 0;
    virtual std::weak_ptr<IPartawn> target() = 0;
    virtual void target(std::weak_ptr<IPartawn> target) = 0;
    
    virtual void update(float elapsed) = 0;
};

class TrivialPartawn : public IPartawn {
public:
    TrivialPartawn(TeamTag team_tag) : team_tag_(team_tag) { life_ = 1.0f; }

    void location(const cocos2d::Vec2& v) { location_ = v; }
    cocos2d::Vec2 location() { return location_; }

    void life(float x) { life_ = x; }
    float life() { return life_; }

    TeamTag team_tag() { return team_tag_; }

    void preattack() {}
    void attack(float, IPartawn*) {}
    void suffer_damage(float damage) {
        life_ -= damage;
    }
    std::weak_ptr<IPartawn> target() {
        return std::weak_ptr<IPartawn>();
    }
    void target(std::weak_ptr<IPartawn> target) {
    }

protected:
    TeamTag team_tag_;
    cocos2d::Vec2  location_;
    float   life_;
    
};

class ImmovablePartawn : public TrivialPartawn {
public:
    ImmovablePartawn(TeamTag team_tag, const cocos2d::Vec2& origin)
        : TrivialPartawn(team_tag) {
        location(origin);
    }

    cocos2d::Vec2 constraint_velocity(const cocos2d::Vec2& av) { return cocos2d::Vec2(0, 0); }
    cocos2d::Vec2 move(const cocos2d::Vec2& p) { return cocos2d::Vec2(0, 0); }

};

#endif // PARTAWN_HPP_
