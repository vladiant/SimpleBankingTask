import os
import shutil
import pytest

import simple_banking

DATA_DIR = "accounts_test"


@pytest.fixture(autouse=True)
def setup_test_env(monkeypatch):
    # Use a separate test directory for accounts
    if os.path.exists(DATA_DIR):
        shutil.rmtree(DATA_DIR)
    os.makedirs(DATA_DIR)
    monkeypatch.setattr(simple_banking, "DATA_DIR", DATA_DIR)
    yield
    shutil.rmtree(DATA_DIR)


def run_cmds(cmds):
    session = simple_banking.Session()
    results = []
    for cmd in cmds:
        results.append(simple_banking.parse_cmd(session, cmd))
    return results


def test_create_and_login():
    cmds = [
        "login alice 1111",
        "get balance",
        "logout",
        "login alice 1111",
        "get balance",
    ]
    results = run_cmds(cmds)
    assert "Welcome, alice" in results[0]
    assert results[1] == "0"
    assert results[2] == "Logged out"
    assert results[3] == "Welcome, alice"
    assert results[4] == "0"


def test_invalid_login():
    run_cmds(["login bob 2222"])
    session = simple_banking.Session()
    assert session.login("bob", "wrong") == "Invalid PIN"


def test_deposit_withdraw():
    cmds = [
        "login carol 3333",
        "deposit 200",
        "get balance",
        "withdraw 50",
        "get balance",
    ]
    results = run_cmds(cmds)
    assert "Welcome, carol" in results[0]
    assert results[1] == "ok!"
    assert results[2] == "200"
    assert results[3] == "ok!"
    assert results[4] == "150"


def test_withdraw_insufficient_funds():
    cmds = ["login dave 4444", "deposit 20", "withdraw 50"]
    results = run_cmds(cmds)
    assert "Welcome, dave" in results[0]
    assert results[1] == "ok!"
    assert results[2] == "Insufficient funds"


def test_transfer_between_users():
    cmds = [
        "login eve 5555",
        "deposit 80",
        "logout",
        "login frank 6666",
        "deposit 10",
        "transfer 20 to eve",
        "get balance",
        "logout",
        "login eve 5555",
        "get balance",
    ]
    results = run_cmds(cmds)
    assert "Welcome, eve" in results[0]
    assert results[1] == "ok!"
    assert results[2] == "Logged out"
    assert "Welcome, frank" in results[3]
    assert results[4] == "ok!"
    assert (
        results[5] == "Insufficient funds or user does not exist"
    )  # frank only has 10
    assert results[6] == "10"
    # Now test a valid transfer
    run_cmds(["login frank 6666", "deposit 30"])
    results = run_cmds(["login frank 6666", "transfer 20 to eve", "get balance"])
    assert results[1] == "ok!"
    assert results[2] == "20"
    results = run_cmds(["login eve 5555", "get balance"])
    assert results[1] == "100"


def test_transfer_to_nonexistent_user():
    cmds = ["login grace 7777", "deposit 50", "transfer 10 to unknown"]
    results = run_cmds(cmds)
    assert results[2] == "Insufficient funds or user does not exist"


def test_history():
    cmds = ["login hank 8888", "deposit 300", "withdraw 100", "get balance", "history"]
    results = run_cmds(cmds)
    assert results[1] == "ok!"
    assert results[2] == "ok!"
    assert results[3] == "200"
    history_lines = results[4].splitlines()
    assert "Account created" in history_lines[0]
    assert any("Deposited 300" in s for s in history_lines)
    assert any("Withdrew 100" in s for s in history_lines)


def test_logout_and_command_restriction():
    cmds = ["login iris 9999", "deposit 100", "logout", "get balance", "deposit 100"]
    results = run_cmds(cmds)
    assert "Welcome, iris" in results[0]
    assert results[1] == "ok!"
    assert results[2] == "Logged out"
    assert results[3] == "Not logged in"
    assert results[4] == "Not logged in"


def test_invalid_amount():
    cmds = ["login joe 1234", "deposit abc", "withdraw xyz"]
    results = run_cmds(cmds)
    assert results[1] == "Invalid amount"
    assert results[2] == "Invalid amount"


def test_unknown_command():
    cmds = ["login kate 4321", "foobar"]
    results = run_cmds(cmds)
    assert results[1] == "Unknown command"
