#ifndef GAMEGUI_H
#define GAMEGUI_H

#include "src/game.h"
#include <GLFW/glfw3.h>
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_glfw.h"

namespace G_GUI
{
    bool RenderLauncher(const char* glsl_ver, int&, int&, int&, bool&);
    bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
    void RenderPostMenu(const bool saveLoaded, bool &playAgain, bool &inputs, bool &loadSameMap);
}

struct GImage
{
    int width = 0;
    int height = 0;
    GLuint texture = 0;
    bool load;
};


class GameGUI
{
private:
    GImage emptyRoom;
    GImage batRoom;
    GImage pitRoom;
    GImage playerRoom;
    GImage goldRoom;
    GImage wumpusRoom;
    GImage ladderUpRoom;
    GImage ladderDownRoom;

public:
    void load_data();
    void RenderMap(const vector<vector<Room> > map, const int, const int, const Tasks player);
    void DisplayPercepts(const vector<string>);
    void DisplayEncounter(const string);
    void RenderPostMenu(const bool saveLoaded, bool &playAgain, bool &inputs, bool &loadSameMap);
    char GetInput();

};

#endif