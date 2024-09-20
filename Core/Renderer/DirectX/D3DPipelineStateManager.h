#ifndef D3DPIPELINESTATEMANAGER_H
#define D3DPIPELINESTATEMANAGER_H

#include <d3d12.h>
#include <wrl/client.h>
#include <unordered_map>
#include <string>
#include <memory>

namespace BINDU
{
	using namespace Microsoft::WRL;

	class D3DDeviceManager;
	class D3DShader;

	class D3DPipelineStateManager
	{
	public:
		D3DPipelineStateManager() = default;
		~D3DPipelineStateManager() = default;

		void		Initialize(const std::shared_ptr<D3DDeviceManager>& deviceManager);

		void		AddPipelineState(const std::string& name, const D3DShader& vertexShader,
									const D3DShader& pixelShader, const D3D12_INPUT_LAYOUT_DESC& inputLayout,
									const D3D12_ROOT_SIGNATURE_DESC& rootSigDesc);

		ID3D12PipelineState*	GetPipelineState(const std::string& name) const;

		ID3D12RootSignature*	GetRootSignature(const std::string& name) const;
		

	private:
		using PipelineStateObjectMap = std::unordered_map<std::string, ComPtr<ID3D12PipelineState>>;

		using RootSignatureMap = std::unordered_map <std::string, ComPtr<ID3D12RootSignature>>;

		std::shared_ptr<D3DDeviceManager>		m_parentDeviceManager{ nullptr };

		PipelineStateObjectMap					m_pipelineStateObjects;
		
		RootSignatureMap						m_rootSignatures;
	};
}

#endif