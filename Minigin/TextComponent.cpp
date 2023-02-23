#include "TextComponent.h"
#include <iostream>

LW2D::TextComponent::TextComponent(std::weak_ptr<GameObject> go)
	: Component(go)
{
}

void LW2D::TextComponent::Update()
{
	std::cout << "Update!\n";
}

void LW2D::TextComponent::Render() const
{
}
