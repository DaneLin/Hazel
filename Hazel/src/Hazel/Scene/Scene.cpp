#include "hzpch.h"
#include "Scene.h"

#include "Components.h"
#include "Hazel/Renderer/Renderer2D.h"
#include <glm/glm.hpp>

#include "Entity.h"

namespace Hazel
{
	static void DoMath(const glm::mat4& transform)
	{

	}

	Scene::Scene()
	{
		struct TransformComponent
		{
			glm::mat4 Transform;

			TransformComponent() = default;
			TransformComponent(const TransformComponent&) = default;
			TransformComponent(const glm::mat4& tranform)
				: Transform(tranform) {}

			operator glm::mat4& () { return Transform; }
			operator const glm::mat4& () const { return Transform; }
		};

		/*
		* Example code
		TransformComponent transform;

		DoMath(transform);*/

		//entt::entity entity = m_Registry.create();
		//m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		////m_Registry.get<TransformComponent>(entity);
		//if (m_Registry.any_of<TransformComponent>(entity))
		//{
		//	TransformComponent& transform = m_Registry.get<TransformComponent>(entity);
		//}

		//auto view = m_Registry.view<TransformComponent>();
		//for (auto entity : view)
		//{
		//	TransformComponent& transform = view.get<TransformComponent>(entity);
		//}

		//multiple component
		//auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>)
	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(Timestep ts)
	{
		// Update scripts
		{
			//auto view = m_Registry.view<NativeScriptComponent>();
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{
					// TODO: Move to Scene::OnScenePlay
					if (!nsc.Instance)
					{
						nsc.Instance = nsc.InstantiateScript();
						nsc.Instance->m_Entity = Entity{ entity, this };
						nsc.Instance->OnCreate();
					}
					
					//if (nsc.OnUpdateFunction)
					nsc.Instance->OnUpdate(ts);
				});
		}

		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{// find mainCamera;
			auto view = m_Registry.view<TransformComponent, CameraComponent >();
			//auto group = m_Registry.group<TransformComponent>(entt::get<CameraComponent>);
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}
		
		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportHeight = height;
		m_ViewportWidth = width;

		// Resize non-fixedAspectRatio Cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
			{
				//how to resize 
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}

}
