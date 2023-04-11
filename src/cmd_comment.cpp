#include <cmd_comment.hpp>
#include <cmm_wrap.hpp>
#include <globals.hpp>
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
    if(!comment_text.empty()) {
        globals::popup_class = "CommentCmd";
        globals::popup_text = comment_text;
    }
    else {
        // UNDONE: InvalidCommand popup class
        globals::popup_class = "CommentCmd";
        globals::popup_text = "<EMPTY>";
    }
}

void CommentCmd::on_draw_imgui()
{
    ImGui::InputTextMultiline("Comment", &comment_text);
}
