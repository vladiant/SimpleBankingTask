import os
import sys
import json
import threading
from socketserver import ThreadingTCPServer, StreamRequestHandler

DATA_DIR = "accounts"
if not os.path.exists(DATA_DIR):
    os.makedirs(DATA_DIR)

LOCKS = {}


def get_lock(user):
    if user not in LOCKS:
        LOCKS[user] = threading.Lock()
    return LOCKS[user]


def account_file(user):
    return os.path.join(DATA_DIR, f"{user}.log")


def append_event(user, event):
    with get_lock(user):
        with open(account_file(user), "a", encoding="utf-8") as f:
            f.write(json.dumps(event) + "\n")


def read_events(user):
    events = []
    try:
        with open(account_file(user), "r", encoding="utf-8") as f:
            for line in f:
                events.append(json.loads(line.strip()))
    except FileNotFoundError:
        pass
    return events


def get_balance(user):
    bal = 0
    for event in read_events(user):
        if event["type"] == "deposit":
            bal += event["amount"]
        elif event["type"] == "withdraw":
            bal -= event["amount"]
        elif event["type"] == "transfer_out":
            bal -= event["amount"]
        elif event["type"] == "transfer_in":
            bal += event["amount"]
    return bal


def user_exists(user):
    return os.path.exists(account_file(user))


def valid_login(user, pin):
    events = read_events(user)
    for event in events:
        if event["type"] == "create":
            return event["pin"] == pin
    return False


def create_user(user, pin):
    if user_exists(user):
        return False
    append_event(user, {"type": "create", "pin": pin})
    return True


def deposit(user, amount):
    append_event(user, {"type": "deposit", "amount": amount})


def withdraw(user, amount):
    bal = get_balance(user)
    if bal < amount:
        return False
    append_event(user, {"type": "withdraw", "amount": amount})
    return True


def transfer(from_user, to_user, amount):
    bal = get_balance(from_user)
    if bal < amount or not user_exists(to_user):
        return False
    append_event(from_user, {"type": "transfer_out", "amount": amount, "to": to_user})
    append_event(to_user, {"type": "transfer_in", "amount": amount, "from": from_user})
    return True


def get_history(user):
    events = read_events(user)
    lines = []
    for event in events:
        if event["type"] == "deposit":
            lines.append(f"Deposited {event['amount']}")
        elif event["type"] == "withdraw":
            lines.append(f"Withdrew {event['amount']}")
        elif event["type"] == "transfer_out":
            lines.append(f"Transferred {event['amount']} to {event['to']}")
        elif event["type"] == "transfer_in":
            lines.append(f"Received {event['amount']} from {event['from']}")
        elif event["type"] == "create":
            lines.append("Account created")
    return "\n".join(lines)


class Session:
    def __init__(self):
        self.user = None

    def login(self, user, pin):
        if not user_exists(user):
            if create_user(user, pin):
                self.user = user
                return f"Welcome, {user} (new account created)"
            else:
                return "Error creating account"
        if valid_login(user, pin):
            self.user = user
            return f"Welcome, {user}"
        else:
            return "Invalid PIN"

    def logout(self):
        self.user = None
        return "Logged out"

    def get_balance(self):
        if not self.user:
            return "Not logged in"
        return str(get_balance(self.user))

    def deposit(self, amount):
        if not self.user:
            return "Not logged in"
        deposit(self.user, amount)
        return "ok!"

    def withdraw(self, amount):
        if not self.user:
            return "Not logged in"
        if withdraw(self.user, amount):
            return "ok!"
        return "Insufficient funds"

    def transfer(self, amount, to_user):
        if not self.user:
            return "Not logged in"
        if transfer(self.user, to_user, amount):
            return "ok!"
        return "Insufficient funds or user does not exist"

    def history(self):
        if not self.user:
            return "Not logged in"
        return get_history(self.user)


def parse_cmd(session, line):
    tokens = line.strip().split()
    if not tokens:
        return ""
    cmd = tokens[0].lower()
    if cmd == "login" and len(tokens) == 3:
        user, pin = tokens[1], tokens[2]
        return session.login(user, pin)
    elif cmd == "logout":
        return session.logout()
    elif cmd == "get" and len(tokens) > 1 and tokens[1] == "balance":
        return session.get_balance()
    elif cmd == "deposit" and len(tokens) == 2:
        try:
            amount = int(tokens[1])
            return session.deposit(amount)
        except:
            return "Invalid amount"
    elif cmd == "withdraw" and len(tokens) == 2:
        try:
            amount = int(tokens[1])
            return session.withdraw(amount)
        except:
            return "Invalid amount"
    elif cmd == "transfer" and len(tokens) == 4 and tokens[2] == "to":
        try:
            amount = int(tokens[1])
            to_user = tokens[3]
            return session.transfer(amount, to_user)
        except:
            return "Invalid amount"
    elif cmd == "history":
        return session.history()
    else:
        return "Unknown command"


def console_main():
    session = Session()
    while True:
        try:
            line = input("$")
        except EOFError:
            break
        if line.strip().lower() in ("exit", "quit"):
            print("Bye!")
            break
        print(parse_cmd(session, line))


### TCP interface ###
class BankingTCPHandler(StreamRequestHandler):
    def handle(self):
        session = Session()
        self.wfile.write(b"Welcome to SimpleBanking ATM\n")
        while True:
            self.wfile.write(b"$")
            line = self.rfile.readline().decode("utf-8")
            if not line:
                break
            resp = parse_cmd(session, line)
            self.wfile.write((resp + "\n").encode("utf-8"))
            if line.strip().lower() in ("exit", "quit"):
                break


def tcp_main(port=12345):
    with ThreadingTCPServer(("0.0.0.0", port), BankingTCPHandler) as server:
        print(f"TCP server running on port {port}")
        server.serve_forever()


if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == "tcp":
        port = int(sys.argv[2]) if len(sys.argv) > 2 else 12345
        tcp_main(port)
    else:
        console_main()
