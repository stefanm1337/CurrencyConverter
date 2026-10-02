#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
	    bool fromDropdownOpen = false;
	    bool toDropdownOpen = false;

	    void drawCurrencyDropdown(int x, int selected, bool open);
	    ofTrueTypeFont titleFont;
	    ofTrueTypeFont bodyFont;
	    std::string currencies[4] = { "GBP", "USD", "EUR", "SEK" };

	    double rates[4] = {};
		bool ratesLoaded = false;
		std::string apiStatus = "Rates not loaded";

		void loadRates();

	    int fromCurrency = 0;
	    int toCurrency = 1;

	    std::string amountText;
	    bool editingAmount = false;
	    ofRectangle amountBox { 40, 330, 440, 45 };
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
};
