#ifndef FACTSYSTEM_H
#define FACTSYSTEM_H

#include "Fact.h"
#include "FactMetaData.h"
#include "QGCToolbox.h"

class FactSystem : public QGCTool
{
    Q_OBJECT
public:
    /// All access to FactSystem is through FactSystem::instance, so constructor is private
    FactSystem(QGCApplication* app, QGCToolbox* toolbox);

    // Override from QGCTool
    virtual void setToolbox(QGCToolbox *toolbox);

    typedef enum {
        ParameterProvider,
    } Provider_t;

    static const int defaultComponentId = -1;

private:
    static const char* _factSystemQmlUri;   ///< URI for FactSystem QML imports

};

#endif // FACTSYSTEM_H
