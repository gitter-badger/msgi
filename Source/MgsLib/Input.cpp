#include "stdafx.h"
#include "Input.hpp"
#include "MgsFunction.hpp"
#include <assert.h>

#define DIRECTINPUT_VERSION 0x700
#include <dinput.h>

MGS_VAR(1, 0x71D664, LPDIRECTINPUTA, pDirectInput, nullptr);
MGS_VAR(1, 0x71D66C, LPDIRECTINPUTDEVICE, pJoystickDevice, nullptr);
MGS_VAR(1, 0x71D668, LPDIRECTINPUTDEVICEA, pMouseDevice, nullptr);

MGS_VAR(1, 0x71D420, DIDEVICEINSTANCEA, JoystickDeviceInfos, {});
MGS_VAR(1, 0x64DA88, DIDATAFORMAT, JoystickDataFormat, {});
MGS_VAR(1, 0x64DA70, DIDATAFORMAT, MouseDataFormat, {});
MGS_VAR(1, 0x71D1D8, DIDEVCAPS, JoystickDeviceCaps, {});

DWORD* dword_6571F4 = (DWORD*)0x6571F4;
char* sidewinderEtc = (char*)0x657298; // TODO: Dump array
GUID& IID_IDirectInput7A_MGS = *((GUID*)0x64B028); // TODO: Use DxGuid
GUID& GUID_SysMouse_MGS = *((GUID*)0x64AEE8); // TODO: Use DxGuid
DWORD* dword_65726C = (DWORD*)0x65726C;
char* buttonNames = (char*)0x65510C; // TODO: Dump array
char* buttonList = (char*)0x654A98; // TODO: Dump array
MGS_VAR(1, 0x71D68C, DWORD, nJoystickDeviceObjects, 0);
MGS_VAR(1, 0x6FD1DC, DWORD, dword_6FD1DC, 0);

MGS_VAR(1, 0x71D670, DWORD, dword_71D670, 0);
MGS_VAR(1, 0x71D790, DWORD, dword_71D790, 0);
MGS_VAR(1, 0x71D798, DWORD, dword_71D798, 0);
MGS_VAR(1, 0x71D41C, DWORD, dword_71D41C, 0);

MGS_ARY(1, 0x65714C, DWORD, 14, dword_65714C, {});
MGS_ARY(1, 0x657184, DWORD, 14, dword_657184, {});

DWORD* dword_6571BC = (DWORD*)0x6571BC;

extern DWORD& dword_717348; // WinMain.cpp
extern HINSTANCE& gHInstance;
extern DWORD& gWindowedMode;
extern HWND& gHwnd;
extern DWORD& gActive_dword_688CDC;

MSG_FUNC_NOT_IMPL(0x00553090, signed int __stdcall(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter), DirectInputCreateExMGS);

LPDIENUMDEVICEOBJECTSCALLBACKA EnumDeviceObjectsCallback = (LPDIENUMDEVICEOBJECTSCALLBACKA)0x0043B0C8;
LPDIENUMDEVICEOBJECTSCALLBACKA CountDeviceObjectsCallback = (LPDIENUMDEVICEOBJECTSCALLBACKA)0x0043B0B3;

// 0x0043B078
BOOL __stdcall Input_EnumDevicesCallback(LPCDIDEVICEINSTANCEA lpddi, PVOID pvRef)
{
    // Stop when worked
    HRESULT hr = pDirectInput->CreateDevice(lpddi->guidInstance, &pJoystickDevice, NULL);
    return !SUCCEEDED(hr);
}


