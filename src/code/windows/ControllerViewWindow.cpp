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
#include "ControllerViewWindow.h"

#include "GamepadInput.h"
#include "../imgui/imgui.h"

#include "imgui-sfml\imgui-SFML.h"

// only used for min() right now
#include <algorithm>


ControllerViewWindow::ControllerViewWindow()
{
	spriteSheetTexture.loadFromFile("Textures/ControllerSpriteSheet.png");
	drawTexture.create(ceil(spriteSheetTexture.getSize().x), ceil(spriteSheetTexture.getSize().y));
	drawTexture.clear(sf::Color::White);
	// for some reason rendertextures are flipped upside down
	auto texSize = drawTexture.getSize();
	auto view = sf::View(sf::Vector2f(texSize.x*0.5f, texSize.y*0.5f), sf::Vector2f(texSize.x, texSize.y));
	drawTexture.setView(sf::View(sf::Vector2f(ceil(texSize.x*0.5f), ceil(texSize.y*0.5f)),sf::Vector2f((float)texSize.x,-(float)texSize.y)));//,sf::Vector2f(texSize.x,-texSize.y)));
	sf::Vector2f basePosition(0, 100.0f);

	baseSprite.setTexture(spriteSheetTexture);
	
	baseSprite.setTextureRect(sf::IntRect(0,116,600.0f,268));
	baseSprite.setPosition(sf::Vector2f(0,100.0f));

	// DON'T LOOK BELOW YOU WILL CRY ;_;
	lJoystickBaseSprite.setTexture(spriteSheetTexture);
	lJoystickBaseSprite.setTextureRect(sf::IntRect(113,3,74,74));
	lJoystickBaseSprite.setPosition(sf::Vector2f(39,118));
	lJoystickSprite.setTexture(spriteSheetTexture);
	lJoystickSprite.setTextureRect(sf::IntRect(188, 1, 55, 55));
	lJoystickSprite.setPosition(sf::Vector2f(50, 125));
	lJoystickSpriteActive.setTexture(spriteSheetTexture);
	lJoystickSpriteActive.setTextureRect(sf::IntRect(188, 57, 55, 55));

	rJoystickBaseSprite.setTexture(spriteSheetTexture);
	rJoystickBaseSprite.setTextureRect(sf::IntRect(113, 3, 74, 74));
	rJoystickBaseSprite.setPosition(sf::Vector2f(223, 223));
	rJoystickSprite.setTexture(spriteSheetTexture);
	rJoystickSprite.setTextureRect(sf::IntRect(188, 1, 55, 55));
	rJoystickSprite.setPosition(sf::Vector2f(230, 230));
	rJoystickSpriteActive.setTexture(spriteSheetTexture);
	rJoystickSpriteActive.setTextureRect(sf::IntRect(188, 57, 55, 55));

	dpadSprite.setTexture(spriteSheetTexture);
	dpadSprite.setTextureRect(sf::IntRect(244,1,74,74));
	dpadSprite.setPosition(sf::Vector2f(90,220));

	lButtonSprite.setTexture(spriteSheetTexture);
	lButtonSprite.setTextureRect(sf::IntRect(33,0,79,26));
	lButtonSprite.setPosition(sf::Vector2f(70,66));

	lButtonSpriteActive.setTexture(spriteSheetTexture);
	lButtonSpriteActive.setTextureRect(sf::IntRect(33, 27, 79, 26));
	lButtonSpriteActive.setPosition(sf::Vector2f(70, 66));

	rButtonSprite.setTexture(spriteSheetTexture);
	rButtonSprite.setTextureRect(sf::IntRect(33, 0, 79, 26));
	rButtonSprite.setPosition(sf::Vector2f(262, 66));
	rButtonSpriteActive.setTexture(spriteSheetTexture);
	rButtonSpriteActive.setTextureRect(sf::IntRect(33, 27, 79, 26));
	rButtonSpriteActive.setPosition(sf::Vector2f(262, 66));

	rTriggerSprite.setTexture(spriteSheetTexture);
	rTriggerSprite.setTextureRect(sf::IntRect(0, 0, 32, 50));
	rTriggerSprite.setPosition(sf::Vector2f(100, 13));
	rTriggerActiveSprite.setTexture(spriteSheetTexture);
	rTriggerActiveSprite.setTextureRect(sf::IntRect(1, 51, 32, 50));
	rTriggerActiveSprite.setPosition(sf::Vector2f(100, 13));

	lTriggerSprite.setTexture(spriteSheetTexture);
	lTriggerSprite.setTextureRect(sf::IntRect(0, 0, 32, 50));
	lTriggerSprite.setPosition(sf::Vector2f(283, 13));
	lTriggerActiveSprite.setTexture(spriteSheetTexture);
	lTriggerActiveSprite.setTextureRect(sf::IntRect(1, 51, 32, 50));
	lTriggerActiveSprite.setPosition(sf::Vector2f(283, 13));

	backSprite.setTexture(spriteSheetTexture);
	backSprite.setTextureRect(sf::IntRect(34, 54, 32, 29));
	backSprite.setPosition(sf::Vector2f(147+32, 170));
	backSprite.setScale(-1,1);
	backSpriteActive.setTexture(spriteSheetTexture);
	backSpriteActive.setTextureRect(sf::IntRect(67, 54, 32, 29));
	backSpriteActive.setPosition(sf::Vector2f(147+32, 170));
	backSpriteActive.setScale(-1, 1);

	startSprite.setTexture(spriteSheetTexture);
	startSprite.setTextureRect(sf::IntRect(34, 54, 32, 29));
	startSprite.setPosition(sf::Vector2f(218, 170));
	startSpriteActive.setTexture(spriteSheetTexture);
	startSpriteActive.setTextureRect(sf::IntRect(67, 54, 32, 29));
	startSpriteActive.setPosition(sf::Vector2f(218, 170));

	ySprite.setTexture(spriteSheetTexture);
	ySprite.setTextureRect(sf::IntRect(34, 84, 31, 31));
	ySprite.setPosition(sf::Vector2f(305, 123));
	ySpriteActive.setTexture(spriteSheetTexture);
	ySpriteActive.setTextureRect(sf::IntRect(244, 76, 31, 31));
	ySpriteActive.setPosition(sf::Vector2f(305, 123));

	bSprite.setTexture(spriteSheetTexture);
	bSprite.setTextureRect(sf::IntRect(66, 84, 31, 31));
	bSprite.setPosition(sf::Vector2f(329, 150));
	bSpriteActive.setTexture(spriteSheetTexture);
	bSpriteActive.setTextureRect(sf::IntRect(276, 76, 31, 31));
	bSpriteActive.setPosition(sf::Vector2f(329, 150));

	xSprite.setTexture(spriteSheetTexture);
	xSprite.setTextureRect(sf::IntRect(98, 84, 31, 31));
	xSprite.setPosition(sf::Vector2f(275, 150));
	xSpriteActive.setTexture(spriteSheetTexture);
	xSpriteActive.setTextureRect(sf::IntRect(308, 76, 31, 31));
	xSpriteActive.setPosition(sf::Vector2f(275, 150));

	aSprite.setTexture(spriteSheetTexture);
	aSprite.setTextureRect(sf::IntRect(130, 84, 31, 31));
	aSprite.setPosition(sf::Vector2f(305, 185));
	aSpriteActive.setTexture(spriteSheetTexture);
	aSpriteActive.setTextureRect(sf::IntRect(340, 76, 31, 31));
	aSpriteActive.setPosition(sf::Vector2f(305, 185));


	lDpadSprite.setTexture(spriteSheetTexture);
	lDpadSprite.setTextureRect(sf::IntRect(320, 1, 25, 24));
	lDpadSprite.setPosition(sf::Vector2f(90+26, 220+25));
	lDpadSprite.setScale(-1, 1);
	rDpadSprite.setTexture(spriteSheetTexture);
	rDpadSprite.setTextureRect(sf::IntRect(320, 1, 25, 24));
	rDpadSprite.setPosition(sf::Vector2f(90+48, 220+25));
	uDpadSprite.setTexture(spriteSheetTexture);
	uDpadSprite.setTextureRect(sf::IntRect(320, 1, 25, 24));
	uDpadSprite.setPosition(sf::Vector2f(90 + 26, 220 + 25));
	uDpadSprite.rotate(-90.0f);
	dDpadSprite.setTexture(spriteSheetTexture);
	dDpadSprite.setTextureRect(sf::IntRect(320, 1, 25, 24));
	dDpadSprite.setPosition(sf::Vector2f(90 + 26+25, 220 + 25+23));
	dDpadSprite.rotate(90.0f);

	//sf::Sprite lDpadSprite;
	//sf::Sprite rDpadSprite;
	//sf::Sprite uDpadSprite;
	//sf::Sprite dDpadSprite;
}

