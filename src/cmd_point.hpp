#ifndef __SRC_CMD_POINT_HPP__
#define __SRC_CMD_POINT_HPP__
#include <icmd.hpp>

class PointCmd final : public ICmd {
public:
    PointCmd() = default;
    virtual ~PointCmd() = default;

    void set_name(const std::string &name);
    const Eigen::Vector3d &get_in_normal() const;
    const Eigen::Vector3d &get_in_position() const;
    const Eigen::Vector3d &get_real_position() const;

    virtual CmdType get_type() const override;
    virtual std::string get_name() const override;
    virtual void on_execute(ICMM *cmm) override;
    virtual void on_draw_imgui() override;
    virtual bool validate() override;

private:
    std::string name {};
    Eigen::Vector3d in_normal {};
    Eigen::Vector3d in_position {};
    Eigen::Vector3d real_position {};
};

#endif /* __SRC_CMD_POINT_HPP__ */
