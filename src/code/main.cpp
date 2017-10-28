#include "SimulateInput.h"
#include "GamepadInput.h"
#include "AppSettings.h"
#include "Script.h"
#include "Timer.h"

#include <SFML/graphics.hpp>
#include <SFML/window.hpp>

#include <iostream>

int main()
{
	//AppSettings appSettings;
	AppSettings::GetInstance().LoadSettingsFromScript("Scripts/globalSettings.lua");

	Script script;
	script.Load("Scripts/test.lua");

	Timer timer;
	timer.Start();
	sf::RenderWindow window(sf::VideoMode(500, 500), "Open Game Controller");
	bool exitApp = false;

	sf::Vector2i mousePos = sf::Mouse::getPosition();
	SimulateInput::GetInstance().OnMouseMoved(mousePos.x, mousePos.y);

	while (window.isOpen())
	{
		timer.Update();
		script.Run(timer.GetDeltaTime());

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::Resized)
				window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
		}
		window.clear(sf::Color(38,43,52,255));
		// Render
		window.display();
		sf::sleep(sf::Time(sf::milliseconds(2)));
		GamepadInput::GetInstance().Update(0.05f);
		//exitApp = gamepadInput.ButtonPressed(0, GamepadInput::GAMEPAD_START);

	}
	return 0;
}