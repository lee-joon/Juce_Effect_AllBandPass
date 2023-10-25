/*
  ==============================================================================

    AllBandPass.cpp
    Created: 7 Jun 2023 1:33:50am
    Author:  JUNLEE

  ==============================================================================
*/

#include "AllBandPass.h"

void AllBandPass::setHighpass(bool highpass)
{
	this->highpass = highpass;
}

void AllBandPass::setCutoffFrequency(float cutoffFrequency)
{
	this->cutoffFrequency = cutoffFrequency;
}

void AllBandPass::setQFrequency(float QFrequency)
{
	this->QFrequency = QFrequency;
}

void AllBandPass::setSamplingRate(float samplingRate)
{
	this->samplingRate = samplingRate;
}

void AllBandPass::setGian(float Gain)
{
	this->Gain	= Gain;
	this->Volt	= pow(10.0f, Gain / 20.0f);
	this->H		= Volt - 1.f;
}

void AllBandPass::setLSGain(float Gain)
{
	this->LSGain = Gain;
	this->LSVolt = pow(10.0f, Gain / 20.0f); 
	this->LSH = LSVolt - 1.0f;
}

void AllBandPass::setLSCutoffFrequency(float cutoffFrequency)
{
	this->LowshelvCutoff = cutoffFrequency;
}

void AllBandPass::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer &)
{
	//x1 = y1 = xy2 = 0;

	dnBuffer.resize(buffer.getNumChannels(), std::vector<float>(4, 0));//Replace

	const bool sign = Gain > 0.0f;//이름 수정하고
	const bool LSSign = LSGain > 0.0f;

	const float tan = lookUp.tan(QFrequency / (2.0f*samplingRate));	  //애매함
	const float cos = -1.f *lookUp.cos(cutoffFrequency / samplingRate);


	float c1;
	if (sign)
	{
		c1 = (tan - 1.f) / (tan + 1.f);
	}
	else
	{
		c1 = (tan - Volt) / (tan + Volt);
	}


	const float LStan = lookUp.tan(LowshelvCutoff / (2.0f*samplingRate));
	float LSC1;
	if (LSSign)
	{
		LSC1 = (LStan - 1.f) / (LStan + 1.f);
	}
	else
	{
		LSC1 = (LStan - LSVolt) / (LStan + LSVolt);
	}

	
	const float d1 = cos * (1 - c1);

	for (int channel = 0; channel < buffer.getNumChannels(); channel++)
	{
		float* channelSamples = buffer.getWritePointer(channel);;
		for (int i = 0; i < buffer.getNumSamples(); i++)
		{
			const float inputSample = channelSamples[i] ;

			const float allpassbanded = (-c1 * inputSample) + (d1 * (dnBuffer[channel][0] - dnBuffer[channel][1])) + dnBuffer[channel][2];
			const float allpassLow = LSC1 * inputSample + dnBuffer[channel][0] - (LSC1 * dnBuffer[channel][3]);

			//----------------------------------------------------------------------------------------------------------------//
			dnBuffer[channel][2] = dnBuffer[channel][0] + c1 * dnBuffer[channel][1];
			dnBuffer[channel][0] = inputSample;
			dnBuffer[channel][1] = allpassbanded;

			dnBuffer[channel][3] = allpassLow;
			//----------------------------------------------------------------------------------------------------------------//

			const float filterOutput = inputSample + (H * 0.5f * (inputSample - allpassbanded)) + (LSH * 0.5 * (inputSample + allpassLow));

			channelSamples[i] = filterOutput;

		}
	}


}