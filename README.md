A lightweight, console-based Bank Management & Transaction System built using modern C++. The system features full CRUD capabilities for client profiles alongside automated file-based state persistence and accurate mathematical handling of basic financial deposits and withdrawals.

🚀 Features
👤 Client Management
Show Client List: Displays a beautifully formatted tabular layout of all registered clients using exact visual padding rules.

Add New Client: Generates a new profile validation step to ensure account numbers are uniquely registered and cannot be duplicated.

Delete Client: Features a distinct "two-step" deletion confirmation workflow. Data is logically flagged before being permanently trimmed from the raw storage file.

Update Client Info: Allows secure overrides for credentials, names, and contact details without changing underlying system identity attributes.

Find Client: Direct-search profile mapping targeting an explicit identifier.

💸 Transaction Sub-System
Deposit Module: Updates single client state values with automatic file flushing updates.

Withdrawal Engine: Built-in balance threshold validation. Prevents account values from dropping below zero.

Total Balances Mapping: Aggregates and calculates total system capital assets instantly across the entire registered user database.

🛠️ System Architecture & Data Flow
Data is dynamically converted using custom tokenization parsing engines to translate structured memory objects into flat file records and vice versa.

Data Storage Protocol
All operational persistence maps directly to a file named clients.txt. Data fields are safely isolated using a clear structural separator token (#//#).

Storage Record Layout:

AccountNo#//#PinCode#//#Client Name#//#Phone#//#Balance

Flow Infrastructure Model
The application relies on highly cohesive vector state arrays, processing inputs from standard streams directly down into system functions:

[ Console UI Inputs ]
         │
         ▼
[ Parsing Layer (mysplit / recordToLine) ]
         │
         ▼
[ In-Memory Cache (std::vector<stClient>) ]
         │
         ▼
[ File Engine Data Persistence (clients.txt) ]
📋 Technical Requirements
Compiler Standard: C++11, C++14, or C++17 compatible compiler (GCC, Clang, or MSVC).

Operating System Platform: Optimal compatibility with Windows Console environments (utilizes specific system("cls") display clearing buffers).
