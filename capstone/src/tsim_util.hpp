#ifndef __TSIM_GEOMETRIC_HPP__
#define __TSIM_GEOMETRIC_HPP__

namespace tsim {
class Point {
   public:
    Point(double x, double y, double z = 0) : x_(x), y_(y), z_(z) {}
    Point(const Point& other) {
        x_ = other.x();
        y_ = other.y();
        z_ = other.z();
    }
    Point(Point&& other) {
        x_ = other.x();
        y_ = other.y();
        z_ = other.z();
    }
    Point operator=(const Point& other) {
        x_ = other.x();
        y_ = other.y();
        z_ = other.z();
    }
    Point operator=(Point&& other) {
        x_ = other.x();
        y_ = other.y();
        z_ = other.z();
    }

    // Point& operator+(const Point& other) {
    //     x_ += other.x();
    //     y_ += other.y();
    //     z_ += other.z();
    // return *this;
    // }
    // Point& operator-(const Point& other) {
    //     x_ -= other.x();
    //     y_ -= other.y();
    //     z_ -= other.z();
    // return *this;
    // }

    double x() const { return x_; };
    double y() const { return y_; };
    double z() const { return z_; };

   private:
    double x_;
    double y_;
    double z_;
};

template <typename T>
int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

}  // namespace tsim

#endif  // __TSIM_GEOMETRIC_HPP__
