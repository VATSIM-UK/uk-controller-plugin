#include "pch/stdafx.h"
#include "historytrail/HistoryTrailDialog.h"
#include "historytrail/HistoryTrailData.h"
#include "dialog/DialogCallArgument.h"

using UKControllerPlugin::Dialog::DialogCallArgument;

namespace UKControllerPlugin {
    namespace HistoryTrail {

        /*
            Public procedure for the window
        */
        LRESULT HistoryTrailDialog::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            if (msg == WM_INITDIALOG) {
                LogInfo("History trail dialog opened");
                SetWindowLongPtr(
                    hwnd,
                    GWLP_USERDATA,
                    reinterpret_cast<DialogCallArgument *>(lParam)->dialogArgument
                );
            } else if (msg == WM_DESTROY) {
                SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
                LogInfo("History trail dialog closed");
            }

            HistoryTrailDialog * dialog = reinterpret_cast<HistoryTrailDialog *>(
                GetWindowLongPtr(hwnd, GWLP_USERDATA)
            );
            return dialog ? dialog->_WndProc(hwnd, msg, wParam, lParam) : FALSE;
        }

        /*
            Internal procedure for the window that can reference the class
        */
        LRESULT HistoryTrailDialog::_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            switch (msg) {
                // Initialise
                case WM_INITDIALOG: {
                    this->InitDialog(hwnd, lParam);
                    return TRUE;
                };
                // Window Closed
                case WM_CLOSE: {
                    this->TidyUpDialog();
                    EndDialog(hwnd, wParam);
                    return TRUE;
                }
                // Drawing
                case WM_DRAWITEM: {
                    LPDRAWITEMSTRUCT drawItem = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
                    if (drawItem->CtlID == IDC_TRAIL_COLOUR) {
                        this->DrawColourButton(drawItem);
                        return TRUE;
                    }
                    break;
                }
                // Buttons pressed
                case WM_COMMAND: {
                    switch (LOWORD(wParam)) {
                        case IDCANCEL: {
                            this->TidyUpDialog();
                            EndDialog(hwnd, wParam);
                            return TRUE;
                        }
                        case IDOK: {
                            // OK clicked, close the window
                            this->OnOk(hwnd);
                            this->TidyUpDialog();
                            EndDialog(hwnd, wParam);
                            return TRUE;
                        }
                        case IDC_TRAIL_COLOUR: {
                            this->ChoseTrailColour(hwnd);
                            return TRUE;
                        }
                    }
                }
            }

