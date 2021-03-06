#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	mGui.setup("oscillator settings");
	mFreqSlider.setup("frequency", 200.0f, 60.0f, 2500.0f);
	mVolumeSlider.setup("volume(amplitude)", 0.0f,0.0f,0.5f);
	mLfoFreqSlider.setup("lfo freq", 0.5, 0.0f, 15.0f);
	mLfoAmpSlider.setup("lfo amplitude",0.0f,0.0f,0.5f);
	//ampersand means we pass in the variable location in memory
	//because inside the add function we create a pointer
	mGui.add(&mFreqSlider);
	mGui.add(&mVolumeSlider);
	mGui.add(&mLfoFreqSlider);
	mGui.add(&mLfoAmpSlider);
	// 2 output channels (stereo), 0 input channels
	// 512 samples per buffer, 2 buffers
	ofSoundStreamSetup(2, 0, sampleRate, 512, 2);
}

//--------------------------------------------------------------
void ofApp::update(){
	
}


//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor::white, ofColor::gray);
	mGui.draw();

}



//void ofApp::AM(float * output, int buffer_size) {
//	for(int i = 0; i < buffer_size; i++) {
//		output[i] = sin(ofMap(mouseX, 0,ofGetWidth(), 0,360.0)+ wavePhase) * sin(ofMap(mouseY, 0,ofGetHeight(), 0,360.0)+ wavePhase);
//	}
//}
//
//void ofApp::FM(float * output, int buffer_size) {
//	for(int i = 0; i < buffer_size; i++) {
////		output[i] = osc1.sinewave(osc2.sinewave(mouseX) * mouseY + 440.0);
//	}
//}

void ofApp::audioOut(ofSoundBuffer &outBuffer){
	ofScopedLock waveformLock(mWaveformMutex);
	for(int i = 0; i < outBuffer.size(); i += 2) {
		if(mCurrentMod == ModulationType::FM){
			mPhase += ofDegToRad(sin(mModPhase)*0.01f);
			
		}
		float sample = generateSample(mPhase, currWaveForm);
		sample = modulateSample(sample, (float) mLfoFreqSlider, mModPhase, mCurrentMod);
		
		sample *= (float)mVolumeSlider; // generating a sine wave sample
		outBuffer[i] = sample; // writing to the left channel
		outBuffer[i+1] = sample; // writing to the right channel
		
		//memorize this equation! phaseOffset = freq / sampleRate
		
		float phaseOffset = ((float)mFreqSlider / (float)sampleRate);
		float modOffset = (float) mLfoFreqSlider / (float)sampleRate;
		mPhase += phaseOffset;
		mModPhase += modOffset;
	}
}

float ofApp::modulateSample(float sample, float modFreq, float modOffset, const ModulationType modType){
	if(modType == ModulationType::AM){
		return sample * sin(modOffset *TWO_PI);
	}
	return sample;
}

//returns a sample given an input phase and desired
//waveform type
//eg. sine, square, sawtooth, triangle
float ofApp::generateSample(float phase, int waveType){
	switch (waveType) {
		case 1://sine
			return sin(phase*TWO_PI);
			break;
		case 2://square
			return sin(phase*TWO_PI)>0?1:-1;
		case 3://sawtooth
			return fmod(phase,TWO_PI);
		case 4://triangle
			return abs(sin(phase*TWO_PI));
		default://default is sine wave
			return sin(phase*TWO_PI);
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
		case '1':
			currWaveForm = 1;
			break;
		case '2':
			currWaveForm = 2;
			break;
		case '3':
			currWaveForm = 3;
			break;
		case '4':
			currWaveForm = 4;
			break;
		case 'f':
			mCurrentMod = ModulationType::FM;
			break;
		case 'a':
			mCurrentMod = ModulationType::AM;
			break;
		default:
			break;
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

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
