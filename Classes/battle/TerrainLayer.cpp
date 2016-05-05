// 2016/04/04 Naoyuki Hirayama

#include "TerrainLayer.h"

//<<<<<<<<<< TerrainLayer

//****************************************************************
// init
bool TerrainLayer::init() {
    if (!Layer::init()) { return false; }

    draw_ = nullptr;
    return true;
}

//****************************************************************
// render
void TerrainLayer::render(
    const std::vector<pv::Primitive>& primitives,
    const cocos2d::Vec2& offset,
    int step) {

    draw_ = cocos2d::DrawNode::create();
    addChild(draw_);
    scheduleUpdate();
    cocos2d::log("init");

    cocos2d::Vec2 cursor(0, 0); 
    cocos2d::Color4F color(0, 0, 0, 0.25f);

    cocos2d::Vec2 o = offset;

    for (int i = 0 ; i < int(primitives.size()) && i < step ; i++) {
        const pv::Primitive& p = primitives[i];
        const float* a = p.operands;

        switch (p.opcode) {
            case pv::Primitive::Color:
                color = cocos2d::Color4F(a[0], a[1], a[2], 0.25f);
                break;
            case pv::Primitive::MoveTo:
                cursor = vec2(a) + o;
                break;
            case pv::Primitive::LineTo:
                draw_->drawSegment(cursor, vec2(a) + o, 1.0f, color);
                cursor = vec2(a) + o;
                break;
            case pv::Primitive::Triangle:
                draw_->drawTriangle(
                    cursor, vec2(a + 0) + o, vec2(a + 2) + o, color);
                break;
            case pv::Primitive::Dot: {
                draw_->drawDot(cursor, a[0], color);
                break;
            }
            default:
                break;
        }
    }
}

//>>>>>>>>>> TerrainLayer
