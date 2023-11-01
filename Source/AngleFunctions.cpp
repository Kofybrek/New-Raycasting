#include <array>
#include <chrono>
#include <SFML/Graphics.hpp>

#include "Headers/AngleFunctions.hpp"
#include "Headers/Global.hpp"

float deg_difference(const float i_angle_0, const float i_angle_1)
{
	//This returns an angle between 0 and 180.
	return 180 - abs(abs(get_degrees(i_angle_0) - get_degrees(i_angle_1)) - 180);
}

float deg_to_rad(const float i_degrees)
{
	return gbl::RAYCASTING::PI * get_degrees(i_degrees) / 180;
}

float get_degrees(const float i_degrees)
{
	//Give this function any angle (in degrees) and it converts it to an angle between 0 and 360.
	return static_cast<float>(fmod(360 + fmod(i_degrees, 360), 360));
}

float get_radians(const float i_radians)
{
	//Same as above but in radians.
	return static_cast<float>(fmod(2 * gbl::RAYCASTING::PI + fmod(i_radians, 2 * gbl::RAYCASTING::PI), 2 * gbl::RAYCASTING::PI));
}

float rad_difference(const float i_angle_0, const float i_angle_1)
{
	//This returns an angle between 0 and PI (I think, I don't know how radians work).
	return gbl::RAYCASTING::PI - abs(abs(get_radians(i_angle_0) - get_radians(i_angle_1)) - gbl::RAYCASTING::PI);
}

float rad_to_deg(const float i_radians)
{
	return 180 * get_radians(i_radians) / gbl::RAYCASTING::PI;
}