#ifndef B9B8A19D_1E12_4686_9383_A5153E38CF69
#define B9B8A19D_1E12_4686_9383_A5153E38CF69
#include <deque>
#include <GLFW/glfw3.h>
#include <icmm.hpp>
#include <icmd.hpp>

namespace globals
{
extern GLFWwindow *window;
extern ICMM *machine;
extern std::deque<ICmd *> commands;
extern ICmd *selected_command;
extern bool run_until_selection;
extern bool popup_dialog;
} // namespace globals

#endif/* B9B8A19D_1E12_4686_9383_A5153E38CF69 */
