#include "ofApp.h"

#include <string>
#include <sstream>
#include <cmath>

std::string intToString(int i)
{
    std::ostringstream oss;
    oss << i;
    return oss.str();
}

std::string floatToString(float f)
{
    f = floorf(f * 10) / 10;
    std::ostringstream oss;
    oss << f;
    return oss.str();
}

//--------------------------------------------------------------
void ofApp::setup()
{
    // Initial values
    defaultCircleRadius = 3.0f;
    defaultGravity = 5;
    defaultCircleThreshold = 0.02f;
    timeSinceLastHit = 0;
    hit = false;
    resetToDefault();
    hitFrames = 0;
    difficulty = 1;

    difficultyHighScore = 0;
    timeHighScore = 0;

    ofSetFrameRate(30);

    vidWidth = 320; //640;
    vidHeight = 240; //480;

    vidGrabber.setVerbose(true);
    vidGrabber.setDesiredFrameRate(30);
    vidGrabber.initGrabber(vidWidth, vidHeight);

    flipHorizontal = true;
    colorImg.allocate(vidWidth, vidHeight);

    std::cout << "actual size is " << vidGrabber.width << " by " << vidGrabber.height << std::endl;

    haarFinder.setup("haarcascade_frontalface_default.xml");
    haarFinder.setScaleHaar(1.2);

    box2d.init();
    box2d.enableEvents();
    box2d.setGravity(0, gravity);
    // physics frame rate
    box2d.setFPS(30);

    sound.loadSound("sfx/punch.wav");
    sound.setMultiPlay(true);
    sound.setLoop(false);

    facesHit = 0;
    facesDetected = 0;
}

//--------------------------------------------------------------
void ofApp::update()
{
    if(facesDetected > 0)
    {
        timeSinceLastHit += 1.0f / ofGetFrameRate();
    }

    difficultyHighScore = std::max(difficultyHighScore, difficulty);
    timeHighScore = std::max(timeHighScore, timeSinceLastHit);

    ofBackground(ofColor::whiteSmoke);

    vidGrabber.update();

    if(vidGrabber.isFrameNew())
    {
        colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);

        // Flip horizontally
        if(flipHorizontal)
        {
            colorImg.mirror(false, true);
        }

        haarFinder.findHaarObjects(colorImg.getPixelsRef());
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
                    hit = true;

                    resetToDefault();
                    timeSinceLastHit = 0.0f;

                    // Delete the circle
                    delete circles[i];
                    circles[i] = NULL;
                    break;
                }
            }
        }
    }

    if(timeSinceLastHit > 2.0f * difficulty)
    {
        increaseDifficulty();
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    // Text headers
    ofSetColor(ofColor::black);
    ofDrawBitmapString("Faces detected: " + intToString(facesDetected), 0, 15);
    ofDrawBitmapString("Faces hit: " + intToString(facesHit), 0, 30);
    ofDrawBitmapString("Current difficulty: " + intToString(difficulty), 0, 45);
    ofDrawBitmapString("Current time: " + floatToString(timeSinceLastHit), 0, 60);

    ofDrawBitmapString("Dodge the falling circles!", 250, 20);
    ofDrawBitmapString("Press 'f' to flip the camera", 250, 50);

    ofDrawBitmapString("High score: ", 570, 35);
    ofDrawBitmapString("Time: " + floatToString(timeHighScore), 580, 50);
    ofDrawBitmapString("Difficulty: " + intToString(difficultyHighScore), 580, 65);


    ofSetColor(ofColor::white);
    if(hit)
    {
        ofSetColor(ofColor::red);
        hitFrames++;
        if(hitFrames > 4)
        {
            hit = false;
            hitFrames = 0;
        }
    }

    ofNoFill();

    // Main video
    ofTranslate(30, 70);
    ofPushMatrix();
    ofScale(2,2,1);
    //vidGrabber.draw(0, 0);
    colorImg.draw(0, 0);
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

    ofPopMatrix();
    std::cout << ofGetFrameRate() << "\n";
}

void ofApp::addCircles()
{
    for(int i = 0; i < haarFinder.blobs.size(); ++i)
    {
        if(ofRandom(0.0f, 1.0f) < circleThreshold)
        {
            ofPoint centroid = haarFinder.blobs[i].centroid;
            float length = haarFinder.blobs[i].boundingRect.width;

            circles.push_back(new ofxBox2dCircle);
            circles.back()->setPhysics(2.0, 0.3, 0.05);
            circles.back()->enableGravity(true);
            circles.back()->setMassFromShape = true;
            circles.back()->setup(box2d.getWorld(), centroid.x + ofRandom(-1.0f, 1.0f) * (length / 2.0f), 0, circleRadius);
            //circles.back()->setVelocity(ofRandomf(), -1 * initialVelocity);
        }
    }
}

void ofApp::resetToDefault()
{
    difficulty = 0;

    gravity = defaultGravity;
    circleRadius = defaultCircleRadius;
    circleThreshold = defaultCircleThreshold;
}

void ofApp::increaseDifficulty()
{
    difficulty++;

    gravity += 10;
    circleRadius += 1.0f;
    circleThreshold += 0.02;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    // Press f to toggle flipping camera
    if(key == 'f')
    {
        flipHorizontal = !flipHorizontal;
    }
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
