#include "pch/stdafx.h"
#include "historytrail/HistoryTrailDialog.h"
#include "historytrail/HistoryTrailData.h"

namespace UKControllerPlugin {
    namespace HistoryTrail {

        IMPLEMENT_DYNAMIC(HistoryTrailDialog, CDialog)

            HistoryTrailDialog::HistoryTrailDialog(CWnd* pParent, HistoryTrailData * trailData)
            : CDialog(IDD_HISTORY_TRAIL, pParent)
        {
            this->data = trailData;
        }

        HistoryTrailDialog::HistoryTrailDialog()
        {

        }

        HistoryTrailDialog::~HistoryTrailDialog()
        {

        }

        /*
            Microsoft function to set up the control variables.
        */
        void HistoryTrailDialog::DoDataExchange(CDataExchange* pDX)
        {
            CDialog::DoDataExchange(pDX);
            DDX_Control(pDX, IDC_CHECK_DEGRADING, this->degradingCheck);
            DDX_Control(pDX, IDC_CHECK_FADING, this->fadingCheck);
            DDX_Control(pDX, IDC_CHECK_AA, this->antiAliasCheck);
            DDX_Control(pDX, IDC_TRAIL_LENGTH, this->trailLength);
            DDX_Control(pDX, IDC_TRAIL_TYPE, this->trailType);
            DDX_Control(pDX, IDC_TRAIL_COLOUR, this->trailColour);
            DDX_Control(pDX, IDC_SPIN_LENGTH, this->lengthSpinner);
            DDX_Control(pDX, IDC_DOT_SIZE, this->trailDotSize);
            DDX_Control(pDX, IDC_DOT_SIZE_SPIN, this->dotSizeSpinner);
            DDX_Control(pDX, IDC_MIN_FILTER_SPIN, this->minAltitudeSpinner);
            DDX_Control(pDX, IDC_MIN_FILTER_TEXT, this->minAltitudeFilter);
            DDX_Control(pDX, IDC_MAX_FILTER_SPIN, this->maxAltitudeSpinner);
            DDX_Control(pDX, IDC_MAX_FILTER_TEXT, this->maxAltitudeFilter);
        }

        /*
            Put the data into the fields.
        */
        BOOL HistoryTrailDialog::OnInitDialog(void)
        {
            CDialog::OnInitDialog();

            // Checkboxes
            this->antiAliasCheck.SetCheck((*this->data->antiAlias) ? BST_CHECKED : BST_UNCHECKED);
            this->fadingCheck.SetCheck((*this->data->fade) ? BST_CHECKED : BST_UNCHECKED);
            this->degradingCheck.SetCheck((*this->data->degrade) ? BST_CHECKED : BST_UNCHECKED);

            // Trail length
            this->lengthSpinner.SetRange(this->minTrailLength, this->maxTrailLength);
            this->lengthSpinner.SetPos(
                *this->data->length <= this->maxTrailLength && this->minTrailLength > 0
                    ? *this->data->length
                    : this->defaultTrailLength
            );

            // Dot size
            this->dotSizeSpinner.SetRange(this->minDotSize, this->maxDotSize);
            this->dotSizeSpinner.SetPos(
                *this->data->dotSize <= this->maxDotSize && this->minDotSize > 0
                    ? *this->data->dotSize
                    : this->defaultDotSize
                );

            // Trail type
            this->trailType.InsertString(this->trailTypeDiamond, L"Diamond");
            this->trailType.InsertString(this->trailTypeSquare, L"Square");
            this->trailType.InsertString(this->trailTypeCircle, L"Circle");
            if (
                *this->data->type == this->trailTypeDiamond ||
                *this->data->type == this->trailTypeCircle ||
                *this->data->type == this->trailTypeSquare
            ) {
                this->trailType.SetCurSel(*this->data->type);
            } else {
                this->trailType.SetCurSel(0);
            }

            // Colour
            this->trailColour.SetColor(*this->data->colour);

            // Max altitude
            this->maxAltitudeSpinner.SetRange32(this->minAltitudeValue, this->maxAltitudeValue);
            this->maxAltitudeSpinner.SetPos32(*this->data->maxAltitude);

            // Min altitude
            this->minAltitudeSpinner.SetRange32(this->minAltitudeValue, this->maxAltitudeValue);
            this->minAltitudeSpinner.SetPos32(*this->data->minAltitude);

            return TRUE;
        }

        /*
            Called when the Save button is pressed. We save data to the provided
            data structure, so whoever called us can get the new data items back.
        */
        void HistoryTrailDialog::OnOK(void)
        {
            *this->data->antiAlias = this->antiAliasCheck.GetCheck() == BST_CHECKED;
            *this->data->fade = this->fadingCheck.GetCheck() == BST_CHECKED;
            *this->data->degrade = this->degradingCheck.GetCheck() == BST_CHECKED;
            *this->data->length = this->lengthSpinner.GetPos();
            *this->data->dotSize = this->dotSizeSpinner.GetPos();
            *this->data->type = this->trailType.GetCurSel();
            *this->data->colour = this->trailColour.GetColor();
            *this->data->maxAltitude = this->maxAltitudeSpinner.GetPos32();
            *this->data->minAltitude = this->minAltitudeSpinner.GetPos32();

            this->EndDialog(0);
        }

        BEGIN_MESSAGE_MAP(HistoryTrailDialog, CDialog)
        END_MESSAGE_MAP()
    }  // namespace HistoryTrail
}  // namespace UKControllerPlugin
