# Tools used in the project
The following lists the tools and frameworks, that are used in the project. 
- [Docker](https://docs.docker.com/get-started/overview/)    
   Docker is an open platform for developing, shipping, and running applications. Docker enables you to separate your applications from your infrastructure so you can deliver software quickly. With Docker, you can manage your infrastructure in the same ways you manage your applications. By taking advantage of Docker's methodologies for shipping, testing, and deploying code, you can significantly reduce the delay between writing code and running it in production.
- [Kubernetes](https://kubernetes.io/docs/concepts/overview/)
- [FastAPI](https://fastapi.tiangolo.com/tutorial/)
- [SQLAlchemy](https://docs.sqlalchemy.org/en/20/orm/quickstart.html)
- [FastAPI with SQLAlchemy](https://fastapi.tiangolo.com/tutorial/sql-databases/)
- [Alembic](https://alembic.sqlalchemy.org/en/latest/tutorial.html)
- [Swagger UI](https://swagger.io/tools/swagger-ui/)

# GitLab CI/CD

The following is a collection of short hints on how to do the most essential things in a GitLab CI/CD pipeline:

- How to delay a job until another job is done: 

  We can use the command "needs: ["job name"]"

- How to change the image used in a task: 

  With the command "image: image_name" for example:
  - pipeline-test-job:
        stage: deploy
        image: python:latest

- How do you start a task manually: 

  With the command: "when: manual"

- The Script part of the config file - what is it good for?

  It's contains instructions supposed to be executed

- If I want a task to run for every branch I put it into the stage ??

  commit

- If I want a task to run for every merge request I put it into the stage ??

 acceptance

- If I want a task to run for every commit to the main branch I put it into the stage ??

 release

# flake8 / flakeheaven

- What is the purpose of flake8?

  Flake8 is a Python tool that checks code for compliance with Python style conventions (like PEP 8) and detects potential errors

- What types of problems does it detect

  Python style related problems (e.g. syntax errors)

- Why should you use a tool like flake8 in a serious project?

  Flake8 should be used in serious projects to avoid committing/merging code with (e.g. syntax) errors to the Production code

## Run flake8 on your local Computer

  It is very annoying (and takes a lot of time) to wait for the pipeline to check the syntax 
  of your code. To speed it up, you may run it locally like this:

### Configure PyCharm (only once)
- find out the name of your docker container containing flake8. Open the tab *services* in PyCharm and look at the container in the service called *web*. The the name should contain the string *1337_pizza_web_dev*.  
- select _Settings->Tools->External Tools_ 
- select the +-sign (new Tool)
- enter Name: *Dockerflake8*
- enter Program: *docker*
- enter Arguments (replace the first parameter with the name of your container): 
    *exec -i NAMEOFYOURCONTAINER flakeheaven lint /opt/project/app/api/ /opt/project/tests/*
- enter Working Directory: *\$ProjectFileDir\$*

If you like it convenient: Add a button for flake8 to your toolbar!
- right click into the taskbar (e.g. on one of the git icons) and select *Customize ToolBar*
- select the +-sign and Add Action
- select External Tools->Dockerflake8

### Run flake8 on your project
  - Remember! You will always need to run the docker container called *1337_pizza_web_dev* of your project, to do this! 
    So start the docker container(s) locally by running your project
  - Now you may run flake8 
      - by clicking on the new icon in your toolbar or 
      - by selecting from the menu: Tools->External Tools->Dockerflake8 

# GrayLog

- What is the purpose of GrayLog?

  The purpose of Graylog is to track and monitor logs of an application in real time in order to find potential errors and bugs and give the customer the best possible response.

- What logging levels are available?

  There are 6 log leves, these are: TRACE, DEBUG, INFO, WARNING, ERROR and FATAL.

- What is the default logging level?

  The default logging level is INFO.

- Give 3-4 examples for logging commands in Python:
  ```python
  logging.info('User with id {} deleted'.format(user_id))
  logging.error(USER_NOT_FOUND.format(user_id))
  logging.info(f'Pizza type created with the name {entity.name}')
  logging.warning(f'Pizza type with the name {pizza_type_found.name} already exists')
  ```

# SonarQube

- What is the purpose of SonarQube?

  Static analyse source code(code smells, bugs, vulnerabilities) and ensure maintainability. This tool allow          programmers to discover potentials issues while coding.

- What is the purpose of the quality rules of SonarQube?

  The quality rules in SonarQube define standards and guidelines in order to seperate the issues into types and help the developers to write maintainable code.

- What is the purpose of the quality gates of SonarQube?

  Quality gates in SonarQube are thresholds or criteria that determine whether the code meets the required quality standards before deploying.


## Run SonarLint on your local Computer

It is very annoying (and takes a lot of time) to wait for the pipeline to run SonarQube. 
To speed it up, you may first run the linting part of SonarQube (SonarLint) locally like this:

### Configure PyCharm for SonarLint (only once)

- Open *Settings->Plugins*
- Choose *MarketPlace*
- Search for *SonarLint* and install the PlugIn

### Run SonarLint

- In the project view (usually to the left) you can run the SonarLint analysis by a right click on a file or a folder. 
  You will find the entry at the very bottom of the menu.
- To run it on all source code of your project select the folder called *app*

# VPN

The servers providing Graylog, SonarQube and your APIs are hidden behind the firewall of Hochschule Darmstadt.
From outside the university it can only be accessed when using a VPN.
https://its.h-da.io/stvpn-docs/de/ 