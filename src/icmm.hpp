#ifndef FAFDC50E_50C6_4604_BCC7_169A58FAD0C0
#define FAFDC50E_50C6_4604_BCC7_169A58FAD0C0
#include <Eigen/Dense>

enum class CmmCap {
    DebugCMM, // see cmm_stub.cpp
};

enum class CmmOpt {
    DebugRandomness, // simulate physical imperfections
    DebugTimeDelay, // simulate machine moving around
};

class ICMM {
public:
    virtual ~ICMM() = default;

    virtual bool get_cap(CmmCap cap) const = 0;
    virtual double get_opt(CmmOpt opt) const = 0;
    virtual void set_opt(CmmOpt opt, double value) = 0;

    virtual const char *get_ident() const = 0;

    // Returns true whenever a command is
    // being executed. Since cmd methods are
    // blocking, this is expected to be called
    // from somewhere like Florida (a separate thread)
    virtual bool is_busy() const = 0;

    // Blocking methods
    virtual void cmd_move_at(const Eigen::Vector3d &pos) = 0;
    virtual void cmd_point(const Eigen::Vector3d &pos, const Eigen::Vector3d &normal, Eigen::Vector3d &out) = 0;
};

#endif/* FAFDC50E_50C6_4604_BCC7_169A58FAD0C0 */
