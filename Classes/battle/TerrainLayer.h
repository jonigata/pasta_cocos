// 2016/04/04 Naoyuki Hirayama

/*!
	@file	  TerrainLayer.h
	@brief	  <ŠT—v>

	<à–¾>
*/

#ifndef TERRAINLAYER_H_
#define TERRAINLAYER_H_

#include "cocos2d.h"
#include "../utils/pathview.hpp"

class TerrainLayer : public cocos2d::Layer {
public:
    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(TerrainLayer);
    
    void render(
        const std::vector<pv::Primitive>& pathview,
        const cocos2d::Vec2& offset,
        int step);
    
private:
    cocos2d::DrawNode* draw_;
    
    cocos2d::Vec2 vec2(const float* a) {
        return cocos2d::Vec2(a[0], a[1]);
    }
    
    cocos2d::Vec4 vec4(const cocos2d::Vec2& v) {
        return cocos2d::Vec4(v.x, v.y, 0, 1);
    }

};



#endif // TERRAINLAYER_H_

