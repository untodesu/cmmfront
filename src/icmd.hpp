#ifndef BB4D5ADF_60E9_4A8E_9A70_49C1D7972980
#define BB4D5ADF_60E9_4A8E_9A70_49C1D7972980
#include <icmm.hpp>
#include <string>
#include <fstream>

enum class CmdType {
    Comment,
    MoveAtXYZ,
    MeasurePoint,
    MeasurePlane,
    MeasureCircle,
    Report,
};

class ICmd {
public:
    virtual ~ICmd() = default;
    virtual CmdType get_type() const = 0;
    virtual std::string get_name() const = 0;
    virtual size_t get_pcounter() const = 0;
    virtual void set_pcounter(size_t val) = 0;
    virtual bool on_execute(ICMM *cmm) = 0;
    virtual void on_draw_imgui() = 0;
    virtual bool validate() = 0;
    virtual void on_load(std::ifstream &file) = 0;
    virtual void on_save(std::ofstream &file) const = 0;
};

#endif/* BB4D5ADF_60E9_4A8E_9A70_49C1D7972980 */
