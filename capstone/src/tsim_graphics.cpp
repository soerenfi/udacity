#include <iostream>

#include "tsim_graphics.hpp"
#include "tsim_simulator.hpp"

MapsRenderer::MapsRenderer(tsim::Simulator* sim)
    : surface_{1280, 1280, io2d::format::argb32, io2d::scaling::none, io2d::refresh_style::fixed, 30}, simulator_(sim) {
    // surface_ =
    // io2d::output_surface{1280, 1280, io2d::format::argb32, io2d::scaling::none, io2d::refresh_style::fixed, 30};
    map_ = simulator_->map();
    findMaxMinValues();
    scale_ = std::max((max_y - min_y), (max_x - min_x)) * 1.1f;
}
void MapsRenderer::display() {
    surface_.size_change_callback(
        [&](io2d::output_surface& surface) { surface_.dimensions(surface_.display_dimensions()); });
    surface_.draw_callback([&](io2d::output_surface& surface) { render(); });
    surface_.begin_show();
}
void MapsRenderer::render() {
    auto px = static_cast<float>(std::min(surface_.dimensions().x(), surface_.dimensions().y()));
    matrix_ = io2d::matrix_2d::create_scale({px / scale_, -px / scale_}) *
              io2d::matrix_2d::create_translate({-min_x / 2 + scale_ * 0.1f, scale_ * 0.2f});

    surface_.paint(m_BackgroundFillBrush);

    objects_ = simulator_->objects();

    drawRoads();
    drawLanes();
    drawVehicles();
    drawAxes();

    // DrawGrid(surface_);
    // DrawPath(surface_);
}

void MapsRenderer::findMaxMinValues() {
    for (const auto& road : map_->roads()) {
        for (const auto& pt : road.points()) {
            if (pt.x() > max_x) max_x = pt.x();
            if (pt.x() < min_x) min_x = pt.x();
            if (pt.y() > max_y) max_y = pt.y();
            if (pt.y() < min_y) min_y = pt.y();  // use std::sort?
        }
    }
};

void MapsRenderer::drawPath() {
    io2d::render_props aliased{io2d::antialias::none};
    io2d::brush foreBrush{io2d::rgba_color::orange};
    float width = 1.0f;

    auto pb = io2d::path_builder{};
    pb.rel_matrix(matrix_);
    // pb.new_figure(io2d::point_2d(float(dimensions.x() / 2), float(dimensions.y() / 2)));
    pb.new_figure(io2d::point_2d(0.0f, 0.0f));

    pb.line(io2d::point_2d(10.f, 0.f));
    pb.line(io2d::point_2d(100.f, -50.f));

    surface_.stroke(foreBrush, io2d::interpreted_path{pb}, std::nullopt, io2d::stroke_props{width});
}

void MapsRenderer::drawAxes() {
    io2d::brush xaxis{io2d::rgba_color::red};
    float width = 1.0f;
    auto sp = io2d::stroke_props{width, io2d::line_cap::square};

    auto pb = io2d::path_builder{};
    pb.rel_matrix(matrix_);
    pb.new_figure(io2d::point_2d(0.0f, 0.0f));

    pb.rel_line({0.f, 0.f});
    pb.rel_line({10.f, 0.f});
    pb.close_figure();
    surface_.stroke(xaxis, pb, std::nullopt, sp);

    auto pb2 = io2d::path_builder{};
    pb2.rel_matrix(matrix_);

    io2d::brush yaxis{io2d::rgba_color::blue};
    pb2.new_figure(io2d::point_2d(0.0f, 0.0f));
    pb2.rel_line({0.f, 0.f});
    pb2.rel_line({0.f, 10.f});
    pb2.close_figure();
    surface_.stroke(yaxis, pb2, std::nullopt, sp);
}

