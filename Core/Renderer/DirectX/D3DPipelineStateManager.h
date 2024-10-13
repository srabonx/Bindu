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

	struct ComputePipelineStateDesc
	{
		ComputePipelineStateDesc() = default;
		~ComputePipelineStateDesc() = default;

		ID3D12RootSignature*			RootSignature{ nullptr };

		D3DShader*						ComputeShader{ nullptr };
	};

	struct PipelineStateDesc
	{
		PipelineStateDesc() = default;
		~PipelineStateDesc() = default;

		using InputElements = std::vector<D3D12_INPUT_ELEMENT_DESC>;

		D3D12_CULL_MODE						CullMode{ D3D12_CULL_MODE_BACK };
		D3D12_FILL_MODE						FillMode{ D3D12_FILL_MODE_SOLID };
		BOOL								FrontFaceCounterClockwise{ FALSE };

		D3DShader*							VertexShader{ nullptr };
		D3DShader*							PixelShader{ nullptr };
		D3DShader*							GeometryShader{ nullptr };


		D3D12_RENDER_TARGET_BLEND_DESC		RTBlendDesc{};

		D3D12_DEPTH_STENCIL_DESC			DepthStencilDesc{};

		InputElements						InputElementDescs;

		ID3D12RootSignature*				RootSignature{ nullptr };

		D3D12_PRIMITIVE_TOPOLOGY_TYPE		PrimitiveTopologyType{ D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE };

	};

	class D3DPipelineStateManager
	{
	public:
		D3DPipelineStateManager() = default;
		~D3DPipelineStateManager() = default;

		void					Initialize(const std::shared_ptr<D3DDeviceManager>& deviceManager);

		ID3D12PipelineState*	CreatePipelineState(const std::string& name, const PipelineStateDesc& pipelineState);

		ID3D12PipelineState*	CreateComputePipelineState(const std::string& name, const ComputePipelineStateDesc& pipelineState);

		ID3D12RootSignature*	CreateRootSignature(const std::string& name, const D3D12_ROOT_SIGNATURE_DESC& rootSigDesc);

		ID3D12PipelineState*	GetPipelineState(const std::string& name) const;

		ID3D12PipelineState*	GetComputePipelineState(const std::string& name) const;

		ID3D12RootSignature*	GetRootSignature(const std::string& name) const;
		

	private:

		using PipelineStateObjectMap = std::unordered_map<std::string, std::pair<PipelineStateDesc, ComPtr<ID3D12PipelineState>>>;
		using ComputePipelineStateObjectMap = std::unordered_map<std::string, std::pair<ComputePipelineStateDesc, ComPtr<ID3D12PipelineState>>>;
		using RootSignatureMap = std::unordered_map <std::string, ComPtr<ID3D12RootSignature>>;


		std::shared_ptr<D3DDeviceManager>		m_parentDeviceManager{ nullptr };

		PipelineStateObjectMap					m_pipelineStateObjects;

		ComputePipelineStateObjectMap			m_computePipelineStateObjects;
		
		RootSignatureMap						m_rootSignatures;
	};
}

#endif