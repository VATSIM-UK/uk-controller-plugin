#pragma once

namespace UKControllerPlugin {
    namespace Bootstrap {

        /*
            Class for recognising when a duplicate plugin
            has been loaded.
        */
        class DuplicatePlugin
        {
            public:
                DuplicatePlugin();
                ~DuplicatePlugin(void);
                bool Duplicate(void) const;

            private:

                // Whether or not the plugin is a dupe
                bool isDuplicate;

                // A handle to the file we're using to recognise duplicates.
                HANDLE hMapFile;

                // The name we use in the page file for the information
                const TCHAR * szName = TEXT("UkcpAlreadyRunningFlag");

                // The buffersize for the flag
                const DWORD buffSize = 256;
        };
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
