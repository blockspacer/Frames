#ifndef EASING_H
#define EASING_H

namespace frames {
namespace easing {

    // See https://easings.net for a list of easing functions
    // See https://github.com/ai/easings.net/blob/master/src/easings/easingsFunctions.ts
    // For the functions code

    enum EasingFunction {
        // Ease In
        InSine,
        InQuad,
        InCubic,

        // Ease Out
        OutSine,
        OutQuad,
        OutCubic,

        // Ease InOut
        InOutSine,
        InOutQuad,
        InOutCubic
    };

    double ease(const EasingFunction fun, const double t);

    double inSine(const double t);
    double inQuad(const double t);
    double inCubic(const double t);

    double outSine(const double t);
    double outQuad(const double t);
    double outCubic(const double t);

    double inOutSine(const double t);
    double inOutQuad(const double t);
    double inOutCubic(const double t);
}
}

#endif // EASING_H
