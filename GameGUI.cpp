#include "GameGUI.h"
#include "src/constants.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

bool G_GUI::RenderLauncher(const char *glsl_ver, int &length, int &width, int &height, bool &debug)
{
    GLFWwindow *window = glfwCreateWindow(1280, 720, "Game Settings", nullptr, nullptr);
    if(window == nullptr)
        return false;
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO(); 
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_ver); 

    bool launchApp = false;
     while (!glfwWindowShouldClose(window) && !launchApp)
    {
        glfwPollEvents();
     
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame(); 

        ImGui::Begin("Launcher");
        ImGui::SliderInt("Set Map Length", &length, 7, 15);
        ImGui::SliderInt("Set Map Width", &width, 7, 15);
        ImGui::SliderInt("Set map height (levels)", &height, 1, 10);
        ImGui::Checkbox("Enable Debug Mode", &debug);
        ImGui::NewLine();
        ImGui::NewLine();
        launchApp = ImGui::Button("Launch Game", ImVec2(1280, 120));
        ImGui::End();

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

    return true;
}

void GameGUI::load_data()
{
    this->batRoom.load = LoadTextureFromFile("images/room_bat.bmp", &this->batRoom.texture, &this->batRoom.width, &this->batRoom.height);
    IM_ASSERT(this->batRoom.load);
    this->emptyRoom.load = LoadTextureFromFile("images/room_empty.bmp", &this->emptyRoom.texture, &this->emptyRoom.width, &this->emptyRoom.height);
    IM_ASSERT(this->emptyRoom.load);
    this->goldRoom.load = LoadTextureFromFile("images/room_gold.bmp", &this->goldRoom.texture, &this->goldRoom.width, &this->goldRoom.height);
    IM_ASSERT(this->goldRoom.load);
    this->playerRoom.load = LoadTextureFromFile("images/room_player.bmp", &this->playerRoom.texture, &this->playerRoom.width, &this->playerRoom.height);
    IM_ASSERT(this->playerRoom.load);
    this->wumpusRoom.load = LoadTextureFromFile("images/room_wumpus.bmp", &this->wumpusRoom.texture, &this->wumpusRoom.width, &this->wumpusRoom.height);
    IM_ASSERT(this->wumpusRoom.load);
    this->ladderUpRoom.load = LoadTextureFromFile("images/room_laddUp.bmp", &this->ladderUpRoom.texture, &this->ladderUpRoom.width, &this->ladderUpRoom.height);
    IM_ASSERT(this->ladderUpRoom.load);
    this->ladderDownRoom.load = LoadTextureFromFile("images/room_laddDown.bmp", &this->ladderDownRoom.texture, &this->ladderDownRoom.width, &this->ladderDownRoom.height);
    IM_ASSERT(this->ladderDownRoom.load);
    this->pitRoom.load = LoadTextureFromFile("images/room_pit.bmp", &this->pitRoom.texture, &this->pitRoom.width, &this->pitRoom.height);
    IM_ASSERT(this->pitRoom.load);

    return;
}

