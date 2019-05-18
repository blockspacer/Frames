#include "easing.h"

#define _USE_MATH_DEFINES
#include <cmath>

namespace frames {
namespace easing {

    double ease(const EasingFunction fun, const double t)
    {
        switch (fun) {

        case InSine:
            return inSine(t);
            break;
        case InQuad:
            return inQuad(t);
            break;
        case InCubic:
            return inCubic(t);
            break;

        // Ease Out
        case OutSine:
            return outSine(t);
            break;
        case OutQuad:
            return outQuad(t);
            break;
        case OutCubic:
            return outCubic(t);
            break;

        // Ease InOut
        case InOutSine:
            return inOutSine(t);
            break;
        case InOutQuad:
            return inOutQuad(t);
            break;
        case InOutCubic:
            return inOutCubic(t);
            break;
        }
    }

    double inSine(const double t)
    {
        return 1. - cos(t * M_PI_2);
    }

    double inQuad(const double t)
    {
        return t * t;
    }

    double inCubic(const double t)
    {
        return t * t * t;
    }

    double outSine(const double t)
    {
        return sin(t * M_PI_2);
    }

    double outQuad(const double t)
    {
        return 1 - (1 - t) * (1 - t);
    }

    double outCubic(const double t)
    {
        return 1 - std::pow(1 - t, 3);
    }

    double inOutSine(const double t)
    {
        return -(std::cos(t * M_PI) - 1) / 2.;
    }

    double inOutQuad(const double t)
    {
        if (t < 0.5) {
            return 2. * t * t;
        } else {
            return 1. - std::pow(-2. * t + 2., 2.) / 2.;
        }
    }

    double inOutCubic(const double t)
    {
        if (t < 0.5) {
            return 4 * t * t * t;
        } else {
            return 1. - std::pow(-2. * t + 2., 3) / 2.;
        }
    }
}
}
