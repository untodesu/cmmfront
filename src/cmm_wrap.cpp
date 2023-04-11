#include <cmm_wrap.hpp>
#include <globals.hpp>
#include <icmd.hpp>
#include <iostream>

// Effectively the program counter
static std::deque<ICmd *>::iterator cmm_pc = globals::commands.end();

void cmm_wrap::run()
{
    if(cmm_pc == globals::commands.end())
        cmm_pc = globals::commands.begin();
    globals::is_running_program = true;
}

void cmm_wrap::step()
{
    if(cmm_pc != globals::commands.end()) {
        ICmd *command = (*cmm_pc);

        // Run whatever code the command implements
        command->on_execute(globals::machine);

        if(globals::run_until_selection && (command == globals::selected_command)) {
            // Stop (finish) the execution
            globals::is_running_program = false;
            cmm_pc = globals::commands.end();
            return;
        }

        cmm_pc++;
    }
    else {
        globals::is_running_program = false;
        return;
    }
}

void cmm_wrap::abort()
{
    globals::is_running_program = false;
    cmm_pc = globals::commands.end();
}