// 0x0043B1D1
//MSG_FUNC_NOT_IMPL(0x0043B1D1, int __cdecl(HWND), InitDirectInput);
int __cdecl InitDirectInput(HWND hWnd)
{
    char productName[0x80];
    char instanceName[0x80];
    dword_71D670 = 0;
    //fputs("InitDirectInput {\n", gLogFile);
    // I'll do log prints later
    HRESULT hr = DirectInputCreateExMGS(gHInstance, DIRECTINPUT_VERSION, IID_IDirectInput7A_MGS, (LPVOID*)&pDirectInput, 0);
    if (hr < 0)
        return hr;

    hr = pDirectInput->EnumDevices(DIDEVTYPE_JOYSTICK, Input_EnumDevicesCallback, 0, DIEDFL_ATTACHEDONLY);
    if (hr >= 0)
    {
        if (pJoystickDevice != 0)
        {
            memset(&JoystickDeviceInfos, 0, sizeof(DIDEVICEINSTANCEA));
            assert(sizeof(DIDEVICEINSTANCEA) == 0x244);
            JoystickDeviceInfos.dwSize = sizeof(DIDEVICEINSTANCEA);
            HRESULT hGetInfosRes = pJoystickDevice->GetDeviceInfo(&JoystickDeviceInfos);
            hr = pJoystickDevice->SetDataFormat(&JoystickDataFormat);
            if (hr >= 0)
            {
                hr = pJoystickDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
                if (hr >= 0)
                {
                    memset(&JoystickDeviceCaps, 0, 0x2Cu);
                    JoystickDeviceCaps.dwSize = 0x2C;
                    hr = pJoystickDevice->GetCapabilities(&JoystickDeviceCaps);
                    if (hr >= 0)
                    {
                        pJoystickDevice->EnumObjects(EnumDeviceObjectsCallback, hWnd, DIDFT_AXIS);
                        pJoystickDevice->EnumObjects(CountDeviceObjectsCallback, hWnd, DIDFT_BUTTON);
                        hr = pJoystickDevice->Acquire();
                        if (hr >= 0)
                        {
                            if (hGetInfosRes >= 0)
                            {
                                strcpy((char*)0x71D690, JoystickDeviceInfos.tszInstanceName);

                                for (int i = 0; i < 6; i++)
                                {
                                    int var14 = 1;
                                    strcpy(productName, JoystickDeviceInfos.tszProductName);
                                    _strlwr(productName);
                                    strcpy(instanceName, JoystickDeviceInfos.tszInstanceName);
                                    _strlwr(instanceName);

                                    for (int j = 0; j < 5; j++)
                                    {
                                        size_t offset = i * 0x140 + j * 0x40;
                                        if (strstr(productName, &sidewinderEtc[offset]) == 0 && strstr(instanceName, &sidewinderEtc[offset]) == 0)
                                        {
                                            var14 = 0;
                                        }
                                    }

                                    if (var14 != 0)
                                    {
                                        if (i == 5)
                                            i = 4;

                                        dword_71D790 = 1;
                                        dword_71D41C = dword_65726C[i * 2];
                                        dword_71D798 = i + 1;

                                        for (int nButton = 0; nButton < 0x38; nButton++)
                                        {
                                            size_t offset = i * 0x672 + nButton * 0x19;
                                            strcpy(&buttonList[nButton * 0x19], &buttonNames[offset]);
                                        }
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                dword_71D790 = 0;
                                for (int i = 0; i < 14; i++)
                                {
                                    dword_65714C[i] = 0;
                                    dword_657184[i] = 0;
                                }
                            }
                            if (dword_71D798 == 5)
                            {
                                for (int i = 0; i < 14; i++)
                                {
                                    dword_6571BC[i] = 0xFF;
                                    dword_6571F4[i] = 0xFF;
                                }
                                dword_6571BC[0] = dword_6571F4[0] = 0;
                                dword_6571BC[1] = dword_6571F4[1] = 1;
                                dword_6571BC[2] = dword_6571F4[2] = 2;
                                dword_6571BC[3] = dword_6571F4[3] = 3;
                                dword_6571BC[4] = dword_6571F4[4] = 6;
                                dword_6571BC[5] = dword_6571F4[5] = 6;
                                dword_6571BC[6] = dword_6571F4[6] = 7;
                                dword_6571BC[7] = dword_6571F4[7] = 7;
                                dword_6571BC[8] = dword_6571F4[8] = 4;
                                dword_6571BC[9] = dword_6571F4[9] = 0x21;
                                dword_6571BC[10] = dword_6571F4[10] = 0x20;
                                dword_6571BC[11] = dword_6571F4[11] = 0x23;
                                dword_6571BC[12] = dword_6571F4[12] = 0x22;
                                dword_6571BC[13] = dword_6571F4[13] = 5;
                                for (int i = 0; i < 14; i++)
                                {
                                    dword_65714C[i] = dword_657184[i];
                                }
                            }
                            else if (dword_71D798 != 1 && dword_71D798 != 4)
                            {
                                for (int i = 0; i < 14; i++)
                                {
                                    dword_6571BC[i] = 0xFF;
                                    dword_6571F4[i] = 0xFF;
                                }
                                int var124 = 0;
                                for (int i = 0; i < 14; i++)
                                {
                                    if (dword_71D790 != 0 && var124 == dword_71D41C)
                                    {
                                        var124++;
                                    }
                                    if (var124 == nJoystickDeviceObjects)
                                        break;

                                    if (i == 9)
                                        i = 13;

                                    dword_6571BC[i] = var124;
                                    dword_6571F4[i] = var124;

                                    var124++;
                                }
                                dword_6571BC[9] = dword_6571F4[9] = 0x21;
                                dword_6571BC[10] = dword_6571F4[10] = 0x20;
                                dword_6571BC[11] = dword_6571F4[11] = 0x23;
                                dword_6571BC[12] = dword_6571F4[12] = 0x22;
                                for (int i = 0; i < 14; i++)
                                {
                                    dword_65714C[i] = 0;
                                    dword_657184[i] = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (hr < 0 || pJoystickDevice == 0)
    {
        for (unsigned int i = 0; i < dword_6FD1DC; i++)
        {
            dword_6571F4[i] = 0xFF;
        }
    }

    // 0x43BBEC
    hr = pDirectInput->CreateDevice(GUID_SysMouse, &pMouseDevice, 0);
    if (hr < 0)
        return hr;

    hr = pMouseDevice->SetDataFormat(&MouseDataFormat);
    if (hr < 0)
        return hr;

    if (gWindowedMode != 0)
    {
        hr = pMouseDevice->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
    }
    else
    {
        hr = pMouseDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
    }
    if (hr < 0)
        return hr;

    hr = pMouseDevice->Acquire();

    return 0;
}

// 0x0042D69E
int __cdecl DoDirectInputInit()
{
    int result; // eax@1

    dword_717348 = 0;
    result = InitDirectInput(gHwnd);
    if (result < 0)
        result = printf("$jim failed to init direct input");
    return result;
}

void __cdecl Input_AcquireOrUnAcquire()
{
    if (pMouseDevice)
    {
        if (gActive_dword_688CDC)
        {
            pMouseDevice->Acquire();
        }
        else
        {
            pMouseDevice->Unacquire();
        }
    }
    if (pJoystickDevice)
    {
        if (gActive_dword_688CDC)
        {
            if (pJoystickDevice->Acquire())
            {
                printf("$jim - cannot acquire joystick\n");
            }
        }
        else
        {
            pJoystickDevice->Unacquire();
        }
    }
}
MSG_FUNC_IMPL(0x0043BCF0, Input_AcquireOrUnAcquire);
