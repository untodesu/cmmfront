#ifndef FAFDC50E_50C6_4604_BCC7_169A58FAD0C0
#define FAFDC50E_50C6_4604_BCC7_169A58FAD0C0
#include <Eigen/Dense>

enum class CmmCap {
    DebugCMM, // see cmm_stub.cpp
};

enum class CmmOpt {
    DebugRandomness, // simulate physical imperfections
    DebugTimeDelay, // simulate machine moving around
    DebugResult, // Always send this as a result
};

enum class CmmResult {
    Ok, // Command finished successfully
    Aborted, // Meat layer between CMM and the computer pressed the big red button
    HardwareFail, // Something happened hardware-side (probe may've `hit' something)
};

class ICMM {
public:
    virtual ~ICMM() = default;

    virtual bool get_cap(CmmCap cap) const = 0;
    virtual double get_opt_double(CmmOpt opt) const = 0;
    virtual int get_opt_int(CmmOpt opt) const = 0;
    virtual void set_opt_double(CmmOpt opt, double value) = 0;
    virtual void set_opt_int(CmmOpt opt, int value) = 0;

    virtual const char *get_ident() const = 0;

    // Returns true whenever a command is
    // being executed. Since cmd methods are
    // blocking, this is expected to be called
    // from somewhere like Florida (a separate thread)
    virtual bool is_busy() const = 0;

    // Blocking methods
    virtual CmmResult cmd_move_at(const Eigen::Vector3d &pos) = 0;
    virtual CmmResult cmd_point(const Eigen::Vector3d &pos, const Eigen::Vector3d &normal, Eigen::Vector3d &out) = 0;
};

constexpr inline static const char *cmm_result_str(CmmResult result)
{
    if(result == CmmResult::Aborted)
        return "Aborted by operator";
    if(result == CmmResult::HardwareFail)
        return "Hardware failure";
    return "No error";
}

#endif/* FAFDC50E_50C6_4604_BCC7_169A58FAD0C0 */
