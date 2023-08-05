module;

#include "common.h"

export module eq_structures;

export
{

namespace eq
{
    struct Vector2f
    {
        float X;
        float Y;
    };

    struct Vector3f
    {
        float X;
        float Y;
        float Z;
    };

    struct Location
    {
        float Y;
        float X;
        float Z;
    };

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