            return FALSE;
        }

        /*
            Put the data into the fields.
        */
        void HistoryTrailDialog::InitDialog(HWND hwnd, LPARAM lParam)
        {
            this->data = reinterpret_cast<HistoryTrailData *>(
                reinterpret_cast<DialogCallArgument *>(lParam)->contextArgument
            );

            // Checkboxes
            CheckDlgButton(
                hwnd,
                IDC_CHECK_AA,
                *this->data->antiAlias ? BST_CHECKED : BST_UNCHECKED
            );

            CheckDlgButton(
                hwnd,
                IDC_CHECK_FADING,
                *this->data->fade ? BST_CHECKED : BST_UNCHECKED
            );

            CheckDlgButton(
                hwnd,
                IDC_CHECK_DEGRADING,
                *this->data->degrade ? BST_CHECKED : BST_UNCHECKED
            );

            CheckDlgButton(
                hwnd,
                IDC_CHECK_FILL,
                *this->data->filledDots ? BST_CHECKED : BST_UNCHECKED
            );

            CheckDlgButton(
                hwnd,
                IDC_TRAIL_ROTATE,
                *this->data->rotate ? BST_CHECKED : BST_UNCHECKED
            );

            // Trail length
            SendDlgItemMessage(
                hwnd,
                IDC_SPIN_LENGTH,
                UDM_SETRANGE,
                this->minTrailLength,
                this->maxTrailLength
            );
            SendDlgItemMessage(
                hwnd,
                IDC_SPIN_LENGTH,
                UDM_SETPOS,
                NULL,
                this->ValueInRange(*this->data->length, this->minTrailLength, this->maxTrailLength)
                    ? *this->data->length
                    : this->defaultTrailLength
            );

            // Dot size
            SendDlgItemMessage(
                hwnd,
                IDC_DOT_SIZE_SPIN,
                UDM_SETRANGE,
                this->minDotSize,
                this->maxDotSize
            );
            SendDlgItemMessage(
                hwnd,
                IDC_DOT_SIZE_SPIN,
                UDM_SETPOS,
                NULL,
                this->ValueInRange(*this->data->dotSize, this->minDotSize, this->maxDotSize)
                    ? *this->data->dotSize
                    : this->defaultDotSize
            );\

            // Trail type - diamond
            SendDlgItemMessage(
                hwnd,
                IDC_TRAIL_TYPE,
                CB_INSERTSTRING,
                this->trailTypeDiamond,
                reinterpret_cast<LPARAM>(L"Diamond")
            );

            // Trail type - square
            SendDlgItemMessage(
                hwnd,
                IDC_TRAIL_TYPE,
                CB_INSERTSTRING,
                this->trailTypeSquare,
                reinterpret_cast<LPARAM>(L"Square")
            );

            // Trail type - circle
            SendDlgItemMessage(
                hwnd,
                IDC_TRAIL_TYPE,
                CB_INSERTSTRING,
                this->trailTypeCircle,
                reinterpret_cast<LPARAM>(L"Circle")
            );

            // Trail type - line
            SendDlgItemMessage(
                hwnd,
                IDC_TRAIL_TYPE,
                CB_INSERTSTRING,
                this->trailTypeLine,
                reinterpret_cast<LPARAM>(L"Line")
            );

            // Set trail type position
            if (
                *this->data->type == this->trailTypeDiamond ||
                *this->data->type == this->trailTypeCircle ||
                *this->data->type == this->trailTypeSquare ||
                *this->data->type == this->trailTypeLine
            ) {
                SendDlgItemMessage(
                    hwnd,
                    IDC_TRAIL_TYPE,
                    CB_SETCURSEL,
                    *this->data->type,
                    NULL
                );
            } else {
                SendDlgItemMessage(
                    hwnd,
                    IDC_TRAIL_TYPE,
                    CB_SETCURSEL,
                    this->trailTypeDiamond,
                    NULL
                );
            }


            // Max altitude
            SendDlgItemMessage(
                hwnd,
                IDC_MAX_FILTER_SPIN,
                UDM_SETRANGE32,
                this->minAltitudeValue,
                this->maxAltitudeValue
            );

            SendDlgItemMessage(
                hwnd,
                IDC_MAX_FILTER_SPIN,
                UDM_SETPOS32,
                NULL,
                this->ValueInRange(*this->data->maxAltitude, this->minAltitudeValue, this->maxAltitudeValue)
                    ? *this->data->maxAltitude
                    : this->maxAltitudeValue
            );

            // Min altitude
            SendDlgItemMessage(
                hwnd,
                IDC_MIN_FILTER_SPIN,
                UDM_SETRANGE32,
                this->minAltitudeValue,
                this->maxAltitudeValue
            );
            SendDlgItemMessage(
                hwnd,
                IDC_MIN_FILTER_SPIN,
                UDM_SETPOS32,
                NULL,
                this->ValueInRange(*this->data->minAltitude, this->minAltitudeValue, this->maxAltitudeValue)
                    ? *this->data->minAltitude
                    : this->minAltitudeValue
            );

            // Colour
            this->colourSelectorBrush = CreateSolidBrush(*this->data->colour);
        }

        /*
            Called when the Save button is pressed. We save data to the provided
            data structure, so whoever called us can get the new data items back.
        */
        void HistoryTrailDialog::OnOk(HWND hwnd)
        {
            LogInfo("History trail dialog saved");

            // Checkboxes
            *this->data->antiAlias = IsDlgButtonChecked(hwnd, IDC_CHECK_AA) == BST_CHECKED;
            *this->data->fade = IsDlgButtonChecked(hwnd, IDC_CHECK_FADING) == BST_CHECKED;
            *this->data->degrade = IsDlgButtonChecked(hwnd, IDC_CHECK_DEGRADING) == BST_CHECKED;
            *this->data->filledDots = IsDlgButtonChecked(hwnd, IDC_CHECK_FILL) == BST_CHECKED;
            *this->data->rotate = IsDlgButtonChecked(hwnd, IDC_TRAIL_ROTATE) == BST_CHECKED;

            // Length
            int trailLength = SendDlgItemMessage(
                hwnd,
                IDC_SPIN_LENGTH,
                UDM_GETPOS,
                NULL,
                NULL
            );
            if (trailLength > this->maxTrailLength) {
                *this->data->length = this->maxTrailLength;
            } else if (trailLength < this->minTrailLength) {
                *this->data->length = this->minTrailLength;
            } else {
                *this->data->length = trailLength;
            }

            // Dot size
            *this->data->dotSize = SendDlgItemMessage(
                hwnd,
                IDC_DOT_SIZE_SPIN,
                UDM_GETPOS,
                NULL,
                NULL
            );

            // Type
            *this->data->type = SendDlgItemMessage(
                hwnd,
                IDC_TRAIL_TYPE,
                CB_GETCURSEL,
                NULL,
                NULL
            );

            // Min and max altitude
            *this->data->maxAltitude = SendDlgItemMessage(
                hwnd,
                IDC_MAX_FILTER_SPIN,
                UDM_GETPOS32,
                NULL,
                NULL
            );
            *this->data->minAltitude = SendDlgItemMessage(
                hwnd,
                IDC_MIN_FILTER_SPIN,
                UDM_GETPOS32,
                NULL,
                NULL
            );
        }

        /*
            Choose the trail colour
        */
        void HistoryTrailDialog::ChoseTrailColour(HWND hwnd)
        {
            this->colourStruct.hwndOwner = hwnd;
            this->colourStruct.rgbResult = *this->data->colour;

            if (ChooseColor(&this->colourStruct) == TRUE) {
                *this->data->colour = this->colourStruct.rgbResult;

                // Delete the old brush and create a new one
                DeleteObject(this->colourSelectorBrush);
                this->colourSelectorBrush = CreateSolidBrush(*this->data->colour);

                // Force redraw of colour picker putton
                HWND buttonHwnd = GetDlgItem(hwnd, IDC_TRAIL_COLOUR);
                RECT buttonRect = {};
                GetWindowRect(buttonHwnd, &buttonRect);
                POINT clientRectTopLeft = { buttonRect.left, buttonRect.top };
                POINT clientRectBottomRight = { buttonRect.right, buttonRect.bottom };
                ScreenToClient(hwnd, &clientRectTopLeft);
                ScreenToClient(hwnd, &clientRectBottomRight);

                buttonRect = {
                    clientRectTopLeft.x,
                    clientRectTopLeft.y,
                    clientRectBottomRight.x,
                    clientRectBottomRight.y
                };
                InvalidateRect(hwnd, &buttonRect, 0);
            }
        }

        /*
            Draw the colour button
        */
        void HistoryTrailDialog::DrawColourButton(LPDRAWITEMSTRUCT drawItem)
        {
            FillRect(
                drawItem->hDC,
                &drawItem->rcItem,
                this->colourSelectorBrush
            );
        }

        /*
            Returns true if value is in a certain range
        */
        bool HistoryTrailDialog::ValueInRange(int value, int min, int max)
        {
            return value >= min && value <= max;
        }

        /*
            Delete the brush to tidy up after the dialog
        */
        void HistoryTrailDialog::TidyUpDialog(void)
        {
            DeleteObject(this->colourSelectorBrush);
        }
    }  // namespace HistoryTrail
}  // namespace UKControllerPlugin
