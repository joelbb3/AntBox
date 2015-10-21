#include <iostream>
#include <fstream>
#include <algorithm>
#include <SFML/Graphics.hpp>

using namespace std;

int main() {
	float input;
	

	ifstream infile;
	infile.open("filteredframes1.txt");
	if (!infile.is_open())
	{
		cout << "Could not open file.";
		cin >> input;
		return 0;
	}
	
	int y, x;
	infile >> y >> x;
	float* data = new float[x*y];
	int i = 0;
	while (infile >> input)
	{
		data[i++] = input;
	}
	infile.close();
	float min, max, dif;
	//min = *min_element(data, data + x*y);
	min = 745;
	//max = *max_element(data, data + x*y);
	max = 808;
	dif = max - min;
	//cout << "The smallest element is " << min << '\n';
	//cout << "The largest element is " << max << '\n';

	typedef unsigned char Uint8;
	Uint8* RGBA;
	RGBA = new Uint8[x*y*4];

	for (int i = 0; i < x*y; ++i) {
		float hue;
		if (data[i] < min) {
			hue = 0;
		}
		else if (data[i] > max) {
			hue = 240;
		}
		else {
			hue = ((data[i] - min) / dif) * 240;
		}

		//Convert from hue to RGB: http://www.rapidtables.com/convert/color/hsl-to-rgb.htm
		float C, X, m;
		C = (1 - fabs(2 * 0.5 - 1)) * 1;
		X = C*(1 - fabs(fmod(hue / 60.0, 2) - 1));
		m = 0.5 - C / 2.0;

		float mods[3];
		if (hue >= 0 && hue < 60) {
			mods[0] = C;
			mods[1] = X;
			mods[2] = 0;
		}
		else if (hue >= 60 && hue < 120) {
			mods[0] = X;
			mods[1] = C;
			mods[2] = 0;
		}
		else if (hue >= 120 && hue < 180) {
			mods[0] = 0;
			mods[1] = C;
			mods[2] = X;
		}
		else if (hue >= 180 && hue < 240) {
			mods[0] = 0;
			mods[1] = X;
			mods[2] = C;
		}
		else if (hue >= 240 && hue < 300) {
			mods[0] = X;
			mods[1] = 0;
			mods[2] = C;
		}
		else {
			cout << "what the hell."; //Something went horribly wrong.
			cin >> input;
		}

		RGBA[i * 4] = (mods[0] + m)*255;
		RGBA[i * 4 + 1] = (mods[1] + m)*255;
		RGBA[i * 4 + 2] = (mods[2] + m)*255;
		RGBA[i * 4 + 3] = 255;

		//cout << mods[0] + m << " " << mods[1] + m << " " << mods[2] + m << "\n";
	}
	//cout << (int)RGBA[0] << " " << (int)RGBA[1] << " " << (int)RGBA[2] << "\n";
	//cout << (int)RGBA[45 * 4 + 0] << " " << (int)RGBA[45 * 4 + 1] << " " << (int)RGBA[45 * 4 + 2] << "\n";
	//cout << (int)RGBA[158 * 4 + 0] << " " << (int)RGBA[158 * 4 + 1] << " " << (int)RGBA[158 * 4 + 2] << "\n";
	//cout << (int)RGBA[4000 * 4 + 0] << " " << (int)RGBA[4000 * 4 + 1] << " " << (int)RGBA[4000 * 4 + 2] << "\n";


	sf::Texture bgTexture;
	bgTexture.create(x, y);
	bgTexture.update(RGBA);
	sf::Sprite bgSprite;
	bgSprite.setTexture(bgTexture);

	sf::RenderWindow window(sf::VideoMode(x, y), "Demo Window");

	while (window.isOpen()) {
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(sf::Color::Black);

		window.draw(bgSprite);

		window.display();
	}


	return 1;
}