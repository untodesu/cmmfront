#ifndef __SRC_CMD_POINT_HPP__
#define __SRC_CMD_POINT_HPP__
#include <icmd.hpp>

enum class PointType {
    Generic = 0,
    PlaneProj = 1,
    CircleCenter = 2,
    Count,
};

class PlaneCmd;
class CircleCmd;
class PointCmd final : public ICmd {
public:
    PointCmd() = default;
    virtual ~PointCmd() = default;

    void set_name(const std::string &name);
    const Eigen::Vector3d &get_calc_normal() const;
    const Eigen::Vector3d &get_calc_point() const;
    const Eigen::Vector3d &get_real_normal() const;
    const Eigen::Vector3d &get_real_point() const;

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
    std::string name {};
    Eigen::Vector3d calc_normal {};
    Eigen::Vector3d calc_point {};
    Eigen::Vector3d real_normal {};
    Eigen::Vector3d real_point {};
    Eigen::Vector3d calc_proj_target {};
    size_t my_pcounter {0};
    PointType point_type {PointType::Generic};
    PlaneCmd *target_plane {nullptr};
    CircleCmd *target_circle {nullptr};
};

#endif /* __SRC_CMD_POINT_HPP__ */
