# SimpleBankingTask

A simple ATM-like banking application in Python supporting both console and TCP remote interfaces. All account data is stored in an **append-only** log for persistence.

## Features

- **User Login**: `login [user] [pin]` (Creates new account if user doesn't exist)
- **Logout**: `logout`
- **Balance Inquiry**: `get balance`
- **Deposit Funds**: `deposit [amount]`
- **Withdraw Funds**: `withdraw [amount]`
- **Transfer Funds**: `transfer [amount] to [user]`
- **Account History**: `history` (optional)
- **Exit Application**: `exit` or `quit`

## Persistence Model

- User account data is stored in individual append-only log files under the `accounts` directory.
- No data modification or deletion—only appending new events.

## Interfaces

- **Console**: Run directly from terminal.
- **TCP Remote**: Run as a TCP server, connect via `telnet` or similar.

## Usage

### Console Mode

```bash
python simple_banking.py
```

### TCP Server Mode

```bash
python simple_banking.py tcp [port]
```
Default port is `12345`.

Connect using:
```bash
telnet localhost 12345
```

### Example Commands

```bash
$ login alice 1234
Welcome, alice
$ deposit 100
ok!
$ get balance
100
$ withdraw 25
ok!
$ get balance
75
$ logout
Logged out
```

## Running the Tests

- Unit tests are provided in `test_simple_banking.py` using [pytest](https://pytest.org).
- Tests cover user creation, login, deposit, withdrawal, transfer, edge cases, and history.

### Run Tests

```bash
pytest test_simple_banking.py
```

> **Note**: Tests use a separate `accounts_test` directory to avoid affecting real account data.

## Project Structure

```
simple_banking.py           # Main application
test_simple_banking.py      # Pytest-based unit tests
accounts/                   # Persistent account logs (created at runtime)
ReadMe.md                   # Documentation (this file)
```

## Requirements

- Python 3.7+
- No external dependencies (other than pytest for testing)

## Functional TCP Tests with Docker Compose

This repository now supports functional integration tests for the TCP interface using `pytest`, `docker-compose`, and two Dockerfiles.

### How It Works

- **`banking-app`** service runs the ATM app TCP server.
- **`banking-tests`** service runs the functional tests and connects to the running TCP server.

### Running Functional Tests

1. **Build and start services:**
   ```bash
   docker-compose up --build --abort-on-container-exit
   ```

2. **Test results:**
   - Output from the `banking-tests` container will show all pytest results for TCP interface.
   - The `banking-app` container persists account data in `/app/accounts` (optional).

3. **Clean up:**
   ```bash
   docker-compose down
   ```

### Test Coverage

- User login/logout via TCP
- Deposits, withdrawals, balance checks via TCP
- Edge cases: insufficient funds, invalid commands, account history, transfer errors
- Full interaction simulated as a real TCP client

### Files

- `docker-compose.yml` — Compose file to orchestrate services
- `Dockerfile` — ATM app server
- `Dockerfile.test` — Test runner
- `test_functional_tcp.py` — Functional TCP tests
- `simple_banking.py` — Main app (required for both containers)

> **Note:** If you want to run only the functional TCP tests, use `docker-compose` as above; for regular unit tests, use `pytest` locally.

