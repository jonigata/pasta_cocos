// 2016/04/04 Naoyuki Hirayama

/*!
	@file	  TerritoryLayer.h
	@brief	  <ŠT—v>

	<à–¾>
*/

#ifndef TERRITORYLAYER_H_
#define TERRITORYLAYER_H_

#include "cocos2d.h"
#include "pathview.hpp"
#include "team.hpp"

class TerritoryLayer : public cocos2d::Layer {
public:
    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(TerritoryLayer);
    
    void update(float delta) override;

    void setTeam(Team* team, cocos2d::Color4F color);

private:
    cocos2d::DrawNode* draw_;
    Team* team_;
    cocos2d::Color4F color_;
    
};



#endif // TERRITORYLAYER_H_

