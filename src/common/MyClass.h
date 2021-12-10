#include <string>
using std::string;
#include<OverlayKey.h>
#include<NodeHandle.h>
#include <omnetpp.h>
#ifndef MYCLASS_H
#define MYCLASS_H
class MyClass {
	public:
		MyClass();
		static void print();

		static std::map<OverlayKey, int> lookupHops;
		static std::map<OverlayKey, double> luDelay;
		static void addHop(int hopCount);
		static void addKeys(int keys);
		static void reset();
		static void printLookups();
		static void addLookup(OverlayKey key, double simtime);
		static void addDelay(double delay);
		static void removeLookup(OverlayKey key);
		static bool incrementHop(OverlayKey key);

		static double hops;
		static double finished_lookups;
		static double removed_lookups;
		static double lookup_calls;
		static int remaining_lookups ;

		static double numberOfNodes;
		static double numberOfKeys;
		static double maxKeys;
		static double minKeys;

		static double stabilizeInterval;
		static double fixFingersInterval;
		static double time;
		static int terminalsAdded;
		static int terminalsRemoved;
		static int networkSize;

		static double hopsPerLookup;
		static double lookupMessageOverhead;
		static double lookupDelay;
		static double keysPerNode;
};
#endif