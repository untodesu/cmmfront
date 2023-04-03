#ifndef FAFDC50E_50C6_4604_BCC7_169A58FAD0C0
#define FAFDC50E_50C6_4604_BCC7_169A58FAD0C0

// ICMM is de-facto a glue interface between the
// overall CMM software code that uses Eigen (I think...)
// and my code that uses (will use - 2023-04-03) GLM,
// so using POD types seems like a good idea.
// NOTE: double the precision, double the fun
using cmm_vec3_t = double[3];

class ICMM {
public:
    virtual ~ICMM() = default;

    // Returns true whenever a command is
    // being executed. Since cmd methods are
    // blocking, this is expected to be called
    // from somewhere like Florida (a separate thread)
    virtual bool is_busy() const = 0;

    // Blocking methods
    virtual void cmd_move_at(const cmm_vec3_t &v) = 0;
    virtual void cmd_point(const cmm_vec3_t &pos, const cmm_vec3_t &normal, cmm_vec3_t &out) = 0;
};

#endif/* FAFDC50E_50C6_4604_BCC7_169A58FAD0C0 */
