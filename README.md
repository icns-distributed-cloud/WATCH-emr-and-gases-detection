# WATCH-emr-and-gases-detection

## Introduction

>WATCH-emr-and-gases-detection project researches and develops a smartwatch for workers' safety. The smartwatch can detect electric wires in the wall by using hall_sensor. And also, it can check oxygen concentration around the smartwatch by using o2_sensor. This repository mainly provides source code for the functions above.

## Developement Enviornment

- STM32CUBEMX
- KEIL uVISION5

## 1. Development project setup
* Step 1. Share a CubeMX file
* Step 2. Create "stm-watch" folder
* Step 3. Execute the shared CubeMX file
* Step 4. Save as CubeMX project (Project name: Watch) in "stm-watch" folder
* Step 5. Click "Generate code" button
* Step 6. Delete main.h file in "Watch/Inc/" and main.c file "Watch/Src/"
* Step 7. Create/init local git repository in "stm-watch" folder
* Step 8. Set git origin "https://github.com/icns-distributed-cloud/safety-watch"
* Step 9. Git pull origin master

## 2. Project development
* Step 1. Rasie an issue and create a branch
* Step 2. Pull the branch to local repository
* Step 3. Checkout the branch
* Step 4. Develop functions, modules, or etc.
* Step 5. Git commit
* Step 6. Push the branch
* Step 7. Make "Pull request"
