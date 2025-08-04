# syntax=docker/dockerfile:1
FROM php:8.2-apache
LABEL version-date="2025-05-22"

# Set TIMEZONE - Important for PHP
ENV TZ=Europe/Paris
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Installation of SSL libraries as well as the PHP MongoDB module
RUN apt-get update && apt-get install -y unzip wget openssl libssl-dev libcurl4-openssl-dev && rm -rf /var/lib/apt/lists/* \
 && pecl install mongodb && echo "extension=mongodb.so" > /usr/local/etc/php/conf.d/mongo.ini

# Install Composer && PHPMAILER
RUN curl -sS https://getcomposer.org/installer | php -- --install-dir=/usr/local/bin --filename=composer \
 && php /usr/local/bin/composer require phpmailer/phpmailer \
 && php /usr/local/bin/composer require easyrdf/easyrdf \
 && php /usr/local/bin/composer require ml/json-ld \
 && mkdir -p /usr/share/php && mv /var/www/html/vendor /usr/share/php/ && rm -f /var/www/html/composer.*

# Do some cleaning
RUN mkdir /etc/pgd-mmdt && rm -rf /tmp/pear

# Apache configuration : Set header in respect of CORS
RUN a2enmod headers \
 && sed -ri -e 's/^([ \t]*)(<\/VirtualHost>)/\1\tHeader set Access-Control-Allow-Origin "*"\n\1\2/g' /etc/apache2/sites-available/*.conf
