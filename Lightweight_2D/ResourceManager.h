#pragma once
#include <string>
#include <memory>
#include <vector>
#include "Singleton.h"

namespace LW2D
{
	struct Level
	{
		std::string name;
		std::vector<std::string> data;
	};

	class Texture2D;
	class Font;
	class ResourceManager final : public Singleton<ResourceManager>
	{

	public:
		void Init(const std::string& data);
		std::shared_ptr<Texture2D> LoadTexture(const std::string& file) const;
		std::shared_ptr<Font> LoadFont(const std::string& file, unsigned int size) const;

		// High scores
		std::vector<std::pair<std::string, int>> LoadHighscores(const std::string& fileName) const;
		bool SaveHighscores(const std::string& fileName, const std::vector<std::pair<std::string, int>>& data) const;

		// Level
		std::vector<std::string> LoadLevel(const std::string& fileName, const std::string& levelName);
		std::vector<std::string> LoadLevelJSON(const std::string& fileName, const std::string& levelName);

		bool SerializeLevels(const std::string& fileName) const;
		bool DeserializeLevels(const std::string& fileName);

	private:
		std::vector<Level> m_Levels;
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_dataPath;
	};
}
