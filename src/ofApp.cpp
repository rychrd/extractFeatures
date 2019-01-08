/* This is an example of how to integrate maximilain into openFrameworks,
 including using audio received for input and audio requested for output.
 
 
 You can copy and paste this and use it as a starting example.
 
 */


#include "ofApp.h"
#include "maximilian.h"/* include the lib */
#include "time.h"



//-------------------------------------------------------------
ofApp::~ofApp() {
    
    delete[] lAudioOut;
    delete[] rAudioOut;
    delete[] lAudioIn;
    delete[] rAudioIn;
    
}


//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetLineWidth(3);
    
    ofEnableAlphaBlending();
    ofSetupScreen();
    ofBackground(0, 0, 0);
    
    
    /* This is stuff you always need.*/
    
    sampleRate 			= 44100; /* Sampling Rate */
    initialBufferSize	= 512;	/* Buffer Size. you have to fill this buffer with sound*/
    lAudioOut			= new float[initialBufferSize];/* outputs */
    rAudioOut			= new float[initialBufferSize];
    lAudioIn			= new float[initialBufferSize];/* inputs */
    rAudioIn			= new float[initialBufferSize];
    
    
    /* This is a nice safe piece of code */
    memset(lAudioOut, 0, initialBufferSize * sizeof(float));
    memset(rAudioOut, 0, initialBufferSize * sizeof(float));
    
    memset(lAudioIn, 0, initialBufferSize * sizeof(float));
    memset(rAudioIn, 0, initialBufferSize * sizeof(float));

    fftSize = 1024;
    mfft.setup(fftSize, 512, 256);
    ifft.setup(fftSize, 512, 256);

    vbMesh.setMode(OF_PRIMITIVE_LINES);
    
    nAverages = 12;
    oct.setup(sampleRate, fftSize/2, nAverages);
    
    mfccs = (double*) malloc(sizeof(double) * 13);
    mfcc.setup(512, 42, 13, 20, 20000, sampleRate);
    
    ofxMaxiSettings::setup(sampleRate, 2, initialBufferSize);
    ofSoundStreamSetup(2,2, this, sampleRate, initialBufferSize, 4);    /* Call this last ! */
    
    string homeDir = ofFilePath::getUserHomeDir();
    myfont.load(homeDir + "/Library/Fonts/arial.ttf", 13);          //will probably only work for Mac OS

    ofSetVerticalSync(true);
    xMouse = 0.01;
    

    
}
//-------------------------------------------------------------
void ofApp::update(){
  //  cout << oct.nAverages << endl;
     vbMesh.clear();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    int counter = 0.;
    
    float horizWidth = 500.;
    float horizOffset = 50;
    float fftTop = 250;
    float mfccTop = 350;
    float chromagramTop = 550;
    
    int col = 0;
    
    float xinc = horizWidth / fftSize / 13;
    
    
    for(int i=0; i < fftSize / 2; i++) {
        
        double height = 30 + (lAudioOut[i]+0.1)*floor(filter.lopass(peakFreq,0.0001)*.8);
    //    cout << (filter.lopass(peakFreq,0.1)) << endl;
        float angle = counter*TWO_PI/512.;

        double xPos2 = cos(angle);
        double yPos2 = sin(angle);
        
//        if (oct.averages[i%oct.nAverages] > 1){
        col = oct.averages[i%oct.nAverages] + peakFreq * 0.05;
         //   cout << col << endl;
//        }
        c.setHsb(100-col,128,50+col*5,200);
        vbMesh.addVertex(glm::vec3((xPos2*height), (yPos2*height),0));
        vbMesh.addColor(c);
        counter++;
        

    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
  
        
    vbMesh.draw();
    
    ofPopMatrix();
//
    }
    
    //	cout << "\nMFCCS: ";
    ofSetColor(0, 255, 0,200);
    xinc = horizWidth / 13;
    for(int i=0; i < 13; i++) {
        float height = mfccs[i] * 100.0;
   //     ofRect(horizOffset + (i*xinc),mfccTop - height,40, height);
        //		cout << mfccs[i] << ",";
    }
    
    //octave analyser
    	ofSetColor(255, 0, 255,200);
    	xinc = horizWidth / oct.nAverages;
    	for(int i=0; i < oct.nAverages; i++) {
    		float height = oct.averages[i] / 20.0 * 50;
    		ofDrawRectangle(horizOffset + 300 + (i*xinc),chromagramTop + 250 - height,2, height);
    }
    
    ofSetColor(255, 255, 255,255);

//    char peakString[255]; // an array of chars
//    sprintf(peakString, "Peak Frequency: %.2f", peakFreq);
//    myfont.drawString(peakString, horizOffset, chromagramTop + 100);
//
//    char centroidString[255]; // an array of chars
//    sprintf(centroidString, "Spectral Centroid: %f", centroid);
//    myfont.drawString(centroidString, horizOffset, chromagramTop + 120);
//
//    char rmsString[255]; // an array of chars
//    sprintf(rmsString, "RMS: %.2f", RMS);
//    myfont.drawString(rmsString, horizOffset, chromagramTop + 200);
    
        char octString[255]; // an array of chars
        sprintf(octString, "Octave averages with high pass:");
        myfont.drawString(octString, horizOffset, chromagramTop + 200);
    
    
}

