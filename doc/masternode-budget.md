Masternode Budget API
=======================

Loonie now supports full decentralized budgets that are paid directly from the blockchain via superblocks once per 30 days.

A GUI (graphical user interface) for the QT wallets are planned for those finding below a bit complex to comprehend.

Budgets go through a series of stages before being paid:
* prepare - create a special transaction that destroys 50 LNI coins in order to make a proposal
* submit - propagate transaction to peers on network
* voting - lobby for votes on your proposal
* get enough votes - make it into the budget
* finalization - at the end of each payment period, proposals are sorted then compiled into a finalized budget
* finalized budget voting - masternodes that agree with the finalization will vote on that budget
* payment - the winning finalized budget is paid


Prepare collateral transaction
------------------------

preparebudget \<proposal-name\> \<url\> \<payment_count\> \<block_start\> \<loonie_address\> \<monthly_payment_loonie\> [use_ix(true|false)]

Example:
```
preparebudget testing https://discord.gg/4nFZeJr 1 43200 LSZSmLaHy4D6M7PU9zhb7iBunGRRkauyqU 50
```

Output: `fda9586854d1665e71f3783508b984299d096f2fe91c99654fe2a816675d5310` - This is the collateral hash, copy this output for the next step

In this transaction we prepare collateral for "testing". This proposal will pay _50_ Loonie, _1_ time totaling _50_ Loonie.

**Warning -- if you change any fields within this command, the collateral transaction will become invalid.**

Submit proposal to network
------------------------

submitbudget \<proposal-name\> \<url\> \<payment_count\> \<block_start\> \<loonie_address\> \<monthly_payment_loonie\> \<collateral_hash\>

Example:
```
submitbudget testing https://discord.gg/4nFZeJr 1 43200 LSZSmLaHy4D6M7PU9zhb7iBunGRRkauyqU 50 fda9586854d1665e71f3783508b984299d096f2fe91c99654fe2a816675d5310
```

Output: `7321a7bc082f933f418563af872a21b9b5707ca386de33866dd36e486faaa49d` - This is your proposal hash, which other nodes will use to vote on it

Lobby for votes
------------------------

Double check your information:

mnbudget getinfo \<proposal-name\>

Example:
```
mnbudget getinfo testing
```
Output:
```
    "Name": "testing",
    "URL": "https://discord.gg/4nFZeJr",
    "Hash": "7321a7bc082f933f418563af872a21b9b5707ca386de33866dd36e486faaa49d",
    "FeeHash": "fda9586854d1665e71f3783508b984299d096f2fe91c99654fe2a816675d5310",
    "BlockStart": 43200,
    "BlockEnd": 86401,
    "TotalPaymentCount": 1,
    "RemainingPaymentCount": 1,
    "PaymentAddress": "LSZSmLaHy4D6M7PU9zhb7iBunGRRkauyqU",
    "Ratio": 1,
    "Yeas": 32,
    "Nays": 0,
    "Abstains": 0,
    "TotalPayment": 50.00000000,
    "MonthlyPayment": 50.00000000,
    "IsEstablished": true,
    "IsValid": true,
    "IsValidReason": "",
    "fValid": true
```

If everything looks correct, you can ask for votes from other masternodes. To vote on a proposal, load a wallet with _masternode.conf_ file. You should not access your cold wallet to vote for proposals.

mnbudgetvote "many" \<proposal_hash\> [yes|no]

Example:
```
mnbudgetvote "many" 7321a7bc082f933f418563af872a21b9b5707ca386de33866dd36e486faaa49d yes
```

Output: `"overall": "Voted successfully 10 time(s) and failed 0 time(s)."` - Your vote has been submitted and accepted.

Make it into the budget
------------------------

After you get enough votes, execute `mnbudget projection` to see if you made it into the budget. If you the budget was finalized at this moment which proposals would be in it. Note: Proposals must be active at least 1 day on the network and receive 10% of the masternode network in yes votes in order to qualify (E.g. if there is 500 masternodes, you will need 50 yes votes.)

Example:
```
mnbudget projection
```

Output:
```
 "Name": "testing",
    "URL": "https://discord.gg/4nFZeJr",
    "Hash": "7321a7bc082f933f418563af872a21b9b5707ca386de33866dd36e486faaa49d",
    "FeeHash": "fda9586854d1665e71f3783508b984299d096f2fe91c99654fe2a816675d5310",
    "BlockStart": 43200,
    "BlockEnd": 86401,
    "TotalPaymentCount": 1,
    "RemainingPaymentCount": 1,
    "PaymentAddress": "LSZSmLaHy4D6M7PU9zhb7iBunGRRkauyqU",
    "Ratio": 1,
    "Yeas": 32,
    "Nays": 0,
    "Abstains": 0,
    "TotalPayment": 50.00000000,
    "MonthlyPayment": 50.00000000,
    "IsEstablished": true,
    "IsValid": true,
    "IsValidReason": "",
    "fValid": true,
    "Alloted": 50.00000000,
    "TotalBudgetAlloted": 50.00000000
```

Finalized budget
------------------------

```
"main" : {
        "FeeTX" : "fda9586854d1665e71f3783508b984299d096f2fe91c99654fe2a816675d5310",
        "Hash" : "7321a7bc082f933f418563af872a21b9b5707ca386de33866dd36e486faaa49d",
        "BlockStart" : 43200,
        "BlockEnd" : 86401,
        "Proposals" : "testing",
        "VoteCount" : 32,
        "Status" : "OK"
    },
```

Get paid
------------------------

When block `43200` is reached you'll receive a payment for `50` Loonie.


RPC Commands
------------------------

The following RPC commands are supported:
- mnbudget "command"... ( "passphrase" )
 * preparebudget      - Prepare proposal for network by signing and creating tx
 * submitbudget       - Submit proposal for network
 * vote-many          - Vote on a Loonie initiative
 * vote-alias         - Vote on a Loonie initiative
 * vote               - Vote on a Loonie initiative/budget
 * getvotes           - Show current masternode budgets
 * getinfo            - Show current masternode budgets
 * show               - Show all budgets
 * projection         - Show the projection of which proposals will be paid the next cycle
 * check              - Scan proposals and remove invalid

- mnfinalbudget "command"... ( "passphrase" )
 * vote-many   - Vote on a finalized budget
 * vote        - Vote on a finalized budget
 * show        - Show existing finalized budgets
