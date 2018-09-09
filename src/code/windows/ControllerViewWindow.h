// This file is part of Enchanted Gamepad
// Copyright (C) <2018> Jonatan Olsson
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <SFML/graphics.hpp>

class ControllerViewWindow
{
private:
	sf::Texture spriteSheetTexture;
	sf::RenderTexture drawTexture;
public:
	// i know... ;_;
	sf::Sprite baseSprite;
	sf::Sprite lJoystickBaseSprite;
	sf::Sprite lJoystickSprite;
	sf::Sprite lJoystickSpriteActive;
	sf::Sprite rJoystickBaseSprite;
	sf::Sprite rJoystickSprite;
	sf::Sprite rJoystickSpriteActive;
	sf::Sprite dpadSprite;
	sf::Sprite backSprite;
	sf::Sprite backSpriteActive;
	sf::Sprite startSprite;
	sf::Sprite startSpriteActive;
	sf::Sprite xSprite;
	sf::Sprite ySprite;
	sf::Sprite bSprite;
	sf::Sprite aSprite;
	sf::Sprite xSpriteActive;
	sf::Sprite ySpriteActive;
	sf::Sprite bSpriteActive;
	sf::Sprite aSpriteActive;
	sf::Sprite lButtonSprite;
	sf::Sprite lButtonSpriteActive;
	sf::Sprite rButtonSprite;
	sf::Sprite rButtonSpriteActive;
	sf::Sprite lTriggerSprite;
	sf::Sprite lTriggerActiveSprite;
	sf::Sprite rTriggerSprite;
	sf::Sprite rTriggerActiveSprite;
	void Render();
	ControllerViewWindow();
	~ControllerViewWindow();
};