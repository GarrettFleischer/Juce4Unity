/*
  ==============================================================================

    DeviceManagement.h
    Created: 25 Sep 2022 1:47:29pm
    Author:  Garrett Fleischer

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "MidiDeviceList.h"


using ExternalMidiInputCallback = void(*)(int id, juce::uint8, juce::uint8, juce::uint8);
using ExternalLogger = void(*)(char* str, int strlen);

class DeviceManagement final : juce::MidiInputCallback, juce::HighResolutionTimer
{
public:
    DeviceManagement();
    ~DeviceManagement() override;

    void registerCallback(ExternalMidiInputCallback callback);
    void registerLogger(ExternalLogger log);

    void refresh();

    [[nodiscard]] int deviceCount() const;

    [[nodiscard]] bool inputDeviceIsEnabled(int id) const;
    [[nodiscard]] bool outputDeviceIsEnabled(int id) const;

    bool setInputDeviceEnabled(int id, bool enabled);
    bool setOutputDeviceEnabled(int id, bool enabled);

    [[nodiscard]] bool deviceHasInput(int id) const;
    [[nodiscard]] bool deviceHasOutput(int id) const;

    void getDeviceName(int id, char* str, int strlen) const;
    void getDeviceIdentifier(int id, char* str, int strlen) const;

    void sendMessage(juce::uint8 byte1, juce::uint8 byte2, juce::uint8 byte3, int id = -1) const;
    void sendMessage(juce::uint8 byte1, juce::uint8 byte2, int id = -1) const;
    void sendMessage(juce::uint8 byte1, int id = -1) const;

    void sendSysEx(const void* data, int dataSize, int id = -1) const;

    void sendNoteOn(int channel, int midi, float velocity, int id = -1) const;
    void sendNoteOff(int channel, int midi, int id = -1) const;

    void sendAllNotesOff(int channel, int id = -1) const;
    void sendAllSoundOff(int channel, int id = -1) const;

private:
    struct MidiInputEvent
    {
        juce::MidiInput* source;
        const juce::MidiMessage& message;
    };

    MidiDeviceList devices;
    juce::Array<ExternalMidiInputCallback> callbacks;
    juce::Array<ExternalLogger> loggers;
    juce::Array<MidiInputEvent> events;
    juce::CriticalSection midiEventLock;

    void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;
    void hiResTimerCallback() override;

    void logExternal(const juce::String& message);

    void sendMidiMessage(int id, const juce::MidiMessage& message) const;
};
