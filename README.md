# NovaHttpPlugin
Mysql Http Request Plugin

* Windows
* MySQL 8.* (x64)

* Don't forget to change include directory (D:\NOVAYAZILIM\mysql-8.0.15-winx64\include)

* Make project to get dll file.

* Copy NovaHttpPlugin.dll to MySQL Plugin folder.

* Create UDF Function by following script. 

CREATE FUNCTION HttpRequest RETURNS STRING
  SONAME 'NovaHttpPlugin.dll';

* e.g.

SELECT HttpRequest('http://www.novayazilim.com.tr', 'GET', 'text/html', null) data;

SELECT convert(HttpRequest('http://www.novayazilim.com.tr', 'GET', 'text/html', null) using utf8) data;
