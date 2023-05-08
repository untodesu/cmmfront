#ifndef FD9FD02B_DB44_4F16_B1F8_DE921B138D6C
#define FD9FD02B_DB44_4F16_B1F8_DE921B138D6C
#include <icmm.hpp>
#include <thread>

class CMMStub final : public ICMM {
public:
    CMMStub() = default;
    virtual ~CMMStub() = default;

    virtual bool is_busy() const override;
    virtual void cmd_move_at(const Eigen::Vector3d &v) override;
    virtual void cmd_point(const Eigen::Vector3d &pos, const Eigen::Vector3d &normal, Eigen::Vector3d &out) override;

private:
    bool cmm_busy {false};
};

#endif /* FD9FD02B_DB44_4F16_B1F8_DE921B138D6C */
