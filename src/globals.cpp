#include <globals.hpp>

GLFWwindow *globals::window = nullptr;
ICMM *globals::machine = nullptr;
ICmd *globals::selected_command = nullptr;
std::deque<ICmd *> globals::commands = {};
std::deque<ICmd *>::iterator globals::current = {};
bool globals::is_running_program = false;
std::string globals::popup_class = {};
std::string globals::popup_text = {};
