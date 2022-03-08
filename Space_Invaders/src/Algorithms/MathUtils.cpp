#include "MathUtils.h"
#include <cmath>
#include <iostream>
std::ostream& operator<<(std::ostream& stream, const sf::Vector2f& vec);
sf::Vector2f VectorMath::getUnitaryVector(const sf::Vector2f& initpos, const sf::Vector2f& finalpos)
{
	sf::Vector2f vec(finalpos - initpos);
	vec /= std::sqrt(vec.x * vec.x + vec.y * vec.y);

	return(vec);

}

float VectorMath::getAngleinDegrees(const sf::Vector2f& finalpos, const sf::Vector2f& initpos)
{
	sf::Vector2f vec(finalpos - initpos);
	//std::cout << getUnitaryVector(vec) << "\n";
	float angle = atan2(vec.y, vec.x) *180.0f / M_PI ;

	//if (angle <0.0f) {
	//
	//	angle=360.0f + angle;
	//	
	//}
	return angle;

}

float VectorMath::getAngleinDegrees(const sf::Vector2f& vectorfromOrigin)
{
	float angle = atan2f(vectorfromOrigin.y, vectorfromOrigin.x) *180.0f /M_PI ;

	if (angle <0.0f) {
		
		angle = 360.0f + angle;
		
		
	}
	return angle;
}

sf::Vector2f VectorMath::getUnitaryVector(const sf::Vector2f& vec)
{
	return (vec/std::sqrt(vec.x * vec.x + vec.y * vec.y));
}


float VectorMath::getAngleinRadians(const sf::Vector2f& finalpos, const sf::Vector2f& initpos)
{
	return degreeToRadians(getAngleinDegrees(finalpos, initpos));
}

float VectorMath::getAngleinRadians(const sf::Vector2f& vectorfromOrigin)
{
	return degreeToRadians(getAngleinDegrees(vectorfromOrigin));
}

sf::Vector2f VectorMath::getAABBMidFront(const sf::Vector2f& topleftpos, const sf::Vector2f& size)
{
	return (sf::Vector2f(topleftpos.x + size.x, topleftpos.y + size.y/2.0f));

}

sf::Vector2f VectorMath::getAABBCenter(const sf::Vector2f& topleftpos, const sf::Vector2f& size)
{
	return (sf::Vector2f(topleftpos.x + size.x/2.0f, topleftpos.y + size.y / 2.0f));
}

float VectorMath::degreeToRadians(float angle)
{
	return(angle * M_PI/180.0f);
}

float VectorMath::radiansToDegrees(float angle)
{
	return(angle * 180.0f/ M_PI);

}