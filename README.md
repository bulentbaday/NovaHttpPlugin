# NovaHttpPlugin
Mysql Http Request Plugin

Windows
MySQL 8.* (x64)

CREATE FUNCTION HttpRequest RETURNS STRING
  SONAME 'NovaHttpPlugin.dll';

SELECT HttpRequest('http://www.novayazilim.com.tr', 'GET', 'text/html', null) data;

