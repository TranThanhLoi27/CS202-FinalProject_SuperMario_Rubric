#include <World/SpikeTrap.h>

void SpikeTrap::SetSpikes(std::vector<sf::FloatRect> _spikes){
    spikes = std::move(_spikes);
}