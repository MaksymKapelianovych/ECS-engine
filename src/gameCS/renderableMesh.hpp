#pragma once

#include "ecs/ecs.hpp"
#include "utilComponents.hpp"
#include "gameRenderContext.hpp"

struct RenderableMeshComponent : public ECSComponent<RenderableMeshComponent>
{
	VertexArray* vertexArray = nullptr;
	Texture* texture = nullptr;
};

inline std::ostream& operator<<(std::ostream& os, const RenderableMeshComponent& value)
{
	os << std::setw(ECS_LOG_MARGIN) << value.texture->getId();
	return os;
}

ECS_COMPONENT_PRINT(RenderableMeshComponent, "texture");

class RenderableMeshSystem : public BaseECSSystem
{
public:
	RenderableMeshSystem(ECS& ecsIn, GameRenderContext& contextIn) : BaseECSSystem(ecsIn), context(contextIn)
	{
		addComponentType(TransformComponent::ID);
		addComponentType(RenderableMeshComponent::ID);
	}

	virtual void updateComponents(float delta, BaseECSComponent** components)
	{
		TransformComponent* transform = (TransformComponent*)components[0];
		RenderableMeshComponent* mesh = (RenderableMeshComponent*)components[1];
		
		context.renderMesh(*mesh->vertexArray, *mesh->texture, transform->transform.toMatrix());
	}
private:
	GameRenderContext& context;
};


