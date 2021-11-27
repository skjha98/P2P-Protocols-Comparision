class MyOverlay1: public BaseOverlay{
	public:
		int myKey;
		NodeHandle prevNode;
		NodeHandle nextNode;

		double dropChance;
		
		int numDropped;

		void initializeOverlay(int stage);
		void setOwnNodeID();
		void joinOverlay();
		void finishOverlay();

		NodeVector* findNode(const Overlay& key, int numRedundantNodes, int numSiblings, BaseOverlayMessage* msg);

		bool isSiblingFor(const NodeHandle& node, const OverlayKey& key, int numSiblings, bool* err);

		int geMaxNumSiblings();

		int getMaxNumRedundantNodes();

};
