#pragma once

#include "vector.hpp"
#include "matrix.hpp"

#define LOG_AABB "LOG_AABB"

class AABB
{
public:
	FORCEINLINE AABB() {}
	FORCEINLINE AABB(const Vector3f& minExtents, const Vector3f& maxExtents);
	AABB(Vector3f* points, uint32_t amt);
	AABB(float* points, uint32_t amt, uint32_t stride=0);

	bool intersectRay(const Vector3f& start, const Vector3f& rayDir, float& point1, float& point2) const;
	bool intersectLine(const Vector3f& start, const Vector3f& end) const;
	FORCEINLINE bool intersects(const AABB& other) const;
	AABB transform(const Matrix& transform) const;

	Vector3f distanceTo(const AABB& other) const;

	FORCEINLINE AABB expand(float distance) const;
	FORCEINLINE AABB expand(const Vector3f& amt) const;
	FORCEINLINE AABB moveTo(const Vector3f& destination) const;
	FORCEINLINE Vector3f getCenter() const;
	FORCEINLINE Vector3f getExtents() const;
	FORCEINLINE Vector3f getMinExtents() const;
	FORCEINLINE Vector3f getMaxExtents() const;
	FORCEINLINE float getLengthByDim(uint32_t dimIndex) const;
	FORCEINLINE float getWidth() const;
	FORCEINLINE float getHeight() const;
	FORCEINLINE float getLength() const;
	FORCEINLINE void getCenterAndExtents(Vector3f& center, Vector3f& extents) const;
	FORCEINLINE float getVolume() const;
	FORCEINLINE AABB overlap(const AABB& other) const;
	FORCEINLINE bool contains(const Vector3f& point) const;
	FORCEINLINE bool contains(const AABB& other) const;

	FORCEINLINE AABB translate(const Vector3f& amt) const;
	FORCEINLINE AABB scaleFromCenter(const Vector3f& amt) const;
	FORCEINLINE AABB scaleFromOrigin(const Vector3f& amt) const;

	FORCEINLINE AABB addPoint(const Vector3f& other) const;
	FORCEINLINE AABB addAABB(const AABB& other) const;

	FORCEINLINE bool operator==(const AABB& other) const;
	FORCEINLINE bool operator!=(const AABB& other) const;
	FORCEINLINE bool equals(const AABB& other, float errorMargin=1.e-4f) const;
private:
	Vector3f extents[2];
};


FORCEINLINE AABB::AABB(const Vector3f& minExtents, const Vector3f& maxExtents)
{
	assert(minExtents <= maxExtents || minExtents >= maxExtents);

	if(minExtents >= maxExtents)
	{
		extents[0] = maxExtents;
		extents[1] = minExtents;
	}
	else
	{
		extents[0] = minExtents;
		extents[1] = maxExtents;
	}
}

FORCEINLINE bool AABB::intersects(const AABB& other) const
{
	return
		((extents[0].toVector() >= other.extents[1].toVector()) |
		 (extents[1].toVector() <= other.extents[0].toVector())).isZero3f();
}

FORCEINLINE AABB AABB::expand(float distance) const
{
	return expand(Vector3f(distance));
}

FORCEINLINE AABB AABB::expand(const Vector3f& amt) const
{
	return AABB(extents[0]-amt, extents[1]+amt);
}

FORCEINLINE AABB AABB::moveTo(const Vector3f& destination) const
{
	return translate(destination-getCenter());
}

FORCEINLINE Vector3f AABB::getCenter() const
{
	return (extents[1]+extents[0]) * 0.5f;
}

FORCEINLINE Vector3f AABB::getExtents() const
{
	return (extents[1]-extents[0]) * 0.5f;
}

FORCEINLINE Vector3f AABB::getMinExtents() const
{
	return extents[0];
}

FORCEINLINE Vector3f AABB::getMaxExtents() const
{
	return extents[1];
}

FORCEINLINE float AABB::getLengthByDim(uint32_t dimIndex) const
{
	return getMaxExtents()[dimIndex] - getMinExtents()[dimIndex];
}


FORCEINLINE float AABB::getWidth() const
{
	return getLengthByDim(0);
}

FORCEINLINE float AABB::getHeight() const
{
	return getLengthByDim(1);
}

FORCEINLINE float AABB::getLength() const
{
	return getLengthByDim(2);
}


FORCEINLINE void AABB::getCenterAndExtents(Vector3f& center, Vector3f& extents) const
{
	extents = (this->extents[1]-this->extents[0]) * 0.5f;
	center = this->extents[0]+extents;
}

FORCEINLINE float AABB::getVolume() const
{
	Vector3f lengths = extents[1]-extents[0];
	return lengths[0]*lengths[1]*lengths[2];
}

FORCEINLINE AABB AABB::overlap(const AABB& other) const
{
	return AABB(extents[0].max(other.extents[0]),
			extents[1].min(other.extents[1]));
}

FORCEINLINE bool AABB::contains(const Vector3f& point) const
{
	return ((point.toVector() <= extents[0].toVector()) |
	 (point.toVector() >= extents[1].toVector())).isZero3f();
}

FORCEINLINE bool AABB::contains(const AABB& other) const
{
	return
		((other.extents[0].toVector() <= extents[0].toVector()) |
	 	(other.extents[0].toVector() >= extents[1].toVector()) |
		(other.extents[1].toVector() <= extents[0].toVector()) |
	 	(other.extents[1].toVector() >= extents[1].toVector())).isZero3f();
}

FORCEINLINE AABB AABB::addPoint(const Vector3f& other) const
{
	return AABB(extents[0].min(other), extents[1].max(other));
}

FORCEINLINE AABB AABB::addAABB(const AABB& other) const
{
	return AABB(extents[0].min(other.extents[0]), extents[1].max(other.extents[1]));
}

FORCEINLINE AABB AABB::translate(const Vector3f& amt) const
{
	return AABB(extents[0]+amt, extents[1]+amt);
}

FORCEINLINE AABB AABB::scaleFromOrigin(const Vector3f& amt) const
{
	return AABB(extents[0]*amt, extents[1]*amt);
}

FORCEINLINE AABB AABB::scaleFromCenter(const Vector3f& amt) const
{
	Vector3f extents, center;
	getCenterAndExtents(center, extents);
	extents = extents * amt;
	return AABB(center-extents, center+extents);
}

FORCEINLINE bool AABB::operator==(const AABB& other) const
{
	return (extents[0] == other.extents[0]) && (extents[1] == other.extents[1]);
}

FORCEINLINE bool AABB::operator!=(const AABB& other) const
{
	return (extents[0] != other.extents[0]) && (extents[1] != other.extents[1]);
}

FORCEINLINE bool AABB::equals(const AABB& other, float errorMargin) const
{
	return
		extents[0].equals(other.extents[0], errorMargin) && 
		extents[1].equals(other.extents[1], errorMargin);
}

