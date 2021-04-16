#pragma once

#include <memory>

#include <bullet/btBulletDynamicsCommon.h>

namespace compleks {

class shape {
public:
    shape();
    shape(std::unique_ptr<btCollisionShape> s);

    btCollisionShape &get_shape(void);
    void set_shape(std::unique_ptr<btCollisionShape> s);

protected:
    std::unique_ptr<btCollisionShape> bt_shape;
};

} // namespace compleks
