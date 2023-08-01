module;

#include "common.h"

export module eq_structures;

export
{

namespace eq
{
    struct Location
    {
        float Y;
        float X;
        float Z;
    };

    struct Orientation
    {
        float Heading;
        float Pitch;
        float Rotation;
    };
}

}
