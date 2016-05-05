// 2016/03/24 Naoyuki Hirayama

/*!
	@file	  gci2pathview.hpp
	@brief	  <ŠT—v>

	<à–¾>
*/

#ifndef GCI2PATHVIEW_HPP_
#define GCI2PATHVIEW_HPP_

#include "gci.hpp"
#include "pathview.hpp"
#include "trapezoidal_map.hpp"

struct SegmentProperty {
    int upper_cell_index;
    int lower_cell_index;
};

void compile_terrain(
    const gci::Document& src, 
    std::vector<pv::Primitive>& view,
    TrapezoidalMap<float, SegmentProperty>& tm);


#endif // GCI2PATHVIEW_HPP_
