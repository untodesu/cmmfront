#ifndef D6712DD6_FE35_4448_8A8A_9334D84F5117
#define D6712DD6_FE35_4448_8A8A_9334D84F5117
#include <icmd.hpp>
#include <string>

class CommentCmd final : public ICmd {
public:
    CommentCmd() = default;
    virtual ~CommentCmd() = default;

    virtual CmdType get_type() const override;
    virtual std::string get_name() const override;
    virtual size_t get_pcounter() const override;
    virtual void set_pcounter(size_t val) override;
    virtual bool on_execute(ICMM *cmm) override;
    virtual void on_draw_imgui() override;
    virtual bool validate() override;
    virtual void on_load(std::ifstream &file) override;
    virtual void on_save(std::ofstream &file) const override;

private:
    std::string comment_text {};
    size_t my_pcounter {0};
    bool active {true};
};

#endif/* D6712DD6_FE35_4448_8A8A_9334D84F5117 */
