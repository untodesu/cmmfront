#ifndef BBBE6561_36D8_450B_8305_D89071D8EDA1
#define BBBE6561_36D8_450B_8305_D89071D8EDA1
#include <icmd.hpp>
#include <string>

class ReportCmd final : public ICmd {
public:
    ReportCmd() = default;
    virtual ~ReportCmd() = default;

    void set_name(const std::string &name);

    virtual CmdType get_type() const override;
    virtual std::string get_name() const override;
    virtual size_t get_pcounter() const override;
    virtual void set_pcounter(size_t val) override;
    virtual void on_execute(ICMM *cmm) override;
    virtual void on_draw_imgui() override;
    virtual bool validate() override;
    virtual void on_load(std::ifstream &file) override;
    virtual void on_save(std::ofstream &file) const override;

private:
    const ICmd *target {nullptr};
    size_t my_pcounter {0};
    std::string name {};
};

#endif/* BBBE6561_36D8_450B_8305_D89071D8EDA1 */
