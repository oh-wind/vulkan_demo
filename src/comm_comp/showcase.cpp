#include <comm_comp/scene.hpp>
#include <comm_comp/showcase.hpp>
#include <core/object.hpp>
#include <comm_comp/transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace vkd
{
	void Showcase::awake()
	{
		auto surface = surface_extent();
		mat_p = glm::perspective(glm::radians(fovy), static_cast<float>(surface.width) / static_cast<float>(surface.height), 
				zNera, zFar);
		dirty = true;
	}

	bool Showcase::dispatchEvent(const evt::Event& e)
	{
		switch (e.type)
		{
		case vkd::evt::EventType::MouseDown:
		{
			auto& ev = e.GetEvent<vkd::evt::MouseButtonEvent>();
			mouseLastPos.x = (float)ev.x;
			mouseLastPos.y = (float)ev.y;
		}
		break;
		case vkd::evt::EventType::MouseMove:
			if (event_constructor().isMouseBtnPress(vkd::evt::MouseBtnLeft))
			{
				auto& ev = e.GetEvent<vkd::evt::MouseButtonEvent>();
				auto surface = surface_extent();
				mouseMoveOffset.x = ((float)ev.x - mouseLastPos.x) / surface.width;
				mouseMoveOffset.y = ((float)ev.y - mouseLastPos.y) / surface.height;
				mouseLastPos.x = (float)ev.x;
				mouseLastPos.y = (float)ev.y;
				return true;
			}
			break;
		case vkd::evt::EventType::MouseUp:
			mouseMoveOffset.y = mouseMoveOffset.x = 0.0f;
			break;
		default:
			break;
		}
		return false;
	}

	void Showcase::recreate_swapchain()
	{
		awake();
	}

	const glm::mat4& Showcase::get_matrix_p() const
	{
		return mat_p;
	}

	const glm::mat4& Showcase::get_matrix_v() const
	{
		return object.lock()->get_comp_raw<Transform>()->get_matrix();
	}

	void Showcase::update(float delta)
	{
		glm::vec2 off = mouseMoveOffset * delta * 42000.0f;
		if(glm::abs(off.x) < glm::epsilon<float>() &&
			glm::abs(off.y) < glm::epsilon<float>()) return;
		if(auto obj = object.lock();obj)
		{
			if(auto trans = obj->get_comp_raw<Transform>();trans)
			{
				auto z = trans->get_rotation().z;
				trans->set_rotation(glm::vec3(off.x,off.y,z));
			}
		}
	}

	bool Showcase::is_dirty() const
	{
		if (auto obj = object.lock(); obj)
		{
			if (auto trans = obj->get_comp_raw<Transform>(); trans)
			{
				return Camera::is_dirty() || trans->matrix_dirty();
			}
		}
		return Camera::is_dirty();
	}


}
