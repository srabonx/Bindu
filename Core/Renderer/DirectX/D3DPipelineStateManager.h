#ifndef D3DPIPELINESTATEMANAGER_H
#define D3DPIPELINESTATEMANAGER_H

#include <d3d12.h>
#include <wrl/client.h>
#include <unordered_map>
#include <string>
#include <memory>
#include "D3DShader.h"

namespace BINDU
{
	using namespace Microsoft::WRL;

	class D3DDeviceManager;

	struct PipelineStateDesc
	{
		using InputElements = std::vector<D3D12_INPUT_ELEMENT_DESC>;

		D3D12_CULL_MODE				CullMode{ D3D12_CULL_MODE_BACK };
		D3D12_FILL_MODE				FillMode{ D3D12_FILL_MODE_SOLID };

		D3DShader					VertexShader;
		D3DShader					PixelShader;

		InputElements				InputElementDescs;

		ID3D12RootSignature*		RootSignature{ nullptr };
	};

	class D3DPipelineStateManager
	{
	public:
		D3DPipelineStateManager() = default;
		~D3DPipelineStateManager() = default;

		void					Initialize(const std::shared_ptr<D3DDeviceManager>& deviceManager);

		ID3D12PipelineState*	CreatePipelineState(const std::string& name, const PipelineStateDesc& pipelineState);

		ID3D12RootSignature*	CreateRootSignature(const std::string& name, const D3D12_ROOT_SIGNATURE_DESC& rootSigDesc);

		ID3D12PipelineState*	GetPipelineState(const std::string& name) const;

		ID3D12RootSignature*	GetRootSignature(const std::string& name) const;
		

	private:

		using PipelineStateObjectMap = std::unordered_map<std::string, std::pair<PipelineStateDesc, ComPtr<ID3D12PipelineState>>>;

		using RootSignatureMap = std::unordered_map <std::string, ComPtr<ID3D12RootSignature>>;

		std::shared_ptr<D3DDeviceManager>		m_parentDeviceManager{ nullptr };

		PipelineStateObjectMap					m_pipelineStateObjects;
		
		RootSignatureMap						m_rootSignatures;
	};
}

#endif