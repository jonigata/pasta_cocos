// 2016/03/19 Naoyuki Hirayama

/*!
	@file	  gci.hpp
	@brief	  <�T�v>

	<����>
*/

#ifndef GCI_HPP_
#define GCI_HPP_

#include <vector>

namespace gci {

struct Vertex {
    float x;
    float y;
};

struct Document {
    struct Site {
        bool is_segment;
        int  p0;
        int  p1;
    };
    struct Triangle {
        int v0;
        int v1;
        int v2;
    };
    struct Cell {
        int                     site_index;
        std::vector<int>        vertex_indices;
        std::vector<Triangle>   triangles;
    };

    float                           bbminx;
    float                           bbminy;
    float                           bbmaxx;
    float                           bbmaxy;
    std::vector<Vertex>             vertices;
    std::vector<Site>               sites;
    std::vector<std::vector<int>>   input_polygons;
    std::vector<Cell>               voronoi_cells;
};

void read_gci(
    const char* filename, 
    Document& doc, 
    bool yflip);

void centering(Document& doc);

}

#endif // GCI_HPP_
