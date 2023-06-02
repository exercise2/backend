# backend
This project contains the solution requested in programming exercise 2.

## Building & running locally
Folow these steps:

* Go to the root folder of the project
* Create the backend-1.0 conan package: `conan create . --build=missing`

## Public container
Public container image is [available in Docker Hub](https://hub.docker.com/repository/docker/exercises/backend/).

Run in a container with:
`docker run exercises/backend:1.0`

**NOTE**

No need for the -d option to run the container in detached mode as it currently just runs the unit tests. There is no real UI using the backend service so far.

## Your own container
If you want to build your own docker image, follow these steps:

* Go to the root folder of the project
* Build the image: `docker build -t backend .`

If you want to push an image to your Docker Hub account, follow these steps:

* Go to the root folder of the project
* Build the image: `docker build -t <username>/backend:1.0 .`
* Login to Docker Hub: `docker login -u <username>` 
* Push the image: `docker push <username>/backend:1.0`

## API documentation
If you want to generate the documentation of the API, follow these steps:

* Go to the root folder of the project
* Build the documentation: `doxygen`

Browse the documentation by opening _./doxy/html/index.html_ file in a web browser.

!!!NOTE
    You need doxygen tool in the machine where you build the documentation. You can download installation binaries [here](https://www.doxygen.nl/download.html).
