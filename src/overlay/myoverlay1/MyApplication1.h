class MyApplication1:public BaseApp{
	simtime_t sendPeriod;
	int numToSend;
	int largestKey;

	int numSent;
	int numReceived;

	cMessage *timerMsg;

	void initializeApp(int stage);
	void finishApp();
	void handleTimerEvent(cMessage* msg);
	void deliver(Overlay& key, cMessage* msg);
	void handleUDPMessage(cMessage* msg);
};
