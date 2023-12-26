#include <windows.h>
#include <stdio.h>

typedef enum _tagSLDATATYPE {
    SL_DATA_NONE = REG_NONE,
    SL_DATA_SZ = REG_SZ,
    SL_DATA_DWORD = REG_DWORD,
    SL_DATA_BINARY = REG_BINARY,
    SL_DATA_MULTI_SZ,
    SL_DATA_SUM = 100
} SLDATATYPE;

typedef struct _tagSUBSCRIPTIONSTATUS {
    int dwEnabled;
    int dwSku;
    int dwState;
} SUBSCRIPTIONSTATUS;

HRESULT WINAPI ClipGetSubscriptionStatus(
    SUBSCRIPTIONSTATUS **ppStatus
);

HRESULT WINAPI SLGetWindowsInformation(
    PCWSTR     pwszValueName,
    SLDATATYPE *peDataType,
    UINT       *pcbValue,
    PBYTE      *ppbValue
);

HRESULT WINAPI SLGetWindowsInformationDWORD(
    PCWSTR pwszValueName,
    DWORD  *pdwValue
);

HRESULT WINAPI SLIsWindowsGenuineLocal(
    DWORD *dwGenuine
);

BOOL PrintStateData() {
    PBYTE pBuffer = 0;
    UINT cbSize = 0;

    if(SLGetWindowsInformation(
        L"Security-SPP-Action-StateData",
        NULL,
        &cbSize,
        &pBuffer
    )) {
        return FALSE;
    }

    for(int i = 0; i < cbSize; i += 2) {
        if(pBuffer[i] == ';' && pBuffer[i+1] == 0)
            pBuffer[i] = '\n';
    }

    wprintf(L"%ws\n", pBuffer);
    LocalFree(pBuffer);

    return TRUE;
}

BOOL PrintLastActivationHRresult() {
    PBYTE pBuffer = 0;
    UINT cbSize = 0;

    if(SLGetWindowsInformation(
        L"Security-SPP-LastWindowsActivationHResult",
        NULL,
        &cbSize,
        &pBuffer
    )) {
        return FALSE;
    }

    wprintf(L"LastActivationHResult=0x%08x\n", *((DWORD*)pBuffer));
    LocalFree(pBuffer);

    return TRUE;
}

BOOL PrintSubscriptionStatus() {
    SUBSCRIPTIONSTATUS *pStatus;
    DWORD dwSupported = 0;

    if(SLGetWindowsInformationDWORD(L"ConsumeAddonPolicySet", &dwSupported)) {
        return FALSE;
    }

    wprintf(L"SubscriptionSupportedEdition=%ws\n", dwSupported ? L"TRUE" : L"FALSE");

    if(dwSupported == 0)
        return TRUE;

    if(ClipGetSubscriptionStatus(&pStatus))
        return FALSE;

    wprintf(L"SubscriptionEnabled=%ws\n", pStatus->dwEnabled ? L"TRUE" : L"FALSE");

    if(pStatus->dwEnabled == 0)
        return TRUE;

    wprintf(L"SubscriptionSku=%d\n", pStatus->dwSku);
    wprintf(L"SubscriptionState=%d\n", pStatus->dwState);

    return TRUE;
}

BOOL PrintIsWindowsGenuine() {
    DWORD dwGenuine = 0;

    if(SLIsWindowsGenuineLocal(&dwGenuine))
        return FALSE;

    wprintf(L"IsWindowsGenuine=%ws\n", !dwGenuine ? L"TRUE" : L"FALSE");
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

    if(!PrintSubscriptionStatus())
        bError = TRUE;

    if(!PrintIsWindowsGenuine())
        bError = TRUE;

    return bError;
}
