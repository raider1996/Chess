#pragma once
#include <SFML/Graphics.hpp>
#include <string>
using namespace sf;
class TextureManager
{
public:
	Texture& getTexture(const std::string& filepath);
	void unloadTexture(const std::string& filepath);
	void clearAll();
private:
	std::map<std::string, Texture> textureMap;
};

