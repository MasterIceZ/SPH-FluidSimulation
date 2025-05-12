#ifndef __IMGUI_HANDLER_HPP_INCLUDED__
#define __IMGUI_HANDLER_HPP_INCLUDED__

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <cstdbool>

extern float rotate_xz_angle;

void initialize_imgui(GLFWwindow *window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void) io;

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");
}

void render_imgui() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  
  ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_FirstUseEver);
  ImGui::Begin("Parameters Controller");
  // ImGui::SliderFloat("Sensitivity", &sensitivity, 0.0f, 1.0f);
  ImGui::SliderFloat("Rotation", &rotate_xz_angle, -180.0f, 180.0f);
  ImGui::End();
}

void render_imgui_draw_data() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void cleanup_imgui() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

#endif