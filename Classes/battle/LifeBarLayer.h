// 2016/04/04 Naoyuki Hirayama

/*!
	@file	  LifeBarLayer.h
	@brief	  <ŠT—v>

	<à–¾>
*/

#ifndef LIFEBARLAYER_H_
#define LIFEBARLAYER_H_

#include "cocos2d.h"
#include "../utils/pathview.hpp"

class LifeBarLayer : public cocos2d::Layer {
public:
    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(LifeBarLayer);
    
    void setValue(float normalized);

private:
    cocos2d::DrawNode* draw_;
    
};



#endif // LIFEBARLAYER_H_

