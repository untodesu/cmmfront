#ifndef FAFDC50E_50C6_4604_BCC7_169A58FAD0C0
#define FAFDC50E_50C6_4604_BCC7_169A58FAD0C0
#include <Eigen/Dense>

class ICMM {
public:
    virtual ~ICMM() = default;

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
