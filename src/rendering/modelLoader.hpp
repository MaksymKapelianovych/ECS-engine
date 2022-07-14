#pragma once

#include "indexedModel.hpp"
#include "material.hpp"

namespace ModelLoader
{
	bool loadModels(const String& fileName,
			Array<IndexedModel>& models, Array<uint32_t>& modelMaterialIndices,
			Array<MaterialSpec>& materials);
}
