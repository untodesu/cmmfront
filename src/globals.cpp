#include <globals.hpp>

GLFWwindow *globals::window = nullptr;
ICMM *globals::machine = nullptr;
std::deque<ICmd *> globals::commands = {};
ICmd *globals::selected_command = nullptr;
bool globals::run_until_selection = false;
bool globals::popup_dialog = false;
