Define_Module(MyApplication1.h);

void MyApplication::initializeApp(int stage){
	if(stage!=MIN_STAGE_APP)	return;
	sendPeriod = par("sendPeriod");
	numToSend = par("numToSend");
	largestKey = par("largestKey");

	numSent = 0;
	numReceived = 0;

	WATCH(numSend);
	WATCH(numReceived);

	timerMsg = new cMessage("MyApplication Working Timer");
	scheduleAt(simTimer() + sendPeriod, timerMsg);

	bindToPort(2048);
}

void MyApplication::finishApp(){
	delete timerMsg;

	globalStatistics->addStdDev("MyApplication: Sent Packets", numSend);
	globalStatistics->addStdDev("MyApplication: Received Packets", numReceived);
}

void MyApplication1::handleTimerEvent(cMessage* msg){
	if(msg == timerMsg){
		scheduleAt(simTime() + sendPeriod, timerMsg);
		if(underlayConfigurator->isInitPhase()) return;

		for(int i =0; i<numToSend; i++){
			OverlayKey randomKey(intuniform(1, largestKey));

			MyMessage1 *myMessage;
			myMessage = new MyMessaged1();
			myMessage->setType(MYMSG_PING);
			myMessgae->setSenderAddress(thisNode);
			myMessage->setByteLength(100);

			RECORD_STATS(numSent++);

			callRoute(randomKey, myMessage);

		}
	}else{
		delete msg;
	}
}

void MyApplication1::deliver(OverlayKey& key, cMessage* msg){
	MyMessage1 *myMsg = dynamic_cast<MyMessage1*>(msg);
	if(myMsg == 0){
		delete msg;
		return;
	}

	if(myMsg->getType() == MYMSG_PING){
		myMsg->setType(MYMSG_PONG);
		sendMessageToUDP(myMsg->getSenderAddress(), myMsg);
	}else{
		delete msg;
	}
}

void MyApplication1::handleUDPMessage(cMessage* msg){
	MyMessage1 *myMsg = dynamic_cast<MyMessage1*>(msg);

	if(myMsg && myMsg->getType() == MYMSG_PONG){
		RECORD_STATS(numReceived++);
	}

	delete msg;
}
