# MySQL setup instructions

## On macOS

```
brew install mysql mysql-client
brew services start mysql
mysql -u root
echo "CREATE DATABASE mpop" | mysql -u root
mysql -u root mpop < 01_create_tables.sql
```


