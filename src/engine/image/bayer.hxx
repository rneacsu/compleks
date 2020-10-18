#ifndef __COMPLEKS_IMAGE_BAYER_HXX
#define __COMPLEKS_IMAGE_BAYER_HXX

#include "image.hxx"

namespace compleks {

class bayer : public image {
public:
    bayer();
    ~bayer();

private:
    static unsigned char pattern[];
    static const size_t size;
};

} // namespace compleks

#endif // __COMPLEKS_IMAGE_BAYER_HXX
