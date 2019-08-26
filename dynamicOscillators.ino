//Example for dynamicly creating oscillators and dynamicly routing them within the teensy audio library. 
//In this example up to 16 oscillators can be created
//Written for teensy 3.2 with audio adapter board

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#define cVoiceAmount   16 
#define cMixerChannels 4
#define cMainVolume    0.5

AudioOutputI2S dac;
AudioControlSGTL5000 audioShield;

AudioSynthWaveform *oscillators;
AudioMixer4 *mixers;
AudioMixer4 synthOut;

AudioConnection outL(synthOut,0, dac, 0);
AudioConnection outR(synthOut,0, dac, 1);

//calculate how many mixers there need to be
byte cNumMixers = cVoiceAmount / cMixerChannels;

void setup() {
  cNumMixers = constrain(cNumMixers,1,4);

  //initialize audio 
  AudioMemory(24);
  audioShield.enable();
  audioShield.volume(cMainVolume);
  
  //create before initialisation
  oscillators = new AudioSynthWaveform[cVoiceAmount];
  mixers = new AudioMixer4[cNumMixers];
  
  //initialize everything
  for(int i = 0; i < cVoiceAmount; i++){
    //initialize oscillators
    float fVoiceAmount = (float)cVoiceAmount;
    float oscillatorVolume = ((1. - (float)i/fVoiceAmount) / fVoiceAmount + 1./fVoiceAmount) * 0.5;
    oscillators[i].begin(oscillatorVolume,i*100+100,WAVEFORM_SINE); 
    delay(500);
        
    byte mixerCount = i/cMixerChannels;
    //make connections
    new AudioConnection(oscillators[i],0,mixers[mixerCount],i%cMixerChannels);
    new AudioConnection(mixers[mixerCount],0,synthOut,mixerCount);   
  }//for

  
}//setup

void loop() {

}
