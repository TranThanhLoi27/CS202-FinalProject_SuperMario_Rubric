#pragma once

#include <string>

/// Observer-pattern interface for gameplay objects that publish sound requests.
class SoundObserver {
public:
    /// Allows safe destruction through the observer interface.
    virtual ~SoundObserver() = default;

    /// Receives a logical sound identifier published by a gameplay object.
    virtual void onSoundRequested(const std::string& soundId) = 0;
};
