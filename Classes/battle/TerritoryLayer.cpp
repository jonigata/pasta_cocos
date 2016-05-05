// 2016/04/04 Naoyuki Hirayama

#include "TerritoryLayer.h"

//<<<<<<<<<< TerritoryLayer

//****************************************************************
// init
bool TerritoryLayer::init() {
    if (!Layer::init()) { return false; }

    draw_ = cocos2d::DrawNode::create();
    addChild(draw_);

    this->scheduleUpdate();

    return true;
}

void TerritoryLayer::setTeam(Team* team, cocos2d::Color4F color) {
    team_ = team;
    color_ = color;
}

void TerritoryLayer::update(float delta) {
    // log("update");

    draw_->clear();

    for (const auto& station: team_->stations()) {
        draw_->drawDot(station->location(), TERRITORY_RANGE, color_);
    }
}

//>>>>>>>>>> TerritoryLayer
