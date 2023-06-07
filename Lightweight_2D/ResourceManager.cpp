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
