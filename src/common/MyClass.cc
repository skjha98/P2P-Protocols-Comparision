// MyClass.cc
#include <iostream>

using std::cout;
using std::cin;
using std::ios;
using std::cerr;
using std::endl;
using namespace std;

#include <fstream>

using std::ofstream;

#include <cstdlib>

#include <string>

using std::string;

#include "MyClass.h"

typedef std::map < OverlayKey, int > lookupHops;
typedef std::map < OverlayKey, double > luDelays;

lookupHops lookups;
luDelays delays;
double MyClass::hops = 0;
double MyClass::finished_lookups = 0;
double MyClass::time = 0;
bool MyClass::iterative = true;

double MyClass::removed_lookups = 0; // used for calculating lookups that take too long and are removed
double MyClass::lookup_calls = 0;
int MyClass::remaining_lookups = 0; // lookups that are in progress when simulation stops

double MyClass::numberOfNodes = 0;
double MyClass::numberOfKeys = 0;
double MyClass::maxKeys = 0;
double MyClass::minKeys = 1000;
int MyClass::maxDelay = INT_MAX;

string MyClass::dht = "empty";
double MyClass::lookupFreq = 0;
double MyClass::churnRate = 0;
int MyClass::terminalsAdded = 0;
int MyClass::terminalsRemoved = 0;

int MyClass::networkSize = 0;
double MyClass::stabilizeInterval = 0;
double MyClass::fixFingersInterval = 0;
int MyClass::routingTableSize = 0;
string MyClass::routingModel = "empty";
int MyClass::aid = 0;

// ### KADEMLIA #######

int MyClass::k = 0;
int MyClass::s = 0;
int MyClass::b = 0;

double MyClass::hopsPerLookup = 0;
double MyClass::lookupMessageOverhead = 0;
double MyClass::lookupDelay = 0;

double MyClass::keysPerNode = 0;

MyClass::MyClass() {}

// calculates the hopcount
void MyClass::addHop(int hopCount) {
    hops = hops + hopCount;
    finished_lookups++;
    hopsPerLookup = hops / finished_lookups;
}
// calculates the number of keys in the network
// and the max/min number of keys in one node
void MyClass::addKeys(int keys) {
    numberOfNodes++;
    numberOfKeys += keys;
    keysPerNode = numberOfKeys / numberOfNodes;

    if (keys < minKeys) {
        minKeys = keys;
    }
    if (keys > maxKeys) {
        maxKeys = keys;
    }
}
// inserts lookups and simTimes to map containers
void MyClass::addLookup(OverlayKey key, double simtime) {
    lookups.insert(make_pair(key, 0));
    delays.insert(make_pair(key, simtime));
    lookup_calls++;
}
void MyClass::addDelay(double delay) {
    lookupDelay += delay;
}
// removes lookups from the map container
void MyClass::removeLookup(OverlayKey key) {
    bool skip = false;

    // checks that the key exists in the lookups map
    lookupHops::iterator it;
    it = lookups.find(key);
    if (it == lookups.end())
        return;

    // if delay > 5 seconds the lookup is removed from the map
    double delaytmp = (simTime().dbl() - delays[key]);
    if (delaytmp > maxDelay) {
        cout << "DELAY " << delaytmp << endl;
        delays.erase(key);
        lookups.erase(key);
        skip = true;
        ++removed_lookups;
    }
    // removes all the lookups currently having a delay > 5 seconds from the map
    luDelays::iterator iter;
    for (iter = delays.begin(); iter != delays.end(); iter++) {
        double delaytmp = (simTime().dbl() - (iter -> second));
        if (delaytmp > maxDelay) {
            OverlayKey temp = iter -> first;

            delays.erase(temp);
            lookups.erase(temp);
            ++removed_lookups;
        }
    }
    if (skip == false) {
        // calculates the number of hops in this lookup
        int hoptmp = (lookups[key] - 1) * 2;
        addHop(hoptmp);
        lookups.erase(key);

        // calculates the delay of this lookup
        double delaytmp = (simTime().dbl() - delays[key]);
        addDelay(delaytmp);
        delays.erase(key);
    }
}

// checks that there is a ongoing lookup for a specific key
bool MyClass::isValidLookup(OverlayKey key) {
    map < OverlayKey, int, double > ::iterator iter;
    bool tmp = false;
    for (iter = lookups.begin(); iter != lookups.end(); iter++) {
        if (iter -> first.compareTo(key) == 0 && time !=
            simTime().dbl()) {
            tmp = true;
            iter -> second++;
        }
    }
    // simulation time stored to allow only one call of this function at a time
    time = simTime().dbl();
    return tmp;
}
// resets the initial values
void MyClass::reset() {
    MyClass::hops = 0;
    MyClass::finished_lookups = 0;
    MyClass::removed_lookups = 0;
    MyClass::lookup_calls = 0;
    MyClass::remaining_lookups = 0;
    MyClass::numberOfNodes = 0;
    MyClass::numberOfKeys = 0;
    MyClass::maxKeys = 0;
    MyClass::minKeys = 1000;
    MyClass::stabilizeInterval = 0;
    MyClass::fixFingersInterval = 0;
    MyClass::dht = "empty";
    MyClass::lookupFreq = 0;
    MyClass::churnRate = 0;
    MyClass::networkSize = 0;
    MyClass::routingTableSize = 0;
    MyClass::routingModel = "empty";
    MyClass::aid = 0;
    MyClass::iterative = true;
    MyClass::k = 0;
    MyClass::s = 0;
    MyClass::b = 0;
    MyClass::hopsPerLookup = 0;
    MyClass::lookupMessageOverhead = 0;
    MyClass::lookupDelay = 0;
    MyClass::keysPerNode = 0;
}
// prints the results to results.dat
void MyClass::print() {
    ofstream outResultFile("results.dat", ios::app);
    outResultFile << "DHTused " << dht << " variable" << " variable" << ' ' << "RoutingModel " << routingModel << ' ' << "LookupCalls " << lookup_calls << ' ' << "FinishedLookups: " << finished_lookups << ' ' << "removedLookups " << removed_lookups << ' ' << "SuccessRate " << finished_lookups / (lookup_calls - remaining_lookups) << ' ' << "Added " << terminalsAdded <<' ' << "Removed " << terminalsRemoved << ' ' << "HopsPerFinishedLookup: " << hopsPerLookup << ' ' << "Lookup_delay: " << lookupDelay / finished_lookups << ' ' << "Network_Size: " << networkSize << ' ' << "Stabilize_interval: " << stabilizeInterval << ' ' << "FixFingers_interval " << fixFingersInterval << ' ' << "Hops: " << hops << ' ' << "Keys_total " << numberOfKeys << ' ' << "Max_keys " << maxKeys << ' ' << "Min_keys " << minKeys << ' ' << "Keys_per_node: " << keysPerNode << endl;
}