/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AllBandPassAudioProcessor::AllBandPassAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#else
	:
#endif
parameters(*this,
	nullptr,
	juce::Identifier("LowpassAndHighpassPlugin"),
	{ std::make_unique<juce::AudioParameterFloat>(
		"cutoff_frequency",
		"Cutoff Frequency",
		juce::NormalisableRange{20.f,
								20000.f,
								0.1f,
								0.2f,
								false},
		500.f),
	std::make_unique<juce::AudioParameterFloat>(
		"Q_frequency",
		"Q Frequency",
		juce::NormalisableRange{20.f,
								20000.f,
								0.1f,
								0.2f,
								false},
		500.f),
		std::make_unique<juce::AudioParameterFloat>(
		"G_Gain",
		"Gain",
		juce::NormalisableRange{-20.f,
								20.f,
								0.01f,
								0.02f,
								false},
		0.f),
		std::make_unique<juce::AudioParameterFloat>(
		"LS_frequency",
		"LS Frequency",
		juce::NormalisableRange{20.f,
								20000.f,
								0.1f,
								0.2f,
								false},
		500.f),
		std::make_unique<juce::AudioParameterFloat>(
		"LS_Gain",
		"LS Gain",
		juce::NormalisableRange{-20.f,
								20.f,
								0.01f,
								0.02f,
								false},
		0.f),
	 std::make_unique<juce::AudioParameterBool>(
		"highpass",
		"Highpass",
		false) }) {
	cutoffFrequencyParameter = parameters.getRawParameterValue("cutoff_frequency");
	QFrequencyParameter = parameters.getRawParameterValue("Q_frequency");
	GainParameter = parameters.getRawParameterValue("G_Gain");
	highpassParameter = parameters.getRawParameterValue("highpass");
	LSParameter = parameters.getRawParameterValue("LS_frequency");
	LSGain = parameters.getRawParameterValue("LS_Gain");
}

AllBandPassAudioProcessor::~AllBandPassAudioProcessor()
{
}

//==============================================================================
const juce::String AllBandPassAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AllBandPassAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AllBandPassAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AllBandPassAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AllBandPassAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AllBandPassAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AllBandPassAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AllBandPassAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AllBandPassAudioProcessor::getProgramName (int index)
{
    return {};
}

void AllBandPassAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AllBandPassAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	filter.setSamplingRate(static_cast<float>(sampleRate));
}

void AllBandPassAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AllBandPassAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AllBandPassAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();


	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	const auto cutoffFrequency = cutoffFrequencyParameter->load();
	const auto QFrequency = QFrequencyParameter->load();
	const auto Gain = GainParameter->load();
	const auto LSCutoff = LSParameter->load();
	const auto LSGains = LSGain->load();
	// in C++, std::atomic<T> to T conversion is equivalent to a load
	const auto highpass = *highpassParameter < 0.5f ? false : true;
	filter.setCutoffFrequency(cutoffFrequency);
	filter.setHighpass(highpass);
	filter.setQFrequency(QFrequency);
	filter.setGian(Gain);
	filter.setLSGain(LSGains);
	filter.setLSCutoffFrequency(LSCutoff);

	// perform the filtering
	filter.processBlock(buffer, midiMessages);
}

//==============================================================================
bool AllBandPassAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AllBandPassAudioProcessor::createEditor()
{
	return new AllBandPassAudioProcessorEditor(*this, parameters);
}

//==============================================================================
void AllBandPassAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{

}

void AllBandPassAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AllBandPassAudioProcessor();
}