ControllerViewWindow::~ControllerViewWindow()
{
}

void ControllerViewWindow::Render()
{
	GamepadInput input = GamepadInput::GetInstance();
	ImGui::Begin("ControllerView", 0);
	drawTexture.clear(sf::Color::Transparent);
	sf::Sprite t;

	t.setTexture(spriteSheetTexture);
	drawTexture.draw(baseSprite);
	drawTexture.draw(lJoystickBaseSprite);
	
	float joystickMoveDist = 30.0f;
	Vector lJoystick = input.GetStick(0, GamepadInput::SIDE::LEFT);
	lJoystickSpriteActive.setPosition(sf::Vector2f(50 + lJoystick.x*joystickMoveDist, 125 + lJoystick.y*-joystickMoveDist));
	if (lJoystick.x != 0.0f && lJoystick.y != 0.0f)
		drawTexture.draw(lJoystickSpriteActive);
	else
		drawTexture.draw(lJoystickSprite);

	drawTexture.draw(rJoystickBaseSprite);
	Vector rJoystick = input.GetStick(0, GamepadInput::SIDE::RIGHT);
	rJoystickSpriteActive.setPosition(sf::Vector2f(226 + rJoystick.x*joystickMoveDist, 229 + rJoystick.y*-joystickMoveDist));
	if (rJoystick.x != 0.0f && rJoystick.y != 0.0f)
		drawTexture.draw(rJoystickSpriteActive);
	else
		drawTexture.draw(rJoystickSprite);

	if(input.ButtonHeld(0, GamepadInput::GAMEPAD_LEFT_SHOULDER))
		drawTexture.draw(lButtonSpriteActive);
	else
		drawTexture.draw(lButtonSprite);
	
	drawTexture.draw(dpadSprite);
	if(input.ButtonHeld(0, GamepadInput::GAMEPAD_DPAD_LEFT))
		drawTexture.draw(lDpadSprite);
	if (input.ButtonHeld(0, GamepadInput::GAMEPAD_DPAD_RIGHT))
		drawTexture.draw(rDpadSprite);
	if (input.ButtonHeld(0, GamepadInput::GAMEPAD_DPAD_UP))
		drawTexture.draw(uDpadSprite);
	if (input.ButtonHeld(0, GamepadInput::GAMEPAD_DPAD_DOWN))
		drawTexture.draw(dDpadSprite);

	if (input.ButtonHeld(0, GamepadInput::GAMEPAD_RIGHT_SHOULDER))
		drawTexture.draw(rButtonSpriteActive);
	else
		drawTexture.draw(rButtonSprite);

	drawTexture.draw(rTriggerSprite);
	if (input.TriggerHeld(0, GamepadInput::SIDE::LEFT))
	{
		rTriggerActiveSprite.setTextureRect((sf::IntRect(1, 51, 32, 50* input.GetTrigger(0, GamepadInput::SIDE::LEFT))));
		drawTexture.draw(rTriggerActiveSprite);
	}

	drawTexture.draw(lTriggerSprite);
	if (input.TriggerHeld(0, GamepadInput::SIDE::RIGHT))
	{
		lTriggerActiveSprite.setTextureRect((sf::IntRect(1, 51, 32, 50 * input.GetTrigger(0, GamepadInput::SIDE::RIGHT))));
		drawTexture.draw(lTriggerActiveSprite);
	}
	if (input.ButtonHeld(0, GamepadInput::GAMEPAD_BACK))
		drawTexture.draw(backSpriteActive);
	else
		drawTexture.draw(backSprite);
	if (input.ButtonHeld(0, GamepadInput::GAMEPAD_START))
		drawTexture.draw(startSpriteActive);
	else
		drawTexture.draw(startSprite);

	if (input.ButtonHeld(0, GamepadInput::GAMEPAD_Y))
		drawTexture.draw(ySpriteActive);
	else
		drawTexture.draw(ySprite);

	if (input.ButtonHeld(0, GamepadInput::GAMEPAD_B))
		drawTexture.draw(bSpriteActive);
	else
		drawTexture.draw(bSprite);

	if (input.ButtonHeld(0, GamepadInput::GAMEPAD_A))
		drawTexture.draw(aSpriteActive);
	else
		drawTexture.draw(aSprite);

	if (input.ButtonHeld(0, GamepadInput::GAMEPAD_X))
		drawTexture.draw(xSpriteActive);
	else
		drawTexture.draw(xSprite);

	sf::Vector2f windowPos = ImGui::GetWindowPos();
	sf::Vector2f windowSize = ImGui::GetWindowSize();
	//ImGui::GetWindowContentRegionMin();
	
	void* textureHandle = (void*)drawTexture.getTexture().getNativeHandle();
	auto drawList = ImGui::GetWindowDrawList();
	ImVec2 uv0 = ImVec2(0,0);
	ImVec2 uv1 = ImVec2(1,1);

	sf::Vector2f size = windowSize;
	// fit and keep perspective
	auto texSize = drawTexture.getSize();
	float scaledHeight = size.x / texSize.x;
	float scaledWidth = size.y / texSize.y;
	float scale = std::min(scaledWidth, scaledHeight);
	size.x = texSize.x*scale;
	size.y = texSize.y*scale;

	drawList->AddImage(textureHandle, windowPos + sf::Vector2f(5, 30), windowPos + size + sf::Vector2f(-5, -5), uv0, uv1);
	ImGui::End();
}
