#pragma once
#include <glm/glm.hpp> 
#include <glm/gtx/string_cast.hpp>
#include "VertexBuffer.h"

typedef glm::vec3 Point;

struct Triangle
{
	Point a;
	Point b;
	Point c;

	Vertex& aV;
	Vertex& bV;
	Vertex& cV;
};

struct Plane {
	glm::vec3 normal;
	float distance;
};

struct TriangleIntersect
{
	int intersectionPointCount = 0;
	std::vector<Point> points;
};

struct Intersection
{
	float DistFromPlane(const Point& pt, const Plane& plane) {
		return glm::dot(pt, plane.normal) - plane.distance;
	}

	bool GetSegmentPlaneIntersection(Point P1, Point P2, Point& outP, Plane p)
	{
		float d1 = DistFromPlane(P1, p);
		float d2 = DistFromPlane(P2, p);

		if (d1 * d2 > 0)  // points on the same side of plane
			return false;

		float t = d1 / (d1 - d2); // 'time' of intersection point on the segment
		outP = P1 + t * (P2 - P1);

		return true;
	}

	void TrianglePlaneIntersection(Triangle tri, Plane p, std::vector<TriangleIntersect>& outSegTips)
	{
		TriangleIntersect triInt;
		Point IntersectionPoint;
		if (GetSegmentPlaneIntersection(tri.a, tri.b, IntersectionPoint, p))
			if (!isnan(IntersectionPoint.x))
			{
				triInt.intersectionPointCount += 1;
				triInt.points.push_back(IntersectionPoint);
				//outSegTips.push_back(IntersectionPoint);
				//std::cout << " A: " << glm::to_string(IntersectionPoint) << std::endl;
			}

		if (GetSegmentPlaneIntersection(tri.b, tri.c, IntersectionPoint, p))
			if (!isnan(IntersectionPoint.x))
			{
				triInt.intersectionPointCount += 1;
				triInt.points.push_back(IntersectionPoint);
				//outSegTips.push_back(IntersectionPoint);
				//std::cout << " B: " << glm::to_string(IntersectionPoint) << std::endl;
			}

		if (GetSegmentPlaneIntersection(tri.c, tri.a, IntersectionPoint, p))
			if (!isnan(IntersectionPoint.x))
			{
				triInt.intersectionPointCount += 1;
				triInt.points.push_back(IntersectionPoint);
				//outSegTips.push_back(IntersectionPoint);
				//std::cout << " C: " << glm::to_string(IntersectionPoint) << std::endl;
			}

		if(triInt.intersectionPointCount > 0)
		{
			outSegTips.push_back(triInt);
		}
	}

};