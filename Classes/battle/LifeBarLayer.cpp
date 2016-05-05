// 2016/04/04 Naoyuki Hirayama

#include "LifeBarLayer.h"
#include "board.hpp" // STATION_COST

//<<<<<<<<<< LifeBarLayer

//****************************************************************
// init
bool LifeBarLayer::init() {
    if (!Layer::init()) { return false; }

    draw_ = cocos2d::DrawNode::create();
    addChild(draw_);

    return true;
}

void LifeBarLayer::setValue(float normalized) {
    draw_->clear();
    // cocos2d::log("%f", normalized);
    
    cocos2d::Vec2 vertices[] = {
        { -16, -256 },
        {  16, -256 },
        {  16,  256 },
        { -16,  256 },
    };

    cocos2d::Color4F c;
    if (STATION_COST < normalized) {
        c = cocos2d::Color4F(0.1f, 0, 1.0f, 1.0f);
    } else {
        c = cocos2d::Color4F(0.1f, 0, 1.0f, 0.3f);
    }


    vertices[2].y = vertices[3].y = -256.0f + 512.0f * normalized;
    draw_->drawPolygon(vertices, 4, c, 0, cocos2d::Color4F::BLACK);
}

//>>>>>>>>>> LifeBarLayer
