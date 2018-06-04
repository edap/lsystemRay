#include "Ball.h"

Ball::Ball(){}

Ball::Ball(float _radius, ofColor _color, glm::vec2 _pos){
    radius = _radius;
    color = _color;
    pos = _pos;
}

void Ball::update(std::vector<glm::vec2>& intersections, float threshold, float vel, int maxHistory){
    //ofLog() << intersections.size();
    if(intersections.empty() || intersections.size() < 2){
        hittedIntersectionIndex = 0;
        history.clear();
        return;
    };

    //l'errore e' qua, se hai solo due punti dentro, vai out of range
    auto next = intersections.at(hittedIntersectionIndex+1);
    auto direction = next - pos;
    if (maxHistory > 0) {
        history.push_back(pos);
        if(history.size() > maxHistory){
            history.erase(history.begin());
        }
    }
    pos = pos + glm::normalize(direction) * vel;

    if (glm::distance(pos, next) < threshold+radius) {
        if (hittedIntersectionIndex == intersections.size()-2) {
            hittedIntersectionIndex = 0;
            history.clear();
            pos = intersections.at(hittedIntersectionIndex);
        } else {
            hittedIntersectionIndex +=1;
        }
    }
}

void Ball::draw(){
    ofPushStyle();
    ofSetColor(color);

    if (history.size()>0) {
        auto startCol = color;
        auto startAlpha = color.a;
        auto dec = startAlpha/history.size();
        for(auto h:history) {
            // make the last part of the tail more transparent and small
            //startCol.a =
            ofSetColor(startCol);
            ofDrawCircle(h, radius);
        }
    }
    ofSetColor(color);
    ofDrawCircle(pos, radius);
    ofPopStyle();
}
