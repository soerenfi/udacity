#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "geometric.cpp"

class Object {
   public:
    void simulate();

   private:
    Point position{0.0f, 0.0f};
};

#endif  // __OBJECT_H__
