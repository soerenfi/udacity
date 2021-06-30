#ifndef __GEOMETRIC_HPP__
#define __GEOMETRIC_HPP__

namespace sim {
class Point {
   public:
    Point(double x, double y) {
        x_ = x;
        y_ = y;
    }
    double x_{0};
    double y_{0};
    double z_{0};
};

}  // namespace sim

#endif  // __GEOMETRIC_HPP__
