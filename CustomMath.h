#pragma once

float Lerp(float A, float B, float Alpha)
{
	return A * (1 - Alpha) + B * Alpha;
}
