module;

#include "common.h"

export module eq_structures;

export
{

namespace eq
{
    struct Vector2f
    {
        float Y;
        float X;
    };

    struct Vector3f
    {
        float Y;
        float X;
        float Z;
    };

    using Location = Vector3f;

    struct Orientation
    {
        float Heading;     // yaw
        float Pitch;       // pitch
        float Rotation;    // roll
    };

    struct ScreenCoordinates
    {
        uint32_t X = 0;
        uint32_t Y = 0;
    };
}

}
