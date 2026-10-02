#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetWindowTitle("Currency Converter");
	ofSetWindowShape(540, 510);
	titleFont.load("segoeui.ttf", 22, true, true);
	bodyFont.load("segoeui.ttf", 11, true, true);
	loadRates();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(18, 23, 34);

	// Header background
	ofSetColor(27, 38, 58);
	ofDrawRectangle(0, 0, ofGetWidth(), 82);

	// Blue accent along the left edge
	ofSetColor(65, 170, 255);
	ofDrawRectangle(0, 0, 5, 82);

	// Title
	ofSetColor(245, 248, 255);
	titleFont.drawString("Currency Converter", 40, 38);

	// API status
	ofSetColor(170, 190, 215);
	bodyFont.drawString(apiStatus, 40, 65);

	// Colour for the currency labels below
	ofSetColor(235, 240, 250);
	bodyFont.drawString("FROM", 40, 100);
	bodyFont.drawString("TO", 280, 100);

	drawCurrencyDropdown(40, fromCurrency, fromDropdownOpen);
	drawCurrencyDropdown(280, toCurrency, toDropdownOpen);

	ofSetColor(255);
	bodyFont.drawString(
		"Amount in " + currencies[fromCurrency], 40, 315);

	ofSetColor(editingAmount
			? ofColor(65, 85, 110)
			: ofColor(45, 55, 70));
	ofDrawRectangle(amountBox);

	std::string display = amountText;
	if (editingAmount) {
		display += "|";
	} else if (display.empty()) {
		display = "Click here and type an amount";
	}

	ofSetColor(255);
	bodyFont.drawString(display, 55, 358);

	if (ratesLoaded && !amountText.empty()) {
		double amount = std::stod(amountText);
		double result = amount
			* rates[toCurrency] / rates[fromCurrency];

		bodyFont.drawString(
			"Result: " + ofToString(result, 2)
				+ " " + currencies[toCurrency],
			40, 420);
	} else {
		bodyFont.drawString("Result: --", 40, 420);
	}

	ofSetColor(180);
	bodyFont.drawString("Use Backspace to edit.", 40, 465);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (!editingAmount) {
		return;
	}

	if (key == OF_KEY_BACKSPACE) {
		if (!amountText.empty()) {
			amountText.pop_back();
		}
	} else if (key >= '0' && key <= '9') {
		if (amountText.size() < 12) {
			amountText += static_cast<char>(key);
		}
	} else if (key == '.') {
		if (amountText.find('.') == std::string::npos
			&& amountText.size() < 12) {
			if (amountText.empty()) {
				amountText = "0";
			}
			amountText += '.';
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	// Open or close the FROM menu
	if (ofRectangle(40, 115, 200, 38).inside(x, y)) {
		fromDropdownOpen = !fromDropdownOpen;
		toDropdownOpen = false;
		editingAmount = false;
		return;
	}

	// Open or close the TO menu
	if (ofRectangle(280, 115, 200, 38).inside(x, y)) {
		toDropdownOpen = !toDropdownOpen;
		fromDropdownOpen = false;
		editingAmount = false;
		return;
	}

	// Select an option from an open menu
	for (int i = 0; i < 4; i++) {
		int rowY = 157 + i * 34;

		if (fromDropdownOpen && ofRectangle(40, rowY, 200, 34).inside(x, y)) {
			fromCurrency = i;
			fromDropdownOpen = false;
			return;
		}

		if (toDropdownOpen && ofRectangle(280, rowY, 200, 34).inside(x, y)) {
			toCurrency = i;
			toDropdownOpen = false;
			return;
		}
	}

	// Clicking elsewhere closes the menus
	fromDropdownOpen = false;
	toDropdownOpen = false;
	editingAmount = amountBox.inside(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::loadRates() {
	ratesLoaded = false;

	std::string apiKey = ofTrim(ofBufferFromFile("apikey.txt").getText());

	if (apiKey.empty()) {
		apiStatus = "Missing API key in bin/data/apikey.txt";
		return;
	}

	std::string url = "https://api.freecurrencyapi.com/v1/latest"
					  "?base_currency=USD&currencies=GBP,USD,EUR,SEK";

	ofHttpRequest request(url, "currencyRates");
	request.headers["apikey"] = apiKey;
	request.timeoutSeconds = 15;

	ofURLFileLoader loader;
	ofHttpResponse response = loader.handleRequest(request);

	if (response.status != 200) {
		apiStatus = "Could not load rates. HTTP: "
			+ ofToString(response.status);
		return;
	}

	try {
		ofJson json = ofJson::parse(response.data.getText());
		double newRates[4];

		for (int i = 0; i < 4; i++) {
			newRates[i] = json.at("data").at(currencies[i]).get<double>();

			if (!(newRates[i] > 0.0)) {
				apiStatus = "API returned an invalid rate";
				return;
			}
		}

		for (int i = 0; i < 4; i++) {
			rates[i] = newRates[i];
		}

		ratesLoaded = true;
		apiStatus = "Exchange rates loaded from API";
	} catch (const std::exception &) {
		apiStatus = "Could not read the API response";
	}
}

//--------------------------------------------------------------
void ofApp::drawCurrencyDropdown(int x, int selected, bool open) {
	// Selected currency
	ofSetColor(45, 60, 80);
	ofDrawRectangle(x, 115, 200, 38);

	ofSetColor(255);
	bodyFont.drawString(currencies[selected], x + 15, 140);

	// Dropdown arrow
	ofDrawTriangle(
		x + 175, 130,
		x + 185, 130,
		x + 180, 136);

	if (open) {
		for (int i = 0; i < 4; i++) {
			int y = 157 + i * 34;

			bool hovered = ofRectangle(x, y, 200, 34).inside(ofGetMouseX(), ofGetMouseY());

			if (hovered) {
				ofSetColor(65, 155, 215); // Highlight under the mouse
			} else if (i == selected) {
				ofSetColor(40, 130, 190); // Selected currency
			} else {
				ofSetColor(40, 50, 68); // Normal background
			}

			ofDrawRectangle(x, y, 200, 34);

			ofSetColor(255);
			bodyFont.drawString(currencies[i], x + 15, y + 23);
		}
	}
}
