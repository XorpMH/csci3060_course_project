# Phase 3 Failure Log

## Test Run Summary

- Total Tests: 32
- Passed: 22
- Failed: 10

## Failed Test Cases

| Test Case | Expected Result | Actual Result | Result | Failure Reason | Fix Applied | Retest Result |
|---|---|---|---|---|---|---|
| invalid/add_credit-admin | SUCCESS | FAILURE | FAIL | Add credit transaction was denied when the test expected it to succeed. Account data or transaction state may not be initialized correctly. | Pending | Pending |
| invalid/create-admin | FAILURE | SUCCESS | FAIL | The create transaction incorrectly allowed an account creation that should have been rejected because of admin/account restrictions. | Pending | Pending |
| invalid/create-exists | FAILURE | SUCCESS | FAIL | The program allowed creation of a username that should already exist. Existing-account data may not be loaded before the test. | Pending | Pending |
| invalid/sell-price | FAILURE | SUCCESS | FAIL | Invalid sell price was accepted instead of being rejected. Price validation needs to be checked. | Pending | Pending |
| valid/add_credit | SUCCESS | FAILURE | FAIL | A valid add credit transaction was denied. Required user/account data may not be available when the test runs. | Pending | Pending |
| valid/buy | SUCCESS | FAILURE | FAIL | A valid buy transaction was denied. Required buyer, seller, available game, or account credit state may not be initialized. | Pending | Pending |
| valid/delete | SUCCESS | FAILURE | FAIL | A valid delete transaction was denied. The target user may not exist in the account data used by the test. | Pending | Pending |
| valid/login | SUCCESS | FAILURE | FAIL | Login input is not currently handled correctly by the harness transaction-processing flow. | Pending | Pending |
| valid/logout | SUCCESS | FAILURE | FAIL | Logout input is not currently handled correctly by the harness transaction-processing flow. | Pending | Pending |
| valid/refund | SUCCESS | FAILURE | FAIL | A valid refund transaction was denied. Required buyer/seller state may not be initialized before the test. | Pending | Pending |

## Confirmed Passing Tests

The following tests were confirmed as passing during this test run:

- invalid/add_credit-credit
- invalid/bad_input_command
- invalid/bad_input_empty
- invalid/bad_input_format
- invalid/buy-account
- invalid/buy-bought
- invalid/buy-credit
- invalid/buy-game
- invalid/create-credit
- invalid/create-username
- invalid/delete-admin
- invalid/delete-current
- invalid/delete-exists
- invalid/login
- invalid/logout
- invalid/refund-buyer
- invalid/refund-seller
- invalid/sell-account
- invalid/sell-bought
- invalid/sell-name
- valid/create
- valid/sell

## Notes

The failure log records the results of the current automated harness run. Failed tests will be investigated and fixed individually. After each fix, the harness will be rerun and the `Fix Applied` and `Retest Result` columns will be updated.