#ifndef D9A993B0_8990_4493_BABC_B6BDD134C20A
#define D9A993B0_8990_4493_BABC_B6BDD134C20A
#include <icmd.hpp>

class MoveCmd final : public ICmd {
public:
    MoveCmd() = default;
    virtual ~MoveCmd() = default;

    virtual CmdType get_type() const override;
    virtual std::string get_name() const override;
    virtual void on_execute(ICMM *cmm) override;
    virtual void on_draw_imgui() override;
    virtual bool validate() override;

private:
    Eigen::Vector3d position {};
    std::string display {};
};

#endif /* D9A993B0_8990_4493_BABC_B6BDD134C20A */
