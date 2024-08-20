#pragma 
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>

#include "Log.hpp"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

struct Table
{
    int row = 5;
    int col = 2;
    std::vector<std::vector<bool>> selected;
    std::vector<char*> cellValues;
    Table()
    {
        
        for (int r = 0; r < row; r++)
        {
            selected.push_back(std::vector<bool>(col, false));
        }

        for (int r=0;r<row;r++)
            for (int c = 0; c < col; c++)
            {
                cellValues.push_back(new char[32]);
                strcpy_s(cellValues[c + r * col],1, "");
            }
                
    
          
    }
    ~Table()
    {
        for (int r = 0; r < row; r++)
        {
            for (int c = 0; c < col; c++)
            {
                
            }
        }
    }
    void addCol()
    {
        col++;
        for (int r=0;r<row;r++)
        {
            selected[r].push_back(false);
        }
        for (int r = 0; r < row; r++)
        {
            
            for (int c = 0; c < col; c++)
            {
                if (c == col - 1)
                {
                    cellValues.insert(cellValues.begin()+(c+r*col), new char[32]);
                    strcpy_s(cellValues[c + r * col], 1, "");
                }

                
                    
            }
            
        }
        
    }
    void addRow()
    {
        row++;

        selected.push_back(std::vector<bool>(col, false));

        for (int r = 0; r < row; r++)
        {
            if (r == row - 1)
            {
                for (int c = 0; c < col; c++)
                {
                    cellValues.push_back(new char[32]);
                    strcpy_s(cellValues[c + r * col], 1, "");
                }
            }

        }

       
        
    }
    void deleteRow()
    {
        row--;
        selected.pop_back();

        for (int r = 0; r < row; r++)
        {
            if (r == row - 1)
            {
                for (int c = col; c >0; c--)
                {
                    cellValues.erase((cellValues.begin() + (c + r * col)));
                    
                }
            }

        }
       
    }
    void deleteCol()
    {
        col--;
        for (int r = 0; r < row; r++)
        {
            selected[r].pop_back();
           
        }
        for (int r = 0; r < row; r++)
        {

            for (int c = 0; c < col; c++)
            {
                if (c == col - 1)
                {
                    cellValues.erase((cellValues.begin()+1)+(c + r * col));
                    
                }

            }

        }
        
    }

};

int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Course Planner", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();
    

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    Table table;
    
    
    while (!glfwWindowShouldClose(window))
    {
   
        glfwPollEvents();
        // Start the Dear ImGui frame


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowSize(viewport->Size);
        //starts the imgui window with flags to make it full window size
        ImGui::Begin("hello",nullptr, ImGuiWindowFlags_NoDecoration| ImGuiWindowFlags_NoMove| ImGuiWindowFlags_NoResize);

        //bottons for adding and deleting col and rows
        if (ImGui::Button("add column +"))
        {
            table.addCol();

        }
            
        if (ImGui::Button("delete column -"))
        {
            if (table.col > 2)
                table.deleteCol();
        }
            
        
        if (ImGui::Button("add row +"))
        {
            table.addRow();
        }
            
        if (ImGui::Button("delete row -"))
        {
            if (table.row > 0)
                table.deleteRow();
        }
        //beginning of child text window
        ImGui::BeginChild("Instructions", ImVec2(0, 100), true);
        ImGui::TextUnformatted(
        "To use: -Double click a cell to enter text or select with arrow keys and use Enter.\n"
        "\t    -The 'Credits' cells will calculate the total credits from planned courses at the bottom.\n"
        "\t    -The 'Semester/Classes' cells are used to label the semesters and classes planned to take.\n"
        "\t    -When a col is added, an odd col will be a Semester col while an even will be a credits col, so click twice to add a semeseter and matching credits col.\n"
        "\t    -Suggestion: use the top cell as the self made label for the semester and fill in the intended classes below it.n\n");
        ImGui::EndChild();


        ImGui::BeginTable("table", table.col, ImGuiTableFlags_Borders| ImGuiTableFlags_RowBg| ImGuiTableFlags_Resizable| ImGuiTableFlags_Reorderable);

        //alternated the semester and credits headings
        for (int i = 0; i < table.col; i++)
        {
            if(i%2==0)
                ImGui::TableSetupColumn("Semester/Classes");
            else
                ImGui::TableSetupColumn("credits");
        }
 
        ImGui::TableHeadersRow();


        
        for (int r = 0; r < table.row; r++)
        {
            ImGui::TableNextRow();
            
            for (int c = 0; c < table.col; c++)
            {
                ImGui::TableNextColumn();
                ImGui::TableSetColumnIndex(c);
                ImGui::PushID(c + r * table.col);
                
                char buffer[64];

                if (table.selected[r][c])
                {
                    ImGui::InputText("##Edit", table.cellValues[c + r * table.col], ImGuiInputTextFlags_EnterReturnsTrue);
                    if (!ImGui::IsItemActive()&&ImGui::IsKeyPressed(ImGuiKey_Enter))
                    {    
                        table.selected[r][c] = false;
                    }
                }
                else
                {

                    snprintf(buffer, sizeof(buffer), table.cellValues[c+r*table.col]);//need to use ImGui IDs to make this work
                    if (ImGui::Selectable(table.cellValues[c + r * table.col], table.selected[r][c], ImGuiSelectableFlags_AllowDoubleClick))
                    {
                        if (ImGui::IsMouseDoubleClicked(0) || ImGui::IsKeyPressed(ImGuiKey_Enter))
                        {
                            
                            table.selected[r][c] = true;
                         }
                            
                    }
                }
                
                ImGui::PopID();
            }
           
       
        }
 
        ImGui::EndTable();

        //calculates the credit info at the bottom
        int credits = 0;
        for (int r = 0; r < table.row; r++)
        {

            for (int c = 0; c < table.col; c++)
            {
                if (c % 2 == 1);
                {               
                    credits+=std::atoi(table.cellValues[c + r * table.col]);
                }

            }

        }
        ImGui::Text("Credits: %d", credits);

        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}