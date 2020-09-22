#ifndef __DEMO_WORLD_HXX
#define __DEMO_WORLD_HXX

#include <compleks.h>

class demo_world : public compleks::world {
public:
    demo_world();

private:
    compleks::lighting::light sun;
    std::shared_ptr<compleks::object> plane, cube;
    std::shared_ptr<compleks::portal> p1, p2;
};

#endif // __DEMO_WORLD_HXX
