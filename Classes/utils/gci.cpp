// 2016/03/19 Naoyuki Hirayama

#include "gci.hpp"
#include <fstream>
#include <cassert>

namespace gci {

void read_gci(const char* filename, Document& doc, bool yflip) {
    std::ifstream ifs(filename);

    // サイズ
    ifs >> doc.bbminx;
    ifs >> doc.bbminy;
    ifs >> doc.bbmaxx;
    ifs >> doc.bbmaxy;

    // 頂点リスト
    int vertex_count;
    ifs >> vertex_count;
    for (int i = 0 ; i <vertex_count ; i++) {
        int id;
        Vertex v;
        ifs >> id >> v.x >> v.y;
        if (yflip) { v.y = doc.bbmaxy - v.y; }
        doc.vertices.push_back(v);
    }

    // 入力ポリゴン
    int input_polygon_count;
    ifs >> input_polygon_count;
    for (int i = 0 ; i <input_polygon_count ; i++) {
        int id, c;
        ifs >> id >> c;
        std::vector<int> p;
        for (int j = 0 ; j <c ; j++) {
            int index;
            ifs >> index;
            p.push_back(index);
        }
        doc.input_polygons.push_back(p);
    }

    // 入力サイト
    int input_site_count;
    ifs >> input_site_count;
    for (int i = 0 ; i <input_site_count ; i++) {
        int id, type;
        ifs >> id >> type;
        Document::Site s;
        if (type == 1) {
            s.is_segment = false;
            ifs >> s.p0;
        } else {
            assert(type == 2);
            s.is_segment = true;
            ifs >> s.p0 >> s.p1;
        }
        doc.sites.push_back(s);
    }

    // ボロノイセル・三角形分割
    int voronoi_cell_count;
    ifs >> voronoi_cell_count;
    for (int i = 0 ; i <voronoi_cell_count ; i++) {
        Document::Cell cell;	
        int id;
        ifs >> id >> cell.site_index;

        int c;
        ifs >> c;
        for (int j = 0 ; j <c ; j++) {
            int index;
            ifs >> index;
            cell.vertex_indices.push_back(index);
        }

        ifs >> c;
        for (int j = 0 ; j <c ; j++) {
            Document::Triangle t;
            ifs >> t.v0 >> t.v1 >> t.v2;
            cell.triangles.push_back(t);
        }

        doc.voronoi_cells.push_back(cell);
    }

    // dprintf("");
}

void centering(Document& doc) {
    float xoffset = (doc.bbminx + doc.bbmaxx) * -0.5f;
    float yoffset = (doc.bbminy + doc.bbmaxy) * -0.5f;
    
    for (auto& v: doc.vertices) {
        v.x += xoffset;
        v.y += yoffset;
    }

    doc.bbminx += xoffset;
    doc.bbminy += yoffset;
    doc.bbmaxx += xoffset;
    doc.bbmaxy += yoffset;
}

} // namespace gci
