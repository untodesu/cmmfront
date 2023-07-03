#include <cmm_wrap.hpp>
#include <globals.hpp>
#include <icmd.hpp>
#include <iostream>

void cmm_wrap::step()
{
    if(!globals::commands.empty() && globals::current != globals::commands.end()) {
        ICmd *command = (*globals::current);

        // Run whatever code the command implements
        command->on_execute(globals::machine);

        globals::current++;
        return;
    }
    else {
        globals::is_running_program = false;
        return;
    }
}

void cmm_wrap::abort()
{
    globals::is_running_program = false;
    globals::current = globals::commands.end();
}
