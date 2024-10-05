#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <map>
#include <string>
#include <memory>
#include "../Resources/Texture.h"

namespace BINDU
{
	class D3DDeviceManager;
	class D3DCommandContext;

	class TextureManager
	{
	public:
		TextureManager() = default;
		~TextureManager() = default;

		void							Initialize(const std::shared_ptr<D3DDeviceManager>& deviceManager,
													const std::shared_ptr<D3DCommandContext>& commandContext);

		std::shared_ptr<Texture>&		LoadDDSTextureFromFile(const std::string& name, const std::string& filepath, bool isTextureArray = false);

		void							LoadDDSTextureFromFile(Texture* texture, bool isTextureArray = false);

		void							AddTexture(const std::string& name, std::shared_ptr<Texture>& texture);

		void							RemoveTexture(const std::string& name);

		std::shared_ptr<Texture>&		GetTexture(const std::string& name);


	private:
		std::map<std::string, std::shared_ptr<Texture>>			m_texturePool;

		std::shared_ptr<D3DDeviceManager>						m_deviceManager{ nullptr };

		std::shared_ptr<D3DCommandContext>						m_commandContext{ nullptr };

		ComPtr<ID3D12Resource>									m_uploadHeap{ nullptr };
	};
}

#endif