# Chinese OS

#### Linux-like operating system simulator

This project was created by team of 5 as end-of-term assignment. Each member had to implement one key element of Linux-like simulator. My responsibility was the filesystem, file API and overall management of those modules.

#### Filesystem

It's only a simulation therefore disk blocks had to be implemented and simulated as abstraction layer -1.
On top of that whole filesystem is created: inodes and management, files, filesystem API.

#### API

API covers only simple data management. Although in terminal there is additional editor, inode and block viewer and other useful tools, API does not provide block level access.
