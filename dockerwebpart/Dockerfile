# syntax=docker/dockerfile:1
FROM php:7.2-apache
LABEL biogeco.version-date="2021-11-24"
ENV TZ=Europe/Paris
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
RUN apt-get update && apt-get install -y openssl libssl-dev libcurl4-openssl-dev && rm -rf /var/lib/apt/lists/* && pecl install mongodb && echo "extension=mongodb.so" > /usr/local/etc/php/conf.d/mongo.ini
# Install Composer
#RUN curl -sS https://getcomposer.org/installer | php -- --install-dir=/usr/local/bin --filename=composer
RUN mkdir /etc/pgd-mmdt && rm -rf /tmp/pear
RUN a2enmod headers && sed -ri -e 's/^([ \t]*)(<\/VirtualHost>)/\1\tHeader set Access-Control-Allow-Origin "*"\n\1\2/g' /etc/apache2/sites-available/*.conf