//--------------------------------------------------------------
void ofApp::audioRequested 	(float * output, int bufferSize, int nChannels){
    //	static double tm;
    
    double bufferFreq = 44100./512.;
    
    
    for (int i = 0; i < bufferSize; i++){
        //		wave = osc.saw(maxiMap::linexp(mouseY + ofGetWindowPositionY(), 0, ofGetScreenHeight(), 200, 8000));
//        		wave = lAudioIn[i];
        //wave = samp.play(1.);
        //get fft
//        wave = sineOsc.sinewave((400*sineOsc2.sinewave(2)));
        wave = (sineOsc.sinewave(bufferFreq*1+sineOsc2.sinewave(bufferFreq)*osc3.sinewave(xMouse)*mouseY*3)) *.5;
        if (mfft.process(wave)) {
            			int bins   = fftSize / 2.0;
            //do some manipulation
            			int hpCutoff = floor(((mouseY - ofGetWindowPositionY()) / (float) ofGetScreenHeight()) * fftSize / 12);
            //highpass
            //			memset(mfft.magnitudes, 0, sizeof(float) * hpCutoff);
            //			memset(mfft.phases, 0, sizeof(float) * hpCutoff);
            //lowpass
            			memset(mfft.magnitudes + hpCutoff, 0, sizeof(float) * (bins - hpCutoff));
            			memset(mfft.phases + hpCutoff, 0, sizeof(float) * (bins - hpCutoff));
            mfft.magsToDB();
            //			for(int z=0; z < 512; z++) cout << mfft.magnitudesDB[z] << ",";
            //			cout << "---------\n";
            oct.calculate(mfft.magnitudesDB);
            
            
           /* for (int j = 0; j < 12; j++) {
                 chromagram[j] = 0;
            }
            int j = 0;
            for (int i = 0; i < oct.nAverages; i++) {
                chromagram[j] += oct.averages[i];
                j++;
                j = j % 12;
            } */
            
            float sum = 0;
            float maxFreq = 0;
            int maxBin = 0;
            
            for (int i = 0; i < fftSize/2; i++) {
                sum += mfft.magnitudes[i];
                if (mfft.magnitudes[i] > maxFreq) {
                    maxFreq=mfft.magnitudes[i];
                    maxBin = i;
                }
            }
            centroid = sum / (fftSize / 2);
            peakFreq = (float)maxBin/fftSize * 44100;
            
            mfcc.mfcc(mfft.magnitudes, mfccs);
            //cout << mfft.spectralFlatness() << ", " << mfft.spectralCentroid() << endl;
        }

        mymix.stereo(wave*.5, outputs, 0.5);
        //		float mix = ((mouseX + ofGetWindowPositionX()) / (float) ofGetScreenWidth());
        //		mymix.stereo((wave * mix) + ((1.0-mix) * ifftVal), outputs, 0.5);
       // lAudioOut[i] = output[i*nChannels    ] = outputs[0]; /* You may end up with lots of outputs. add them here */
//rAudioOut[i] = output[i*nChannels + 1] = outputs[1];
        lAudioOut[i] = output[i*nChannels]   =  outputs[0];
        rAudioOut[i] = output[i*nChannels+1] =  outputs[1];
        
        
    }
    
    
    
}

//--------------------------------------------------------------
void ofApp::audioReceived 	(float * input, int bufferSize, int nChannels){
    
    
    /* You can just grab this input and stick it in a double, then use it above to create output*/
    
    float sum = 0;
    for (int i = 0; i < bufferSize; i++){
        
        /* you can also grab the data out of the arrays*/
        
        lAudioIn[i] = input[i*2];
        rAudioIn[i] = input[i*2+1];
        
        sum += input[i*2] * input[i*2];
        
    } 
    RMS = sqrt(sum);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
    xMouse = (mouseX + ofGetWindowPositionX() / ofGetWidth()) * 0.001;
  
    //cout << xMouse << endl;
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

