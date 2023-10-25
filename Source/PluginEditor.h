/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class AllBandPassAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    AllBandPassAudioProcessorEditor (AllBandPassAudioProcessor&, juce::AudioProcessorValueTreeState& vts);
    ~AllBandPassAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AllBandPassAudioProcessor& audioProcessor;

	juce::Slider cutoffFrequencySlider;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffFrequencyAttachment;
	juce::Label cutoffFrequencyLabel;

	juce::Slider QFrequencySlider;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> QFrequencyAttachment;
	juce::Label QFrequencyLabel;

	juce::Slider GainSlider;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> GainAttachment;
	juce::Label GainLabel;

	juce::Slider LSFrequencySlider;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> LSFrequencyAttachment;
	juce::Label LSFrequencyLabel;

	juce::Slider LSGainSlider;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> LSGainAttachment;
	juce::Label LSGainLabel;

	juce::ToggleButton highpassButton;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> highpassAttachment;
	juce::Label highpassButtonLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AllBandPassAudioProcessorEditor)
};
