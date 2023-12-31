#ifndef CLIC_H
#define CLIC_H

typedef enum _tagSLDATATYPE {
    SL_DATA_NONE = REG_NONE,
    SL_DATA_SZ = REG_SZ,
    SL_DATA_DWORD = REG_DWORD,
    SL_DATA_BINARY = REG_BINARY,
    SL_DATA_MULTI_SZ,
    SL_DATA_SUM = 100
} SLDATATYPE;

typedef struct _tagIEditionUpgradeManagerVtbl {
    HRESULT (WINAPI *QueryInterface)(
        VOID *pThis,
        const IID *riid,
        void **ppvObject
    );

    ULONG (WINAPI *AddRef)(
        VOID *pThis
    );

    ULONG (WINAPI *Release)(
        VOID *pThis
    );

    HRESULT (WINAPI *InitializeWindow)(
        VOID *pThis,
        OLE_HANDLE parentWindow
    );

    HRESULT (WINAPI *UpdateOperatingSystem)(
        VOID *pThis,
        LPWSTR lpwszContentId,
        DWORD dwAsync
    );

    HRESULT (WINAPI *ShowProductKeyUI)(
        VOID *pThis,
        DWORD dwAsync
    );

    HRESULT (WINAPI *UpdateOperatingSystemWithParams)(
        VOID *pThis,
        LPWSTR lpwszPKey,
        BOOL bReboot,
        BOOL bShowUI,
        BOOL bShowPrompt,
        BOOL bCommandLine,
        DWORD dwAsync
    );

    HRESULT (WINAPI *AcquireModernLicenseForWindows)(
        VOID *pThis,
        DWORD dwAsync,
        DWORD *pdwReturnCode
    );

    HRESULT (WINAPI *AcquireModernLicenseWithPreviousId)(
        VOID *pThis,
        LPWSTR lpwszPreviousId,
        DWORD *pdwReturnCode
    );
} IEditionUpgradeManagerVtbl;

typedef struct _tagIEditionUpgradeManager {
    IEditionUpgradeManagerVtbl *lpVtbl;
} IEditionUpgradeManager;

typedef struct _tagSUBSCRIPTIONSTATUS {
    DWORD dwEnabled;
    DWORD dwSku;
    DWORD dwState;
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

#endif
