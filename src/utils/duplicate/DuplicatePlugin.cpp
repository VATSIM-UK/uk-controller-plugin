#include "duplicate/DuplicatePlugin.h"

namespace UKControllerPlugin {
    namespace Duplicate {

        DuplicatePlugin::DuplicatePlugin()
        {
            // Check for existing handle
            this->hMapFile = OpenFileMapping(
                FILE_MAP_ALL_ACCESS,  // read access
                FALSE,  // do not inherit the name
                this->szName  // name of mapping object
            );

            // We are a duplicate plugin
            if (hMapFile != NULL) {
                CloseHandle(this->hMapFile);
                this->isDuplicate = true;
                this->hMapFile = NULL;
                return;
            }

            hMapFile = CreateFileMapping(
                INVALID_HANDLE_VALUE,    // use paging file
                NULL,                    // default security
                PAGE_READWRITE,          // read/write access
                0,                       // maximum object size (high-order DWORD)
                this->buffSize,          // maximum object size (low-order DWORD)
                this->szName             // name of mapping object
            );

            this->isDuplicate = false;
        }

        /*
            Close the handle if we need to.
        */
        DuplicatePlugin::~DuplicatePlugin()
        {
            if (this->hMapFile != NULL) {
                CloseHandle(hMapFile);
            }
        }

        bool DuplicatePlugin::Duplicate(void) const
        {
            return this->isDuplicate;
        }

    } // namespace Duplicate
}  // namespace UKControllerPlugin
