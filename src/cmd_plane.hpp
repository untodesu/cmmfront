#ifndef BF45BB31_9558_454D_B340_0FFE709441C9
#define BF45BB31_9558_454D_B340_0FFE709441C9
#include <cmd_point.hpp>
#include <unordered_set>
#include <plane.hpp>

class PlaneCmd final : public ICmd {
public:
    PlaneCmd() = default;
    virtual ~PlaneCmd() = default;

    void set_name(const std::string &name);
    const Plane &get_calc_plane() const;
    const Plane &get_real_plane() const;

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
    std::string name {};
    Plane calc_plane {};
    Plane real_plane {};
    std::unordered_set<const PointCmd *> points {};
    size_t my_pcounter {0};
};

#endif /* BF45BB31_9558_454D_B340_0FFE709441C9 */
