#pragma once

#include "ofMain.h"
#include "ofxRaycaster.h"
#include "Segment.h"
#include "ofxOscSender.h"

class Ray : public ofxraycaster::Ray<glm::vec2> {
public:

    void init(const glm::vec2 orig, const glm::vec2 dir, const int maxBounces);
    void bounce(const vector<Segment> segments,
                const ofParameter<float> velocity,
                ofxOscSender &sender);
    void drawRayControls();
    void drawHistory();
    void reset();

private:

    void sendSegmentSound(const Segment segment,
                          ofxOscSender &sender);

    void sendOSC(const float note, ofxOscSender &sender);

    // these two variable store the origin and the direction of the ray
    // before it starts to bounce
    glm::vec2 startingOrig;
    glm::vec2 startingDir;

    float radius = 5;
    int maxBounces = 10;
    int currentBounces = 0;
    int history = 20;



};
