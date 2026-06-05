Bank & User Management System (CLI)

A robust, console-based banking application built in modern C++. The system features a dual-layer architecture separating Client Banking Operations (deposits, withdrawals, balances) and a User Administration Subsystem controlled via an explicit, low-level bitwise masking permission architecture.

🚀 Features. 

Client Management SystemView Client Ledger: Displays a formatted structural table listing client accounts, pin codes, names, contact numbers, and real-time balances using explicit column-width alignments (std::setw).
Secure Registration: Registers new clients with real-time collision detection preventing duplicate account numbers.
Cascaded Deletion: Implements a two-pass mark-and-sweep deletion protocol, preventing structural fragmentation of records in flat-file configurations.
Dynamic Modifiers: Updates existing structural fields on confirmed targets seamlessly.
Transaction EngineDeposit Pipeline: Modifies targeted currency balances with automated system confirmation.
Validated Withdrawals: Safely checks requests against current balances to prevent overdraft and invalid balances.
Aggregate Reporting: Dynamically computes total financial value distributed across all active ledger records.
Secure Login Gateway: Authenticates operations against an external file repository.
Bitwise Permission Engine: Permissions are evaluated using lightning-fast bitwise masking operations, evaluating authorization constraints in a single CPU clock cycle.

🛠️ System Architecture & Bitmask Scheme

The access control model relies on specific bit offsets mapping directly to functions in the main menu. When checking if a user has clearance, the system uses a bitwise AND operations (&) to isolate relevant bits.

📂 Database Specifications

The system reads and writes to two flat-file databases using a high-density string delimiter (#//#). clients.txt Records are serialized in the following layout:AccountNo#//#PinCode#//#Client Name#//#PhoneNumber#//#CurrentBalance
users.txt System operator access controls are tracked as follows:Username#//#Password#//#Permission.
