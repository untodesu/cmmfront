#include <cmd_comment.hpp>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <iostream>

CmdType CommentCmd::get_type() const
{
    return CmdType::Comment;
}

const char *CommentCmd::get_name() const
{
    return "CommentCmd";
}

void CommentCmd::on_execute(ICMM *cmm)
{
    // UNDONE: show a window
    std::cerr << "This should pause execution" << std::endl;
    std::cerr << comment_text << std::endl;
}

void CommentCmd::on_draw_imgui()
{
    ImGui::InputTextMultiline("Comment", &comment_text);
}
