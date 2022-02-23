#include <Engine/pch.h>
#include <Engine/ImGuiLayer.h>

#include <Engine/Application.h>

#include <imgui.h>

// ############################################
// ##### TEMPORARY ############################
// ############################################

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//GLFWwindow* glfwGetCurrentContext();
//void glfwMakeContextCurrent(GLFWwindow*);

// ############################################
// ############################################
// ############################################



namespace cee
{
	namespace engine
	{
		ImGuiLayer::ImGuiLayer()
			: Layer("ImGuiLayer")
		{
		}
		
		void ImGuiLayer::OnAttach()
		{
			CEE_PROFILE_FUNCTION();
			
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
			
			ImGui::StyleColorsDark();
			
			Application& app = Application::Get();
			GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindowPtr());
			
			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init("#version 410");
		}
		
		void ImGuiLayer::OnDetach()
		{
			CEE_PROFILE_FUNCTION();
			
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}
		
		void ImGuiLayer::Begin()
		{
			CEE_PROFILE_FUNCTION();
			
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}
		
		void ImGuiLayer::End()
		{
			CEE_PROFILE_FUNCTION();
			
			ImGuiIO& io = ImGui::GetIO();
			Application& app = Application::Get();
			io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());
			
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_context_current = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_context_current);
			}
		}
		
		void ImGuiLayer::OnEvent(Event& e)
		{
		}
		
		void ImGuiLayer::OnUpdate()
		{
		}
		
		void ImGuiLayer::OnImGuiRender()
		{
		}
	}
}
