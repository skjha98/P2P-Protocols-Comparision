package oversim.overlay.myoverlay1;

import oversim.common.BaseOverlay;

simple MyOverlay1 extends BaseOverlay
{
	parameters:
		@class(MyOverlay1);
		bool enableDrops;
		double dropChange;
}

