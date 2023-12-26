clic
====

A simple tool to read the activation and subscription status of Windows.

Usage
-----

Run clic using the command prompt.

Values documentation
--------------------

|Value name|Meaning|
|----------|-------|
| ruleId | Software Protection Platform Rule ID |
| AppId | Software Protection Platform Application ID |
| SkuId | Software Protection Platform SKU ID |
| LastNotificationId | Last Software Protection Platform Notification |
| GraceEndDate | Grace period/volume activation expiration |
| LicenseState | Current state of the license |
| LicenseExpirationDate | Time-based license expiration |
| LastConsumptionReason | Reason of last license installation |
| KernelTimebombDate | Expiration of prerelease Windows version |
| PartialProductKey | Last 5 characters of the product key |
| ProductKeyType | Channel of the product key |
| volumeActivationOrder | Order of the volume activation |
| uxDifferentiator | User Experience Differentiator |
| LastActivationHResult | Error code of a last activation attempt |
| SubscriptionSupportedEdition | Can subscriptions be used on the current edition |
| SubscriptionEnabled | Is the subscription enabled |
| SubscriptionSku | Edition ID of the subscription edition |
| SubscriptionState | State of the subscription (`1` = Active, `2` = Not active) |
| IsWindowsGenuine | Is the copy of Windows correctly licensed (`FALSE` means an `Activate Windows` watermark and a personalization block) |

License
-------
The project is licensed under the terms of the MIT License.
