#ifndef _OF_APP
#define _OF_APP


#include "ofMain.h"
#include "ofxMaxim.h"
#include "ofxGui.h"
#include "ofxOsc.h"

#include <sys/time.h>

#include "maxiMFCC.h"
#define HOST "localhost"
#define PORT 12345

class ofApp : public ofBaseApp{
    
public:
    ~ofApp();/* desructor very useful */
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    void audioRequested 	(float * input, int bufferSize, int nChannels); /* output method */
    void audioReceived 	(float * input, int bufferSize, int nChannels); /* input method */
    
    float 	* lAudioOut; /* outputs */
    float   * rAudioOut;
    
    float * lAudioIn; /* inputs */
    float * rAudioIn;
    
    int		initialBufferSize; /* buffer size */
    int		sampleRate;
    
    //MAXIMILIAN STUFF:
    double wave,sample,outputs[2], ifftVal, xMouse;
    maxiMix mymix;
    maxiOsc osc;
    
    ofxMaxiFFTOctaveAnalyzer oct;
    int nAverages;
    float *ifftOutput;
    int ifftSize;
    
    float peakFreq = 0;
    float centroid = 0;
    float RMS = 0;
    
    ofxMaxiIFFT ifft;
    ofxMaxiFFT mfft;
    int fftSize;
    int bins, dataSize;
    
    float callTime;
    timeval callTS, callEndTS;
    
    maxiMFCC mfcc;
    double *mfccs;
    
    maxiSample samp;
    
    maxiFilter filter;
    
    maxiOsc sineOsc, sineOsc2, osc3, pulse, noise;
    
    ofPolyline line, line2;
    ofVboMesh vbMesh;
    ofColor c;
    
    //GUI STUFF
    bool bHide;
    
    ofxToggle mfccToggle;
    ofxToggle fftToggle;
    ofxToggle chromagramToggle;
    ofxToggle peakFrequencyToggle;
    ofxToggle centroidToggle;
    ofxToggle rmsToggle;

    ofEasyCam cam;
    
    ofxPanel gui;
    
    ofTrueTypeFont myfont;

    ofxOscSender sender;

    
};

#endif
