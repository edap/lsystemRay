#pragma once
#include "ofMain.h"


class Ball
{
public:
    Ball();
    Ball(float _radius, ofColor _color, glm::vec2 _pos);

    float radius;
    ofColor color;
    glm::vec2 pos;
    float velocity = 0.;
    std::vector<glm::vec2> history;


    int hittedIntersectionIndex = 0;
    float pct = 0.;

    void draw();
    void update(std::vector<glm::vec2>& intersections, float threshold, float vel, int maxHistory);
};
