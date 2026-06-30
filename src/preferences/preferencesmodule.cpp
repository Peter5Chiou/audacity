#define LOG_TAG "Preferences"
#include "log.h"
#include "preferencesmodule.h"

#include <QtQml>

#include "framework/global/modularity/ioc.h"

#include "framework/interactive/iinteractiveuriregister.h"

#include "importexport/import/types/importtypes.h"

using namespace au::preferences;

std::string PreferencesModule::moduleName() const
{
    return "preferences";
}

void PreferencesModule::registerUiTypes()
{
    qmlRegisterUncreatableType<au::importexport::TempoDetectionPref>(
        "Audacity.Preferences", 1, 0, "TempoDetection", "Not creatable from QML");
}

void PreferencesModule::resolveImports()
{
    LOGI() << "PreferencesModule::resolveImports() called";
    auto ir = globalIoc()->resolve<muse::interactive::IInteractiveUriRegister>(moduleName());
    if (ir) {
        LOGI() << "Registering URI audacity://preferences";
        ir->registerQmlUri(muse::Uri("audacity://preferences"), "Audacity.Preferences", "PreferencesDialog");
    } else {
        LOGE() << "Failed to resolve IInteractiveUriRegister for " << moduleName();
    }
}
