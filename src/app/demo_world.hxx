#ifndef __DEMO_WORLD_HXX
#define __DEMO_WORLD_HXX

#include <compleks.h>

class demo_world : public compleks::world {
public:
    demo_world();

    void key_down(int, int) override;

private:
    int current_scene_num;
};

#endif // __DEMO_WORLD_HXX
