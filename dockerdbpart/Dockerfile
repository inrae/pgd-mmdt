# syntax=docker/dockerfile:1
FROM mongo:6.0.14
LABEL version-date="2024-09-10"

ENV TZ=Europe/Paris
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone \
 && apt-get update && apt-get install -y python3 python3-pymongo && rm -rf /var/lib/apt/lists/* \
 && touch /var/log/pgd.log && mkdir -p /js && mkdir -p /json
