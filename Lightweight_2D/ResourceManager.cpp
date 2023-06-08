#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <utility>

#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"

#include "rapidjson.h"
#include "document.h"
#include "istreamwrapper.h"

void LW2D::ResourceManager::Init(const std::string& dataPath)
{
	m_dataPath = dataPath;

	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

std::shared_ptr<LW2D::Texture2D> LW2D::ResourceManager::LoadTexture(const std::string& file) const
{
	const auto fullPath = m_dataPath + file;
	auto texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}
	return std::make_shared<Texture2D>(texture);
}

std::shared_ptr<LW2D::Font> LW2D::ResourceManager::LoadFont(const std::string& file, unsigned int size) const
{
	return std::make_shared<Font>(m_dataPath + file, size);
}

std::vector<std::pair<std::string, int>> LW2D::ResourceManager::LoadHighscores(const std::string& fileName) const
{
    std::vector<std::pair<std::string, int>> data;

    std::ifstream file(m_dataPath + fileName, std::ios::binary);
    if (!file)
    {
        return data;
    }

    size_t vectorSize;
    file.read(reinterpret_cast<char*>(&vectorSize), sizeof(size_t));

    // Read each pair from the file
    for (size_t i = 0; i < vectorSize; ++i)
    {
        size_t stringSize;
        file.read(reinterpret_cast<char*>(&stringSize), sizeof(size_t));

        std::string str(stringSize, '\0');
        file.read(&str[0], stringSize);

        int value;
        file.read(reinterpret_cast<char*>(&value), sizeof(int));

        data.emplace_back(std::move(str), value);
    }

    file.close();

    return data;
}

bool LW2D::ResourceManager::SaveHighscores(const std::string& fileName, const std::vector<std::pair<std::string, int>>& data) const
{
    std::ofstream file(m_dataPath + fileName, std::ios::binary);
    if (!file)
    {
        return false;
    }

    size_t vectorSize = data.size();
    file.write(reinterpret_cast<const char*>(&vectorSize), sizeof(size_t));

    // Write each pair in the vector to the file
    for (const auto& pair : data)
    {
        size_t stringSize = pair.first.size();
        file.write(reinterpret_cast<const char*>(&stringSize), sizeof(size_t));

        file.write(pair.first.c_str(), stringSize);
        file.write(reinterpret_cast<const char*>(&pair.second), sizeof(int));
    }

    file.close();

    return true;
}

std::vector<std::string> LW2D::ResourceManager::LoadLevel(const std::string& fileName, const std::string& levelName)
{
    if (m_Levels.empty())
        DeserializeLevels(fileName);

    auto it = std::find_if(begin(m_Levels), end(m_Levels), [&](const auto& level) {
        return level.name == levelName;
        });

    if (it != end(m_Levels))
        return it->data;

    return std::vector<std::string>();
}

std::vector<std::string> LW2D::ResourceManager::LoadLevelJSON(const std::string& fileName, const std::string& levelName)
{
    // Load from file if not already loaded
    if (m_Levels.empty())
    {
        using namespace rapidjson;
        std::vector<std::string> map;
        if (std::ifstream is{ m_dataPath + fileName })
        {
            IStreamWrapper isw{ is };
            Document levelDoc;
            levelDoc.ParseStream(isw);

            if (levelDoc.IsArray())
            {
                for (Value::ConstValueIterator it = levelDoc.Begin(); it != levelDoc.End(); ++it)
                {
                    const Value& data = *it;

                    const Value& level = data["level"];
                    const Value& grid = data["grid"];
                    for (int i = 0; i < static_cast<int>(grid.Size()); i++)
                    {
                        map.emplace_back(grid[i].GetString());
                    }

                    m_Levels.emplace_back(level.GetString(), map);
                    map.clear();
                }
            }
            else
                std::cerr << "ResourceManager::LoadLevel() >> Error: Level file is not an array" << std::endl;
        }
    }

    auto it = std::find_if(begin(m_Levels), end(m_Levels), [&](const auto& level) {
        return level.name == levelName;
        });

    if (it != end(m_Levels))
        return it->data;

    return std::vector<std::string>();
}

bool LW2D::ResourceManager::SerializeLevels(const std::string& fileName) const
{
    std::ofstream file(m_dataPath + fileName, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return false;
    }

    uint32_t numLevels = static_cast<uint32_t>(m_Levels.size());
    file.write(reinterpret_cast<const char*>(&numLevels), sizeof(uint32_t));

    // Serialize each level
    for (const Level& level : m_Levels)
    {
        uint32_t nameLength = static_cast<uint32_t>(level.name.length());
        file.write(reinterpret_cast<const char*>(&nameLength), sizeof(uint32_t));
        file.write(level.name.c_str(), nameLength);

        uint32_t numStrings = static_cast<uint32_t>(level.data.size());
        file.write(reinterpret_cast<const char*>(&numStrings), sizeof(uint32_t));

        for (const std::string& str : level.data)
        {
            uint32_t strLength = static_cast<uint32_t>(str.length());
            file.write(reinterpret_cast<const char*>(&strLength), sizeof(uint32_t));
            file.write(str.c_str(), strLength);
        }
    }

    file.close();
    return true;
}

bool LW2D::ResourceManager::DeserializeLevels(const std::string& fileName)
{
    std::ifstream file(m_dataPath + fileName, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return false;
    }

    uint32_t numLevels = 0;
    file.read(reinterpret_cast<char*>(&numLevels), sizeof(uint32_t));
    m_Levels.resize(numLevels);

    // Deserialize each level
    for (Level& level : m_Levels)
    {
        uint32_t nameLength = 0;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(uint32_t));
        level.name.resize(nameLength);
        file.read(&level.name[0], nameLength);

        uint32_t numStrings = 0;
        file.read(reinterpret_cast<char*>(&numStrings), sizeof(uint32_t));
        level.data.resize(numStrings);

        for (std::string& str : level.data)
        {
            uint32_t strLength = 0;
            file.read(reinterpret_cast<char*>(&strLength), sizeof(uint32_t));
            str.resize(strLength);
            file.read(&str[0], strLength);
        }
    }

    file.close();
    return true;
}
