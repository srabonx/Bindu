
#ifndef RENDERLAYER_H
#define RENDERLAYER_H

#include <string>
#include <vector>
#include <memory>
#include <d3d12.h>
#include <DirectXMath.h>
#include <wrl/client.h>

namespace BINDU
{
	struct FrameResource;
	struct LightConstants;
	class GameObjectManager;
	class MeshObject;
	class Light;
	class D3DCommandContext;

	using namespace Microsoft::WRL;
	using namespace DirectX;
	

	class RenderLayer
	{
	public:
		RenderLayer() = default;
		virtual ~RenderLayer() {};

		void				Initialize(const std::shared_ptr<GameObjectManager>& gameObjectManager);

		void				SetLightConstantRootParamIndex(std::uint16_t index);

		void				AddMeshObject(std::shared_ptr<MeshObject> meshObject);

		void				RemoveMeshObject(MeshObject* meshObject, bool removeFromParentObjectManager = true);

		std::vector<std::weak_ptr<MeshObject>>& GetMeshObjects();

		virtual void		Update(double dt, const FrameResource* currFrame);

		virtual void		Render(const D3DCommandContext& commandContext, const FrameResource* currFrame);

		void				SetName(const std::string& name);

		std::string			GetName() const;

		void				SetIndex(int index);

		void				SetAmbientLight(const XMFLOAT4& ambientLight);

		int					GetIndex() const;

		void				SetPSO(ID3D12PipelineState* pso);

		ID3D12PipelineState* GetPSO() const;

		void				SetRootSig(ID3D12RootSignature* rootSig);

		ID3D12RootSignature* GetRootSig() const;

		void				SetRenderActive(bool active);

		bool				IsRenderActive() const;

		void				SetUpdateActive(bool active);

		bool				IsUpdateActive() const;

		std::weak_ptr<GameObjectManager>& GetParentObjectManager();

	protected:

		LightConstants		BindLights(MeshObject* gameObject, const std::vector<Light*>& lights) const;

	protected:

		// Name of this Render Layer
		std::string										m_name;

		// Index of this layer
		int												m_index = -1;

		// Mesh Objects of this layer
		std::vector<std::weak_ptr<MeshObject>>			m_meshObjectPool; 

		// Main object manager for this layer
		std::weak_ptr<GameObjectManager>				m_parentObjectManager;

		// Pipeline State that will be used to render this layers objects
		ComPtr<ID3D12PipelineState>						m_pipelineState{ nullptr };

		// Root Signature that will be used for this layers objects
		ComPtr<ID3D12RootSignature>						m_rootSignature{ nullptr };

		std::uint16_t									m_lightConstantRootParamIndex{ 0 };

		bool											m_isRenderActive{ true };

		bool											m_isUpdateActive{ true };

		XMFLOAT4										m_ambientLight{ 0.25f,0.25f,0.25f,1.0f };

	private:
		bool											m_initialized{ false };
	};
}

#endif