void MapsRenderer::drawVehicles() {
    io2d::render_props aliased{io2d::antialias::none};
    io2d::brush foreBrush{io2d::rgba_color::blue};

    auto pb = io2d::path_builder{};
    pb.matrix(matrix_);

    for (const auto object : objects_) {
        pb.new_figure({object->position().x(), object->position().y()});
        float constexpr l_marker = 2.f;
        pb.rel_line({l_marker, 0.f});
        pb.rel_line({0.f, l_marker});
        pb.rel_line({-l_marker, 0.f});
        pb.rel_line({0.f, -l_marker});
        pb.close_figure();

        surface_.fill(foreBrush, pb);
        surface_.stroke(foreBrush, io2d::interpreted_path{pb}, std::nullopt, std::nullopt, std::nullopt, aliased);
    }
}

void MapsRenderer::drawGrid() {
    auto rp = io2d::render_props{};
    rp.compositing(io2d::compositing_op::source);
    int m_BoardHeight = 10;
    int m_BoardWidth = 10;
    int m_CellPxSize = 100;
    auto b = io2d::brush{io2d::rgba_color::gray};
    auto sp = io2d::stroke_props{1., io2d::line_cap::none};
    auto dp = io2d::dashes{1.f, {1.f, 1.f}};

    auto pb = io2d::path_builder{};
    pb.new_figure({0.5f, 0.f});
    pb.rel_line({0.f, m_BoardHeight * m_CellPxSize});
    auto vert_line = io2d::interpreted_path{pb};
    for (auto i = 0; i < m_BoardWidth; ++i) {
        rp.surface_matrix(io2d::matrix_2d::create_translate({i * m_CellPxSize, 0.f}));
        surface_.stroke(b, vert_line, std::nullopt, sp, dp, rp);
    }

    pb.clear();
    pb.new_figure({0.f, 0.5f});
    pb.rel_line({m_BoardWidth * m_CellPxSize, 0.f});
    auto hor_line = io2d::interpreted_path{pb};
    for (auto i = 0; i < m_BoardHeight; ++i) {
        rp.surface_matrix(io2d::matrix_2d::create_translate({0.f, i * m_CellPxSize}));
        surface_.stroke(b, hor_line, std::nullopt, sp, dp, rp);
    }
}

void MapsRenderer::drawRoads() {
    io2d::render_props aliased{io2d::antialias::none};
    io2d::brush roadBrush{io2d::rgba_color::orange};

    for (const auto& road : map_->roads()) {
        // if (auto rep_it = m_RoadReps.find(line.type); rep_it != m_RoadReps.end()) {
        // auto& rep = rep_it->second;
        auto width = .5f;
        auto sp = io2d::stroke_props{width, io2d::line_cap::round};
        auto pb = io2d::path_builder{};
        pb.rel_matrix(matrix_);

        // pb.matrix(m_Matrix);
        for (const auto& pt : road.points()) pb.line(io2d::point_2d(pt.x(), pt.y()));
        surface_.stroke(roadBrush, io2d::interpreted_path{pb}, std::nullopt, sp, io2d::dashes{});
    }
}

void MapsRenderer::drawLanes() {
    io2d::render_props aliased{io2d::antialias::none};
    io2d::brush lanebrush{io2d::rgba_color::blue};
    for (const auto& road : map_->roads()) {
        for (const auto& section : road.sections()) {
            for (const auto& lane : section.lanes()) {
                // if (auto rep_it = m_RoadReps.find(line.type); rep_it != m_RoadReps.end()) {
                // auto& rep = rep_it->second;
                auto width = .3f;
                auto sp = io2d::stroke_props{width, io2d::line_cap::round};

                auto pb = io2d::path_builder{};
                pb.rel_matrix(matrix_);
                for (const auto& pt : lane.points()) {
                    pb.line(io2d::point_2d(pt.x(), pt.y()));
                }
                surface_.stroke(lanebrush, io2d::interpreted_path{pb}, std::nullopt, sp, io2d::dashes{});
            }
        }
    }
}

// }
// io2d::interpreted_path Render::PathLine() const {
//     if (m_Model.path.empty()) return {};

//     const auto nodes = m_Model.path;

//     auto pb = io2d::path_builder{};
//     pb.matrix(m_Matrix);
//     pb.new_figure(ToPoint2D(m_Model.path[0]));

//     for (int i = 1; i < m_Model.path.size(); i++) pb.line(io2d::point_2d(m_Model.path[i]));

//     return io2d::interpreted_path{pb};
// }
