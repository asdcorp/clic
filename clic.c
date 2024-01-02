#include <windows.h>
#include <objbase.h>
#include <ocidl.h>
#include <stdio.h>
#include "clic.h"

#define BoolToWStr(bVal) ((bVal) ? L"TRUE" : L"FALSE")

BOOL InitializeDigitalLicenseCheck(IEditionUpgradeManager **m_IEditionUpgradeManager) {
    GUID guidEditionUpgradeManager = {
        0x17CCA47D, 0xDAE5, 0x4E4A,
        {0xAC, 0x42, 0xCC, 0x54, 0xE2, 0x8F, 0x33, 0x4A}
    };

    GUID guidIEditionUpgradeManager = {
        0xF2DCB80D, 0x0670, 0x44BC,
        {0x90, 0x02, 0xCD, 0x18, 0x68, 0x87, 0x30, 0xAF}
    };

    if(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED))
        return FALSE;

    if(CoCreateInstance(
        &guidEditionUpgradeManager,
        0,
        CLSCTX_INPROC_SERVER,
        &guidIEditionUpgradeManager,
        (PVOID*)m_IEditionUpgradeManager
    )) {
        return FALSE;
    }

    return TRUE;
}

BOOL PrintStateData() {
    PWSTR pwszStateData = 0;
    UINT cbSize = 0;

    if(SLGetWindowsInformation(
        L"Security-SPP-Action-StateData",
        NULL,
        &cbSize,
        (PBYTE*)&pwszStateData
    )) {
        return FALSE;
    }

    for(INT i = 0; i < (cbSize / 2); i++) {
        if(pwszStateData[i] == L';')
            pwszStateData[i] = L'\n';
    }

    wprintf(L"%ws\n", pwszStateData);

    LocalFree(pwszStateData);
    return TRUE;
}

BOOL PrintLastActivationHRresult() {
    PDWORD pdwLastHResult = 0;
    UINT cbSize = 0;

    if(SLGetWindowsInformation(
        L"Security-SPP-LastWindowsActivationHResult",
        NULL,
        &cbSize,
        (PBYTE*)&pdwLastHResult
    )) {
        return FALSE;
    }

    wprintf(L"LastActivationHResult=0x%08x\n", *pdwLastHResult);

    LocalFree(pdwLastHResult);
    return TRUE;
}

BOOL PrintDigitalLicenseStatus() {
    IEditionUpgradeManager *m_IEditionUpgradeManager;
    DWORD dwReturnCode = 0;
    BOOL bDigitalLicense = FALSE;

    if(!InitializeDigitalLicenseCheck(&m_IEditionUpgradeManager))
        return FALSE;

    if(m_IEditionUpgradeManager->lpVtbl->AcquireModernLicenseForWindows(
        m_IEditionUpgradeManager,
        1,
        &dwReturnCode
    )) {
        return FALSE;
    }

    bDigitalLicense = (dwReturnCode != 1 && dwReturnCode <= INT_MAX);
    wprintf(L"DigitalLicense=%ws\n", BoolToWStr(bDigitalLicense));

    return TRUE;
}

BOOL PrintSubscriptionStatus() {
    SUBSCRIPTIONSTATUS *pStatus;
    DWORD dwSupported = 0;

    if(SLGetWindowsInformationDWORD(L"ConsumeAddonPolicySet", &dwSupported))
        return FALSE;

    wprintf(L"SubscriptionSupportedEdition=%ws\n", BoolToWStr(dwSupported));

    if(ClipGetSubscriptionStatus(&pStatus))
        return FALSE;

    wprintf(L"SubscriptionEnabled=%ws\n", BoolToWStr(pStatus->dwEnabled));

    if(pStatus->dwEnabled == 0) {
        LocalFree(pStatus);
        return TRUE;
    }

    wprintf(L"SubscriptionSku=%d\n", pStatus->dwSku);
    wprintf(L"SubscriptionState=%d\n", pStatus->dwState);

    LocalFree(pStatus);
    return TRUE;
}

BOOL PrintIsWindowsGenuine() {
    DWORD dwGenuine = 0;
    PCWSTR ppwszGenuineStates[] = {
        L"SL_GEN_STATE_IS_GENUINE",
        L"SL_GEN_STATE_INVALID_LICENSE",
        L"SL_GEN_STATE_TAMPERED",
        L"SL_GEN_STATE_OFFLINE",
        L"SL_GEN_STATE_LAST"
    };

    if(SLIsWindowsGenuineLocal(&dwGenuine))
        return FALSE;

    if(dwGenuine < 5) {
        wprintf(L"IsWindowsGenuine=%ws\n", ppwszGenuineStates[dwGenuine]);
    } else {
        wprintf(L"IsWindowsGenuine=%d\n", dwGenuine);
    }

    return TRUE;
}

int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PWSTR pCmdLine,
    int nCmdShow
) {
    BOOL bError = FALSE;

    if(!PrintStateData())
        bError = TRUE;

    if(!PrintLastActivationHRresult())
        bError = TRUE;

    if(!PrintDigitalLicenseStatus())
        bError = TRUE;

    if(!PrintSubscriptionStatus())
        bError = TRUE;

    if(!PrintIsWindowsGenuine())
        bError = TRUE;

    exit(bError);
}
