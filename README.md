## Install guide for WebotsR2019b on Ubuntu 18.04

### 1. Install Webots Open Source Version

> If you already have webots installed on your machine you don't need to follow the steps below just make sure that webots is installed into  `/usr/local/webotsVERSION` where VERSION is the webots version (e.g. R2019b).

If you don't have Webots installed on your machine go to : http://www.cyberbotics.com. Any version greater than R2018a should work if not you can open an issue. 

After downloading webots, do : 

```
	tar xvf ~/Downloads/webots-R2019b-x86-64.tar.bz2
	sudo mv webots /usr/local/webotsR2019b
	sudo ln -sf /usr/local/webotsR2019b/webots /usr/local/bin/webots
	sudo ln -sf /usr/local/webotsR2019b/lib/libpng12.so.0 /usr/local/lib
```


### 2. Install libNMM

> **Requirements.**
The installation will be done using git, make and the g++ compilator. The library depends on the boost library and the liboptimization library (although the latter is not required to compile the code as presented in this guide). On ubuntu launch the following command to install the required softwares and library:  
On Ubuntu do : 
	    `sudo apt-get install g++ git libboost1.65-all-dev libjpeg62-dev`
> On Macosx (with brew) do :
	    `brew install boost libjpeg cmake`

1. Clone this repository


	```
	git clone https://github.com/NickKok/libnmm.git
	```
	
2. Get the submodule.
		run the following command from the libraries root folder (i.e. the `libnmm` folder)

	```
    git submodule update --init --recursive
	```

2.  Compile `spinalDynamics`

	The new version of the library now depends on the spinalDynamics library. In the long run this library will replace most of the component of libnmm.  SpinalDynamics is shipped as a submodule of libnmm. You can compile it as follow

	```
		$ ./compile spinalDynamics
	``` 
	It will ask for the password for the make install because the compiled file goes by default in /usr/local/.

	> **Note for macosx**. You might need to change the compiler to /usr/bin/g++ instead of clang. You can do so with ccmake. 
	> **Note for macosx**. Replace yaml-cpp in sml/spinalDynamics with https://github.com/jbeder/yaml-cpp

