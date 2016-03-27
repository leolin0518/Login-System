# **Login System by Redhart** #

This is actually my first Qt 5 project driven by SQLite3.
I was basically cloning a PHP/MySQL authentication system I wrote earlier when I arrived at this idea.


This app currently has the following features:

- Registration/Login

- Profile System

- Avatar System

- Edit Profile/Password

- Delete Own Account

- Admin Panel

- Basic GUI


## **Demo** ##

Before build, you can modify the database path in 'loginsystem.cpp' or just make sure all project files are under folder 'LogSys' in Qt's default project directory.
Login as "user", "pass" for user experience or "admin", "pass" for admin rights.
The accounts are subject to modification in future so if these credentials don't match any rows in the database, browse the 'sys_users' table in the .s3db file.


## **Screenshots** ##

![19-38-50.png](https://bitbucket.org/repo/azAkE8/images/3466898737-19-38-50.png)

![19-45-43.png](https://bitbucket.org/repo/azAkE8/images/941292736-19-45-43.png)

![19-46-30.png](https://bitbucket.org/repo/azAkE8/images/1191543840-19-46-30.png)


While this app is driven by an SQLite database, it is targeted at a unified database server.
Yes, one can always serve the .s3db file over the network but why bother when MySQL basically does this?

Choice is always yours. ![E057.png](https://bitbucket.org/repo/azAkE8/images/3065839784-E057.png)