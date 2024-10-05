#include "TextureManager.h"

#include "../../Utility/Common/CommonUtility.h"
#include "../Renderer/DirectX/GpuDescriptorHeap.h"
#include "../Renderer/DirectX/D3DCommandContext.h"
#include "../Renderer/DirectX/D3DDeviceManager.h"
#include "../Renderer/DirectX/D3DUtillity.h"
#include "../Renderer/DirectX/DDSTextureLoader.h"

namespace BINDU
{
	void TextureManager::Initialize(const std::shared_ptr<D3DDeviceManager>& deviceManager, const std::shared_ptr<D3DCommandContext>& commandContext)
	{
		if (!deviceManager)
			THROW_EXCEPTION(3, "Invalid Device Manager");
		if (!commandContext)
			THROW_EXCEPTION(3, "Invalid Command Context");

		m_deviceManager = deviceManager;
		m_commandContext = commandContext;
	}

	std::shared_ptr<Texture>& TextureManager::LoadDDSTextureFromFile(const std::string& name, const std::string& filepath, bool isTextureArray)
	{
		auto texture = std::make_shared<Texture>(name, filepath);

	/*	auto& resource = texture->GetResource();

		auto d3dDevice = m_deviceManager->GetD3DDevice();

		auto cmdList = m_commandContext->GetCommandList();

		auto filepathwstr = StringToWstring(filepath);

		DXThrowIfFailed(
			DirectX::CreateDDSTextureFromFile12(d3dDevice, cmdList.Get(), filepathwstr.c_str(), resource, m_uploadHeap)
		); */

		LoadDDSTextureFromFile(texture.get(), isTextureArray);

		m_texturePool.emplace(name, std::move(texture));

		return m_texturePool[name];
	}

	void TextureManager::LoadDDSTextureFromFile(Texture* texture, bool isTextureArray)
	{
		auto& resource = texture->GetResource();

		auto& uploadHeap = texture->GetUploadHeap();

		auto d3dDevice = m_deviceManager->GetD3DDevice();

		auto cmdList = m_commandContext->GetCommandList();

		auto filepathwstr = StringToWstring(texture->GetFilepath());

		DXThrowIfFailed(
			DirectX::CreateDDSTextureFromFile12(d3dDevice, cmdList.Get(), filepathwstr.c_str(), resource, uploadHeap)
		);

		auto allocation = m_deviceManager->GetCbvSrvUavGpuHeap()->Allocate(1);


		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = resource->GetDesc().Format;

		if (!isTextureArray)
		{
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = resource->GetDesc().MipLevels;
			srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
		}
		else
		{
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
			srvDesc.Texture2DArray.MostDetailedMip = 0;
			srvDesc.Texture2DArray.MipLevels = resource->GetDesc().MipLevels;
			srvDesc.Texture2DArray.ResourceMinLODClamp = 0.0f;
			srvDesc.Texture2DArray.FirstArraySlice = 0;
			srvDesc.Texture2DArray.ArraySize = resource->GetDesc().DepthOrArraySize;
		}


		d3dDevice->CreateShaderResourceView(resource.Get(), &srvDesc, allocation.GetCpuHandle());

		texture->SetHeapAllocation(std::move(allocation));
	}

	void TextureManager::AddTexture(const std::string& name, std::shared_ptr<Texture>& texture)
	{
		m_texturePool.emplace(name, texture);
	}

	void TextureManager::RemoveTexture(const std::string& name)
	{
		m_texturePool.erase(name);
	}

	std::shared_ptr<Texture>& TextureManager::GetTexture(const std::string& name)
	{
		return m_texturePool[name];
	}
}