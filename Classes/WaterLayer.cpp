// 2016/04/04 Naoyuki Hirayama

#include "WaterLayer.h"

namespace {

class Renderer {
public:

public:
    Renderer(cocos2d::DrawNode* draw) : draw_(draw) {
    }
    cocos2d::DrawNode* draw_;
    
    void operator()(
        int     id,
        const cocos2d::Vec2& pos,
        float    mass,
        float    density_plain,
        float    density_balance_corrected,
        float    density_repulsive_corrected,
        float    boundariness,
        const WaterTraits::load_type& load) {

        cocos2d::Color4F c = cocos2d::Color4F::BLACK;
        switch (load->team_tag()) {
            case TeamTag::Alpha:
                c = cocos2d::Color4F(0.1f, 0, 1.0f, load->life());
                break;
            case TeamTag::Beta:
                c = cocos2d::Color4F(0.8f, 0, 1.0f, load->life());
                break;
        }

        float r = 5.0f;
        switch (load->appearance()) {
            case PartawnAppearance::Standard: r = 3.0f; break;
            case PartawnAppearance::Station: r = 7.0f; break;
            case PartawnAppearance::Basecamp: r = 10.0f; break;
        }

        draw_->drawDot(pos, r, c);
    }

    void render_frame() {
    }

};

struct RendererWrapper {
    RendererWrapper(Renderer& ar) : r(ar) {}
    Renderer& r;

    void operator()(
        int     id,
        const cocos2d::Vec2& pos,
        float    mass,
        float    density_plain,
        float    density_balance_corrected,
        float    density_repulsive_corrected,
        float    boundariness,
        const WaterTraits::load_type& load) {
        r(
            id,
            pos,
            mass,
            density_plain,
            density_balance_corrected,
            density_repulsive_corrected,
            boundariness,
            load);
    }
};

}

//<<<<<<<<<< WaterLayer

//****************************************************************
// init
bool WaterLayer::init() {
    if (!Layer::init()) { return false; }

    draw_ = cocos2d::DrawNode::create();
    addChild(draw_);
    scheduleUpdate();

    return true;
}

void WaterLayer::update(float delta) {
    draw_->clear();
    Renderer r(draw_);
    water->sph().foreach(RendererWrapper(r));
}

//>>>>>>>>>> WaterLayer
