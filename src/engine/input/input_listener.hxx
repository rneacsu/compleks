#ifndef __COMPLEKS_INPUT_INPUT_LISTENER_HXX
#define __COMPLEKS_INPUT_INPUT_LISTENER_HXX

namespace compleks
{
    
class input_listener {
public:
    virtual void key_down(int, int) {};
    virtual void key_up(int, int) {};
    virtual void mouse_down(int) {};
    virtual void mouse_up(int) {};
    virtual void mouse_move(float, float) {};
    virtual void mouse_enter() {};
    virtual void mouse_leave() {};
    virtual void mouse_scroll(float) {};
};

}

#endif // __COMPLEKS_INPUT_INPUT_LISTENER_HXX