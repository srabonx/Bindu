#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include <map>
#include <memory>
#include <string>

#include "../GameObject/Material.h"
#include "../Renderer/DirectX/VariableSizeMemoryAllocator.h"


namespace BINDU
{

	class MaterialManager
	{
	public:
		explicit MaterialManager(std::uint64_t maxMaterialCount);
		~MaterialManager() = default;

		void						AddMaterial(const std::string& name,const std::shared_ptr<Material>& material);

		std::shared_ptr<Material>&	GetMaterial(const std::string& name);

		std::shared_ptr<Material> GetMaterialDynamic(const std::string& name);

		void						RemoveMaterial(const std::string& name);

		void						Update() const;


	private:
		using MaterialPool = std::map<std::string, std::shared_ptr<Material>>;

		using MatCbAllocator = std::shared_ptr<VariableSizeMemoryAllocator>;

		MaterialPool								m_materialPool;

		MatCbAllocator								m_matCbIndexAllocator;

		std::uint64_t								m_maxMaterialCount;
	};
}

#endif