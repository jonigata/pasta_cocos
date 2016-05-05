// 2016/04/04 Naoyuki Hirayama

/*!
	@file	  WaterLayer.h
	@brief	  <ŠT—v>

	<à–¾>
*/

#ifndef WATERLAYER_H_
#define WATERLAYER_H_

#include "cocos2d.h"
#include "../utils/pathview.hpp"
#include "water.hpp"

class WaterLayer : public cocos2d::Layer {
public:
    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(WaterLayer);
    
    Water* water;

    void update(float delta);

private:
    cocos2d::DrawNode* draw_;

};



#endif // WATERLAYER_H_

