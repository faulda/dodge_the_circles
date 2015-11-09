#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxBox2d.h"


#include <deque>

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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void addCircles();
		void resetToDefault();
        void increaseDifficulty();

    private:
		ofVideoGrabber vidGrabber;
		int vidWidth, vidHeight;

        bool flipHorizontal;
        ofxCvColorImage	colorImg;
		ofxCvHaarFinder haarFinder;

        ofxBox2d box2d;

		int facesHit;
		int facesDetected;

		int difficulty;
		int difficultyHighScore;
		float circleRadius;
		float gravity;
		float circleThreshold;
		float defaultCircleRadius;
		float defaultGravity;
		float defaultCircleThreshold;
        float timeSinceLastHit;
        float timeHighScore;
		std::vector<ofxBox2dCircle*> circles;

		bool hit;
        int hitFrames;

		ofSoundPlayer sound;
};
