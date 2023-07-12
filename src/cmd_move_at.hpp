#ifndef D9A993B0_8990_4493_BABC_B6BDD134C20A
#define D9A993B0_8990_4493_BABC_B6BDD134C20A
#include <icmd.hpp>

class MoveCmd final : public ICmd {
public:
    MoveCmd() = default;
    virtual ~MoveCmd() = default;

    virtual CmdType get_type() const override;
    virtual std::string get_name() const override;
    virtual size_t get_pcounter() const override;
    virtual void set_pcounter(size_t val) override;
    virtual void on_execute(ICMM *cmm) override;
    virtual void on_draw_imgui() override;
    virtual bool validate() override;
    virtual void json_import(const rapidjson::Document &json, size_t pcounter) override;
    virtual void json_export(rapidjson::Document &json, size_t pcounter) const override;

private:
    Eigen::Vector3d position {};
    std::string display {};
    size_t my_pcounter {0};
};

#endif /* D9A993B0_8990_4493_BABC_B6BDD134C20A */
