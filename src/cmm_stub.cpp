#include <cmm_stub.hpp>
#include <random>
#include <thread>

bool CMMStub::is_busy() const
{
    return cmm_busy;
}

void CMMStub::cmd_move_at(const Eigen::Vector3d &v)
{
    cmm_busy = true;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    cmm_busy = false;
}

void CMMStub::cmd_point(const Eigen::Vector3d &pos, const Eigen::Vector3d &normal, Eigen::Vector3d &out)
{
    std::mt19937_64 device = std::mt19937_64{std::random_device{}()};
    std::uniform_real_distribution<float> dist = {};
    cmm_busy = true;
    out[0] = pos[0] + pos[0] * dist(device) * 0.01f; // 1% fake error
    out[1] = pos[1] + pos[1] * dist(device) * 0.01f; // 1% fake error
    out[2] = pos[2] + pos[2] * dist(device) * 0.01f; // 1% fake error
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cmm_busy = false;
}
