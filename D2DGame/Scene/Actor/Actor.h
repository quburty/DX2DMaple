#pragma once

class Actor: public Object
{
public:
    Actor(class Context* const context);
    virtual ~Actor();

    virtual void Start();
    virtual void Update(const float& delta_time_sec);
    virtual void End();

    /*
        Property
    */

    auto IsActive() const { return is_active; }
	void SetActive(const bool& is_active) { this->is_active = is_active; }

    auto GetAllComponents() const -> const std::vector<std::shared_ptr<class IComponent>>& { return components; }

    /*
        Components
    */
public:
    template <typename T>
    auto AddComponent() -> const std::shared_ptr<T>;

	template <typename T>
	auto GetComponent() -> const std::shared_ptr<T>;

	auto GetAnimator() -> const std::shared_ptr<class AnimatorComponent>;

    void RemoveComponent(const std::string& component_name);

public:
	auto GetScale() const -> const D3DXVECTOR2;
	void SetScale(const D3DXVECTOR2& scale);
	void SetScale(const float& x, const float& y) { SetScale(D3DXVECTOR2(x, y)); }

	auto GetPosition() const -> const D3DXVECTOR2 { return position; }
	void SetPosition(const D3DXVECTOR2& position) { this->position = position; }
	void SetPosition(const float& x, const float& y) { SetPosition(D3DXVECTOR2(x, y)); }
	
	auto GetCenterPosition() const -> const D3DXVECTOR2;

	auto GetXMin() const -> const float& { return actor_x_min; }
	auto GetXMax() const -> const float& { return actor_x_max; }
	auto GetYMin() const -> const float& { return actor_y_min; }
	auto GetYMax() const -> const float& { return actor_y_max; }

	void SetXMin(const float& x) { actor_x_min = x; }
	void SetXMax(const float& x) { actor_x_max = x; }
	void SetYMin(const float& y) { actor_y_min = y; }
	void SetYMax(const float& y) { actor_y_max = y; }

	auto GetTexture() const -> const std::shared_ptr<Texture2D>& { return texture; }
	void SetTexture(const std::string& path);

	bool IsDirectionRight() { return is_dir_right; }
	void SetDirectionRight(const bool& dir) { is_dir_right = dir; }

	bool IsCollide(const std::shared_ptr<Actor>& rhs);

	bool IsRenderTextureSize() const { return render_texture_size; }
	void SetRenderSizeTextureOrActor(const bool& render_texture_size) { this->render_texture_size = render_texture_size; }

	bool IsUIActor() const { return is_ui_actor; }
	void SetIsUIActor(const bool& is_ui) { is_ui_actor = is_ui; }

	auto GetRenderOrder() const -> const int& { return render_order; }
	void SetRenderOrder(const int& render_order = 0);

public:
	virtual Actor& operator= (const Actor& org_actor);

protected:
    class Context* context = nullptr;
    bool is_active = true;
	
	bool is_ui_actor = false;

	bool is_dir_right = false;
	bool render_texture_size = true;

	float actor_x_min = -50.0f;
	float actor_x_max = +50.0f;
	float actor_y_min = -50.0f;
	float actor_y_max = +50.0f;

	D3DXVECTOR2 position{ 0.0f,0.0f };

	std::shared_ptr<Texture2D> texture;

	std::vector<std::shared_ptr<class IComponent>> components;

	int render_order = 0;
};

template<typename T>
inline auto Actor::AddComponent() -> const std::shared_ptr<T>
{
    static_assert(std::is_base_of<IComponent, T>::value, "Provider type does not implement IComponent");
    
	for (const auto& component : components)
	{
		if (component->GetComponentName() == typeid(T).name())
		{
			return std::static_pointer_cast<T>(component);
		}
	}

    components.emplace_back
    (
        std::make_shared<T>(context, this)
    );

    auto new_component = std::static_pointer_cast<T>(components.back());

    return new_component;
}

template<typename T>
inline auto Actor::GetComponent() -> const std::shared_ptr<T>
{
    static_assert(std::is_base_of<IComponent, T>::value, "Provider type does not implement IComponent");
	
    for (const auto& component : components)
    {
        if (component->GetComponentName() == typeid(T).name())
            return std::static_pointer_cast<T>(component);
    }

    return nullptr;
}
