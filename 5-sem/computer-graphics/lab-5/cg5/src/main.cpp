#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GLWrapper.h"
#include "SceneManager.h"

static int wind_width = 1280;
static int wind_height = 720;



int main()
{
	GLWrapper glWrapper(wind_width, wind_height, false);
	// fullscreen
//	GLWrapper glWrapper(true);
	// window with monitor resolution
//	GLWrapper glWrapper(false);

	// set SMAA quality preset
	glWrapper.enable_SMAA(ULTRA);
	
	glWrapper.init_window();
	glfwSwapInterval(1); // vsync
	wind_width = glWrapper.getWidth();
	wind_height = glWrapper.getHeight();

	// fix ray direction issues
	if (wind_width % 2 == 1) wind_width++;
	if (wind_height % 2 == 1) wind_height++;

	scene_container scene = {};

	scene.scene = SceneManager::create_scene(wind_width, wind_height);
	scene.scene.camera_pos = { 0, 3, -7 };
	scene.shadow_ambient = glm::vec3{ 0.1, 0.1, 0.1 };
	scene.ambient_color = glm::vec3{ 0.025, 0.025, 0.025 };

	// lights
	scene.lights_point.push_back(SceneManager::create_light_point({ 3, 5, 0, 0.1 }, { 1, 1, 1 }, 25.5));
	scene.lights_direct.push_back(SceneManager::create_light_direct({ 3, -1, 1 }, { 1, 1, 1 }, 1.5));

	// blue sphere
	scene.spheres.push_back(SceneManager::create_sphere({ 0, 4, 4 }, 1,
		SceneManager::create_material({ 0, 0, 1 }, 50, 0.35)));
	// red sphere
//	scene.spheres.push_back(SceneManager::create_sphere({ 0, 2, 6 }, 1,
//		SceneManager::create_material({ 1, 0, 0 }, 100, 0.1), true));
	// transparent sphere
//	scene.spheres.push_back(SceneManager::create_sphere({ -3, 2, 6 }, 1,
//		SceneManager::create_material({ 1, 1, 1 }, 200, 0.1, 1.125, { 1, 0, 2 }, 1), true));

	// floor and wall
	rt_box box = SceneManager::create_box({ 0, 0, 6 }, { 10, 0.2, 5 },
		SceneManager::create_material({ 0.8,0.7,0 }, 50, 0.0));
	box.textureNum = 1;
	scene.boxes.push_back(box);

	rt_box box1 = SceneManager::create_box({ 0, 5.2, 10.8}, { 10, 5, 0.2 },
		SceneManager::create_material({ 0.8,0.7,0 }, 50, 0.0));
	box1.textureNum = 1;
	scene.boxes.push_back(box1);

	rt_defines defines = scene.get_defines();
	glWrapper.init_shaders(defines);

	std::vector<std::string> faces =
	{
		ASSETS_DIR "/textures/sb_nebula/GalaxyTex_PositiveX.jpg",
		ASSETS_DIR "/textures/sb_nebula/GalaxyTex_NegativeX.jpg",
		ASSETS_DIR "/textures/sb_nebula/GalaxyTex_PositiveY.jpg",
		ASSETS_DIR "/textures/sb_nebula/GalaxyTex_NegativeY.jpg",
		ASSETS_DIR "/textures/sb_nebula/GalaxyTex_PositiveZ.jpg",
		ASSETS_DIR "/textures/sb_nebula/GalaxyTex_NegativeZ.jpg"
	};

	glWrapper.set_skybox(GLWrapper::load_cubemap(faces, false));

	auto boxTex = glWrapper.load_texture(1, "rat.png", "texture_box");

	SceneManager scene_manager(wind_width, wind_height, &scene, &glWrapper);
	scene_manager.init();

	float currentTime = static_cast<float>(glfwGetTime());
	float lastFramesPrint = currentTime;
	float framesCount = 0;

	while (!glfwWindowShouldClose(glWrapper.window))
	{
		framesCount++;
		float newTime = static_cast<float>(glfwGetTime());
		float deltaTime = newTime - currentTime;
		currentTime = newTime;

		if (newTime - lastFramesPrint > 1.0f)
		{
			std::cout << "FPS: " << framesCount << std::endl;
			lastFramesPrint = newTime;
			framesCount = 0;
		}

		scene_manager.update(deltaTime);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, boxTex);
		glWrapper.draw();
		glfwSwapBuffers(glWrapper.window);
		glfwPollEvents();
	}

	glWrapper.stop(); // stop glfw, close window
	return 0;
}

