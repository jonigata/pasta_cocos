// 2008/10/27 Naoyuki Hirayama

/*!
	@file	  water.hpp
	@brief	  <ŠT—v>

	<à–¾>
*/

#ifndef WATER_HPP_
#define WATER_HPP_

#include "cocos2d.h"
#include "sph.hpp"
//#include "vector.hpp"
#include "team_tag.hpp"
#include "partawn.hpp"

const float INITIAL_DISTANCE	   = 10.0f;	// 1cm(10mm)ŠÔŠu‚ÌŠiŽq‚ðì‚é
const float DT                     = 0.01f;	// 100ƒtƒŒ[ƒ€/s
const float SEARCH_RADIUS          = INITIAL_DISTANCE;
const float VISCOSITY              = 1.0f;
const float DUMPING                = 0.99f;
const float GRAVITY                = 0.0f;	// mm/(s*s)
const float MASS                   = 1.0f;
const float IDEAL_DENSITY          = 7.0f; // 10.0f;
const float PRESSURE_BALANCE_COEFFICIENT   = 0.0f;
const float PRESSURE_REPULSIVE_COEFFICIENT = 5.0f;

struct WaterTraits {
    typedef float  real_type;
    typedef cocos2d::Vec2 vector_type;
    typedef IPartawn* load_type;
    enum { DIMENSION = 2 };

    static real_type epsilon() {
        return 1.0e-6f;
    }
    static vector_type zero_vector() {
        return vector_type(0.0f, 0.0f);
    }
    static vector_type unit_vector() {
        return vector_type(1.0f, 1.0f);
    }
    static real_type length(const vector_type& v) {
        return v.length();
    }
    static real_type length_sq(const vector_type& v) {
        return v.lengthSquared();
    }

    static int coord(real_type n) {
        return int(floor(n));
    }
    static void make_coords(int a[2], const vector_type& v) {
        a[0] = coord(v.x);
        a[1] = coord(v.y);
    }
    static void make_vector(vector_type& v, const real_type a[2]) {
        v.x = a[0];
        v.y = a[1];
    }
    static int hash(const int a[2], int table_size) {
        // large prime numbers
        const int p1 = 73856093;
        const int p2 = 19349663;

        return size_t((a[0] * p1)^(a[1] * p2))% table_size;
    }
    static vector_type constraint_velocity(
        const load_type& load, const vector_type& v) {
        if (load == nullptr) { return v; }
        return load->constraint_velocity(v);
    }
    static vector_type move(const load_type& load, const vector_type& p) {
        if (load == nullptr) { return p; }
        return load->move(p);
    }

};


class Water {
public:
    Water();
    ~Water() {}

    void add(const cocos2d::Vec2& v, float mass, IPartawn* partawn);

    void click(cocos2d::Vec2& p) {}
    void update();

    void  set_viscosity(float);
    float  get_viscosity();
    void  set_dumping(float);
    float  get_dumping();
    void  set_ideal_density(float);
    float  get_ideal_density();

    void  set_constraint(IConstraint* constraint);

    sph::sph<WaterTraits>& sph() { return sph_; }

private:
    sph::sph<WaterTraits> sph_;
    IConstraint*     constraint_;

};

#endif // WATER_HPP_
