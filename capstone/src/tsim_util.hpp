#ifndef __TSIM_GEOMETRIC_HPP__
#define __TSIM_GEOMETRIC_HPP__

namespace tsim {
class Point {
   public:
    Point(double x, double y) {
        x_ = x;
        y_ = y;
        z_ = 0;
    }
    Point& operator+(const Point& rhs);
    Point& operator-(const Point& rhs);

    double x() const { return x_; };
    void setX(double value) { x_ = value; };
    double y() const { return y_; };
    void setY(double value) { y_ = value; };
    double z() const { return z_; };
    void setZ(double value) { z_ = value; };

   private:
    double x_{0};
    double y_{0};
    double z_{0};
};

template <typename T>
int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

}  // namespace tsim

#endif  // __TSIM_GEOMETRIC_HPP__
