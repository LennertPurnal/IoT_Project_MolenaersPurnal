# IoT_Project_MolenaersPurnal
Welcome to our project for the 2021-2022 IoT course from UHasselt.
## Instalation
### WebServer code
the webserver in this git repository is added as a submodule because it is a heroku git repository itself.
- to import the submodule when cloning:
> `git clone --recursive https://github.com/LennertPurnal/IoT_Project_MolenaersPurnal.git`\
after this navigate to the webserver submodule with `cd IoT_Project_MolenaersPurnal/webServer` and execute `git checkout master` 
- if you already cloned as usual, add the submodule with the following command:
> `git submodule update --init`

- To pull all changes on all submodules at once, use the following command:
> `git pull --recurse-submodules`

### Psoc6 code
The Psoc6 code is located in the `SmartHome_IoT_device` folder. In modustoolbox click "Open projects from filesystem..." and select this folder to open the project, you can than build and program. 

**imortant! :** do not forget to copy the mtb_shared/ folder from modustoolbox manually into the root project folder (`/IoT_Project_MolenaersPurnal`) to avoid errors when building the *SmartHome_IoT_device* project in modustoolbox.

## Details
### Temperature sensor
The used temperature sensor is a breakout board containing the [BMP180 Pressure and Temperature sensor](https://cdn-shop.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf). This module uses I2C to be configured and read. 

the I2C address is 0x77.