// 2016/03/24 Naoyuki Hirayama

#include "gci2pathview.hpp"
#include "color.hpp"
#include <map>
#include <random>

namespace {

void post_random_color(std::vector<pv::Primitive>& pb, int& color_index) {
    unsigned long color;
    do {
        color = get_color(color_index++);
    } while (get_color_distance(0xffffff, color)<0.1f);

    float r =((color & 0xff0000) >> 16)/ 255.0f;
    float g =((color & 0x00ff00) >>  8)/ 255.0f;
    float b =((color & 0x0000ff))/ 255.0f;

    pv::Primitive p;
    p.opcode = pv::Primitive::Color;
    p.operands[0] = r;
    p.operands[1] = g;
    p.operands[2] = b;
    pb.push_back(p);
}

}

void compile_terrain(
    const gci::Document& src, 
    std::vector<pv::Primitive>& pathview,
    TrapezoidalMap<float, SegmentProperty>& trapezoidal_map) {

    // LINE
    pv::Primitive p;
    p.opcode = pv::Primitive::Color;
    p.operands[0] = 0;
    p.operands[1] = 0;
    p.operands[2] = 0;
    pathview.push_back(p);

    for (size_t i = 0 ; i < src.voronoi_cells.size() ; i++) {
        const std::vector<int>& v = src.voronoi_cells[i].vertex_indices;
        pv::Primitive p;
        p.opcode = pv::Primitive::MoveTo;
        p.operands[0] = src.vertices[v.back()].x;
        p.operands[1] = src.vertices[v.back()].y;
        pathview.push_back(p);
        for (size_t j = 0 ; j <v.size(); j++) {
            p.opcode = pv::Primitive::LineTo;
            p.operands[0] = src.vertices[v[j]].x;
            p.operands[1] = src.vertices[v[j]].y;
            pathview.push_back(p);
        }
    }

    // POLYGON
    int color_index = 0;
    for (size_t i = 0 ; i < src.voronoi_cells.size(); i++) {
        post_random_color(pathview, color_index);

        const std::vector<gci::Document::Triangle>& v =
            src.voronoi_cells[i].triangles;

        for (size_t j = 0 ; j <v.size(); j++) {
            p.opcode = pv::Primitive::MoveTo;
            p.operands[0] = src.vertices[v[j].v0].x;
            p.operands[1] = src.vertices[v[j].v0].y;
            pathview.push_back(p);

            p.opcode = pv::Primitive::Triangle;
            p.operands[0] = src.vertices[v[j].v1].x;
            p.operands[1] = src.vertices[v[j].v1].y;
            p.operands[2] = src.vertices[v[j].v2].x;
            p.operands[3] = src.vertices[v[j].v2].y;
            pathview.push_back(p);
        }
    }

    // point location
    std::vector<TrapezoidalMap<float, SegmentProperty>::Point> points;
    for (size_t i = 0 ; i < src.vertices.size() ; i++) {
        points.push_back(
            trapezoidal_map.make_point(
                src.vertices[i].x,
                src.vertices[i].y));
    }

    int m = 0;
    std::map<std::pair<int, int>, SegmentProperty> segments;
    for (size_t i = 0 ; i <src.voronoi_cells.size() ; i++) {
        const std::vector<int>& v = src.voronoi_cells[i].vertex_indices;

        for (size_t j = 0 ; j <v.size(); j++) {
            int index0 = v[j];
            int index1 = v[(j+1)% v.size()];

            // lexicographical compare
            bool invert = !(points[index0] <points[index1]);

            if (index1 <index0) { std::swap(index0, index1); }
            std::pair<int, int> s = std::make_pair(index0, index1);

            if (segments.find(s) == segments.end()) {
                SegmentProperty sp;
                sp.upper_cell_index = -1;
                sp.lower_cell_index = -1;
                if (invert) {
                    sp.upper_cell_index = int(i);
                } else {
                    sp.lower_cell_index = int(i);
                }
                segments[s] = sp;
                m++;
                //trapezoidal_map.add_segment(points[index0], points[index1]);
            } else {
                SegmentProperty& sp = segments[s];
                if (invert) {
                    sp.upper_cell_index = int(i);
                } else {
                    sp.lower_cell_index = int(i);
                }
            }
        }
    }

    struct Seg {
        int    index0;
        int    index1;
        SegmentProperty sp;
    };

    std::vector<Seg> segments2;
    for (const auto& p: segments){
        Seg seg;
        seg.index0 = p.first.first;
        seg.index1 = p.first.second;
        seg.sp = p.second;
        segments2.push_back(seg);
    }

    // ƒVƒƒƒbƒtƒ‹
    std::shuffle(segments2.begin(), segments2.end(), std::mt19937());

    for (std::vector<Seg>::const_iterator i = segments2.begin();
         i != segments2.end();
         ++i) {
        trapezoidal_map.add_segment(
            points[(*i).index0],
            points[(*i).index1],
            true,
            (*i).sp);
    }
}
