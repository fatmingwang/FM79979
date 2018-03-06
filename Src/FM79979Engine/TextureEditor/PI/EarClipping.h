#pragma once
//https://github.com/mapbox/earcut.hpp
#include "earcut.hpp"
namespace mapbox
{
	namespace util
	{

		template <>
		struct nth<0, Vector2>
		{
			inline static auto get(const Vector2 &t)
			{
				return t.x;
			};
		};
		template <>
		struct nth<1, Vector2>
		{
			inline static auto get(const Vector2 &t)
			{
				return t.y;
			};
		};

	} // namespace util
} // namespace mapbox


std::vector<size_t> GetIndicesFromPolygon(std::vector<Vector2>e_InputVector);
//