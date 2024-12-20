<pre>
MM'""""'YMM       dP                         dP 
M' .mmm. `M       88                         88 
M  MMMMMooM .d888b88 .d8888b. .d8888b. .d888b88 
M  MMMMMMMM 88'  `88 88ooood8 88'  `88 88'  `88 
M. `MMM' .M 88.  .88 88.  ... 88.  .88 88.  .88 
MM.     .dM `88888P8 `88888P' `88888P8 `88888P8 
MMMMMMMMMMM                                     
                                                                                                                                                                                                            
</pre>

# Clusters Dead

A program for blocking telegram accounts, with support for custom proxies and custom reports.

## Installation
The entire instruction below describes the process of installing libraries only for Visual Studio 2022 using vcpkg.

Install vcpkg :

```
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
```

Install libcurl : 
```
.\vcpkg install curl
```
Integrate to Visual Studio : 
```
.\vcpkg integrate install
```

## Build

Create a project with any name, it doesn't matter, and throw our main.cpp file there, then run the build with the parameters :

```
[Realese] [x64]
```

## Run 

Open the compiled ".exe" file and use it.


## Features

- Support custom reports ( in .txt format ) with placeholders
- Random email generation 
- Random number generation ( +1, +44 ) 
- Proxy support

## Make custom reports 

Create .txt file ( UTF-8 ) :
```
Dear telegram support, I want to complain about a user with a username : {user}. Proofs can be found here {proof_link}.
```
Placeholders to use : 
```
{user}
{user_id}
{proof_link}
```
The program parses line by line, so divide the lines correctly!

## FAQ

#### Will there be constant updates?

It's possible.

#### Is it possible to modify your code and create your own versions?

Yes, but it is advisable to write to me about your proposals in my telegram, or open an Issue. Still, if you want to make your own version of the program based on mine, tag me in your repository, thanks in advance!


## Disclaimer
I am not responsible for the actions that this program will perform in relation to other users. This program can only be used for educational purposes.

## License

[MIT](https://choosealicense.com/licenses/mit/)

## Authors

- [@teaineyes](https://www.github.com/teaineyes) 
- Team Enclosure
