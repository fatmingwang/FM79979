#ifndef _LINE_H_
#define _LINE_H_
#include "Vector3.h"
namespace FATMING_CORE
{
	class cLine
	{
	public:
		Vector3 m_vBegine;
		Vector3	m_vEnd;

		cLine(const Vector3 e_vBegin, const Vector3 e_vEnd)
		{
			m_vBegine = e_vBegin; 
			m_vEnd = e_vEnd;
		}

		enum IntersectResult { PARALLEL, COINCIDENT, NOT_INTERESECTING, INTERESECTING };

		inline	IntersectResult Intersect(const cLine e_Line, Vector3* e_pvIntersection)
		{
			float denom = ((e_Line.m_vEnd.y - e_Line.m_vBegine.y)*(m_vEnd.x - m_vBegine.x)) -
						  ((e_Line.m_vEnd.x - e_Line.m_vBegine.x)*(m_vEnd.y - m_vBegine.y));

			float nume_a = ((e_Line.m_vEnd.x - e_Line.m_vBegine.x)*(m_vBegine.y - e_Line.m_vBegine.y)) -
						   ((e_Line.m_vEnd.y - e_Line.m_vBegine.y)*(m_vBegine.x - e_Line.m_vBegine.x));

			float nume_b = ((m_vEnd.x - m_vBegine.x)*(m_vBegine.y - e_Line.m_vBegine.y)) -
						   ((m_vEnd.y - m_vBegine.y)*(m_vBegine.x - e_Line.m_vBegine.x));

			if(denom == 0.0f)
			{
				if(nume_a == 0.0f && nume_b == 0.0f)
				{
					return COINCIDENT;
				}
				return PARALLEL;
			}

			float ua = nume_a / denom;
			float ub = nume_b / denom;

			if(ua >= 0.0f && ua <= 1.0f && ub >= 0.0f && ub <= 1.0f)
			{
				// Get the intersection point.
				e_pvIntersection->x = m_vBegine.x + ua*(m_vEnd.x - m_vBegine.x);
				e_pvIntersection->y = m_vBegine.y + ua*(m_vEnd.y - m_vBegine.y);

				return INTERESECTING;
			}

			return NOT_INTERESECTING;
		}
	};
//end namespace FATMING_CORE
}

#endif