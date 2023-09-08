#include <iostream>
#include <stdio.h>
#include "src/game.h"
#include "GameGUI.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_glfw.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>


//TODO: implement an output window that lets displays output events
//TODO: implement an input functionality to feed the game state
 
static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
	if(!glfwInit())
        return 1;

#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
	////////////////////////////
    int len = 7, wid = 7, hei = 1;  //For the grid size of the the game's length, width, and height
    bool debug = false;             //True if the game should be ran in debug mode

    if(!G_GUI::RenderLauncher(glsl_version, len, wid, hei, debug)){
        return 1;
    }
    
	GLFWwindow *window = glfwCreateWindow(1280, 720, "Hunt the Wumpus", nullptr, nullptr);
    if(window == nullptr){
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO(); 
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
   
    ImGui::StyleColorsDark();
    io.AddKeyEvent(ImGuiKey_UpArrow, false);
    io.AddKeyEvent(ImGuiKey_DownArrow, false);
    io.AddKeyEvent(ImGuiKey_LeftArrow, false);
    io.AddKeyEvent(ImGuiKey_RightArrow, false);
    io.AddKeyEvent(ImGuiKey_W, false);
    io.AddKeyEvent(ImGuiKey_A, false);
    io.AddKeyEvent(ImGuiKey_S, false);
    io.AddKeyEvent(ImGuiKey_D, false);
    io.AddKeyEvent(ImGuiKey_E, false);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //Initialize ImGui for glfw/ OpenGL
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version); 

    //Creating the game
    Game instance;
    instance.create_instance(wid, len, hei, debug);
    Game backup = instance;
    bool playAgain = true;
    bool loadSameMap = false;
    bool inputs = false;

    //Run until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        //Process glfw event que
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame(); 


        char moveInput = ' ';

        //Game loop
        if(!instance.check_win() && instance.player_alive()){

            if(moveInput == ' ')
                io.SetAppAcceptingEvents(true);
            else
                io.SetAppAcceptingEvents(false);

            //Refresh the game map
            instance.update_map();

            //Creating a GameGUI object to render game's data
            GameGUI instanceGUI;
            //Load corresponding image data into the GameGUI object
            instanceGUI.load_data();
            //Render the game map, uses the loaded image data and information fed from the game map
            instanceGUI.RenderMap(instance.get_map(), len, wid, instance.get_player_data());
            //Display percepts through the GUI
            instanceGUI.DisplayPercepts(instance.get_percepts());

            moveInput = instanceGUI.GetInput();
            if(moveInput != ' '){
                instance.move(moveInput);
                io.ClearEventsQueue();
            }

            string encounterMsg = instance.get_encounter_message();
            if(!encounterMsg.empty()){
                instance.process_encounter();
                instanceGUI.DisplayEncounter(encounterMsg);
            }

        }
        else{
             if(!instance.player_alive())
                instance.set_save_loaded(true);
            else 
                instance.set_save_loaded(false);

            G_GUI::RenderPostMenu(instance.is_save_loaded(), playAgain, inputs, loadSameMap);

            if(inputs){
                if(!playAgain)
                    break;
            
                if(!loadSameMap){
                    instance = Game();
                    instance.create_instance(wid, len, hei, debug);
                    backup = instance;
                    loadSameMap = -1;
                    inputs = false;
                }
                else{
                    instance = backup;
                    backup = instance;
                    loadSameMap = -1;
                    inputs = false;
                }
            }
        }
        //Render the frame
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    

    return 0;
}
