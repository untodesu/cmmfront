#include <cmm_stub.hpp>
#include <random>
#include <thread>

bool CMMStub::get_cap(CmmCap cap) const
{
    switch(cap) {
        case CmmCap::DebugCMM:
            return true;
        default:
            return false;
    }
}

double CMMStub::get_opt_double(CmmOpt opt) const
{
    switch(opt) {
        case CmmOpt::DebugRandomness:
            return opt_randomness;
        case CmmOpt::DebugTimeDelay:
            return opt_time_delay;
        default:
            return std::numeric_limits<double>::max();
    }
}

int CMMStub::get_opt_int(CmmOpt opt) const
{
    switch(opt) {
        case CmmOpt::DebugResult:
            return static_cast<int>(result);
        default:
            return std::numeric_limits<int>::max();
    }
}

void CMMStub::set_opt_double(CmmOpt opt, double value)
{
    switch(opt) {
        case CmmOpt::DebugRandomness:
            opt_randomness = value;
            if(opt_randomness < 0.0)
                opt_randomness = 0.0;
            return;
        case CmmOpt::DebugTimeDelay:
            opt_time_delay = value;
            if(opt_time_delay < 0.0)
                opt_time_delay = 0.0;
            return;
    }
}

void CMMStub::set_opt_int(CmmOpt opt, int value)
{
    switch(opt) {
        case CmmOpt::DebugResult:
            result = static_cast<CmmResult>(value);
            return;
    }
}

const char *CMMStub::get_ident() const
{
    return "DebugCMM";
}

bool CMMStub::is_busy() const
{
    return cmm_busy;
}

CmmResult CMMStub::cmd_move_at(const Eigen::Vector3d &v)
{
    cmm_busy = true;
    if(opt_time_delay != 0.0)
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int64_t>(opt_time_delay * 1000.0)));
    cmm_busy = false;
    return result;
}

CmmResult CMMStub::cmd_point(const Eigen::Vector3d &pos, const Eigen::Vector3d &normal, Eigen::Vector3d &out)
{
    std::mt19937_64 device = std::mt19937_64{std::random_device{}()};
    std::uniform_real_distribution<float> dist = {};
    cmm_busy = true;
    out[0] = pos[0] + pos[0] * dist(device) * opt_randomness; // fake error
    out[1] = pos[1] + pos[1] * dist(device) * opt_randomness; // fake error
    out[2] = pos[2] + pos[2] * dist(device) * opt_randomness; // fake error
    if(opt_time_delay != 0.0)
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int64_t>(opt_time_delay * 1000.0)));
    cmm_busy = false;
    return result;
}

void CMMStub::set_result(CmmResult result)
{
    this->result = result;
}
