#pragma once

#include "renderDevice.hpp"
#include "math/aabb.hpp"

class IndexedModel
{
public:
	IndexedModel() :
		instancedElementsStartIndex((uint32_t)-1) {}
	uint32_t createVertexArray(RenderDevice& device,
			enum RenderDevice::BufferUsage usage) const;

	void allocateElement(uint32_t elementSize);
	void setInstancedElementStartIndex(uint32_t elementIndex);

	void addElement1f(uint32_t elementIndex, float e0);
	void addElement2f(uint32_t elementIndex, float e0, float e1);
	void addElement3f(uint32_t elementIndex, float e0, float e1, float e2);
	void addElement4f(uint32_t elementIndex, float e0, float e1, float e2, float e3);

	void addIndices1i(uint32_t i0);
	void addIndices2i(uint32_t i0, uint32_t i1);
	void addIndices3i(uint32_t i0, uint32_t i1, uint32_t i2);
	void addIndices4i(uint32_t i0, uint32_t i1, uint32_t i2, uint32_t i3);

	inline AABB getAABBForElementArray(uint32_t index) {
		return AABB(&elements[index][0], elements[index].size()/elementSizes[index]);
	}

	uint32_t getNumIndices() const;
private:
	Array<uint32_t> indices;
	Array<uint32_t> elementSizes;
	Array<Array<float> > elements;
	uint32_t instancedElementsStartIndex;
};
