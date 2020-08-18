#ifndef __DEMO_WORLD_HXX
#define __DEMO_WORLD_HXX

#include <compleks.h>

class demo_world : public compleks::world {
public:
    demo_world();
private:
    compleks::lighting::light sun;
    compleks::object plane;
    compleks::object cube;
};

#endif // __DEMO_WORLD_HXX