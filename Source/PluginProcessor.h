#pragma once
#include <JuceHeader.h>

struct SampleInfo {
    juce::String name;
    juce::String path;
    int keyRoot = 0;
    bool isMinor = true;
    double bpm = 90;
    int fileSizeKB = 0;
    juce::String getKeyName() const {
        const juce::String n[]={"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};
        return n[keyRoot] + (isMinor ? "m" : " Maj");
    }
    int getTransposeTo(int targetRoot) const {
        int d = targetRoot - keyRoot;
        if(d>6) d-=12; if(d<-6) d+=12;
        return d;
    }
};

class KeyDetector {
public:
    static std::pair<int,bool> detectKeyFromChroma(const std::array<float,12>& chroma) {
        const float major[12]={6.35,2.23,3.48,2.33,4.38,4.09,2.52,5.19,2.39,3.66,2.29,2.88};
        const float minor[12]={6.33,2.68,3.52,5.38,2.60,3.53,2.54,4.75,3.98,2.69,3.34,3.17};
        float best=-1; int br=0; bool bm=true;
        for(int r=0;r<12;++r){ float ms=0, mns=0; for(int i=0;i<12;++i){ ms+=chroma[(i+r)%12]*major[i]; mns+=chroma[(i+r)%12]*minor[i]; }
            if(ms>best){best=ms; br=r; bm=false;} if(mns>best){best=mns; br=r; bm=true;} }
        return {br,bm};
    }
};

class SampleMatchLiteAudioProcessor : public juce::AudioProcessor
{
public:
    SampleMatchLiteAudioProcessor();
    void prepareToPlay(double s,int) override;
    void releaseResources() override {}
    bool isBusesLayoutSupported(const BusesLayout& l) const override {return true;}
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override {return true;}
    const juce::String getName() const override {return "Totakan SampleMatch PRO";}
    bool acceptsMidi() const override {return false;}
    bool producesMidi() const override {return false;}
    bool isMidiEffect() const override {return false;}
    double getTailLengthSeconds() const override {return 0.0;}
    int getNumPrograms() override {return 1;}
    int getCurrentProgram() override {return 0;}
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override {return {};}
    void changeProgramName(int,const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock& d) override;
    void setStateInformation(const void* data,int size) override;
    juce::AudioProcessorValueTreeState apvts;
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    std::pair<int,bool> getDetectedKey() const {return {detectedRoot,detectedIsMinor};}
    double getDetectedBPM() const {return detectedBPM;}
    float getConfidence() const {return confidence;}
    juce::Array<SampleInfo> getLibrary() const {return library;}
    void analyzeIncomingAudio(const juce::AudioBuffer<float>& buffer);
private:
    std::atomic<float>* matchParam=nullptr; std::atomic<float>* blendParam=nullptr;
    std::atomic<float>* syncParam=nullptr; std::atomic<float>* transposeParam=nullptr;
    std::atomic<float>* flavorParam=nullptr; std::atomic<float>* mixParam=nullptr;
    int detectedRoot=9; bool detectedIsMinor=true; double detectedBPM=90; float confidence=0.96f;
    std::array<float,12> chroma{}; juce::Array<SampleInfo> library; float rms=0;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleMatchLiteAudioProcessor)
};
