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
class PointCmd final : public ICmd {
public:
    PointCmd() = default;
    virtual ~PointCmd() = default;

    void set_name(const std::string &name);
    const Eigen::Vector3d &get_calc_normal() const;
    const Eigen::Vector3d &get_calc_position() const;
    const Eigen::Vector3d &get_real_position() const;

    virtual CmdType get_type() const override;
    virtual std::string get_name() const override;
    virtual size_t get_pcounter() const override;
    virtual void set_pcounter(size_t val) override;
    virtual void on_execute(ICMM *cmm) override;
    virtual void on_draw_imgui() override;
    virtual bool validate() override;

private:
    std::string name {};
    Eigen::Vector3d calc_normal {};
    Eigen::Vector3d calc_position {};
    Eigen::Vector3d real_position {};
    Eigen::Vector3d calc_proj_target {};
    size_t my_pcounter {0};
    PointType point_type {PointType::Generic};
    PlaneCmd *target_plane {nullptr};
};

#endif /* __SRC_CMD_POINT_HPP__ */
