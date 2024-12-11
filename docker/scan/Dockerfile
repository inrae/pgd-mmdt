# syntax=docker/dockerfile:1
FROM ubuntu:20.04
LABEL version-date="2022-07-29"

# Install modules
RUN apt-get update && \
    apt-get install -y cron python3 python3-pymongo vim && \
    rm -rf /var/lib/apt/lists/* && \
    mkdir /pgd && mkdir /json

COPY scripts/config.py \
     scripts/pgd.py \
     scripts/db-insert.py /pgd/

COPY scripts/pgd-cron /etc/cron.d/pgd-cron

RUN chmod 0664 /etc/cron.d/pgd-cron && \
    touch /var/log/cron.log && \
    crontab /etc/cron.d/pgd-cron

# Run the command on container startup
ENTRYPOINT ["cron", "-f"]
