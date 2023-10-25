/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AllBandPassAudioProcessorEditor::AllBandPassAudioProcessorEditor (AllBandPassAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
	addAndMakeVisible(cutoffFrequencySlider);
	cutoffFrequencySlider.setSliderStyle(
		juce::Slider::SliderStyle::LinearVertical);
	cutoffFrequencyAttachment.reset(
		new juce::AudioProcessorValueTreeState::SliderAttachment(
			vts, "cutoff_frequency", cutoffFrequencySlider));

	addAndMakeVisible(cutoffFrequencyLabel);
	cutoffFrequencyLabel.setText("Cutoff Frequency",
		juce::dontSendNotification);
	///////////////////////////////////////////////////////////////////////////////////////
	
	addAndMakeVisible(QFrequencySlider);
	QFrequencySlider.setSliderStyle(
		juce::Slider::SliderStyle::LinearVertical);
	QFrequencyAttachment.reset(
		new juce::AudioProcessorValueTreeState::SliderAttachment(
			vts, "Q_frequency", QFrequencySlider));

	addAndMakeVisible(QFrequencyLabel);
	QFrequencyLabel.setText("Q Frequency",
		juce::dontSendNotification);

	///////////////////////////////////////////////////////////////////////////////////////

	addAndMakeVisible(GainSlider);
	GainSlider.setSliderStyle(
		juce::Slider::SliderStyle::LinearVertical);
	GainAttachment.reset(
		new juce::AudioProcessorValueTreeState::SliderAttachment(
			vts, "G_Gain", GainSlider));

	addAndMakeVisible(GainLabel);
	GainLabel.setText("Gain",
		juce::dontSendNotification);
	
	///////////////////////////////////////////////////////////////////////////////////////


	addAndMakeVisible(LSFrequencySlider);
	LSFrequencySlider.setSliderStyle(
		juce::Slider::SliderStyle::LinearVertical);
	LSFrequencyAttachment.reset(
		new juce::AudioProcessorValueTreeState::SliderAttachment(
			vts, "LS_frequency", LSFrequencySlider));

	addAndMakeVisible(LSFrequencyLabel);
	LSFrequencyLabel.setText("LS Frequency",
		juce::dontSendNotification);

	///////////////////////////////////////////////////////////////////////////////////////

	addAndMakeVisible(LSGainSlider);
	LSGainSlider.setSliderStyle(
		juce::Slider::SliderStyle::LinearVertical);
	LSGainAttachment.reset(
		new juce::AudioProcessorValueTreeState::SliderAttachment(
			vts, "LS_Gain", LSGainSlider));

	addAndMakeVisible(GainLabel);
	GainLabel.setText("Gain",
		juce::dontSendNotification);

	///////////////////////////////////////////////////////////////////////////////////////

	addAndMakeVisible(highpassButton);
	highpassAttachment.reset(
		new juce::AudioProcessorValueTreeState::
		ButtonAttachment(vts, "highpass", highpassButton));

	addAndMakeVisible(highpassButtonLabel);
	highpassButtonLabel.setText("Highpass", juce::dontSendNotification);

	setSize(1000, 400);
}

AllBandPassAudioProcessorEditor::~AllBandPassAudioProcessorEditor()
{
}

//==============================================================================
void AllBandPassAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void AllBandPassAudioProcessorEditor::resized()
{
	cutoffFrequencySlider.setBounds({ 15, 35, 100, 300 });
	cutoffFrequencyLabel.setBounds({ cutoffFrequencySlider.getX() + 30,
									cutoffFrequencySlider.getY() - 30,
									200, 50 });
	highpassButton.setBounds(
		{ cutoffFrequencySlider.getX(),
		 cutoffFrequencySlider.getY() +
		  cutoffFrequencySlider.getHeight() + 15,
		 30, 50 });
	///////////////////////////////////////////////////////////////////////////////////////
	
	QFrequencySlider.setBounds({ 115, 35, 300, 300 });
	QFrequencyLabel.setBounds({ QFrequencySlider.getX() + 30,
									QFrequencySlider.getY() - 30,
									200, 50 });

	GainSlider.setBounds({ 315, 35, 300, 300 });
	GainLabel.setBounds({ GainSlider.getX() + 30,
									GainSlider.getY() - 30,
									200, 50 });

	
	///////////////////////////////////////////////////////////////////////////////////////

	LSFrequencySlider.setBounds({ 515, 35, 300, 300 });
	LSFrequencyLabel.setBounds({ LSFrequencySlider.getX() + 30,
									LSFrequencySlider.getY() - 30,
									200, 50 });

	LSGainSlider.setBounds({ 715, 35, 300, 300 });
	LSGainLabel.setBounds({ LSGainSlider.getX() + 30,
									LSGainSlider.getY() - 30,
									200, 50 });


	///////////////////////////////////////////////////////////////////////////////////////

	highpassButtonLabel.setBounds(
		{ cutoffFrequencySlider.getX() + highpassButton.getWidth() + 15,
		 highpassButton.getY(),
		 cutoffFrequencySlider.getWidth() - highpassButton.getWidth(),
		 highpassButton.getHeight() });
}
