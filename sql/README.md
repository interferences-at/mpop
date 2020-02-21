# MySQL setup instructions

## On macOS

```
brew install mysql mysql-client
brew services start mysql
mysql -u root
echo "CREATE DATABASE mpop" | mysql -u root
mysql -u root mpop < 01_create_tables.sql
```


TODO FIXME

```
install_name_tool /usr/local/Cellar/mysql/8.0.19/lib/libmysqlclient.21.dylib /usr/local/mysql/lib/libmysqlclient.21.dylib /Applications/Qt/5.11.2/clang_64/plugins/sqldrivers/libqsqlmysql.dylib
```