void GameGUI::RenderMap(const vector<vector<Room> > map, const int len, const int wid, const Tasks player)
{
    ImGui::Begin("Game Map");
    for(int i = 0; i < len; ++i){
        //Each row is represented by rendering a 'line'
        ImGui::NewLine();
        for(int j = 0; j < wid; ++j){
            if(map[i][j].get_event() != NULL)
            switch(map[i][j].get_event()->get_id())
            {
                case WumpID:
                ImGui::SameLine(j * 64, 0); //Each room image is 64x64
                ImGui::Image((void*)(intptr_t) this->wumpusRoom.texture, ImVec2(this->wumpusRoom.width, this->wumpusRoom.height));
                break;
                case GoldID:
                ImGui::SameLine(j * 64, 0); //Each room image is 64x64
                ImGui::Image((void*)(intptr_t) this->goldRoom.texture, ImVec2(this->goldRoom.width, this->goldRoom.height));
                break;
                case BatsID:
                ImGui::SameLine(j * 64, 0); //Each room image is 64x64
                ImGui::Image((void*)(intptr_t) this->batRoom.texture, ImVec2(this->batRoom.width, this->batRoom.height));
                break;
                case PitID:
                ImGui::SameLine(j * 64, 0); //Each room image is 64x64
                ImGui::Image((void*)(intptr_t) this->pitRoom.texture, ImVec2(this->pitRoom.width, this->pitRoom.height));
                break;
                case LaddUID:
                ImGui::SameLine(j * 64, 0); //Each room image is 64x64
                ImGui::Image((void*)(intptr_t) this->ladderUpRoom.texture, ImVec2(this->ladderUpRoom.width, this->ladderUpRoom.height));
                break;
                case LaddID:
                ImGui::SameLine(j * 64, 0); //Each room image is 64x64
                ImGui::Image((void*)(intptr_t) this->ladderDownRoom.texture, ImVec2(this->ladderDownRoom.width, this->ladderDownRoom.height));
                break;
                default:
                break;
            }
            else if(i == player.x && j == player.y){
                ImGui::SameLine(j * 64, 0);
                ImGui::Image((void*)(intptr_t) this->playerRoom.texture, ImVec2(this->playerRoom.width, this->playerRoom.height));
            }
            else{
                ImGui::SameLine(j * 64, 0);
                ImGui::Image((void*)(intptr_t) this->emptyRoom.texture, ImVec2(this->emptyRoom.width, this->emptyRoom.height));
            }
        }
    }
    ImGui::End();
    return;
}

void GameGUI::DisplayPercepts(const vector<string> percepts)
{
    if(percepts.size() == 0)
        return;
    
    ImGui::Begin("Percept Notifications");

    for(int i = 0; i < percepts.size(); ++i){
        string percept = percepts[i];
        ImGui::Text(percept.c_str());
        ImGui::NewLine();
    }

    ImGui::End();
    return;
}

char GameGUI::GetInput()
{
    //Check for arrow key inputs
    bool upArrow = ImGui::IsKeyPressed(ImGuiKey_UpArrow);
    bool downArrow = ImGui::IsKeyPressed(ImGuiKey_DownArrow);
    bool leftArrow = ImGui::IsKeyPressed(ImGuiKey_LeftArrow);
    bool rightArrow = ImGui::IsKeyPressed(ImGuiKey_RightArrow);
    bool fireBow = ImGui::IsKeyPressed(ImGuiKey_F);
    //Check for wasd inputs
    bool wKey = ImGui::IsKeyPressed(ImGuiKey_W);
    bool aKey = ImGui::IsKeyPressed(ImGuiKey_A);
    bool sKey = ImGui::IsKeyPressed(ImGuiKey_S);
    bool dKey = ImGui::IsKeyPressed(ImGuiKey_D);
    bool eKey = ImGui::IsKeyPressed(ImGuiKey_E);

    if(upArrow || wKey)
        return 'w';
        
    if(downArrow || sKey)
        return 's';
        
    if(leftArrow || aKey)
        return 'a';
        
    if(rightArrow || dKey)
        return 'd';
        
    if(fireBow)
        return 'f';
    
    if(eKey)
        return 'e';

    return ' ';
}

void GameGUI::DisplayEncounter(const string msg)
{
    ImGui::Begin("Encounter notification");
    ImGui::Text(msg.c_str());
    ImGui::End();

    return;
}

void GameGUI::RenderPostMenu(const bool saveLoaded, bool &playAgain, bool &inputs, bool &loadSameMap)
{
    ImGui::Begin("Post-game menu");
    ImGui::Text("Do you want to play again?");

    if(ImGui::Button("Yes", ImVec2(128, 64)))
        playAgain = true;
    
    ImGui::SameLine(128, 8);
    if(ImGui::Button("No", ImVec2(128, 64))){
        playAgain = false;
        if(ImGui::Button("Confirm", ImVec2(64, 32)))
            inputs = true;
    }

    ImGui::NewLine();
    if(playAgain && saveLoaded){
        ImGui::Checkbox("Replay map configuration?", &loadSameMap);
        if(ImGui::Button("Confirm", ImVec2(64, 32)))
            inputs = true;
    }
    else
        if(ImGui::Button("Confirm", ImVec2(64, 32)))
            inputs = true;
    
    ImGui::End();

    return;
}