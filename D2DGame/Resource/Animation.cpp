#include "stdafx.h"
#include "Animation.h"

Animation::Animation(Context * const context)
    : IResource(context, ResourceType::Animation)
{
}

Animation::~Animation()
{
    keyframes.clear();
    keyframes.shrink_to_fit();
}

bool Animation::SaveToFile(const std::string & path)
{
    Xml::XMLDocument doc;

    Xml::XMLDeclaration* decl = doc.NewDeclaration();
    doc.LinkEndChild(decl);

    Xml::XMLElement* root = doc.NewElement("Animation");
    doc.LinkEndChild(root);

    root->SetAttribute("Name", resource_name.c_str());
    root->SetAttribute("Type", static_cast<int>(repeat_type));
    root->SetAttribute("TexturePath", sprite_texture_path.c_str());
    root->SetAttribute("TextureSizeX", sprite_texture_size.x);
    root->SetAttribute("TextureSizeY", sprite_texture_size.y);

    for (const auto& keyframe : keyframes)
    {
        Xml::XMLElement* first_element = doc.NewElement("Keyframe");
        root->LinkEndChild(first_element);

        first_element->SetAttribute("OffsetX", keyframe.offset.x);
        first_element->SetAttribute("OffsetY", keyframe.offset.y);
        first_element->SetAttribute("SizeX", keyframe.size.x);
        first_element->SetAttribute("SizeY", keyframe.size.y);
        first_element->SetAttribute("Time", keyframe.time);
    }

    return Xml::XMLError::XML_SUCCESS == doc.SaveFile(path.c_str());   
}

bool Animation::LoadFromFile(const std::string & path)
{
    Xml::XMLDocument doc;
    Xml::XMLError error = doc.LoadFile(path.c_str());  
    if (error != Xml::XMLError::XML_SUCCESS)
    {
        assert(false);
        return false;
    }

    Xml::XMLElement* root   = doc.FirstChildElement();

	resource_name = resource_name + "/" + root->Attribute("Name");

    repeat_type             = static_cast<RepeatType>(root->IntAttribute("Type"));
    sprite_texture_path     = root->Attribute("TexturePath");
    sprite_texture_size.x   = root->FloatAttribute("TextureSizeX");
    sprite_texture_size.y   = root->FloatAttribute("TextureSizeY");
	
	
	
	SetSpriteTexture(sprite_texture_path);

    Xml::XMLElement* first_element = root->FirstChildElement();
    for (; first_element != nullptr; first_element = first_element->NextSiblingElement())
    {
        D3DXVECTOR2 offset;
        offset.x = first_element->FloatAttribute("OffsetX");
        offset.y = first_element->FloatAttribute("OffsetY");

        D3DXVECTOR2 size;
        size.x = first_element->FloatAttribute("SizeX");
        size.y = first_element->FloatAttribute("SizeY");

        float time = first_element->FloatAttribute("Time");

        AddKeyframe(offset, size, time);
    }

	if (float x = root->FloatAttribute("CenterX"))
		pivot.x = x;
	else
		pivot.x = keyframes[0].size.x * 0.5f;

	if (float y = root->FloatAttribute("CenterY"))
			pivot.y = y;
	else
		pivot.y = keyframes[0].size.y * 0.5f;

    return true;
}

void Animation::SetSpriteTexture(const std::string & path)
{
    sprite_texture_path = path;
    sprite_texture = context->GetSubsystem<ResourceManager>()->Load<Texture2D>(path);
}

auto Animation::GetKeyframe(const uint & index) const -> const Keyframe * const
{
    assert(index < keyframes.size());
    return &keyframes[index];
}

void Animation::AddKeyframe(const Keyframe & keyframe)
{
    keyframes.emplace_back(keyframe);
}

void Animation::AddKeyframe(const D3DXVECTOR2 & offset, const D3DXVECTOR2 & size, const double & time)
{
    keyframes.emplace_back(offset, size, time);
}
