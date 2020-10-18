#ifndef __COMPLEKS_IMAGE_IMAGE_HXX
#define __COMPLEKS_IMAGE_IMAGE_HXX

#include "../utils/resource.hxx"

namespace compleks {

class image {
public:
    int width, height;
    unsigned char *pixels;

    image(void);
    image(const resource &res);
    ~image();
};

} // namespace compleks

#endif // __COMPLEKS_IMAGE_IMAGE_HXX
