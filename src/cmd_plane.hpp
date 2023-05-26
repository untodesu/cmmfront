#ifndef BF45BB31_9558_454D_B340_0FFE709441C9
#define BF45BB31_9558_454D_B340_0FFE709441C9
#include <cmd_point.hpp>
#include <vector>

class PlaneCmd final : public ICmd {
public:
    PlaneCmd() = default;
    virtual ~PlaneCmd() = default;

    void set_name(const std::string &name);
    const Eigen::Vector3d &get_calc_normal() const;
    const Eigen::Vector3d &get_calc_position() const;
    const Eigen::Vector3d &get_real_normal() const;
    const Eigen::Vector3d &get_real_position() const;

    virtual CmdType get_type() const override;
    virtual std::string get_name() const override;
    virtual void on_execute(ICMM *cmm) override;
    virtual void on_draw_imgui() override;
    virtual bool validate() override;

private:
    std::string name {};
    Eigen::Vector3d calc_normal {};
    Eigen::Vector3d calc_offset {};
    Eigen::Vector3d real_normal {};
    Eigen::Vector3d real_offset {};
    std::vector<std::string> point_names {};
    std::vector<const PointCmd *> points {};
};

#endif /* BF45BB31_9558_454D_B340_0FFE709441C9 */
