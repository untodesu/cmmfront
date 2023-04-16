#include <cmm_stub.hpp>
#include <thread>

bool CMMStub::is_busy() const
{
    return cmm_busy;
}

void CMMStub::cmd_move_at(const cmm_vec3_t &v)
{
    cmm_busy = true;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cmm_busy = false;
}

void CMMStub::cmd_point(const cmm_vec3_t &pos, const cmm_vec3_t &normal, cmm_vec3_t &out)
{
    cmm_busy = true;
    out[0] = pos[0];
    out[1] = pos[1];
    out[2] = pos[2];
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cmm_busy = false;
}
