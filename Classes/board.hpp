//2016/03/20 Naoyuki Hirayama

/*!
	@file	  board.hpp
	@brief	  <äTóv>

	<ê‡ñæ>
*/

#ifndef BOARD_HPP_
#define BOARD_HPP_

#include "trapezoidal_map.hpp"
#include "gci.hpp"
#include "pathview.hpp"
#include "gci2pathview.hpp"
#include "color.hpp"
#include "castle.hpp"
#include "standard_partawn.hpp"
#include "station.hpp"
#include "basecamp.hpp"
#include "team.hpp"
#include "water.hpp"
#include <memory>

const int HCOUNT				   = 10;
const int VCOUNT				   = 10;

const float STATION_COST = 0.2f;

class Board {
public:
    Board()
        : tm_(0, 0, 512, 512), constraint_(terrain_, tmm_), ready_(false) {
    }

    void setup() {
        read_gci("data/cave.gci", terrain_, true);
        centering(terrain_);
        compile_terrain(terrain_, pathview_, tm_);

        water_.set_constraint(&constraint_);

        tmm_.init(tm_);

        // mockup();

        teams_.push_back(build_team(TeamTag::Alpha, cocos2d::Vec2(-160, -160)));
        teams_.push_back(build_team(TeamTag::Beta, cocos2d::Vec2(160, 160)));
/*
        teams_.push_back(build_team(TeamTag::Alpha, cocos2d::Vec2(-255, -255)));
        teams_.push_back(build_team(TeamTag::Beta, cocos2d::Vec2(-255, 255)));
*/
        ready_ = true;
    }

    void update(float elapsed) {
        for (const auto& team: teams_) {
            team->update(elapsed);
            for (const auto& station: team->stations()) {
                auto target = station->target().lock();
                if (!target) {
                    auto enemy = get_enemy_team(station->team_tag());
                    auto target = 
                        enemy->find_nearest_station(station->location());
                    station->target(target);
                }
            }
        }

        water_.update();

        for (const auto& team: teams_) {
            team->cleanup();
        }

        Castle::EmitEntry e;
        while (castle_.fetch(e)) {
            settle_partawn(e.team_tag, e.origin, e.target);
        }
    }

    bool ready() { return ready_; }

    const gci::Document& terrain() { return terrain_; }
    const std::vector<pv::Primitive>& pathview() { return pathview_; }

    Water& water() { return water_; }

public: 
    // player operation
    std::shared_ptr<Team>
    build_team(TeamTag team_tag, const cocos2d::Vec2& v) {
        // ãíì_
        auto basecamp = std::make_shared<Basecamp>(team_tag, castle_, v);
        water_.add(v, MASS, basecamp.get());

        auto team = std::make_shared<Team>(castle_, team_tag, basecamp);
        return team;
    }

    std::shared_ptr<Team>
    team(TeamTag tag) { return teams_[(int)tag]; }        

    void settle_station(
        TeamTag team_tag, 
        const cocos2d::Vec2& origin, 
        std::shared_ptr<IPartawn> target) {

        auto t = team(team_tag);
        if (STATION_COST <= t->energy()) {
            if (constraint_.apply(origin) == origin) {
                t->energy(t->energy() - 0.2f);
                auto p = t->settle_station(origin, target);
                water_.add(origin, MASS, p.get());
            }
        }
    }

    void settle_partawn(
        TeamTag team_tag, const cocos2d::Vec2& origin, const cocos2d::Vec2& target) {
        auto p = team(team_tag)->settle_partawn(origin, target);
        water_.add(origin, MASS, p.get());
    }

    std::shared_ptr<Team> get_friend_team(TeamTag team_tag) {
        std::shared_ptr<Team> p;
        switch(team_tag) {
            case TeamTag::Alpha:
                p = teams_[0];
            case TeamTag::Beta:
                p = teams_[1];
        }
        return p;
    }

    std::shared_ptr<Team> get_enemy_team(TeamTag team_tag) {
        std::shared_ptr<Team> p;
        switch(team_tag) {
            case TeamTag::Alpha:
                p = get_friend_team(TeamTag::Beta);
            case TeamTag::Beta:
                p = get_friend_team(TeamTag::Alpha);
        }
        return p;
    }

private:
    class TrapezoidalMapConstraint : public IConstraint {
    public:
        TrapezoidalMapConstraint(
            gci::Document& doc,
            TrapezoidalMapMachine<float, SegmentProperty>& tmm)
            : document_(doc), tmm_(tmm) {}

        cocos2d::Vec2 apply(const cocos2d::Vec2& vv) {
            cocos2d::Vec2 v = vv;

            float minx = document_.bbminx + 1.0f;
            float miny = document_.bbminy + 1.0f;
            float maxx = document_.bbmaxx - 1.0f;
            float maxy = document_.bbmaxy - 1.0f;

            if (v.x <minx) { v.x = minx; }
            if (v.y <miny) { v.y = miny; }
            if (maxx <= v.x) { v.x = maxx; }
            if (maxy <= v.y) { v.y = maxy; }

            TrapezoidalMap<float, SegmentProperty>::Point q;
            int score;
            cocos2d::Vec2 qv(v.x, v.y);
            q.x(qv.x);
            q.y(qv.y);

            SegmentProperty tsp;
            SegmentProperty bsp;

            if (tmm_.find(q, score, tsp, bsp)) {
                int index = tsp.upper_cell_index; // yflipÇµÇƒÇ»Ç¢èÍçáÇÕlower
                if (0 <= index) {
                    const gci::Document::Cell& cell =
                        document_.voronoi_cells[index];

                    const gci::Document::Site& site =
                        document_.sites[cell.site_index];

                    cocos2d::Vec2 qv2;
                    if (site.is_segment) {
                        qv2 = nearest_point_on_line(
                            vertex_to_vec(document_.vertices[site.p0]),
                            vertex_to_vec(document_.vertices[site.p1]),
                            qv);
                    } else {
                        qv2 = vertex_to_vec(document_.vertices[site.p0]);
                    }

                    v =(qv2 - qv)* 0.5f + qv;
                }
            }

            return v;
        }

    private:
        cocos2d::Vec2 vertex_to_vec(const gci::Vertex& p) {
            return cocos2d::Vec2(p.x, p.y);
        }

        cocos2d::Vec2 nearest_point_on_line(
            const cocos2d::Vec2& p0,
            const cocos2d::Vec2& p1,
            const cocos2d::Vec2& q) {
            float dx = p1.x - p0.x;
            float dy = p1.y - p0.y;
            float a = dx * dx + dy * dy;
            if (a == 0) { return p0; }

            float b = dx *(p0.x - q.x)+ dy *(p0.y - q.y);
            float t = -(b / a);
            if (t <0.0f) { t = 0.0f; }
            if (1.0f <t) { t = 1.0f; }
            return cocos2d::Vec2(p0.x + dx * t, p0.y + dy * t);
        }

    private:
        gci::Document&                                      document_;
        TrapezoidalMapMachine<float, SegmentProperty>&      tmm_;
    };

    gci::Document terrain_;
    std::vector<pv::Primitive>  pathview_; 
    TrapezoidalMap<float, SegmentProperty> tm_;
    TrapezoidalMapMachine<float, SegmentProperty> tmm_;
    TrapezoidalMapConstraint constraint_;
    Castle      castle_;
    Water       water_;

    bool ready_;

private:
    void mockup() {
    }

private:
    std::vector<std::shared_ptr<Team>> teams_;
    
};

#endif // BOARD_HPP_
