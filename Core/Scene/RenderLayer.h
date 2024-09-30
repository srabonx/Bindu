
#ifndef RENDERLAYER_H
#define RENDERLAYER_H

#include <vector>
#include "../GameObject/MeshObject.h"

namespace BINDU
{
	struct FrameResource;

	class RenderLayer
	{
	public:
		RenderLayer() = default;
		~RenderLayer() = default;

		void				SetLightConstantRootParamIndex(std::uint16_t index);

		void				AddMeshObject(MeshObject* meshObject);

		void				RemoveMeshObject(const MeshObject* meshObject);

		std::vector<MeshObject*>& GetMeshObjects();

		void				Render(const D3DCommandContext& commandContext, const FrameResource* currFrame);

		void				SetName(const std::string& name);

		std::string			GetName() const;

		void				SetIndex(int index);

		int					GetIndex() const;

		void				SetPSO(ID3D12PipelineState* pso);

		ID3D12PipelineState* GetPSO() const;

		void				SetRootSig(ID3D12RootSignature* rootSig);

		ID3D12RootSignature* GetRootSig() const;

		void				SetActive(bool active);

		bool				IsActive() const;



	private:

		// Name of this Render Layer
		std::string						m_name;

		// Index of this layer
		int								m_index = -1;

		// Mesh Objects of this layer
		std::vector<MeshObject*>		m_meshObjectPool;

		// Pipeline State that will be used to render this layers objects
		ComPtr<ID3D12PipelineState>		m_pipelineState{ nullptr };

		// Root Signature that will be used for this layers objects
		ComPtr<ID3D12RootSignature>		m_rootSignature{ nullptr };

		std::uint16_t					m_lightConstantRootParamIndex{ 0 };

		bool							m_active{ true };
	};
}

#endif