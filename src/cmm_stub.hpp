#ifndef FD9FD02B_DB44_4F16_B1F8_DE921B138D6C
#define FD9FD02B_DB44_4F16_B1F8_DE921B138D6C
#include <icmm.hpp>
#include <thread>

class CMMStub final : public ICMM {
public:
    CMMStub() = default;
    virtual ~CMMStub() = default;

    virtual bool get_cap(CmmCap cap) const override;
    virtual double get_opt_double(CmmOpt opt) const override;
    virtual int get_opt_int(CmmOpt opt) const override;
    virtual void set_opt_double(CmmOpt opt, double value) override;
    virtual void set_opt_int(CmmOpt, int value) override;

    virtual const char *get_ident() const override;

    virtual bool is_busy() const override;

    virtual CmmResult cmd_move_at(const Eigen::Vector3d &v) override;
    virtual CmmResult cmd_point(const Eigen::Vector3d &pos, const Eigen::Vector3d &normal, Eigen::Vector3d &out) override;

    void set_result(CmmResult result);

private:
    bool cmm_busy {false};
    double opt_randomness {0.05};
    double opt_time_delay {1.0};
    CmmResult result {CmmResult::Ok};
};

#endif /* FD9FD02B_DB44_4F16_B1F8_DE921B138D6C */
