#include "RenderLayer.h"

#include "../Renderer/DirectX/D3DCommandContext.h"
#include "../Renderer/DirectX/FlyFrame.h"

namespace BINDU
{
	void RenderLayer::SetLightConstantRootParamIndex(std::uint16_t index)
	{
		m_lightConstantRootParamIndex = index;
	}

	void RenderLayer::AddMeshObject(MeshObject* meshObject)
	{
		m_meshObjectPool.emplace_back(meshObject);
	}

	void RenderLayer::RemoveMeshObject(const MeshObject* meshObject)
	{
		m_meshObjectPool.erase(
			std::remove_if(m_meshObjectPool.begin(), m_meshObjectPool.end(), [meshObject](const MeshObject* obj)->bool
				{
					return meshObject->GetGuid() == obj->GetGuid();
				}), m_meshObjectPool.end());
	}

	std::vector<MeshObject*>& RenderLayer::GetMeshObjects()
	{
		return m_meshObjectPool;
	}

	void RenderLayer::Render(const D3DCommandContext& commandContext, const FrameResource* currFrame)
	{

		if (m_pipelineState || m_rootSignature)
		{
			commandContext.BindPipeline(m_pipelineState.Get(), m_rootSignature.Get());
		}

		auto cmdList = commandContext.GetCommandList();

		auto matCb = currFrame->MaterialCb.get();

		auto lightCb = currFrame->LightCb.get();

		for(auto* m : m_meshObjectPool)
		{

			auto mat = m->GetMaterialComponent();
			cmdList->SetGraphicsRootConstantBufferView(mat->GetConstantRootParameterIndex(),
				matCb->GetGPUVirtualAddressAt(mat->GetCbAllocation()->Index));

			cmdList->SetGraphicsRootConstantBufferView(m_lightConstantRootParamIndex,
				lightCb->GetGPUVirtualAddressAt(m->GetCBAllocation()->Index));

			m->Render(commandContext, currFrame->ObjectsCb.get());
		}
	}

	void RenderLayer::SetName(const std::string& name)
	{
		m_name = name;
	}

	std::string RenderLayer::GetName() const
	{
		return m_name;
	}

	void RenderLayer::SetIndex(int index)
	{
		m_index = index;
	}

	int RenderLayer::GetIndex() const
	{
		return m_index;
	}

	void RenderLayer::SetPSO(ID3D12PipelineState* pso)
	{
		m_pipelineState = pso;
	}

	ID3D12PipelineState* RenderLayer::GetPSO() const
	{
		return m_pipelineState.Get();
	}

	void RenderLayer::SetRootSig(ID3D12RootSignature* rootSig)
	{
		m_rootSignature = rootSig;
	}

	ID3D12RootSignature* RenderLayer::GetRootSig() const
	{
		return m_rootSignature.Get();
	}

	void RenderLayer::SetActive(bool active)
	{
		m_active = active;
	}

	bool RenderLayer::IsActive() const
	{
		return m_active;
	}
}
