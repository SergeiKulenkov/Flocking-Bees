#pragma once
#include <vector>
#include <memory>
#include <string_view>
#include <functional>

#include "imgui_impl_vulkan.h"

#include "DrawableObject.h"

struct GLFWwindow;

static void check_vk_result(VkResult err)
{
	if (err == 0)
		return;
	fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
	if (err < 0)
		abort();
}

////////////////////

class App
{
public:
	App();
	App(std::string_view name);
	~App();

	App(const App& app) = delete;
	App& operator=(const App& app) = delete;

	void Run();

	void AddObjectToDraw(std::shared_ptr<IDrawableObject> object) { m_ObjectsToDraw.emplace_back(object); }

	GLFWwindow* GetWindow() { return m_Window; }

	ImVec2 GetWindowSize() const { return ImVec2(width, height); }

	static VkInstance GetInstance();
	static VkPhysicalDevice GetPhysicalDevice();
	static VkDevice GetDevice();

	static VkCommandBuffer GetCommandBuffer(bool begin);
	static void FlushCommandBuffer(VkCommandBuffer commandBuffer);

	static void SubmitResourceFree(std::function<void()>&& func);

private:
	void Init(std::string_view name = "");

	void Shutdown();

	bool m_Running = false;

	float m_TimeStep = 0.0f;
	float m_FrameTime = 0.0f;
	float m_LastFrameTime = 0.0f;

	GLFWwindow* m_Window = nullptr;
	std::vector<std::shared_ptr<IDrawableObject>> m_ObjectsToDraw;

	static constexpr uint16_t width = 1920;
	static constexpr uint16_t height = 1080;
	static constexpr ImVec4 clearColour = ImVec4(17 / 255.f, 150 / 255.f, 250 / 255.f, 1.00f);
};