#include "stdafx.h"
#include "EarClipping.h"

std::vector<size_t> GetIndicesFromPolygon(std::vector<Vector2>e_InputVector)
{
	std::vector<std::vector<Vector2>> polygon;
	polygon.push_back(e_InputVector);
	std::vector<size_t> indices = mapbox::earcut<size_t>(polygon);
	return indices;
}