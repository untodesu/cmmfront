#ifndef D6712DD6_FE35_4448_8A8A_9334D84F5117
#define D6712DD6_FE35_4448_8A8A_9334D84F5117
#include <icmd.hpp>
#include <string>

class CommentCmd final : public ICmd {
public:
    CommentCmd() = default;
    virtual ~CommentCmd() = default;

    virtual CmdType get_type() const override;
    virtual const char *get_name() const override;
    virtual void on_execute(ICMM *cmm) override;
    virtual void on_draw_imgui() override;

private:
    std::string comment_text {};
};

#endif/* D6712DD6_FE35_4448_8A8A_9334D84F5117 */
