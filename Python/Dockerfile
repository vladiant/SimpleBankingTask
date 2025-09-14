FROM python:3.11-slim

WORKDIR /app
COPY simple_banking.py ./

# For append-only logs
RUN mkdir /app/accounts

RUN pip install pytest
COPY test_simple_banking.py ./
RUN pytest -v test_simple_banking.py

EXPOSE 12345

CMD ["python", "simple_banking.py", "tcp", "12345"]