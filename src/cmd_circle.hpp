#ifndef EF2E8C9C_D0CB_4851_A86F_25CADAA77580
#define EF2E8C9C_D0CB_4851_A86F_25CADAA77580
#include <cmd_point.hpp>
#include <unordered_set>
#include <plane.hpp>

class CircleCmd final : public ICmd {
public:
    CircleCmd() = default;
    virtual ~CircleCmd() = default;

    void set_name(const std::string &name);
    const Plane &get_calc_plane() const;
    const Plane &get_real_plane() const;
    const Eigen::Vector3d &get_calc_point() const;
    const Eigen::Vector3d &get_real_point() const;
    const float get_calc_radius() const;
    const float get_real_radius() const;

    virtual CmdType get_type() const override;
    virtual std::string get_name() const override;
    virtual size_t get_pcounter() const override;
    virtual void set_pcounter(size_t val) override;
    virtual void on_execute(ICMM *cmm) override;
    virtual void on_draw_imgui() override;
    virtual bool validate() override;

    void solve_calc();
    void solve_real(); 

private:
    std::string name {};
    Plane calc_plane {};
    Plane real_plane {};
    std::unordered_set<const PointCmd *> points {};
    Eigen::Vector3d calc_center {};
    Eigen::Vector3d real_center {};
    float calc_radius {0.0f};
    float real_radius {0.0f};
    size_t my_pcounter {0};
};

#endif/* EF2E8C9C_D0CB_4851_A86F_25CADAA77580 */
