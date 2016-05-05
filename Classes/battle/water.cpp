// 2008/12/25 Naoyuki Hirayama

#include "water.hpp"
//#include "zw/dprintf.hpp"
//#include "performance_counter.hpp"

const float DISPLAY_MAG			   = 1.0f;
const float DOT_SIZE			   = 9.0f;

/*============================================================================
 *
 * class Water 
 *
 * 
 *
 *==========================================================================*/
//<<<<<<<<<< Water

//****************************************************************
// constructor
Water::Water() {
    constraint_ = NULL;

    sph_.initialize(
        SEARCH_RADIUS,
        VISCOSITY,
        DUMPING,
        cocos2d::Vec2(0, GRAVITY),
        IDEAL_DENSITY,
        PRESSURE_BALANCE_COEFFICIENT,
        PRESSURE_REPULSIVE_COEFFICIENT);
}

//****************************************************************
// add
void Water::add(const cocos2d::Vec2& v, float mass, IPartawn* partawn) {
    sph_.add_particle(v, mass, partawn);
}

//****************************************************************
// update
void Water::update() {
    //PerformanceCounter pc(true);
    sph_.update(0.01f);
    //pc.print("update");

    if (constraint_) {
        sph_.constraint(
            [this](const cocos2d::Vec2& v){return constraint_->apply(v);});
        //pc.print("constraint");
    }
 
    sph_.foreach_pair(
        [](IPartawn* car, IPartawn* cdr, float distance) {
            if (car->team_tag() != cdr->team_tag()) {
                car->preattack();
                cdr->preattack();
            }
        });

    sph_.foreach_pair(
        [](IPartawn* car, IPartawn* cdr, float distance) {
            if (car->team_tag() != cdr->team_tag()) {
                car->attack(0.01f, cdr);
                cdr->attack(0.01f, car);
            }
        });

    sph_.discard(
        [](IPartawn* load) {
            return load->life() < 0.0f;
        });
}

//****************************************************************
// set_viscosity
void Water::set_viscosity(float v) {
    sph_.set_viscosity(v);
}

//****************************************************************
// get_viscosity
float Water::get_viscosity() {
    return sph_.get_viscosity();
}

//****************************************************************
// set_dumping
void Water::set_dumping(float d) {
    if (1.0f <d) { d = 1.0f; }
    sph_.set_dumping(d);
}

//****************************************************************
// get_dumping
float Water::get_dumping() {
    return sph_.get_dumping();
}

//****************************************************************
// set_ideal_density
void Water::set_ideal_density(float x) {
    sph_.set_ideal_density(x);
}

//****************************************************************
// get_ideal_density
float Water::get_ideal_density() {
    return sph_.get_ideal_density();
}

//****************************************************************
// set_constraint
void Water::set_constraint(IConstraint* constraint) {
    constraint_ = constraint;
}

//>>>>>>>>>> Water

