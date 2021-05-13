#ifndef __DEMO_HXX
#define __DEMO_HXX

#include <compleks.h>

#include "overlay.hxx"

class demo : public compleks::engine {
public:
    demo();

    void key_down(int, int) override;
    void render(double delta) override;

    void load_scene(int scene_num);

private:
    const int NUM_SCENES = 5;

    int current_scene_num;
    overlay screen_overlay;
    std::shared_ptr<compleks::body> goal;
};

#endif // __DEMO_HXX
