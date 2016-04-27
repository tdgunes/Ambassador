//
// Created by Taha Doğan Güneş on 27/04/16.
//

#ifndef AMBASSADOR_BEACON_H
#define AMBASSADOR_BEACON_H


#include <stdint.h>
#include <functional>
#include <string>

class Beacon {
public:
    uint16_t major; // 2 bytes - defines a sub-region within a larger region defined by the UUID.
    uint16_t minor; // 2 bytes - allows further subdivision of region or use case.

    friend bool operator==(const Beacon &p1, const Beacon &p2) {
        return ((p1.major == p2.major) && (p1.minor == p2.minor));
    }
};


namespace std {
    template<>
    struct hash<Beacon> {
        std::size_t operator()(Beacon const &b) const {
            std::string s = std::to_string(b.major) + std::to_string(b.minor);
            return std::hash<std::string>()(s);
        }
    };
}


#endif //AMBASSADOR_BEACON_H
