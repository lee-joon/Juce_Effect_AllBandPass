/*
  ==============================================================================

    AllBandPass.h
    Created: 7 Jun 2023 1:33:50am
    Author:  JUNLEE

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include <vector>
#include "lookUpTable.h"

class AllBandPass {
public:
	void setHighpass(bool highpass);  //boost or cut
	void setCutoffFrequency(float cutoffFrequency);
	void setQFrequency(float QFrequency);
	void setSamplingRate(float samplingRate);
	void setGian(float Gain);
	void setLSGain(float Gain);
	void setLSCutoffFrequency(float cutoffFrequency);

	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&);

private:
	bool highpass;
	float cutoffFrequency;
	float QFrequency;
	float samplingRate;

	float Gain;
	float Volt;
	float H;
	//////////////////////////////

	float LowshelvCutoff;
	float LSGain;
	float LSVolt;
	float LSH;
	


	// allpass filters' buffers: 1 sample per channel
	std::vector<std::vector<float>> dnBuffer;

	SinCosTanLookupTable lookUp;
};