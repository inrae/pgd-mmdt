# syntax=docker/dockerfile:1
FROM php:7.4-apache
LABEL version-date="2023-11-14"

# Set TIMEZONE - Important for PHP
ENV TZ=Europe/Paris
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Installation of SSL libraries as well as the PHP MongoDB module
RUN apt-get update && apt-get install -y openssl libssl-dev libcurl4-openssl-dev && rm -rf /var/lib/apt/lists/* \
 && pecl install mongodb && echo "extension=mongodb.so" > /usr/local/etc/php/conf.d/mongo.ini

# Install Composer - Not necessary in production version
#RUN curl -sS https://getcomposer.org/installer | php -- --install-dir=/usr/local/bin --filename=composer

# Do some cleaning
RUN mkdir /etc/pgd-mmdt && rm -rf /tmp/pear

# Apache configuration : Set header in respect of CORS
RUN a2enmod headers \
 && sed -ri -e 's/^([ \t]*)(<\/VirtualHost>)/\1\tHeader set Access-Control-Allow-Origin "*"\n\1\2/g' /etc/apache2/sites-available/*.conf
