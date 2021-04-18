#ifndef __DEMO_HXX
#define __DEMO_HXX

#include <compleks.h>

#include "overlay.hxx"

class demo : public compleks::engine {
public:
    demo();

    void key_down(int, int) override;
    void render(double delta) override;

    std::unique_ptr<compleks::scene> create_scene(int scene_num);

private:
    int current_scene_num;
    overlay screen_overlay;
};

#endif // __DEMO_HXX
