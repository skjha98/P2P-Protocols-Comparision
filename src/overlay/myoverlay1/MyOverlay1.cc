// Important! This line must be present for each module you extend (see MyApplication)
Define_Module(MyOverlay);

// To convert between IP addresses (which have bit 24 active), and keys (which don't), we'll need to set or remove this bit.
#define BIGBIT (1 << 24)

// Called when the module is being initialized
void MyOverlay::initializeOverlay(int stage)
{
    if (stage != MIN_STAGE_OVERLAY) return;         // see BaseApp.cc

    myKey = thisNode.getAddress().get4().getInt() & ~BIGBIT;  // get our key from our IP address

    // initialize the rest of variables
    numDropped = 0;
    if (!(par("enableDrops"))) {
        dropChance = 0;
    } else {
        dropChance = par("dropChance");
    }
}

// Called to set our own overlay key (optional)
void MyOverlay::setOwnNodeID()
{
    thisNode.key = OverlayKey(myKey);   // create the corresponding overlay key
}

// Called when the module is ready to join the overlay
void MyOverlay::joinOverlay()
{
    // Set the information of the previous step in the chain
    prevNode.setAddress(IPAddress(BIGBIT | (myKey - 1)));
    prevNode.setPort(thisNode.getPort());
    prevNode.key = OverlayKey(myKey - 1);

    // Set the information of the next step in the chain
    nextNode.setAddress(IPAddress(BIGBIT | (myKey + 1)));
    nextNode.setPort(thisNode.getPort());
    nextNode.key = OverlayKey(myKey + 1);

    // tell the simulator that we're ready
    setOverlayReady(true);
}

// Return whether we know if the given node is responsible for the key
bool MyOverlay::isSiblingFor(const NodeHandle& node,
                             const OverlayKey& key,
                             int numSiblings,
                             bool* err)
{
    if (node == thisNode && key == thisNode.key) { // is it our node and our key?
        return true;
    }
    return false;  // we don't know otherwise
}

// Return the next step for the routing of the given message
NodeVector *MyOverlay::findNode(const OverlayKey& key,
                                int numRedundantNodes,
                                int numSiblings,
                                BaseOverlayMessage* msg)
{
    NodeVector* nextHops;

    if (uniform(0, 1) < dropChance) {          // do we drop the packet?
        nextHops = new NodeVector(0);          // if yes, return an empty node vector 
        numDropped++;                          // (don't delete msg here! the overlay will delete it later)
        return nextHops;
    }

    // numRedundantNodes and numSiblings are both maximal 1    
    nextHops = new NodeVector(1);              

    if (key == thisNode.key) {                 // are we responsible? next step is this node
        nextHops->add(thisNode);
    } else if (key < thisNode.key) {           // is the key behind us? next step is the previous node
        nextHops->add(prevNode);
    } else {                                   // otherwise, the next step is the next node
        nextHops->add(nextNode);
    }
    return nextHops;
}

// Called when the module is about to be destroyed
void MyOverlay::finishOverlay()
{
    // remove this node from the overlay
    setOverlayReady(false);

    // save the statistics (see MyApplication)
    globalStatistics->addStdDev("MyOverlay: Dropped packets", numDropped);
}

// Return the max amount of siblings that can be queried about
int MyOverlay::getMaxNumSiblings()
{
    return 1;
}

// Return the max amount of redundant that can be queried about
int MyOverlay::getMaxNumRedundantNodes()
{
    return 1;
}