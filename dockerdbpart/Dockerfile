# syntax=docker/dockerfile:1
FROM mongo:4.4
LABEL version-date="2022-07-29"

ENV TZ=Europe/Paris
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone \
 && apt-get update && apt-get install -y python3 python3-pymongo && rm -rf /var/lib/apt/lists/* \
 && touch /var/log/pgd.log && mkdir -p /js && mkdir -p /json
