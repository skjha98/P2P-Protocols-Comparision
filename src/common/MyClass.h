// MyClass.h
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
		static void addMaintenance(int join, int notify, int stabilize,
		int newSuc, int fixFing);
		static void addHop(int hopCount);
		static void addHop2(int hopCount);
		static void addKeys(int keys);
		static void reset();
		static void printLookups();
		static void addLookup(OverlayKey key, double simtime);
		static void addDelay(double delay);
		static void removeLookup(OverlayKey key);
		static bool isValidLookup(OverlayKey key);
		static void addHandle(IPvXAddress ip, NodeHandle handle);
		static NodeHandle getHandle(IPvXAddress ip);

		static double hops;
		static bool iterative; 
		static double finished_lookups;
		static double removed_lookups;
		static double lookup_calls;
		static int remaining_lookups ;

		static double numberOfNodes;
		static double numberOfKeys;
		static double maxKeys;
		static double minKeys;
		static int maxDelay;

		static double stabilizeInterval;
		static double fixFingersInterval;
		static double time;
		static string dht;
		static double lookupFreq;
		static double churnRate;
		static int terminalsAdded;
		static int terminalsRemoved;
		static int networkSize;
		static int routingTableSize;
		static string routingModel;
		static int aid;
		// #### KADEMLIA ############
		static int k;
		static int s;
		static int b;
		static double hopsPerLookup;
		static double lookupMessageOverhead;
		static double lookupDelay;
		static double keysPerNode;
};
#endif