#pragma once
#include "ofMain.h"


class Segment {
public:
    glm::vec2 a;
    glm::vec2 b;
    ofColor color;

    int synthType;
    float note;
    float attack;
    float decay;
    float sustain;
    float release;
};