3. Compile the library
The library is compatible with an optimization toolbox which let you optimize for the weights of the model, info on the toolbox and installation procedure can be found here : [biorob-optimization framework](https://biorob.epfl.ch/page-36418-en.html). The installation is made easier for ubuntu by providing a ppa. 
The compilation is done by running the following command :
	```
	$ ./compile 5 ENABLE_OPTIMIZATION
	```
   if you don't want the optimization support simply run 

	```
	$ ./compile 5
	```


4. Make sure the library is accessible. The library uses pkg-config. Since the compile script install the library locally we need to make sure that pkg-config will find it. You can do so in two steps
	- run this in a terminal
	 `export PKG_CONFIG_PATH=~/.local/usr/lib/pkgconfig:$PKG_CONFIG_PATH`
	pkg-config should now find the library. Launch `pkg-config --cflags sml-5.0`. The output should be :
	`		-I$HOME/.local/usr/include/sml-5.0  `
	- now add this line to your bashrc file (bashrc_profile in MacOsx or zshrc if you use zsh) to make changes permanent
	`export PKG_CONFIG_PATH=~/.local/usr/lib/pkgconfig:$PKG_CONFIG_PATH`


### 3. Install humanWebotsNmm : a Webots implementation of a neuromuscular model


1. Clone the repository :

	```
	git clone https://github.com/NickKok/humanWebotsNmm.git
	```

2. Config
	- enable optimization (disabled by default)
open the file `./controller_current/webots/mf/optimization.mf` and make sur that the Optimization variable is set to "1".

	- choose the right webots version 
open the file `./controller_current/webots/mf/webots8-version.mf`  and change the name to your webots version (latest is R2019b)

3. Compile the controller
	from the humanWebotsNmm folder run 
	```
	./maker
	```

4. Launch the simulation.
The simulation can be launched as follow : 

	Depending on your system configuration you might be able to run directly the software like this

	```
	$ ./launcher 3D
	```


5. **Optional** : Compile the physics plugin (only for 2D simulation)
    - make sur the following package are installed `libgl1-mesa-dev libglu1-mesa-dev`
	- go in the `./controller_current/webots/plugins/physics/no_lift` and run :


	```
	make -j
	```

### 4. Control the behavior of the simulation

A bunch of config files are used by the simulation. The config files are located in the `./config/NAME` folder where **NAME** is the config prefix used. The config prefix used depends on the experiment you are running. There are config for a 3D model for reflex gains optimization and cpg testing, and a 3D_RL model for reinforcement learning.

When a simulation starts, the first config file being loaded is the settings file. By default the file `config/NAME/settings.xml` is used. 

```
$ ./launcher 3D
```
Will launch a simulation using the world ./webots/

### 5. Use the optimization framework.


1. Install the biorob optimization toolbox.

    - Follow the instruction at http://biorob.epfl.ch/page-36418-en.html
       ( section **Ubuntu Software Repository )
    - Install the biorob-optimization software suits

	sudo apt-get install biorob-optimization biorob-optimization-dev

	> You can find Information related to the optimization framework can be found on the same web page.

2. Make sure you compiled both `libnmm` and `sml` with the optimization flag.

	> In the sml directory open the file `./current/webots/mf/optimization.mf` and make sur that the Optimization variable is set to "1".


3. Launch an optimization

	1. Go to the `./data/optimization/tutorial` folder

	    In this folder you will see two xml file: a  `exp_PSO.xml` and `exp_stagePSO.xml`. The first file will run a standard PSO the fitness expression is defined in the XML file `fitness`  section, where you can use different parameters returned by the optimization. All the logged parameters can be found in the `libnmm/sml/sml-tools/EventManager.cc` more precisely the `EventManager::getMap()` function.

		The second file is an optimization that uses stage PSO to optimize first for distance then for speed and finally for energy (this is what has been used to produce the results showed in the Frontiers2014 article). You can control the number of iteration and the number of particles as well as all other optimization related parameters.


	2. Launch the optimaster and optiworker in two different terminal

	3. Launch the actual optimization

	     To launch the optimization simply do (from the tutorial folder)

		optirunner ./exp.xml

	> If you get an error complaining that the sml-5.0 library does not exist you shold
	> Open the /usr/bin/webots scripts and change the line 8 from

	> `WEBOTS_LIB_DIR="$webotsBinaryDir/lib"`
	> to
	> `WEBOTS_LIB_DIR="$webotsBinaryDir/lib:/home/{USERNAME}/.local/usr/lib"`
	> Replace {USERNAME} by your unix username.


5. Monitoring the optimization.

	The optimization will log different variable related to the optimisation in the results database. The parameters logged have to be defined in the

6. Extracting solutions

	- With the optiextractor

		after you finished the optimization a database is created.
		You can view the results using the `optiextractor` command.

	- With sml python scripts

		you can also use the `extract.py` script to extract the best solution. The script is located in  `./tools/job_tools/extract.py`. Run it with the `--help` to see what you can do. An example usage is as follow :

		`./extract2.py parameters -s -f 1.3 pso_1.3-IP.1.db`

		This will extract the best parameters of a stage pso experiment ( `-s` option) save it in a file called `1.3` from database `pso_1.3-IP.1.db`. You can then make your model use this parameter file by:
		 -  placing it in the config folder used by your controller and
		 -  editing the settings file used by your controller to use this setting file.

		For instance if you open the 743.wbt world file and look for controller you will see this :

		```
		controller "version52_wbt7"
		controllerArgs "settings_jw_notf.xml sample"
		```

		In this example our controller uses the `settings_jw_notfl.xml` file located in the folder `./controller_current/webots/conf_sample/settings` folder.  This folder contains several folder. Here we are interested in two of them. The settings folder which contains a set of xml files (and especially the one you are interested in here which is in our case **settings_jw_notf.xml** and the **fullReflex_gate** folder.

		You want to use our newly optimized reflex parameters. We can do this by moving our file in the fullReflex_gate folder. We then open the xml settings file and locate the **experiment** xml tag. It should contains something like this :

		      fullReflex{p_file=param3.txt,start_after_step=300}

		Now the p_file is the name of your file (**in this case 1.3 WITHOUT the .txt**). the start_after_step parameter says when your model will switch from launching gait parameter to those one.





