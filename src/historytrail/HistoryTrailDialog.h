#pragma once

namespace UKControllerPlugin {
    namespace HistoryTrail {
        struct HistoryTrailData;
    }  // namespace HistoryTrail
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace HistoryTrail {
        /*
            This class handles the dialog box for changing history trail settings.
            It takes a pointer to a structure that in turn takes pointers to data - allowing
            it to change variables as required when it closes.
        */
        class HistoryTrailDialog : public CDialog
        {
            DECLARE_DYNAMIC(HistoryTrailDialog)

        public:
            HistoryTrailDialog(CWnd* pParent, HistoryTrailData * trailData);
            virtual ~HistoryTrailDialog();
            BOOL OnInitDialog();
            void OnOK(void);

            // Maximum renderable trail length
            const int maxTrailLength = 50;

            // Minimum trail length
            const int minTrailLength = 1;

            // Default trail length
            const int defaultTrailLength = 15;

            // Highest dot size we can have
            const int minDotSize = 1;

            // Lowest dot size we can have
            const int maxDotSize = 10;

            // The minimum altitude on the altitude spinners
            const int minAltitudeValue = 0;

            // The max altitude
            const int maxAltitudeValue = 99999;

            // The default dotSize;
            const int defaultDotSize = 4;

            // Diamond trail type
            const int trailTypeDiamond = 0;

            // Square trail type
            const int trailTypeSquare = 1;

            // Circle trail type
            const int trailTypeCircle = 2;

            // Dialog Data
#ifdef AFX_DESIGN_TIME
            enum { IDD = IDD_DIALOG1 };  // namespace HistoryTrail
#endif

        protected:
            HistoryTrailDialog(void);
            virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

            DECLARE_MESSAGE_MAP()

        private:
            // Data to set up the dialog and return on close
            HistoryTrailData * data;

            // Checkbox for degrading trails
            CButton degradingCheck;

            // Checkbox for fading trails
            CButton fadingCheck;

            // Checkbox for anti-aliased trails
            CButton antiAliasCheck;

            // The actual box for trail length
            CEdit trailLength;

            // The spinner that controls the trail length box
            CSpinButtonCtrl lengthSpinner;

            // Box for the trail dot size.
            CEdit trailDotSize;

            // Spinner to control dot size.
            CSpinButtonCtrl dotSizeSpinner;

            // The trail type drop down
            CComboBox trailType;

            // The trail colour picker
            CMFCColorButton trailColour;

            // Edit box for minimum altitude filter
            CEdit minAltitudeFilter;

            // Spinner for minimum altitude
            CSpinButtonCtrl minAltitudeSpinner;

            // Edit box for the maximum altitude filter
            CEdit maxAltitudeFilter;

            // Spinner for maximum altitude
            CSpinButtonCtrl maxAltitudeSpinner;
        };
    }  // namespace HistoryTrail
}  // namespace UKControllerPlugin
