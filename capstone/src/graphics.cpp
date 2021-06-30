#include <iostream>

#include "graphics.hpp"
MapsRenderer::MapsRenderer(std::vector<sim::Road>& roads)
    : surface_{1280, 1280, io2d::format::argb32, io2d::scaling::none, io2d::refresh_style::fixed, 30}, roads_(roads) {
    // surface_ =
    // io2d::output_surface{1280, 1280, io2d::format::argb32, io2d::scaling::none, io2d::refresh_style::fixed, 30};
    findMaxMinValues();
    std::cout << "min_x " << min_x << std::endl;
    std::cout << "max_x " << max_x << std::endl;
    std::cout << "min_y " << min_y << std::endl;
    std::cout << "max_y " << max_y << std::endl;

    scale_ = std::max((max_y - min_y), (max_x - min_x)) * 1.1f;
    std::cout << "scale " << scale_ << std::endl;
}
void MapsRenderer::display() {
    // surface_.size_change_callback([&]() { surface_.dimensions(surface_.display_dimensions()); });
    surface_.draw_callback([&](io2d::output_surface& surface) { render(); });
    surface_.begin_show();
}
void MapsRenderer::render() {
    auto px = static_cast<float>(std::min(surface_.dimensions().x(), surface_.dimensions().y()));
    matrix_ = io2d::matrix_2d::create_scale({px / scale_, -px / scale_}) *
              io2d::matrix_2d::create_translate({-min_x / 2 + scale_ * 0.1f, scale_ * 0.2f});

    surface_.paint(m_BackgroundFillBrush);

    drawHighways();
    drawVehicle();

    // DrawGrid(surface_);
    // DrawPath(surface_);
}

void MapsRenderer::findMaxMinValues() {
    for (auto road : roads_) {
        for (auto pt : road.points_) {
            if (pt.x_ > max_x) max_x = pt.x_;
            if (pt.x_ < min_x) min_x = pt.x_;
            if (pt.y_ > max_y) max_y = pt.y_;
            if (pt.y_ < min_y) min_y = pt.y_;
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

void MapsRenderer::drawVehicle() {
    io2d::render_props aliased{io2d::antialias::none};
    io2d::brush foreBrush{io2d::rgba_color::blue};

    auto pb = io2d::path_builder{};
    pb.matrix(matrix_);

    pb.new_figure({(float)(std::rand() % ((int)max_x + 1)), (float)(-std::rand() % ((int)min_y + 1))});
    float constexpr l_marker = 1.f;
    pb.rel_line({l_marker, 0.f});
    pb.rel_line({0.f, l_marker});
    pb.rel_line({-l_marker, 0.f});
    pb.rel_line({0.f, -l_marker});
    pb.close_figure();

    surface_.fill(foreBrush, pb);
    surface_.stroke(foreBrush, io2d::interpreted_path{pb}, std::nullopt, std::nullopt, std::nullopt, aliased);
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

void MapsRenderer::drawHighways() {
    io2d::render_props aliased{io2d::antialias::none};
    io2d::brush roadBrush{io2d::rgba_color::orange};

    for (auto road : roads_) {
        // if (auto rep_it = m_RoadReps.find(line.type); rep_it != m_RoadReps.end()) {
        // auto& rep = rep_it->second;
        auto width = .5f;
        auto sp = io2d::stroke_props{width, io2d::line_cap::round};
        auto pb = io2d::path_builder{};
        pb.rel_matrix(matrix_);

        // pb.matrix(m_Matrix);
        for (auto pt : road.points_) pb.line(io2d::point_2d(pt.x_, pt.y_));
        surface_.stroke(roadBrush, io2d::interpreted_path{pb}, std::nullopt, sp, io2d::dashes{});
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
