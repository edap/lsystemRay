#pragma once

#include "ofMain.h"
#include "ofxRaycaster.h"
#include "ofxBlurUtils.h"
#include "ofxOscSender.h"
#include "ofxGui.h"
#include "Segment.h"
#include "ofxLSystem.h"

#include "Ray.h"

// send host (aka ip address)
#define HOST "localhost"

/// send port
#define PORT 4559


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void collectBounces(ofxraycaster::Ray<glm::vec2>& myRay, int &limit, vector<glm::vec2>& intersections);
        void drawLine(glm::vec2 o, glm::vec2 e, ofColor col);

private:
    void startLSystem(
                      string axiom,
                      string selectedRule,
                      float theta,
                      int depth,
                      string constants,
                      bool randomZRotation);
    map<string,float> getConstants(string _constants) const;
    vector<string> getRules(string rules) const;

    vector<Segment> segments;
    Ray ray;

    // blur effect
    ofxBlurUtils blur;
    bool bypass;
    int mode;

    //gui
    bool drawGui = true;
    bool drawSegments = true;
    bool isDrawing = false;
    bool rotateRay = true;
    float oldTime = 0;

    // draw segment with mouse
    glm::vec2 startPoint;
    glm::vec2 endPoint;

    ofxPanel gui;
    ofParameter <float> freq;
    ofParameter <float> maxLineLength;
    ofParameter <float> velocity;
    ofParameterGroup parameters;
    
    ofParameterGroup lsysParameters;

    ofXml settings;

    ofxOscSender sender;

    //Lsystem
    ofxLSystem  lsystem;
    vector<Segment> lsegments;
    vector<Segment> getSegments(const ofMesh mesh);



};
