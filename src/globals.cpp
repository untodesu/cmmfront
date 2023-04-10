#include <globals.hpp>

GLFWwindow *globals::window = nullptr;
ICMM *globals::machine = nullptr;
ICmd *globals::selected_command = nullptr;
std::deque<ICmd *> globals::commands = {};
bool globals::run_until_selection = false;
bool globals::is_running_program = false;
bool globals::is_showing_popup = false;
std::string globals::popup_text = {};
