#ifndef BB4D5ADF_60E9_4A8E_9A70_49C1D7972980
#define BB4D5ADF_60E9_4A8E_9A70_49C1D7972980
#include <icmm.hpp>
#include <string>

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
    virtual void on_execute(ICMM *cmm) = 0;
    virtual void on_draw_imgui() = 0;
    virtual bool validate() = 0;
};

#endif/* BB4D5ADF_60E9_4A8E_9A70_49C1D7972980 */
