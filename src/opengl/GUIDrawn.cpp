#include "GUIDrawn.h"

GuiModule::GuiModule(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void GuiModule::draw()
{
    ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Ustawienia");
	if (ImGui::CollapsingHeader("Sily"))
	{
		ImGui::Checkbox("Grawitacja", &PhysicsModule::gravity);
		ImGui::Checkbox("Opor aerodynamiczny", &PhysicsModule::aero);
		ImGui::SliderFloat("Potega stalej oporu aerodyn.", &PhysicsModule::mu, -1.0f,-5.0f);
	}

	if (ImGui::CollapsingHeader("Punkty przyciagania grawitacyjnego"))
	{
		for (int i=0; i<PhysicsModule::gravityPoints.size(); i++)
		{

			if (ImGui::TreeNode(("Punkt " +  std::to_string(i+1)).c_str()))
			{
				ImGui::SliderFloat("Polozenie X", &PhysicsModule::gravityPoints[i].x, -PhysicsModule::borderOfDomain, PhysicsModule::borderOfDomain);
				ImGui::SliderFloat("Polozenie Y", &PhysicsModule::gravityPoints[i].y, -PhysicsModule::borderOfDomain, PhysicsModule::borderOfDomain);
				ImGui::SliderFloat("Polozenie Z", &PhysicsModule::gravityPoints[i].z, -PhysicsModule::borderOfDomain-10, PhysicsModule::borderOfDomain-10);
				
				ImGui::TreePop();
            	ImGui::Spacing();
			}

		}
		if(ImGui::Button("Dodaj nowy punkt"))
		{
			PhysicsModule::addNewGravityCenter(glm::vec3(0.0f,0.0f,-10.0f));
		}
	}
	if (ImGui::CollapsingHeader("Box collision"))
	{
		ImGui::SliderFloat("Polozenie X", &PhysicsModule::centerOfDomain.x, -10, 10);
		ImGui::SliderFloat("Polozenie Y", &PhysicsModule::centerOfDomain.y, -10, 10);
		ImGui::SliderFloat("Polozenie Z", &PhysicsModule::centerOfDomain.z, -20, 0);
		ImGui::SliderFloat("Szerokosc polowy boku", &PhysicsModule::borderOfDomain, 1, 10);
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}