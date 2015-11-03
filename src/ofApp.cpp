#include "ofApp.h"

#include <string>
#include <sstream>

std::string intToString(int i)
{
    std::ostringstream oss;
    oss << i;
    return oss.str();
}

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetFrameRate(30);

    vidWidth = 320; //640;
    vidHeight = 240; //480;

    vidGrabber.setVerbose(true);
    vidGrabber.setDesiredFrameRate(30);
    vidGrabber.initGrabber(vidWidth, vidHeight);

    std::cout << "actual size is " << vidGrabber.width << " by " << vidGrabber.height << std::endl;

    haarFinder.setup("haarcascade_frontalface_default.xml");
    haarFinder.setScaleHaar(1.09);

    box2d.init();
    box2d.enableEvents();
    box2d.setGravity(0, 30);
    // physics frame rate
    box2d.setFPS(60);

    sound.loadSound("sfx/2.mp3");
    sound.setMultiPlay(true);
    sound.setLoop(false);

    facesHit = 0;
    facesDetected = 0;

}

//--------------------------------------------------------------
void ofApp::update()
{
    ofBackground(ofColor::whiteSmoke);

    vidGrabber.update();

    if(vidGrabber.isFrameNew())
    {
        haarFinder.findHaarObjects(vidGrabber.getPixelsRef());
    }

    facesDetected = haarFinder.blobs.size();

    // Shoot objects at the faces
    box2d.update();

    addCircles();
    for(int i = 0; i < circles.size(); ++i)
    {
        if(circles[i] != NULL)
        {
            circles[i]->update();

            for(int j = 0; j < haarFinder.blobs.size(); j++)
            {
                if(haarFinder.blobs[j].boundingRect.inside(circles[i]->getPosition()))
                {
                    // Hit!
                    sound.play();
                    facesHit++;

                    // Delete the circle
                    delete circles[i];
                    circles[i] = NULL;
                    break;
                }
            }
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw()
{
    // Text headers
    ofSetColor(ofColor::black);
    ofDrawBitmapString("Faces detected: " + intToString(facesDetected), 0, 15);
    ofDrawBitmapString("Faces hit: " + intToString(facesHit), 0, 30);

    ofSetColor(ofColor::white);
    ofNoFill();
    // Main video
    ofTranslate(50, 50);
    vidGrabber.draw(0, 0);
    for (int i = 0; i < haarFinder.blobs.size(); i++) {
        ofRect(haarFinder.blobs[i].boundingRect);
    }

    ofSetColor(ofColor::red);
    ofFill();
    for(int i = 0; i < circles.size(); ++i)
    {
        if(circles[i] != NULL)
        {
            circles[i]->draw();
        }

    }
    std::cout << ofGetFrameRate() << "\n";
}

void ofApp::addCircles()
{
    for(int i = 0; i < haarFinder.blobs.size(); ++i)
    {
        if(ofRandom(0.0f, 1.0f) < 0.1)
        {
            ofPoint centroid = haarFinder.blobs[i].centroid;

            circles.push_back(new ofxBox2dCircle);
            circles.back()->setPhysics(3.0, 0.3, 0.1);
            circles.back()->enableGravity(true);
            circles.back()->setMassFromShape = true;
            circles.back()->setup(box2d.getWorld(), centroid.x + ofRandomf() * 10, 0, 7.0f);
            //circles.back()->setVelocity(ofRandomf(), -1 * initialVelocity);
        }
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{

}
