#pragma once

float Lerp(float A, float B, float Alpha)
{
	return A * (1 - Alpha) + B * Alpha;
}

enum ItemType {
	Professor,
	GirlFriend,
	Book,
	GamePad,
	Calculator,
	Beer,

	Nothing
};

int ScoreOfItem[] = {
	-50,
	50,
	-20,
	20,
	-30,
	30
};