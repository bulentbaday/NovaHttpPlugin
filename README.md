# NovaHttpPlugin
Mysql Http Request Plugin

* Windows
* MySQL 8.* (x64)

* Copy NovaHttpPlugin.dll to Mysql Plugin folder.

* Create UDF Function by following script.

CREATE FUNCTION HttpRequest RETURNS STRING
  SONAME 'NovaHttpPlugin.dll';

* e.g.

SELECT HttpRequest('http://www.novayazilim.com.tr', 'GET', 'text/html', null) data;

