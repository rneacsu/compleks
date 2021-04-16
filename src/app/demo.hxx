#ifndef __DEMO_HXX
#define __DEMO_HXX

#include <compleks.h>

class demo : public compleks::engine {
public:
    demo();

    void key_down(int, int) override;
    void render(double delta) override;

private:
    int current_scene_num;
    float overlay_alpha = 0;

    std::shared_ptr<compleks::object> overlay;
};

#endif // __DEMO_HXX
