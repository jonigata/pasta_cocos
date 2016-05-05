// 2016/03/21 Naoyuki Hirayama

/*!
	@file	  team.hpp
	@brief	  <�T�v>

	<����>
*/

#ifndef TEAM_HPP_
#define TEAM_HPP_

#include "team_tag.hpp"
#include "partawn.hpp"
#include "standard_partawn.hpp"
#include "station.hpp"
#include "castle.hpp"
#include "basecamp.hpp"

const float TERRITORY_RANGE = 80.0f;

class Team {
public:
    Team(Castle& castle, TeamTag team_tag, std::shared_ptr<Basecamp> basecamp)
        : castle_(castle) {
        team_tag_ = team_tag;
        partawns_.push_back(basecamp);
        stations_.push_back(basecamp);

        energy_ = 0.5f;
    }

    void update(float elapsed) {
        for (auto& p: partawns_) {
            p->update(elapsed);
        }
        
        energy_ += 0.025f * elapsed;
        energy_ = (std::min)(energy_, 1.0f);
    }

    void cleanup() {
        partawns_.erase(
            std::remove_if(
                partawns_.begin(),
                partawns_.end(),
                [](auto p) {
                    return p->life() < 0.0f;
                }),
            partawns_.end());

        stations_.erase(
            std::remove_if(
                stations_.begin(),
                stations_.end(),
                [](auto p) { return p->life() < 0.0f; }),
            stations_.end());

    }

    const std::vector<std::shared_ptr<IPartawn>>& stations() { 
        return stations_;
    }

    std::shared_ptr<IPartawn>
    settle_station(
        const cocos2d::Vec2& origin, 
        std::shared_ptr<IPartawn> target) {
        auto p = std::make_shared<Station>(
            team_tag_, castle_, origin, target, 25.0f);
        partawns_.push_back(p);
        stations_.push_back(p);
        return p;
    }

    std::shared_ptr<IPartawn>
    settle_partawn(const cocos2d::Vec2& origin, const cocos2d::Vec2& target) {
        auto p = std::make_shared<StandardPartawn>(team_tag_, target, 25.0f);
        partawns_.push_back(p);
        return p;
    }

    bool in_teritory(const cocos2d::Vec2& v) {
        for (auto& s: stations_) {
            cocos2d::Vec2 d = s->location() - v;
            if (d.length() < TERRITORY_RANGE) {
                return true;
            }
        }
        return false;
    }

    float energy() {return energy_;}
    void energy(float x) { energy_ = x; }

    std::shared_ptr<IPartawn>
    find_nearest_station(const cocos2d::Vec2 &v) {
        if (stations_.empty()) {
            return std::shared_ptr<IPartawn>();
        }
        return *std::min_element(
            stations_.begin(),
            stations_.end(),
            [=](std::shared_ptr<IPartawn> car, std::shared_ptr<IPartawn> cdr) {
                auto d0 = car->location() - v;
                auto d1 = cdr->location() - v;
                return d0.length() < d1.length();
            });
    }

private:
    Castle& castle_;
    TeamTag team_tag_;
    std::vector<std::shared_ptr<IPartawn>> partawns_;
    std::vector<std::shared_ptr<IPartawn>> stations_;

    float energy_;
    
};


#endif // TEAM_HPP_
