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

void GuiModule::draw(PhysicsModule* physics)
{
    ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Ustawienia");
	if (ImGui::CollapsingHeader("Sily"))
	{
		ImGui::Checkbox("Grawitacja", &(physics->gravity));
		ImGui::Checkbox("Opor aerodynamiczny", &(physics->aero));
		ImGui::SliderFloat("Potega stalej oporu aerodyn.", &physics->mu, -3.0f,-5.0f);
	}

	if (ImGui::CollapsingHeader("Punkty przyciagania grawitacyjnego"))
	{
		for (int i=0; i<physics->gravityPoints.size(); i++)
		{

			if (ImGui::TreeNode(("Punkt " +  std::to_string(i+1)).c_str()))
			{
				ImGui::SliderFloat("Polozenie X", &physics->gravityPoints[i].x, -physics->borderOfDomain, physics->borderOfDomain);
				ImGui::SliderFloat("Polozenie Y", &physics->gravityPoints[i].y, -physics->borderOfDomain, physics->borderOfDomain);
				ImGui::SliderFloat("Polozenie Z", &physics->gravityPoints[i].z, -physics->borderOfDomain-10, physics->borderOfDomain-10);
				
				ImGui::TreePop();
            	ImGui::Spacing();
			}

		}
		if(ImGui::Button("Dodaj nowy punkt"))
		{
				physics->addNewGravityCenter(glm::vec3(0.0f,0.0f,-10.0f));
		}
	}
	if (ImGui::CollapsingHeader("Box collision"))
	{
		ImGui::SliderFloat("Polozenie X", &physics->centerOfDomain.x, -10, 10);
		ImGui::SliderFloat("Polozenie Y", &physics->centerOfDomain.y, -10, 10);
		ImGui::SliderFloat("Polozenie Z", &physics->centerOfDomain.z, -20, 0);
		ImGui::SliderFloat("Szerokosc polowy boku", &physics->borderOfDomain, 1, 10);
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}