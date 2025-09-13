import socket
import pytest
import time
import socket


class BankingClient:
    def __init__(self, host, port):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.connect((host, port))
        # Read welcome message
        self._read_response()

    def send_command(self, command):
        self.socket.send(f"{command}\n".encode("utf-8"))
        return self._read_response()

    def _read_response(self):
        response = ""
        while True:
            data = self.socket.recv(1024).decode("utf-8")
            if not data:
                break
            response += data
            if response.endswith("$"):
                response = response[:-1]  # Remove prompt
                break
        return response.strip()

    def close(self):
        self.socket.close()


@pytest.fixture(scope="module")
def banking_client():
    # Give the service some time to start
    time.sleep(2)
    client = BankingClient("banking-app", 12345)
    yield client
    client.close()


def test_create_account(banking_client):
    response = banking_client.send_command("login user1 1234")
    assert "Welcome, user1 (new account created)" in response


def test_deposit(banking_client):
    response = banking_client.send_command("deposit 100")
    assert "ok!" in response


def test_get_balance(banking_client):
    response = banking_client.send_command("get balance")
    assert "100" in response


def test_withdraw(banking_client):
    response = banking_client.send_command("withdraw 30")
    assert "ok!" in response

    response = banking_client.send_command("get balance")
    assert "70" in response


def test_transfer(banking_client):
    # Create second user
    response = banking_client.send_command("login user2 5678")
    assert "Welcome, user2 (new account created)" in response

    # Log back as first user
    response = banking_client.send_command("login user1 1234")
    assert "Welcome, user1" in response

    # Transfer money
    response = banking_client.send_command("transfer 20 to user2")
    assert "ok!" in response

    # Check balance
    response = banking_client.send_command("get balance")
    assert "50" in response


def test_history(banking_client):
    response = banking_client.send_command("history")
    assert "Account created" in response
    assert "Deposited 100" in response
    assert "Withdrew 30" in response
    assert "Transferred 20 to user2" in response


def test_invalid_commands(banking_client):
    response = banking_client.send_command("deposit invalid")
    assert "Invalid amount" in response

    response = banking_client.send_command("withdraw abc")
    assert "Invalid amount" in response

    response = banking_client.send_command("transfer abc to user2")
    assert "Invalid amount" in response


def test_insufficient_funds(banking_client):
    response = banking_client.send_command("withdraw 1000")
    assert "Insufficient funds" in response
