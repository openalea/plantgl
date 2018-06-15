add_subdirectory(plantgl)

install(DIRECTORY ./plantgl
        DESTINATION include
        FILES_MATCHING PATTERN "*.h"
        PATTERN "gui/exe" EXCLUDE
        PATTERN "gui/base/interface" EXCLUDE
        PATTERN "gui/viewer/interface" EXCLUDE)