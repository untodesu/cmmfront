#ifndef FD9FD02B_DB44_4F16_B1F8_DE921B138D6C
#define FD9FD02B_DB44_4F16_B1F8_DE921B138D6C
#include <icmm.hpp>
#include <thread>

class CMMStub final : public ICMM {
public:
    CMMStub() = default;
    virtual ~CMMStub() = default;

    virtual bool is_busy() const override;
    virtual void cmd_move_at(const cmm_vec3_t &v) override;
    virtual void cmd_point(const cmm_vec3_t &pos, const cmm_vec3_t &normal, cmm_vec3_t &out) override;

private:
    bool cmm_busy {false};
};

#endif /* FD9FD02B_DB44_4F16_B1F8_DE921B138D6C */
