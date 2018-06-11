#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    sender.setup(HOST, PORT);

    blur.setupRGB(ofGetWindowWidth(), ofGetWindowHeight());
    blur.setMode(3);
    parameters.setName("settings");
    parameters.add(blur.parametersRGB);
    parameters.add(freq.set("freq",0.5,0.1, 3.0));
    parameters.add(maxLineLength.set("max l length", 1,3,15));
    parameters.add(velocity.set("ball vel", 4.5, 0.1, 40.5));

    gui.setup(parameters);
    gui.loadFromFile("settings.xml");

    bypass = false; //bypass shader;

    auto orig = glm::vec2(ofGetWidth()/2, ofGetHeight()/2);
    auto sinedT = sin(ofGetElapsedTimef() * freq);
    auto cosinedT = cos(ofGetElapsedTimef() * freq);
    auto startDir = glm::vec2(sinedT, cosinedT);

    ray.init(orig, startDir, 500);

    startLSystem("F",
                 "F -> FF+[+F-F-F]-[-F+F+F]",
                 15,
                 2,
                 "",
                 false
                 );
}



//--------------------------------------------------------------
void ofApp::update(){
    ray.bounce(lsegments, velocity, sender);
}

//--------------------------------------------------------------
void ofApp::draw(){
    blur.begin();
    ray.draw();

    //lsystem.draw();

    if (drawSegments) {
        for(auto s:lsegments){
            ofPushStyle();
            ofSetColor(s.color);
            ofDrawLine(s.a, s.b);
            ofPopStyle();
        }
    }


    if (isDrawing) {
        auto mouse = glm::vec2(ofGetMouseX(), ofGetMouseY());
        ofDrawLine(startPoint, mouse);
    }
    blur.endRGB();

    if(drawGui){
        gui.draw();
    }
}


void ofApp::drawLine(glm::vec2 o, glm::vec2 e, ofColor c){
    ofPushStyle();
    ofSetColor(c);
    ofDrawLine(o,e);
    ofPopStyle();

};

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'g'){
        drawGui = !drawGui;
    }
    if (key == 'c') {
        segments.clear();
        ray.reset();
    }
    if (key == 's') {
        drawSegments = !drawSegments;
    }
    if (key == 'r') {
        oldTime = ofGetElapsedTimef();
        rotateRay = !rotateRay;
    }

    if(key == ' '){
        bypass = !bypass;
        blur.setBypass(bypass);
    }
    else if(key == '0'){
        blur.setMode(0);
    }
    else if(key == '1'){
        blur.setMode(1);
    }
    else if(key == '2'){
        blur.setMode(2);
    }
    else if(key == '3'){
        blur.setMode(3);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    startPoint = glm::vec2(x, y);
    isDrawing = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    endPoint = glm::vec2(x, y);
    // sined color
    auto sinedCol = abs(sin(ofGetElapsedTimef()) * 255);
    auto col = ofColor::fromHsb(sinedCol, 255, 255);

    Segment segTemp;
    segTemp.a = startPoint;
    segTemp.b = endPoint;
    segTemp.color = col;
    segTemp.note = ofRandom(10, 70);
    segments.push_back(segTemp);
    isDrawing = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::startLSystem(string axiom,
                         string rulesString,
                         float theta,
                         int depth,
                         string _constants,
                         bool randomZRotation){
    auto constants = getConstants(_constants);
    auto rules = getRules(rulesString);
    lsystem.setAxiom(axiom);
    lsystem.setRules(rules);
    lsystem.setTheta(theta);
    lsystem.setStep(depth);
    lsystem.setStepLength(40);
    //lsystem.setScaleWidth(true);
    lsystem.setGeometry(LINES);
    lsystem.setConstants(constants);
    lsystem.setRandomYRotation(false);
    lsystem.build();

    // the ofxLsystem addon is thought for 3D world coordinate system
    lsystem.setPosition(ofGetWidth()/2, 600, 0);
    lsystem.rollDeg(180);

    lsegments.clear();
    lsegments = getSegments(lsystem.getMesh());
}

vector<Segment>ofApp::getSegments(const ofMesh mesh){
    vector<Segment> result;
    auto vertices = mesh.getVertices();
    for (int i = 0; i< vertices.size(); i+=2) {
        Segment tmpSegment;
        auto a = lsystem.getGlobalTransformMatrix() * glm::vec4(vertices[i], 1.f);
        auto b = lsystem.getGlobalTransformMatrix() * glm::vec4(vertices[i+1], 1.f);
        tmpSegment.a = glm::vec2(a.x, a.y);
        tmpSegment.b = glm::vec2(b.x, b.y);

        result.push_back(tmpSegment);
    }
    return result;
};

//it expects a single string like "y=0.54353;t=1"
map<string,float>ofApp::getConstants(string _constant) const{
    map<string,float> result;
    auto parts = ofSplitString(_constant, ";");
    for (auto con : parts){
        auto def = ofSplitString(con, "=");
        if(def.size() == 2){
            result.insert(make_pair(def[0], ofToFloat(def[1])));
        }
    }
    return result;
}

vector<string>ofApp::getRules(string rules) const{
    vector<string> stringRules;
    if(rules.find(";") != std::string::npos){
        //multiple rules
        auto rule_container = ofSplitString(rules, ";");
        for(auto r:rule_container){
            stringRules.push_back(r);
        }
    }else{
        //single rule
        stringRules.push_back(rules);
    }
    return stringRules;
}
