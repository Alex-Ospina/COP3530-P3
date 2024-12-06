README
Since our dataset is too large, it could only be uploaded in a .zip file. Therefore, in orther to be able to properly run the code, it needs to be downloaded and the .zip needs to be extracted. After unzipping the file, open it through excel and delete the header row. Then redownload this edited csv and put it in your working directory for the project.
The code was written in CLion using C++ 17. 
Also, in CLion in the Run/Debug configurations, the working library needs to be updated to the library the code and additional files are in, so that it can find the files.
Similarily, the CMake file will need to be updated to the location of the SFML library on the computer, since currently it is pointing to the one on the developers' computer.
The finalised code is found in the main branch.
