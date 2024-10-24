#include "MaterialManager.h"

namespace BINDU
{
	MaterialManager::MaterialManager(std::uint64_t maxMaterialCount) : m_maxMaterialCount(maxMaterialCount)
	{
		m_matCbIndexAllocator = std::make_shared<VariableSizeMemoryAllocator>(maxMaterialCount);
	}

	void MaterialManager::AddMaterial(const std::string& name, const std::shared_ptr<Material>& material)
	{
		CbAllocation allocation;

		allocation.Index = m_matCbIndexAllocator->Allocate(1);
		allocation.ParentAllocator = m_matCbIndexAllocator;

		material->SetCbAllocation(allocation);

		m_materialPool.emplace(name, material);
	}

	std::shared_ptr<Material>& MaterialManager::GetMaterial(const std::string& name)
	{
		return m_materialPool[name];
	}

	std::shared_ptr<Material> MaterialManager::GetMaterialDynamic(const std::string& name)
	{
		auto material =  m_materialPool[name];

		auto mat = std::make_shared<Material>();

		mat->SetConstantRootParameterIndex(material->GetConstantRootParameterIndex());

		mat->SetDiffuseAlbedo(material->GetDiffuseAlbedo4f());

		mat->SetFresnelR0(material->GetFresnelR03f());

		mat->SetRoughness(material->GetRoughness());

		mat->SetTransform(material->GetTransform4x4());

		CbAllocation cbAllocation;
		cbAllocation.Index = m_matCbIndexAllocator->Allocate(1);
		cbAllocation.ParentAllocator = m_matCbIndexAllocator;

		mat->SetCbAllocation(cbAllocation);

		return mat;
	}

	void MaterialManager::RemoveMaterial(const std::string& name)
	{
		m_materialPool.erase(name);
	}

	void MaterialManager::Update() const
	{
		for(auto& m : m_materialPool)
		{
			m.second->Update();
		}
	}
}
